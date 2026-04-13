#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include "cart.h"

// 订单状态
enum class OrderStatus
{
    PENDING,   // 待处理
    CONFIRMED, // 已确认
    PREPARING, // 备餐中
    COMPLETED, // 已完成
    CANCELLED  // 已取消
};

std::string statusToStr(OrderStatus s)
{
    switch (s)
    {
    case OrderStatus::PENDING:
        return "待处理";
    case OrderStatus::CONFIRMED:
        return "已确认";
    case OrderStatus::PREPARING:
        return "备餐中";
    case OrderStatus::COMPLETED:
        return "已完成";
    case OrderStatus::CANCELLED:
        return "已取消";
    }
    return "未知";
}

// 单条订单
struct Order
{
    int id;
    std::string tableNo;
    std::vector<CartItem> items;
    double totalAmount;
    OrderStatus status;
    std::string createTime;
    std::string remark;

    void print() const
    {
        std::cout << "\n  +------------------------------------------+\n";
        std::cout << "  |            订 单 详 情                   |\n";
        std::cout << "  +------------------------------------------+\n";
        std::cout << "  | 订单编号：" << std::left << std::setw(36) << id << "|\n";
        std::cout << "  | 桌号：   " << std::setw(37) << tableNo << "|\n";
        std::cout << "  | 下单时间：" << std::setw(36) << createTime << "|\n";
        std::cout << "  | 状态：   " << std::setw(37) << statusToStr(status) << "|\n";
        if (!remark.empty())
            std::cout << "  | 备注：   " << std::setw(37) << remark << "|\n";
        std::cout << "  +------------------------------------------+\n";
        std::cout << "  | " << std::setw(4) << "编号"
                  << std::setw(16) << "菜名"
                  << std::setw(8) << "单价"
                  << std::setw(5) << "数量"
                  << std::setw(10) << "小计   |\n";
        std::cout << "  +------------------------------------------+\n";
        for (const auto &item : items)
        {
            std::cout << "  | "
                      << std::setw(4) << item.dish.id
                      << std::setw(16) << item.dish.name
                      << "CNY" << std::setw(6) << std::fixed << std::setprecision(2)
                      << item.dish.price
                      << std::setw(5) << item.quantity
                      << "CNY" << std::setw(8) << item.subtotal() << " |\n";
        }
        std::cout << "  +------------------------------------------+\n";
        std::cout << "  | 合计：CNY" << std::left << std::setw(39)
                  << std::fixed << std::setprecision(2) << totalAmount << "|\n";
        std::cout << "  +------------------------------------------+\n";
    }
};

// 订单管理器
class OrderManager
{
public:
    // 从购物车创建订单
    Order createOrder(const Cart &cart, const std::string &tableNo,
                      const std::string &remark = "")
    {
        Order o;
        o.id = ++nextId_;
        o.tableNo = tableNo;
        o.items = cart.getItems();
        o.totalAmount = cart.total();
        o.status = OrderStatus::CONFIRMED;
        o.remark = remark;

        // 获取当前时间
        time_t now = time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        o.createTime = buf;

        orders_.push_back(o);
        return o;
    }

    // 获取全部订单
    const std::vector<Order> &getAll() const { return orders_; }

    // 按 ID 查找订单
    Order *findById(int id)
    {
        for (auto &o : orders_)
            if (o.id == id)
                return &o;
        return nullptr;
    }

    // 更新订单状态
    bool updateStatus(int id, OrderStatus status)
    {
        Order *o = findById(id);
        if (!o)
            return false;
        o->status = status;
        return true;
    }

    // 打印所有订单列表
    void printAll() const
    {
        if (orders_.empty())
        {
            std::cout << "\n  暂无订单记录。\n";
            return;
        }
        std::cout << "\n  " << std::left
                  << std::setw(6) << "订单号"
                  << std::setw(8) << "桌号"
                  << std::setw(10) << "金额"
                  << std::setw(10) << "状态"
                  << "下单时间\n";
        std::cout << "  " << std::string(52, '-') << "\n";
        for (const auto &o : orders_)
        {
            std::cout << "  "
                      << std::setw(6) << o.id
                      << std::setw(8) << o.tableNo
                      << "CNY" << std::setw(9) << std::fixed << std::setprecision(2)
                      << o.totalAmount
                      << std::setw(10) << statusToStr(o.status)
                      << o.createTime << "\n";
        }
    }

private:
    std::vector<Order> orders_;
    int nextId_ = 0;
};