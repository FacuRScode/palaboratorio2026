//
// Created by facun on 2/6/2026.
//

#include "MenuPrincipal.h"
#include <iostream>

using namespace std;

MenuPrincipal::MenuPrincipal(AdminController& admin, EmpleadoController& empleado, VentaController& ventas)
	: menuAdmin(admin), menuEmpleado(empleado), menuCliente(ventas) {}

void MenuPrincipal::mostrar() {
	while (true) {
		cout << "\n--- Sistema ---\n";
		cout << "1. Administrador\n";
		cout << "2. Empleado\n";
		cout << "3. Cliente\n";
		cout << "0. Salir\n";
		int op;
		cout << "Seleccione una opcion: ";
		cin >> op;
		if (op == 0) break;
		if (op == 1) menuAdmin.mostrar();
		else if (op == 2) menuEmpleado.mostrar();
		else if (op == 3) menuCliente.mostrar();
		else cout << "Opcion invalida." << endl;
	}
}
