#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "menu.h"

// 购物车条目
struct CartItem
{
    Dish dish;
    int quantity;
    double subtotal() const { return dish.price * quantity; }
};

class Cart
{
public:
    // 添加菜品（已存在则增加数量）
    void addItem(const Dish &dish, int qty = 1)
    {
        for (auto &item : items_)
        {
            if (item.dish.id == dish.id)
            {
                item.quantity += qty;
                return;
            }
        }
        items_.push_back({dish, qty});
    }

    // 删除菜品（通过菜品ID）
    bool removeItem(int dishId)
    {
        for (auto it = items_.begin(); it != items_.end(); ++it)
        {
            if (it->dish.id == dishId)
            {
                items_.erase(it);
                return true;
            }
        }
        return false;
    }

    // 修改数量（qty <= 0 则删除）
    bool updateQty(int dishId, int qty)
    {
        if (qty <= 0)
            return removeItem(dishId);
        for (auto &item : items_)
        {
            if (item.dish.id == dishId)
            {
                item.quantity = qty;
                return true;
            }
        }
        return false;
    }

    // 清空购物车
    void clear() { items_.clear(); }

    // 判断是否为空
    bool isEmpty() const { return items_.empty(); }

    // 获取总价
    double total() const
    {
        double sum = 0;
        for (const auto &item : items_)
            sum += item.subtotal();
        return sum;
    }

    // 获取所有条目
    const std::vector<CartItem> &getItems() const { return items_; }

    // 打印购物车
    void print() const
    {
        if (isEmpty())
        {
            std::cout << "\n  购物车为空。\n";
            return;
        }
        std::cout << "\n";
        std::cout << "  " << std::left
                  << std::setw(4) << "编号"
                  << std::setw(18) << "菜名"
                  << std::setw(8) << "单价"
                  << std::setw(6) << "数量"
                  << "小计\n";
        std::cout << "  " << std::string(46, '-') << "\n";
        for (const auto &item : items_)
        {
            std::cout << "  "
                      << std::setw(4) << item.dish.id
                      << std::setw(18) << item.dish.name
                      << "CNY" << std::setw(7) << std::fixed << std::setprecision(2)
                      << item.dish.price
                      << std::setw(6) << item.quantity
                      << "CNY" << std::fixed << std::setprecision(2)
                      << item.subtotal() << "\n";
        }
        std::cout << "  " << std::string(46, '-') << "\n";
        std::cout << "  合计：CNY" << std::fixed << std::setprecision(2)
                  << total() << "\n";
    }

private:
    std::vector<CartItem> items_;
};