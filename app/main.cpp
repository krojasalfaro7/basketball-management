#include <iostream>
#include <vector>
#include <algorithm>
#include "models/equipo.hpp"
#include "repository/repository.cpp"

void mostrar_menu()
{
    std::cout << "\n--- Gestión de Equipos de Baloncesto ---\n";
    std::cout << "1. Agregar equipo\n2. Editar equipo\n3. Eliminar equipo\n4. Consultar todos\n5. Consultar uno\n6. Salir\nOpción: ";
}

Equipo pedir_equipo(const std::string &id = "")
{
    std::string nombre, eid = id;
    if (eid.empty())
    {
        std::cout << "ID: ";
        std::cin.ignore(); // Limpiar buffer
        std::getline(std::cin, eid);
    }
    else
    {
        std::cin.ignore(); // Limpiar buffer si viene de edición
    }
    std::cout << "Nombre: ";
    std::getline(std::cin, nombre);
    return Equipo(eid, nombre);
}

int main()
{
    Repository repo("equipos.json");
    int opcion;
    do
    {
        mostrar_menu();
        std::cin >> opcion;
        if (opcion == 1) // Agregar
        {
            Equipo eq = pedir_equipo();
            // Validar que el id no exista
            auto equipos = repo.read_all();
            bool existe = std::any_of(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                                      { return e["id"] == eq.id; });
            if (existe)
                std::cout << "ID ya existe.\n";
            else
            {
                repo.create(eq.to_json());
                std::cout << "Equipo agregado.\n";
            }
        }
        else if (opcion == 2) // Editar
        {
            std::string id;
            std::cout << "ID a editar: ";
            std::cin >> id;
            auto equipos = repo.read_all();
            auto it = std::find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                                   { return e["id"] == id; });
            if (it == equipos.end())
                std::cout << "No existe ese equipo.\n";
            else
            {
                Equipo eq = pedir_equipo(id);
                repo.update(id, eq.to_json());
                std::cout << "Equipo actualizado.\n";
            }
        }
        else if (opcion == 3) // Eliminar
        {
            std::string id;
            std::cout << "ID a eliminar: ";
            std::cin >> id;
            if (repo.remove(id))
                std::cout << "Equipo eliminado.\n";
            else
                std::cout << "No existe ese equipo.\n";
        }
        else if (opcion == 4) // Consultar todos
        {
            auto equipos = repo.read_all();
            std::cout << "\nEquipos registrados:\n";
            for (const auto &e : equipos)
            {
                Equipo eq = Equipo::from_json(e);
                std::cout << "ID: " << eq.id << ", Nombre: " << eq.nombre << "\n";
            }
        }
        else if (opcion == 5) // Consultar uno
        {
            std::string id;
            std::cout << "ID a consultar: ";
            std::cin >> id;
            auto equipos = repo.read_all();
            auto it = std::find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                                   { return e["id"] == id; });
            if (it == equipos.end())
                std::cout << "No existe ese equipo.\n";
            else
            {
                Equipo eq = Equipo::from_json(*it);
                std::cout << "ID: " << eq.id << ", Nombre: " << eq.nombre << "\n";
            }
        }
    } while (opcion != 6);
    return 0;
}
