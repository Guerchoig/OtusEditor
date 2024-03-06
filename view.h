#pragma once

#include <string>
#include <iostream>

using OutType = std::string;

class IShow
{
public:
    virtual const OutType show_me() = 0;
};

class simply_showable : IShow
{
protected:
    std::string label;

public:
    simply_showable() = default;
    ~simply_showable() = default;
    virtual const OutType show_me() { return "Show " + label; };
};

class IView
{
public:
    static void display_it(IShow &it)
    {
        std::cout << it.show_me() << '\n';
    }
};
