#include <iostream>
#include <vector>
#include <algorithm>
#include "models/equipo.hpp"
#include "models/jugador.hpp"
#include "repository/repository.cpp"

void mostrar_menu_equipos()
{
    std::cout << "\n--- Gestión de Equipos de Baloncesto ---\n";
    std::cout << "1. Agregar equipo\n2. Editar equipo\n3. Eliminar equipo\n4. Consultar todos\n5. Consultar uno\n6. Salir\nOpción: ";
}

void mostrar_menu_jugadores()
{
    std::cout << "\n--- Gestión de Jugadores ---\n";
    std::cout << "1. Agregar jugador\n2. Editar jugador\n3. Eliminar jugador\n4. Consultar todos\n5. Consultar por equipo\n6. Volver\nOpción: ";
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

Jugador pedir_jugador(const std::vector<nlohmann::json> &equipos, const std::string &id = "")
{
    std::string nombre, posicion, equipo_id, jid = id;
    if (jid.empty())
    {
        std::cout << "ID: ";
        std::cin.ignore();
        std::getline(std::cin, jid);
    }
    else
    {
        std::cin.ignore();
    }
    std::cout << "Nombre: ";
    std::getline(std::cin, nombre);
    std::cout << "Posición: ";
    std::getline(std::cin, posicion);
    std::cout << "ID del equipo: ";
    std::getline(std::cin, equipo_id);
    // Validar que el equipo exista
    auto it = std::find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                           { return e["id"] == equipo_id; });
    if (it == equipos.end())
    {
        std::cout << "Equipo no existe.\n";
        return Jugador();
    }
    return Jugador(jid, nombre, posicion, equipo_id);
}

void menu_jugadores(Repository &repo_equipos, Repository &repo_jugadores)
{
    int op;
    do
    {
        mostrar_menu_jugadores();
        std::cin >> op;
        auto equipos = repo_equipos.read_all();
        if (op == 1)
        { // Agregar
            Jugador j = pedir_jugador(equipos);
            if (j.id.empty())
                continue;
            auto jugadores = repo_jugadores.read_all();
            bool existe = std::any_of(jugadores.begin(), jugadores.end(), [&](const nlohmann::json &e)
                                      { return e["id"] == j.id; });
            if (existe)
                std::cout << "ID ya existe.\n";
            else
            {
                repo_jugadores.create(j.to_json());
                std::cout << "Jugador agregado.\n";
            }
        }
        else if (op == 2)
        { // Editar
            std::string id;
            std::cout << "ID a editar: ";
            std::cin >> id;
            auto jugadores = repo_jugadores.read_all();
            auto it = std::find_if(jugadores.begin(), jugadores.end(), [&](const nlohmann::json &e)
                                   { return e["id"] == id; });
            if (it == jugadores.end())
                std::cout << "No existe ese jugador.\n";
            else
            {
                Jugador j = pedir_jugador(equipos, id);
                if (j.id.empty())
                    continue;
                repo_jugadores.update(id, j.to_json());
                std::cout << "Jugador actualizado.\n";
            }
        }
        else if (op == 3)
        { // Eliminar
            std::string id;
            std::cout << "ID a eliminar: ";
            std::cin >> id;
            if (repo_jugadores.remove(id))
                std::cout << "Jugador eliminado.\n";
            else
                std::cout << "No existe ese jugador.\n";
        }
        else if (op == 4)
        { // Consultar todos
            auto jugadores = repo_jugadores.read_all();
            std::cout << "\nJugadores registrados:\n";
            for (const auto &e : jugadores)
            {
                Jugador j = Jugador::from_json(e);
                std::cout << "ID: " << j.id << ", Nombre: " << j.nombre << ", Posición: " << j.posicion << ", Equipo: " << j.equipo_id << "\n";
            }
        }
        else if (op == 5)
        { // Consultar por equipo
            std::string equipo_id;
            std::cout << "ID del equipo: ";
            std::cin >> equipo_id;
            auto jugadores = repo_jugadores.read_all();
            std::cout << "\nJugadores del equipo " << equipo_id << ":\n";
            for (const auto &e : jugadores)
            {
                if (e["equipo_id"] == equipo_id)
                {
                    Jugador j = Jugador::from_json(e);
                    std::cout << "ID: " << j.id << ", Nombre: " << j.nombre << ", Posición: " << j.posicion << "\n";
                }
            }
        }
    } while (op != 6);
}

int main()
{
    Repository repo_equipos("equipos.json");
    Repository repo_jugadores("jugadores.json");
    int opcion;
    do
    {
        mostrar_menu_equipos();
        std::cin >> opcion;
        if (opcion == 1) // Agregar
        {
            Equipo eq = pedir_equipo();
            // Validar que el id no exista
            auto equipos = repo_equipos.read_all();
            bool existe = std::any_of(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                                      { return e["id"] == eq.id; });
            if (existe)
                std::cout << "ID ya existe.\n";
            else
            {
                repo_equipos.create(eq.to_json());
                std::cout << "Equipo agregado.\n";
            }
        }
        else if (opcion == 2) // Editar
        {
            std::string id;
            std::cout << "ID a editar: ";
            std::cin >> id;
            auto equipos = repo_equipos.read_all();
            auto it = std::find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                                   { return e["id"] == id; });
            if (it == equipos.end())
                std::cout << "No existe ese equipo.\n";
            else
            {
                Equipo eq = pedir_equipo(id);
                repo_equipos.update(id, eq.to_json());
                std::cout << "Equipo actualizado.\n";
            }
        }
        else if (opcion == 3) // Eliminar
        {
            std::string id;
            std::cout << "ID a eliminar: ";
            std::cin >> id;
            if (repo_equipos.remove(id))
                std::cout << "Equipo eliminado.\n";
            else
                std::cout << "No existe ese equipo.\n";
        }
        else if (opcion == 4) // Consultar todos
        {
            auto equipos = repo_equipos.read_all();
            auto jugadores = repo_jugadores.read_all();
            std::cout << "\nEquipos registrados:\n";
            for (const auto &e : equipos)
            {
                Equipo eq = Equipo::from_json(e);
                std::cout << "ID: " << eq.id << ", Nombre: " << eq.nombre << "\n";
                // Mostrar jugadores del equipo
                bool hay_jugadores = false;
                for (const auto &j : jugadores)
                {
                    if (j["equipo_id"] == eq.id)
                    {
                        Jugador jug = Jugador::from_json(j);
                        std::cout << "  ID: " << jug.id << ", Nombre: " << jug.nombre << ", Posición: " << jug.posicion << std::endl;
                        hay_jugadores = true;
                    }
                }
                if (!hay_jugadores)
                    std::cout << "  (Sin jugadores registrados)" << std::endl;
            }
        }
        else if (opcion == 5) // Consultar uno
        {
            std::string id;
            std::cout << "ID a consultar: ";
            std::cin >> id;
            auto equipos = repo_equipos.read_all();
            auto it = std::find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                                   { return e["id"] == id; });
            if (it == equipos.end())
                std::cout << "No existe ese equipo.\n";
            else
            {
                Equipo eq = Equipo::from_json(*it);
                std::cout << "ID: " << eq.id << ", Nombre: " << eq.nombre << "\n";
                // Mostrar jugadores del equipo
                auto jugadores = repo_jugadores.read_all();
                std::cout << "Jugadores del equipo:" << std::endl;
                bool hay_jugadores = false;
                for (const auto &e : jugadores)
                {
                    if (e["equipo_id"] == eq.id)
                    {
                        Jugador j = Jugador::from_json(e);
                        std::cout << "  ID: " << j.id << ", Nombre: " << j.nombre << ", Posición: " << j.posicion << std::endl;
                        hay_jugadores = true;
                    }
                }
                if (!hay_jugadores)
                    std::cout << "  (Sin jugadores registrados)" << std::endl;
            }
        }
    } while (opcion != 6);

    menu_jugadores(repo_equipos, repo_jugadores);

    return 0;
}
