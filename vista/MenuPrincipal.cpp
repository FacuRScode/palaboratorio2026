#include "MenuPrincipal.h"
#include <iostream>
#include <limits>

using namespace std;

MenuPrincipal::MenuPrincipal(AdminController& admin, EmpleadoController& empleado, VentaController& ventas)
	: adminCtrl(admin), empleadoCtrl(empleado), ventaCtrl(ventas),
	  authCtrl(&admin, &empleado),
	  menuAdmin(admin, &authCtrl, &ventas, &empleado),
	  menuEmpleado(empleado, &authCtrl),
	  menuCliente(ventas, &authCtrl) {}

void MenuPrincipal::mostrar() {
	while (true) {
		cout << "\n=== Bienvenido al Sistema ===\n";
		if (!authCtrl.haySesionActiva()) {
			mostrarLogin();
		} else {
			mostrarMenuSegunRol();
		}
	}
}

void MenuPrincipal::mostrarLogin() {
	while (true) {
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
			cout << "2. Cancelar (salir)\n";
			int op;
			cout << "Seleccione una opcion: ";
			cin >> op;
			if (op == 2) {
				cout << "Operacion cancelada. Saliendo del sistema..." << endl;
				exit(0);
			}
			// Si op == 1, reintenta
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
