#pragma once
#include <iostream>
#include <string>
#include <limits>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

namespace Utils
{
    // 清屏
    void clearScreen()
    {
        system(CLEAR);
    }

    // 打印分隔线
    void printLine(char c = '=', int len = 50)
    {
        std::cout << std::string(len, c) << std::endl;
    }

    // 打印居中标题
    void printTitle(const std::string &title, int width = 50)
    {
        int padding = (width - (int)title.size()) / 2;
        if (padding < 0)
            padding = 0;
        printLine();
        std::cout << std::string(padding, ' ') << title << std::endl;
        printLine();
    }

    // 获取整数输入（带校验）
    int getInt(const std::string &prompt)
    {
        int val;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> val)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] 输入无效，请输入数字。\n";
        }
    }

    // 暂停等待用户按键
    void pause()
    {
        std::cout << "\n  按 Enter 继续...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}