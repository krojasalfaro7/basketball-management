#pragma once
#include <string>
#include "../../nlohmann/json.hpp"

class Jugador
{
public:
    std::string id;
    std::string nombre;
    std::string edad;
    std::string altura;
    std::string peso;
    std::string nacionalidad;
    std::string fecha_ingreso;
    std::string posicion;
    std::string equipo_id;

    Jugador() = default;
    Jugador(const std::string &id, const std::string &nombre, const std::string &edad, const std::string &altura, const std::string &peso, const std::string &nacionalidad, const std::string &fecha_ingreso, const std::string &posicion, const std::string &equipo_id)
        : id(id), nombre(nombre), edad(edad), altura(altura), peso(peso), nacionalidad(nacionalidad), fecha_ingreso(fecha_ingreso), posicion(posicion), equipo_id(equipo_id) {}

    nlohmann::json to_json() const
    {
        return {{"id", id}, {"nombre", nombre}, {"edad", edad}, {"altura", altura}, {"peso", peso}, {"nacionalidad", nacionalidad}, {"fecha_ingreso", fecha_ingreso}, {"posicion", posicion}, {"equipo_id", equipo_id}};
    }

    static Jugador from_json(const nlohmann::json &j)
    {
        return Jugador(j.at("id"), j.at("nombre"), j.at("edad"), j.at("altura"), j.at("peso"), j.at("nacionalidad"), j.at("fecha_ingreso"), j.at("posicion"), j.at("equipo_id"));
    }
};
