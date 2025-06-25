#pragma once
#include <string>
#include "../../nlohmann/json.hpp"

class Equipo
{
public:
    std::string id;
    std::string nombre;
    std::string ciudad;

    Equipo() = default;
    Equipo(const std::string &id, const std::string &nombre, const std::string &ciudad)
        : id(id), nombre(nombre), ciudad(ciudad) {}

    nlohmann::json to_json() const
    {
        return {{"id", id}, {"nombre", nombre}, {"ciudad", ciudad}};
    }

    static Equipo from_json(const nlohmann::json &j)
    {
        return Equipo(j.at("id"), j.at("nombre"), j.at("ciudad"));
    }
};
