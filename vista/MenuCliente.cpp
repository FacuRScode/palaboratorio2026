#include "MenuCliente.h"
#include <iostream>
#include <limits>

using namespace std;

MenuCliente::MenuCliente(VentaController& controller, AuthController* auth) : ctrl(controller), authCtrl(auth) {}

void MenuCliente::mostrar() {
	while (true) {
		cout << "\n--- Menu Cliente ---\n";
		if (authCtrl && authCtrl->haySesionActiva()) {
			Sesion s = authCtrl->getSesionActual();
			cout << "Usuario: " << s.nombre << " (" << s.rol << ")\n";
		}
		cout << "1. Calificar producto\n";
		cout << "0. Cerrar sesion\n";
		int op;
		cout << "Seleccione una opcion: ";
		cin >> op;
		if (op == 0) {
			if (authCtrl) authCtrl->cerrarSesion();
			cout << "Sesion cerrada." << endl;
			return;
		}
		if (op == 1) {
			int codigo;
			int punt;
			string comentario;
			int dia, mes, anio;
			cout << "Codigo producto: "; cin >> codigo;
			cout << "Puntaje (1-5): "; cin >> punt;
			cout << "Comentario: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, comentario);
			cout << "Fecha calificacion (dia mes anio): "; cin >> dia >> mes >> anio;
			Puntaje p = Puntaje::UNO;
			if (punt == 1) p = Puntaje::UNO; else if (punt == 2) p = Puntaje::DOS; else if (punt == 3) p = Puntaje::TRES; else if (punt == 4) p = Puntaje::CUATRO; else if (punt == 5) p = Puntaje::CINCO;
			DTFecha f(dia, mes, anio);
			bool ok = ctrl.calificarProducto(codigo, p, comentario, f);
			cout << (ok ? "Producto calificado." : "No se pudo calificar (producto no existe).") << endl;
		} else {
			cout << "Opcion invalida." << endl;
		}
	}
}
