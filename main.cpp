#include <iostream>
#include "logica/controladores/AdminController.h"
#include "logica/controladores/VentaController.h"
#include "vista/MenuAdministrador.h"
#include "vista/MenuCliente.h"

int main() {
    AdminController admin;
    VentaController ventas(&admin);

    MenuAdministrador menuAdmin(admin);
    MenuCliente menuCliente(ventas);

    while (true) {
        std::cout << "\n--- Sistema ---\n";
        std::cout << "1. Administrador\n";
        std::cout << "2. Cliente\n";
        std::cout << "0. Salir\n";
        int op;
        std::cout << "Seleccione una opcion: "; std::cin >> op;
        if (op == 0) break;
        if (op == 1) menuAdmin.mostrar();
        else if (op == 2) menuCliente.mostrar();
        else std::cout << "Opcion invalida." << std::endl;
    }

    return 0;
}