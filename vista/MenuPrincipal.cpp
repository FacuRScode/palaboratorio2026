#include "MenuPrincipal.h"
#include <iostream>
#include <limits>
#include <exception>

using namespace std;

MenuPrincipal::MenuPrincipal(AdminController& admin, EmpleadoController& empleado, VentaController& ventas)
	: adminCtrl(admin), empleadoCtrl(empleado), ventaCtrl(ventas),
	  authCtrl(&admin, &empleado),
	  menuAdmin(admin, &authCtrl, &empleado, &ventaCtrl),
	  menuEmpleado(empleado, &authCtrl, &ventaCtrl),
	  menuCliente(ventas, &authCtrl, &empleado) {}

void MenuPrincipal::mostrar() {
	while (true) {
		try {
			cout << "\n=== Bienvenido al Sistema ===\n";
			if (!authCtrl.haySesionActiva()) {
				mostrarLogin();
			} else {
				mostrarMenuSegunRol();
			}
		} catch (const exception& ex) {
			cout << "Error inesperado: " << ex.what() << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} catch (...) {
			cout << "Error inesperado en la operacion." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

void MenuPrincipal::mostrarLogin() {
	while (true) {
		try {
			cout << "\n--- Iniciar Sesion ---\n";
			string correo, contrasena;
			cout << "Correo electronico: ";
			cin >> correo;
			cout << "Contrasena: ";
			cin >> contrasena;

			if (authCtrl.iniciarSesion(correo, contrasena)) {
				Sesion sesion = authCtrl.getSesionActual();
				cout << "\nInicio de sesion exitoso. Bienvenido/a " << sesion.nombre << " (" << sesion.rol << ")" << endl;
				return; // sale del login, va al menu de rol
			} else {
				cout << "\nError: Correo o contrasena incorrectos." << endl;
				cout << "1. Reintentar\n";
				cout << "2. Cancelar\n";
				int op;
				cout << "Seleccione una opcion: ";
				if (!(cin >> op)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Entrada invalida. Intente nuevamente." << endl;
					continue;
				}
				if (op == 2) {
					cout << "Operacion cancelada." << endl;
					return; // vuelve al menu principal sin salir del sistema
				}
				// Si op == 1, reintenta
			}
		} catch (const exception& ex) {
			cout << "Error inesperado: " << ex.what() << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} catch (...) {
			cout << "Error inesperado en la operacion." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

void MenuPrincipal::mostrarMenuSegunRol() {
	Sesion sesion = authCtrl.getSesionActual();
	if (sesion.rol == "Administrador") {
		menuAdmin.mostrar();
	} else if (sesion.rol == "Empleado") {
		menuEmpleado.mostrar();
	} else if (sesion.rol == "Cliente") {
		menuCliente.mostrar();
	}
}
