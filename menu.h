#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// 菜品结构体
struct Dish
{
    int id;
    std::string name;
    double price;
    std::string category;
    std::string description;
};

class Menu
{
public:
    Menu()
    {
        // ---- 初始化菜单数据 ----
        dishes_ = {
            // 主食
            {1, "红烧肉饭", 22.0, "主食", "秘制红烧肉，软糯入味"},
            {2, "鱼香肉丝饭", 18.0, "主食", "川味经典，酸甜微辣"},
            {3, "宫保鸡丁饭", 20.0, "主食", "花生爆香，鲜嫩可口"},
            {4, "番茄炒蛋饭", 15.0, "主食", "家常风味，营养丰富"},
            {5, "扬州炒饭", 16.0, "主食", "蛋炒黄金饭，配料丰富"},
            // 面食
            {6, "牛肉拉面", 25.0, "面食", "手工拉面，汤底鲜浓"},
            {7, "炸酱面", 18.0, "面食", "北京传统炸酱，劲道爽滑"},
            {8, "担担面", 20.0, "面食", "麻辣鲜香，芝麻酱浓郁"},
            // 小吃
            {9, "饺子（10个）", 16.0, "小吃", "猪肉白菜馅，皮薄馅足"},
            {10, "春卷（5条）", 12.0, "小吃", "酥脆可口，素菜馅"},
            // 饮品
            {11, "绿茶", 6.0, "饮品", "清香提神，现泡"},
            {12, "可乐", 5.0, "饮品", "冰镇可口可乐"},
            {13, "酸梅汤", 8.0, "饮品", "传统酸梅汤，消暑解渴"},
            // 甜品
            {14, "红豆汤", 10.0, "甜品", "香甜软糯，暖胃"},
            {15, "双皮奶", 12.0, "甜品", "顺滑细腻，港式传统"},
        };
    }

    // 获取所有菜品
    const std::vector<Dish> &getAllDishes() const
    {
        return dishes_;
    }

    // 按分类获取菜品
    std::vector<Dish> getByCategory(const std::string &cat) const
    {
        std::vector<Dish> result;
        for (const auto &d : dishes_)
            if (d.category == cat)
                result.push_back(d);
        return result;
    }

    // 通过 ID 查找菜品（找不到返回 nullptr）
    const Dish *findById(int id) const
    {
        for (const auto &d : dishes_)
            if (d.id == id)
                return &d;
        return nullptr;
    }

    // 打印完整菜单
    void printMenu() const
    {
        std::vector<std::string> categories = {"主食", "面食", "小吃", "饮品", "甜品"};
        for (const auto &cat : categories)
        {
            std::cout << "\n  [" << cat << "]\n";
            std::cout << "  " << std::string(46, '-') << "\n";
            std::cout << std::left
                      << "  " << std::setw(4) << "编号"
                      << std::setw(18) << "菜名"
                      << std::setw(8) << "价格"
                      << "描述\n";
            std::cout << "  " << std::string(46, '-') << "\n";
            for (const auto &d : getByCategory(cat))
            {
                std::cout << "  "
                          << std::setw(4) << d.id
                          << std::setw(18) << d.name
                          << "CNY" << std::setw(7) << std::fixed
                          << std::setprecision(2) << d.price
                          << d.description << "\n";
            }
        }
    }

private:
    std::vector<Dish> dishes_;
};