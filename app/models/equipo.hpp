#pragma once
#include <string>
#include "../../nlohmann/json.hpp"

class Equipo
{
public:
    std::string id;
    std::string nombre;

    Equipo() = default;
    Equipo(const std::string &id, const std::string &nombre)
        : id(id), nombre(nombre) {}

    nlohmann::json to_json() const
    {
        return {{"id", id}, {"nombre", nombre}};
    }

    static Equipo from_json(const nlohmann::json &j)
    {
        return Equipo(j.at("id"), j.at("nombre"));
    }
};
