#include <iostream>
#include <vector>
#include <algorithm>
#include "models/equipo.hpp"
#include "models/jugador.hpp"
#include "repository.cpp"
using namespace std;

void mostrar_menu_equipos()
{
    cout << "\n---------- Menu de Equipos de Baloncesto ----------\n";
    cout << "1. Agregar equipo\n2. Editar equipo\n3. Eliminar equipo\n4. Consultar todos\n5. Consultar uno\n6. Menu de jugadores\n7. Salir\nOpcion: ";
}

void mostrar_menu_jugadores()
{
    cout << "\n---------- Menu de Jugadores ----------\n";
    cout << "1. Agregar jugador\n2. Editar jugador\n3. Eliminar jugador\n4. Consultar todos\n5. Consultar por equipo\n6. Volver a menu principal\nOpcion: ";
}

Equipo pedir_equipo(const string &id = "")
{
    string nombre, eid = id;
    if (eid.empty())
    {
        cout << "ID: ";
        cin.ignore(); // Limpiar buffer
        getline(cin, eid);
    }
    else
    {
        cin.ignore(); // Limpiar buffer si viene de edición
    }
    cout << "Nombre: ";
    getline(cin, nombre);
    return Equipo(eid, nombre);
}

Jugador pedir_jugador(const vector<nlohmann::json> &equipos, const string &id = "")
{
    string nombre, posicion, nacionalidad, edad, altura, equipo_id, jid = id;
    if (jid.empty())
    {
        cout << "ID: ";
        cin.ignore();
        getline(cin, jid);
    }
    else
    {
        cin.ignore();
    }
    cout << "Nombre: ";
    getline(cin, nombre);
    cout << "Posicion: ";
    getline(cin, posicion);
    cout << "Nacionalidad: ";
    getline(cin, nacionalidad);
    cout << "Edad: ";
    getline(cin, edad);
    cout << "Altura: ";
    getline(cin, altura);
    cout << "ID del equipo: ";
    getline(cin, equipo_id);
    // Validar que el equipo exista
    auto it = find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                      { return e["id"] == equipo_id; });
    if (it == equipos.end())
    {
        cout << "Equipo no existe.\n";
        return Jugador();
    }
    return Jugador(jid, nombre, posicion, nacionalidad, edad, altura, equipo_id);
}

void menu_jugadores(Repository &repo_equipos, Repository &repo_jugadores)
{
    int opcion;
    do
    {
        mostrar_menu_jugadores();
        cin >> opcion;
        auto equipos = repo_equipos.read_all();
        if (opcion == 1)
        { // Agregar
            Jugador jugador = pedir_jugador(equipos);
            if (jugador.id.empty())
                continue;
            auto jugadores = repo_jugadores.read_all();
            bool existe = any_of(jugadores.begin(), jugadores.end(), [&](const nlohmann::json &e)
                                 { return e["id"] == jugador.id; });
            if (existe)
                cout << "ID ya existe.\n";
            else
            {
                repo_jugadores.create(jugador.to_json());
                cout << "Jugador agregado.\n";
            }
        }
        else if (opcion == 2)
        { // Editar
            string id;
            cout << "ID a editar: ";
            cin >> id;
            auto jugadores = repo_jugadores.read_all();
            auto it = find_if(jugadores.begin(), jugadores.end(), [&](const nlohmann::json &e)
                              { return e["id"] == id; });
            if (it == jugadores.end())
                cout << "No existe ese jugador.\n";
            else
            {
                Jugador jugador = pedir_jugador(equipos, id);
                if (jugador.id.empty())
                    continue;
                repo_jugadores.update(id, jugador.to_json());
                cout << "Jugador actualizado.\n";
            }
        }
        else if (opcion == 3)
        { // Eliminar
            string id;
            cout << "ID a eliminar: ";
            cin >> id;
            if (repo_jugadores.remove(id))
                cout << "Jugador eliminado.\n";
            else
                cout << "No existe ese jugador.\n";
        }
        else if (opcion == 4)
        { // Consultar todos
            auto jugadores = repo_jugadores.read_all();
            cout << "\nJugadores registrados:\n";
            for (const auto &e : jugadores)
            {
                Jugador jugador = Jugador::from_json(e);
                cout << "ID: " << jugador.id << ", Nombre: " << jugador.nombre << ", Posicion: " << jugador.posicion << ", Nacionalidad: " << jugador.nacionalidad << ", Edad: " << jugador.edad << ", Altura: " << jugador.altura << ", Equipo: " << jugador.equipo_id << "\n";
            }
        }
        else if (opcion == 5)
        { // Consultar por equipo
            string equipo_id;
            cout << "ID del equipo: ";
            cin >> equipo_id;
            auto jugadores = repo_jugadores.read_all();
            cout << "\nJugadores del equipo " << equipo_id << ":\n";
            for (const auto &e : jugadores)
            {
                if (e["equipo_id"] == equipo_id)
                {
                    Jugador jugador = Jugador::from_json(e);
                    cout << "ID: " << jugador.id << ", Nombre: " << jugador.nombre << ", Posicion: " << jugador.posicion << ", Nacionalidad: " << jugador.nacionalidad << ", Edad: " << jugador.edad << ", Altura: " << jugador.altura << "\n";
                }
            }
        }
    } while (opcion != 6);
}

int main()
{
    Repository repo_equipos("equipos.json");
    Repository repo_jugadores("jugadores.json");
    int opcion;
    do
    {
        mostrar_menu_equipos();
        cin >> opcion;
        if (opcion == 1) // Agregar
        {
            Equipo eq = pedir_equipo();
            // Validar que el id no exista
            auto equipos = repo_equipos.read_all();
            bool existe = any_of(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                                 { return e["id"] == eq.id; });
            if (existe)
                cout << "ID ya existe.\n";
            else
            {
                repo_equipos.create(eq.to_json());
                cout << "Equipo agregado.\n";
            }
        }
        else if (opcion == 2) // Editar
        {
            string id;
            cout << "ID a editar: ";
            cin >> id;
            auto equipos = repo_equipos.read_all();
            auto it = find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                              { return e["id"] == id; });
            if (it == equipos.end())
                cout << "No existe ese equipo.\n";
            else
            {
                Equipo eq = pedir_equipo(id);
                repo_equipos.update(id, eq.to_json());
                cout << "Equipo actualizado.\n";
            }
        }
        else if (opcion == 3) // Eliminar
        {
            string id;
            cout << "ID a eliminar: ";
            cin >> id;
            if (repo_equipos.remove(id))
            {
                cout << "Equipo eliminado.\n";

                // Eliminar jugadores del equipo
                auto jugadores_del_equipo = repo_jugadores.read_all();
                for (const auto &jugador : jugadores_del_equipo)
                {
                    if (jugador["equipo_id"] == id)
                    {
                        repo_jugadores.remove(jugador["id"]);
                    }
                }
            }
            else
                cout << "No existe ese equipo.\n";
        }
        else if (opcion == 4) // Consultar todos
        {
            auto equipos = repo_equipos.read_all();
            auto jugadores = repo_jugadores.read_all();
            cout << "\nEquipos registrados:\n";
            for (const auto &e : equipos)
            {
                Equipo eq = Equipo::from_json(e);
                cout << "ID: " << eq.id << ", Nombre: " << eq.nombre << "\n";
                // Mostrar jugadores del equipo
                bool hay_jugadores = false;
                for (const auto &jugador : jugadores)
                {
                    if (jugador["equipo_id"] == eq.id)
                    {
                        Jugador jug = Jugador::from_json(jugador);
                        cout << "  ID: " << jug.id << ", Nombre: " << jug.nombre << ", Posicion: " << jug.posicion << ", Nacionalidad: " << jug.nacionalidad << ", Edad: " << jug.edad << ", Altura: " << jug.altura << endl;
                        hay_jugadores = true;
                    }
                }
                if (!hay_jugadores)
                    cout << "  (Sin jugadores registrados)" << endl;
            }
        }
        else if (opcion == 5) // Consultar uno
        {
            string id;
            cout << "ID a consultar: ";
            cin >> id;
            auto equipos = repo_equipos.read_all();
            auto it = find_if(equipos.begin(), equipos.end(), [&](const nlohmann::json &e)
                              { return e["id"] == id; });
            if (it == equipos.end())
                cout << "No existe ese equipo.\n";
            else
            {
                Equipo eq = Equipo::from_json(*it);
                cout << "ID: " << eq.id << ", Nombre: " << eq.nombre << "\n";
                // Mostrar jugadores del equipo
                auto jugadores = repo_jugadores.read_all();
                cout << "Jugadores del equipo:" << endl;
                bool hay_jugadores = false;
                for (const auto &e : jugadores)
                {
                    if (e["equipo_id"] == eq.id)
                    {
                        Jugador jugador = Jugador::from_json(e);
                        cout << "  ID: " << jugador.id << ", Nombre: " << jugador.nombre << ", Posicion: " << jugador.posicion << ", Nacionalidad: " << jugador.nacionalidad << ", Edad: " << jugador.edad << ", Altura: " << jugador.altura << endl;
                        hay_jugadores = true;
                    }
                }
                if (!hay_jugadores)
                    cout << "  (Sin jugadores registrados)" << endl;
            }
        }

        else if (opcion == 6) // Menu de jugadores
        {
            menu_jugadores(repo_equipos, repo_jugadores);
        }
    } while (opcion != 7);

    return 0;
}