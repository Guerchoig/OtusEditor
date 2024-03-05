#include "graphics_general.h"
#include <string>
#include <iostream>

class ILabel{
private:
    std::string label;
public:
    void show(){std::cout << label << '\n';}
};

class ICommand {
public:
    virtual void execute() = 0;
    virtual ~ICommand() = default;
};

class base_control:rect, ILabel
{
private:
    ICommand *p_command;
public:
    base_control(ICommand *p_command) : p_command(p_command){}
    virtual ~base_control() = default;
};

class Hexagone;


