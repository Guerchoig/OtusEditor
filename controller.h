#pragma once

#include "general.h"
#include "view.h"
#include <string>
#include <iostream>

class ICommand
{
public:
    virtual void execute() = 0;
    virtual ~ICommand() = default;
};

class base_control : simply_showable
{
private:
    ICommand *p_command;

public:
    base_control(ICommand *p_command, std::string _label) : p_command(p_command)
    {
        label = _label;
    }
    virtual ~base_control() = default;
};
