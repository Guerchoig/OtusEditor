/// @brief Iplements long term storage functionality
#pragma once
#include "vision.h"
#include <memory>
#include <string>

/// @brief A storage namespace
namespace stg
{
    using serialized_t = std::string;
    using path_t = std::string;

    /// @brief A general storage interface
    class IStorage
    {
    public:
        virtual void export_doc(const serialized_t &doc, const path_t &path) = 0;
        virtual serialized_t import_doc(const path_t &path) = 0;
        virtual ~IStorage() {}
    };

    /// @brief API for file storage
    class FileStorage : IStorage
    {
    public:
        FileStorage() {}
        static void export_doc([[maybe_unused]] const serialized_t doc, [[maybe_unused]] path_t path) {}
        static serialized_t import_doc([[maybe_unused]] const path_t path) { return *(new serialized_t); }
    };
}