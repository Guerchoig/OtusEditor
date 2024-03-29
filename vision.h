/// @brief A representation module
#pragma once
#include <string>
#include <iostream>
#include <memory>

namespace vi
{
    /// @brief Square place on screen
    struct Place
    {
        int left;
        int top;
        int w;
        int h;
    };

    using show_t = std::string;

    /// @brief Abstract interface to output objects in any view
    class IView
    {
    public:
        /// @brief updates the view in show_t format
        virtual void update_view() = 0;

        /// @brief displays the the view in show_t format on the screen
        virtual void display() = 0;
        virtual ~IView(){};
    };

    /// @brief Entity, representing an object in a text view
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