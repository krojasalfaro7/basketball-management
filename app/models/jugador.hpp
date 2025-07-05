#pragma once
#include <string>
#include "../../nlohmann/json.hpp"

class Jugador
{
public:
    std::string id;
    std::string nombre;
    std::string posicion;
    std::string equipo_id; // Relación con Equipo

    Jugador() = default;
    Jugador(const std::string &id, const std::string &nombre, const std::string &posicion, const std::string &equipo_id)
        : id(id), nombre(nombre), posicion(posicion), equipo_id(equipo_id) {}

    nlohmann::json to_json() const
    {
        return {{"id", id}, {"nombre", nombre}, {"posicion", posicion}, {"equipo_id", equipo_id}};
    }

    static Jugador from_json(const nlohmann::json &j)
    {
        return Jugador(j.at("id"), j.at("nombre"), j.at("posicion"), j.at("equipo_id"));
    }
};