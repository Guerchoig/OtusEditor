/// @brief The file contains business model of the app
#pragma once
#include "error.h"
#include "vision.h"
#include "storage.h"
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <map>

/// @brief Namespace contains business model of the app
namespace mod
{
    enum class FigType
    {
        Rect,
        Triangle
    };

    class Document;
    using spdoc_t = std::shared_ptr<mod::Document>;

    /// @brief A parent class for any object, which can be put onto the drawing canvas
    class Gliph : public vi::TextView
    {
    protected:
        vi::Place pos;
        int id;
        std::string name;
        FigType fig_type;
        Gliph() = delete;
        std::shared_ptr<Document> p_doc;
        void update_view() override
        {
            view = name + " #" + std::to_string(id) + " Place:" +
                   " " + std::to_string(pos.left) +
                   " " + std::to_string(pos.w) +
                   " " + std::to_string(pos.top) +
                   " " + std::to_string(pos.h) + "\n";
        }

    public:
        Gliph(vi::Place _pos, std::string _name,
              FigType _fig_type,
              std::shared_ptr<Document> _pdoc,
              int _id = 0) : vi::TextView(_name),
                             pos(_pos), id(_id), name(_name),
                             fig_type(_fig_type),
                             p_doc(_pdoc)
        {
            update_view();
        }

        virtual ~Gliph(){};

        void set_id(int _id)
        {
            id = _id;
            update_view();
        }
        int get_id() { return id; }
        vi::show_t get_view()
        {
            return view;
        }
    };

    /// @brief  A rectangle
    class Rect : public Gliph
    {
    protected:
        Rect() = delete;

    public:
        Rect(vi::Place _pos,
             std::shared_ptr<Document> _pdoc) : Gliph(_pos,
                                                      std::string("Rect"),
                                                      FigType::Rect,
                                                      _pdoc)

        {
        }
        ~Rect() {}
    };

    using p_gliph_t = std::shared_ptr<Gliph>;
    using gliphs_t = std::vector<std::shared_ptr<Gliph>>;

    /// @brief  A collection, which receive gliphs,
    /// being created elsewhere
    class GliphsCollection
    {

    protected:
        gliphs_t gliphs;

    public:
        // Include a gliph into collection
        void add_gliph(p_gliph_t pg)
        {
            pg->set_id(gliphs.size());
            gliphs.push_back(pg);
        }
        // Delete a gliph from collection and destroy it
        void del_gliph(int id)
        {
            gliphs.erase(gliphs.begin() + id);
        }
        void clear()
        {
            gliphs.clear();
        }
        std::string make_view()
        {
            std::string view("");
            for (auto i : gliphs)
                view = view + i->get_view();
            return view;
        }
    };

    /// @brief Implements doc, which contains gliphs collection
    class Document : public vi::TextView
    {
    private:
        GliphsCollection gliphs;
        int imp_exp;
        void update_view() override
        {
            std::string s = "State: ";
            switch (imp_exp)
            {
            case 0:
                break;
            case 1:
                s = s + "imported";
                break;
            case -1:
                s = s + "exported";
                break;
            }
            view = gliphs.make_view() + s + "\n";
        }

    public:
        explicit Document() : vi::TextView("")
        {
            imp_exp = 0;
        }
        explicit Document([[maybe_unused]] Document &doc) : vi::TextView("")
        {
            gliphs.clear();
            imp_exp = 0;
        }
        explicit Document([[maybe_unused]] stg::serialized_t sdoc) : vi::TextView("")
        {
            gliphs.clear();
            imp_exp = 0;
        }

        ~Document() {}

        void add_fig(std::shared_ptr<Gliph> f)
        {
            gliphs.add_gliph(f);
            update_view();
        }
        void del_fig(int id)
        {
            gliphs.del_gliph(id);
            update_view();
        }
        stg::serialized_t serialize()
        {
            return "serialize";
        }
        void deserialize([[maybe_unused]] stg::serialized_t data, [[maybe_unused]] std::shared_ptr<Document> &doc)
        {
        }
        void set_imp_exp(int val)
        {
            imp_exp = val;
            update_view();
        }
    };

    using sp_doc_t = std::shared_ptr<Document>;
}
