#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

class Repository
{
    string filename;

public:
    // Ajusta la ruta para que todos los métodos usen repository/data/
    Repository(const string &file) : filename(file) {}

    // Create: agrega una nueva entidad al archivo json
    void create(const json &entity)
    {
        vector<json> entities = read_all();
        entities.push_back(entity);
        write_all(entities);
    }

    // Read: obtiene todas las entidades
    vector<json> read_all()
    {
        ifstream in(filename);
        if (!in.is_open())
            return {};
        json j;
        in >> j;
        if (!j.is_array())
            return {};
        return j.get<vector<json>>();
    }

    // Update: actualiza una entidad por id
    bool update(const string &id, const json &new_entity)
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
    bool remove(const string &id)
    {
        auto entities = read_all();
        auto it = remove_if(entities.begin(), entities.end(), [&](const json &e)
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
    void write_all(const vector<json> &entities)
    {
        ofstream out(filename);
        out << json(entities).dump(4);
    }
};