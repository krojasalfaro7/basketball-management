#include "../../nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

using json = nlohmann::json;

class Repository
{
    std::string filename;

public:
    // Ajusta la ruta para que todos los métodos usen repository/data/
    Repository(const std::string &file) : filename("repository/data/" + file) {}

    // Create: agrega una nueva entidad al archivo json
    void create(const json &entity)
    {
        std::vector<json> entities = read_all();
        entities.push_back(entity);
        write_all(entities);
    }

    // Read: obtiene todas las entidades
    std::vector<json> read_all()
    {
        std::ifstream in(filename);
        if (!in.is_open())
            return {};
        json j;
        in >> j;
        if (!j.is_array())
            return {};
        return j.get<std::vector<json>>();
    }

    // Update: actualiza una entidad por id
    bool update(const std::string &id, const json &new_entity)
    {
        auto entities = read_all();
        bool updated = false;
        for (auto &e : entities)
        {
            if (e.contains("id") && e["id"] == id)
            {
                e = new_entity;
                updated = true;
                break;
            }
        }
        if (updated)
            write_all(entities);
        return updated;
    }

    // Delete: elimina una entidad por id
    bool remove(const std::string &id)
    {
        auto entities = read_all();
        auto it = std::remove_if(entities.begin(), entities.end(), [&](const json &e)
                                 { return e.contains("id") && e["id"] == id; });
        if (it != entities.end())
        {
            entities.erase(it, entities.end());
            write_all(entities);
            return true;
        }
        return false;
    }

private:
    void write_all(const std::vector<json> &entities)
    {
        std::string dir = "repository/data";
        MKDIR(dir.c_str());
        std::ofstream out(filename);
        out << json(entities).dump(4);
    }
};
