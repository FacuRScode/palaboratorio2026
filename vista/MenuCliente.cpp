
//
// Created by facun on 2/6/2026.
//

#include "MenuCliente.h"
#include <iostream>
#include <limits>

using namespace std;

MenuCliente::MenuCliente(VentaController& controller) : ctrl(controller) {}

void MenuCliente::mostrar() {
	while (true) {
		cout << "\n--- Menu Cliente ---\n";
		cout << "1. Registrar cliente\n";
		cout << "2. Crear venta\n";
		cout << "3. Agregar linea a venta\n";
		cout << "4. Listar ventas de un cliente\n";
		cout << "5. Calificar producto\n";
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
			cout << "RUT cliente: "; cin >> rut;
			int dia, mes, anio, hora, minuto, segundo;
			cout << "Fecha (dia mes anio): "; cin >> dia >> mes >> anio;
			cout << "Hora (hora minuto segundo): "; cin >> hora >> minuto >> segundo;
			DTFecha fecha(dia, mes, anio);
			DTHora dh(hora, minuto, segundo);
			Venta* v = ctrl.crearVenta(rut, fecha, dh);
			cout << (v ? "Venta creada." : "Error creando venta (cliente no existe).") << endl;
		} else if (op == 3) {
			auto ventas = ctrl.listarVentas();
			if (ventas.empty()) { cout << "No hay ventas creadas." << endl; continue; }
			cout << "Ventas disponibles:\n";
			for (size_t i = 0; i < ventas.size(); ++i) {
				auto v = ventas[i];
				cout << i << ": fecha=" << v->getFecha().getDia() << "/" << v->getFecha().getMes() << "/" << v->getFecha().getAnio();
				cout << " hora=" << v->getHora().getHora() << ":" << v->getHora().getMinuto() << ":" << v->getHora().getSegundo() << "\n";
			}
			int idx;
			cout << "Seleccione indice de venta: "; cin >> idx;
			if (idx < 0 || idx >= (int)ventas.size()) { cout << "Indice invalido." << endl; continue; }
			int codigo, cantidad;
			cout << "Codigo producto: "; cin >> codigo;
			cout << "Cantidad: "; cin >> cantidad;
			bool ok = ctrl.agregarLineaAVenta(ventas[idx], codigo, cantidad);
			cout << (ok ? "Linea agregada." : "No se pudo agregar linea (producto no existe/stock insuficiente/otro error).") << endl;
		} else if (op == 4) {
			string rut;
			cout << "RUT cliente: "; cin >> rut;
			auto vlist = ctrl.listarVentasPorCliente(rut);
			cout << "Ventas de " << rut << ":\n";
			for (auto v : vlist) {
				cout << "- Fecha: " << v->getFecha().getDia() << "/" << v->getFecha().getMes() << "/" << v->getFecha().getAnio();
				cout << " Hora: " << v->getHora().getHora() << ":" << v->getHora().getMinuto() << ":" << v->getHora().getSegundo();
				cout << " Total: " << v->calcularTotal() << "\n";
			}
		} else if (op == 5) {
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

