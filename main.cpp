#include <iostream>
#include <string>
#include "utils.h"
#include "menu.h"
#include "cart.h"
#include "order.h"

// ============================================================
//  全局对象
// ============================================================
Menu g_menu;
Cart g_cart;
OrderManager g_orderMgr;

// ============================================================
//  各页面声明
// ============================================================
void pageMain();
void pageMenu();
void pageCart();
void pageCheckout();
void pageOrders();
void pageOrderDetail();
void pageAdmin();

// ============================================================
//  页面：主菜单（首页）
// ============================================================
void pageMain()
{
    while (true)
    {
        Utils::clearScreen();
        Utils::printTitle("*  欢迎光临  美食小馆  *");
        std::cout << "\n";
        std::cout << "  [1]  浏览菜单\n";
        std::cout << "  [2]  查看购物车";
        if (!g_cart.isEmpty())
            std::cout << "  (共 " << g_cart.getItems().size() << " 种，CNY"
                      << std::fixed << std::setprecision(2) << g_cart.total() << ")";
        std::cout << "\n";
        std::cout << "  [3]  我的订单\n";
        std::cout << "  [4]  管理员界面\n";
        std::cout << "  [0]  退出系统\n";
        std::cout << "\n";

        int choice = Utils::getInt("  请选择 > ");
        switch (choice)
        {
        case 1:
            pageMenu();
            break;
        case 2:
            pageCart();
            break;
        case 3:
            pageOrders();
            break;
        case 4:
            pageAdmin();
            break;
        case 0:
            Utils::clearScreen();
            std::cout << "\n  感谢惠顾，欢迎再次光临！\n\n";
            return;
        default:
            std::cout << "  [!] 无效选项，请重新输入。\n";
            Utils::pause();
        }
    }
}

// ============================================================
//  页面：菜单浏览 & 点餐
// ============================================================
void pageMenu()
{
    while (true)
    {
        Utils::clearScreen();
        Utils::printTitle("菜  单");
        g_menu.printMenu();

        std::cout << "\n  输入菜品编号加入购物车，输入 0 返回主页\n";
        int id = Utils::getInt("  菜品编号 > ");
        if (id == 0)
            return;

        const Dish *dish = g_menu.findById(id);
        if (!dish)
        {
            std::cout << "  [!] 找不到该菜品，请重新输入。\n";
            Utils::pause();
            continue;
        }

        int qty = Utils::getInt("  数量 > ");
        if (qty <= 0)
        {
            std::cout << "  [!] 数量须大于 0。\n";
            Utils::pause();
            continue;
        }

        g_cart.addItem(*dish, qty);
        std::cout << "  [OK] 已将 [" << dish->name << "] x" << qty
                  << " 加入购物车！\n";
        Utils::pause();
    }
}

// ============================================================
//  页面：购物车
// ============================================================
void pageCart()
{
    while (true)
    {
        Utils::clearScreen();
        Utils::printTitle("购  物  车");
        g_cart.print();

        std::cout << "\n";
        std::cout << "  [1]  继续点餐\n";
        std::cout << "  [2]  修改菜品数量\n";
        std::cout << "  [3]  删除菜品\n";
        std::cout << "  [4]  清空购物车\n";
        std::cout << "  [5]  去结账\n";
        std::cout << "  [0]  返回主页\n";

        int choice = Utils::getInt("  请选择 > ");
        switch (choice)
        {
        case 1:
            pageMenu();
            break;
        case 2:
        {
            if (g_cart.isEmpty())
            {
                std::cout << "  购物车为空。\n";
                Utils::pause();
                break;
            }
            int id = Utils::getInt("  输入要修改的菜品编号 > ");
            int qty = Utils::getInt("  新数量（输入 0 删除）> ");
            g_cart.updateQty(id, qty)
                ? std::cout << "  [OK] 已更新。\n"
                : std::cout << "  [!] 未找到该菜品。\n";
            Utils::pause();
            break;
        }
        case 3:
        {
            if (g_cart.isEmpty())
            {
                std::cout << "  购物车为空。\n";
                Utils::pause();
                break;
            }
            int id = Utils::getInt("  输入要删除的菜品编号 > ");
            g_cart.removeItem(id)
                ? std::cout << "  [OK] 已删除。\n"
                : std::cout << "  [!] 未找到该菜品。\n";
            Utils::pause();
            break;
        }
        case 4:
            g_cart.clear();
            std::cout << "  [OK] 购物车已清空。\n";
            Utils::pause();
            break;
        case 5:
            pageCheckout();
            break;
        case 0:
            return;
        default:
            std::cout << "  [!] 无效选项。\n";
            Utils::pause();
        }
    }
}

// ============================================================
//  页面：结账确认
// ============================================================
void pageCheckout()
{
    Utils::clearScreen();
    Utils::printTitle("结  账  确  认");

    if (g_cart.isEmpty())
    {
        std::cout << "\n  购物车为空，无法结账。\n";
        Utils::pause();
        return;
    }

    g_cart.print();

    std::cout << "\n  请输入桌号（例如：A1, B3）：";
    std::string tableNo;
    std::getline(std::cin, tableNo);
    if (tableNo.empty())
        tableNo = "未指定";

    std::cout << "  备注（可选，直接回车跳过）：";
    std::string remark;
    std::getline(std::cin, remark);

    std::cout << "\n  确认下单？（y/n）：";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y')
    {
        Order o = g_orderMgr.createOrder(g_cart, tableNo, remark);
        g_cart.clear();
        std::cout << "\n  [OK] 下单成功！订单号：" << o.id << "\n";
        o.print();
    }
    else
    {
        std::cout << "\n  已取消，返回购物车。\n";
    }
    Utils::pause();
}

// ============================================================
//  页面：我的订单列表
// ============================================================
void pageOrders()
{
    while (true)
    {
        Utils::clearScreen();
        Utils::printTitle("订  单  记  录");
        g_orderMgr.printAll();

        std::cout << "\n  [1]  查看订单详情\n";
        std::cout << "  [0]  返回主页\n";

        int choice = Utils::getInt("  请选择 > ");
        if (choice == 0)
            return;
        if (choice == 1)
            pageOrderDetail();
    }
}

// ============================================================
//  页面：订单详情
// ============================================================
void pageOrderDetail()
{
    int id = Utils::getInt("  输入订单号 > ");
    Order *o = g_orderMgr.findById(id);
    if (!o)
    {
        std::cout << "  [!] 订单不存在。\n";
        Utils::pause();
        return;
    }
    Utils::clearScreen();
    o->print();
    Utils::pause();
}

// ============================================================
//  页面：管理员界面
// ============================================================
void pageAdmin()
{
    // 简单密码验证
    std::cout << "\n  请输入管理员密码：";
    std::string pwd;
    std::getline(std::cin, pwd);
    if (pwd != "admin123")
    {
        std::cout << "  [!] 密码错误。\n";
        Utils::pause();
        return;
    }

    while (true)
    {
        Utils::clearScreen();
        Utils::printTitle("管  理  员  界  面");
        std::cout << "\n";
        std::cout << "  [1]  查看所有订单\n";
        std::cout << "  [2]  更新订单状态\n";
        std::cout << "  [3]  查看菜单\n";
        std::cout << "  [0]  返回主页\n";

        int choice = Utils::getInt("  请选择 > ");
        switch (choice)
        {
        case 1:
            Utils::clearScreen();
            Utils::printTitle("所有订单");
            g_orderMgr.printAll();
            Utils::pause();
            break;
        case 2:
        {
            Utils::clearScreen();
            g_orderMgr.printAll();
            int id = Utils::getInt("\n  输入订单号 > ");
            std::cout << "  状态选项：\n";
            std::cout << "  [1] 待处理  [2] 已确认  [3] 备餐中\n";
            std::cout << "  [4] 已完成  [5] 已取消\n";
            int s = Utils::getInt("  选择状态 > ");
            OrderStatus st = OrderStatus::PENDING;
            switch (s)
            {
            case 1:
                st = OrderStatus::PENDING;
                break;
            case 2:
                st = OrderStatus::CONFIRMED;
                break;
            case 3:
                st = OrderStatus::PREPARING;
                break;
            case 4:
                st = OrderStatus::COMPLETED;
                break;
            case 5:
                st = OrderStatus::CANCELLED;
                break;
            default:
                std::cout << "  [!] 无效状态。\n";
                Utils::pause();
                continue;
            }
            g_orderMgr.updateStatus(id, st)
                ? std::cout << "  [OK] 状态已更新。\n"
                : std::cout << "  [!] 订单不存在。\n";
            Utils::pause();
            break;
        }
        case 3:
            Utils::clearScreen();
            Utils::printTitle("菜单总览");
            g_menu.printMenu();
            Utils::pause();
            break;
        case 0:
            return;
        default:
            std::cout << "  [!] 无效选项。\n";
            Utils::pause();
        }
    }
}

// ============================================================
//  程序入口
// ============================================================
int main()
{
    pageMain();
    return 0;
}