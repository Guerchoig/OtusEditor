/// @brief Module contents command interface
/// and the main function

#include "vision.h"
#include "model.h"
#include "storage.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <memory>
#include <utility>

namespace cmd
{

    /// @brief Abstract command interface
    class ICommand
    {
    public:
        virtual bool execute() = 0; // Takes no parameters,
                                    // which are instead passed to the constructor of the specific command
        virtual ~ICommand(){};
    };

    /// @brief Prototype class for all commands
    ///  make it possible to visualise commands execution
    class Command : public ICommand, public vi::TextView
    {
    protected:
        mod::spdoc_t p_doc;
        Command() = delete;
        std::string name;
        virtual void update_view() override
        {
            view = name;
        }

    public:
        explicit Command(const std::string _name,
                         mod::spdoc_t _p_doc = nullptr) : TextView("")
        {
            p_doc = _p_doc;
            name = _name;
            update_view();
        }
        virtual ~Command(){};
        virtual bool execute() override
        {
            return true;
        }
    };

    /// @brief "Exit application" command
    class Exit : public Command
    {
    public:
        Exit() : Command(vi::show_t("Exit"))
        {
        }
        ~Exit(){};

        bool execute() override
        {
            update_view();
            return false; // returned false is used as signal to quit app
        }
    };

    /// @brief Exports the file using storage interface and serialization
    class ExportToFile : public Command
    {
    private:
        std::string path;
        void update_view() override
        {
            view = name + " to " + path;
        }

    public:
        explicit ExportToFile(std::string _filename,
                              mod::spdoc_t _p_doc) : Command(vi::show_t("ExportToFile"),
                                                             _p_doc),
                                                     path(_filename) {}
        ~ExportToFile(){};
        bool execute() override
        {
            p_doc->serialize();
            stg::FileStorage::export_doc(p_doc->serialize(), path);
            p_doc->set_imp_exp(-1); // sets imported/exported state of the doc
            update_view();
            return true;
        }
    };

    /// @brief Imports the file using storage interface and deserialization
    class ImportFromFile : public Command
    {
    private:
        stg::path_t path;
        void update_view() override
        {
            view = name + " from " + path;
        }

    public:
        explicit ImportFromFile(std::string _path, mod::spdoc_t _p_doc) : Command(vi::show_t("ImportFromFile"),
                                                                                  _p_doc),
                                                                          path(_path) {}
        ~ImportFromFile(){};
        bool execute() override
        {
            p_doc->deserialize(stg::FileStorage::import_doc(path), p_doc);
            p_doc->set_imp_exp(1); // sets imported/exported state of the doc
            update_view();
            return true;
        }
    };

    /// @brief Constructs new doc in-place
    class NewDoc : public Command
    {
    public:
        explicit NewDoc(mod::spdoc_t _pdoc) : Command(vi::show_t("NewDoc"), _pdoc) {}
        ~NewDoc(){};
        bool execute() override
        {
            auto p = new (&(*p_doc)) mod::Document(*p_doc);
            (void)p;
            update_view();
            return true;
        }
    };

    /// @brief Adds a rectangle to the collection of gliphs
    class AddRect : public Command
    {
    private:
        mod::p_gliph_t p_fig;

    public:
        AddRect(vi::Place _pos,
                std::shared_ptr<mod::Document> _p_doc) : Command(vi::show_t("AddRect"),
                                                                 _p_doc),
                                                         p_fig(mod::p_gliph_t(new mod::Rect(_pos, _p_doc))) {}
        ~AddRect(){};
        bool execute() override
        {
            p_doc->add_fig(p_fig);
            p_doc->set_imp_exp(0);
            update_view();
            return true;
        }
    };

    /// @brief Deletes choosen figure from the collection of gliphs
    class DeleteFigure : public Command
    {
    private:
        int id;

    public:
        DeleteFigure(int _id,
                     std::shared_ptr<mod::Document> _p_doc) : Command(vi::show_t("DeleteFigure"),
                                                                      _p_doc),
                                                              id(_id) {}
        ~DeleteFigure(){};
        bool execute() override
        {
            p_doc->del_fig(id);
            p_doc->set_imp_exp(0);
            update_view();
            return true;
        }
    };
}
class App
{
private:
    class Queue
    {
    private:
        std::queue<std::shared_ptr<cmd::Command>> q;

    public:
        Queue() = default;
        void push_command(std::shared_ptr<cmd::Command> p)
        {
            q.push(p);
        }
        std::shared_ptr<cmd::Command> pop_command()
        {
            auto p = q.front();
            q.pop();
            return p;
        };
    };

    Queue cmd_q;
    mod::spdoc_t p_doc;

public:
    App()
    {
        p_doc = mod::spdoc_t(new mod::Document());
    }

    int proceed_commands()
    {
        while (true)
        {
            auto comm = cmd_q.pop_command();
            auto res = comm->execute();
            comm->display();
            if (!res)
                break;
            p_doc->display();
        }
        return 0;
    }

    // Test command pool
    void prepare_test_run()
    {
        cmd_q.push_command(std::shared_ptr<cmd::Command>(new cmd::NewDoc(p_doc)));
        cmd_q.push_command(std::shared_ptr<cmd::Command>(new cmd::ImportFromFile("file_path", p_doc)));
        cmd_q.push_command(std::shared_ptr<cmd::Command>(new cmd::AddRect(vi::Place{0, 0, 5, 5}, p_doc)));
        cmd_q.push_command(std::shared_ptr<cmd::Command>(new cmd::DeleteFigure(0, p_doc)));
        cmd_q.push_command(std::shared_ptr<cmd::Command>(new cmd::ExportToFile("file_path", p_doc)));
        cmd_q.push_command(std::shared_ptr<cmd::Command>(new cmd::Exit()));
    }
};

int main()
{
    App app;
    app.prepare_test_run();
    auto res = app.proceed_commands();
    return res;
}