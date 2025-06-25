#include "repository/repository.cpp"
#include <iostream>

int main()
{
    Repository repo("teams.json");
    // Crear un equipo
    json equipo = {
        {"id", "1"},
        {"nombre", "Lakers"},
        {"ciudad", "Los Angeles"}};
    repo.create(equipo);

    // Leer todos los equipos
    auto equipos = repo.read_all();
    std::cout << "Equipos registrados:" << std::endl;
    for (const auto &e : equipos)
    {
        std::cout << e.dump(4) << std::endl;
    }

    // Actualizar un equipo
    json equipo_actualizado = {
        {"id", "1"},
        {"nombre", "Lakers"},
        {"ciudad", "LA"}};
    repo.update("1", equipo_actualizado);

    // Eliminar un equipo
    // repo.remove("1");
    return 0;
}
