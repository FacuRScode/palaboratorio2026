//
// Created by facun on 2/6/2026.
//

#include "MenuEmpleado.h"
#include <iostream>
#include <limits>

using namespace std;

MenuEmpleado::MenuEmpleado(EmpleadoController& controller) : ctrl(controller) {}

void MenuEmpleado::mostrar() {
	while (true) {
		cout << "\n--- Menu Empleado ---\n";
		cout << "1. Registrar cliente\n";
		cout << "2. Buscar cliente\n";
		cout << "3. Listar clientes\n";
		cout << "4. Agregar orden de compra\n";
		cout << "5. Eliminar orden de compra\n";
		cout << "0. Volver\n";
		int op;
		cout << "Seleccione una opcion: ";
		cin >> op;
		if (op == 0) return;
		if (op == 1) {
			string rut, nombre, apellido, direccion, correo;
			cout << "RUT: "; cin >> rut;
			cout << "Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre);
			cout << "Apellido: "; getline(cin, apellido);
			cout << "Direccion: "; getline(cin, direccion);
			cout << "Correo: "; getline(cin, correo);
			auto c = ctrl.registrarCliente(rut, nombre, apellido, direccion, correo);
			cout << (c ? "Cliente registrado." : "Error: cliente ya existe.") << endl;
		} else if (op == 2) {
			string rut;
			cout << "RUT: "; cin >> rut;
			auto c = ctrl.buscarCliente(rut);
			if (c) cout << "Encontrado: " << c->getNombre() << " " << c->getApellido() << " - " << c->getRut() << '\n';
			else cout << "Cliente no encontrado." << endl;
		} else if (op == 3) {
			auto lista = ctrl.listarClientes();
			cout << "Clientes:\n";
			for (auto c : lista) {
				if (c) cout << "- " << c->getRut() << " - " << c->getNombre() << " " << c->getApellido() << '\n';
			}
		} else if (op == 4) {
			int diaE, mesE, anioE, diaR, mesR, anioR;
			cout << "Fecha emision (dia mes anio): "; cin >> diaE >> mesE >> anioE;
			cout << "Fecha recepcion (dia mes anio): "; cin >> diaR >> mesR >> anioR;
			cout << "Estado (0=Pendiente, 1=Recibida, 2=Cancelada): ";
			int est;
			cin >> est;
			Estado estado = Estado::Pendiente;
			if (est == 1) estado = Estado::Recibida;
			else if (est == 2) estado = Estado::Cancelada;
			DTFecha fE(diaE, mesE, anioE);
			DTFecha fR(diaR, mesR, anioR);
			OrdenDeCompra* orden = new OrdenDeCompra(fE, estado, fR, nullptr);
			ctrl.agregarOrdenDeCompra(orden);
			cout << "Orden de compra agregada." << endl;
		} else if (op == 5) {
			auto lista = ctrl.listarOrdenesDeCompra();
			if (lista.empty()) {
				cout << "No hay ordenes de compra." << endl;
				continue;
			}
			cout << "Ordenes de compra:\n";
			for (size_t i = 0; i < lista.size(); ++i) {
				auto o = lista[i];
				cout << i << ": Emision=" << o->getFechaEmision().getDia() << "/" << o->getFechaEmision().getMes() << "/" << o->getFechaEmision().getAnio();
				cout << " Recepcion=" << o->getFechaRecepcion().getDia() << "/" << o->getFechaRecepcion().getMes() << "/" << o->getFechaRecepcion().getAnio();
				cout << " Estado=";
				if (o->getEstado() == Estado::Pendiente) cout << "Pendiente";
				else if (o->getEstado() == Estado::Recibida) cout << "Recibida";
				else cout << "Cancelada";
				cout << "\n";
			}
			int idx;
			cout << "Seleccione indice a eliminar: "; cin >> idx;
			if (idx < 0 || idx >= (int)lista.size()) {
				cout << "Indice invalido." << endl;
				continue;
			}
			ctrl.eliminarOrdenDeCompra(lista[idx]);
			cout << "Orden de compra eliminada." << endl;
		} else {
			cout << "Opcion invalida." << endl;
		}
	}
}
