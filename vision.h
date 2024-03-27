#pragma once

#include <string>
#include <iostream>
#include <memory>

namespace vi
{
    // Square place on screen
    struct Place
    {
        int left;
        int top;
        int w;
        int h;
    };

    using show_t = std::string;

    // Abstract interface to output objects in any view
    class IView
    {
    public:
        virtual void update_view() = 0;
        virtual void display() = 0; // displays the prepared view on the screen
        virtual ~IView(){};
    };

    // Entity, representing an object in a text view

    class TextView : public IView
    {

    protected:
        show_t view; // representation, which TextView knows how to display
        std::string simple_representation;
        void update_view() override
        {
            view = simple_representation;
        }

    public:
        explicit TextView(std::string s)
            : simple_representation(s)
        {
            update_view();
        }
        virtual ~TextView() {}

        void display() override
        {
            std::cout << view << '\n';
        }
    };

    void clear_output()
    {
        ;
    } // clears the screen
}