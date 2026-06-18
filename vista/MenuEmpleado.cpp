#include "MenuEmpleado.h"
#include <iostream>
#include <limits>

using namespace std;

MenuEmpleado::MenuEmpleado(EmpleadoController& controller, AuthController* auth) : ctrl(controller), authCtrl(auth) {}

void MenuEmpleado::mostrar() {
	while (true) {
		cout << "\n--- Menu Empleado ---\n";
		if (authCtrl && authCtrl->haySesionActiva()) {
			Sesion s = authCtrl->getSesionActual();
			cout << "Usuario: " << s.nombre << " (" << s.rol << ")\n";
		}
		cout << "1. Registrar cliente\n";
		cout << "2. Buscar cliente\n";
		cout << "3. Listar clientes\n";
		cout << "4. Agregar orden de compra\n";
		cout << "5. Eliminar orden de compra\n";
		cout << "6. Crear venta\n";
		cout << "7. Agregar linea a venta\n";
		cout << "8. Listar ventas\n";
		cout << "9. Listar ventas por cliente\n";
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
			string rut, nombre, apellido, direccion, correo, contrasena;
			cout << "RUT: "; cin >> rut;
			cout << "Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre);
			cout << "Apellido: "; getline(cin, apellido);
			cout << "Direccion: "; getline(cin, direccion);
			cout << "Correo: "; getline(cin, correo);
			cout << "Contrasena: "; getline(cin, contrasena);
			auto c = ctrl.registrarCliente(rut, nombre, apellido, direccion, correo, contrasena);
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
		} else if (op == 6) {
			string rut;
			cout << "RUT cliente: "; cin >> rut;
			int dia, mes, anio, hora, minuto, segundo;
			cout << "Fecha (dia mes anio): "; cin >> dia >> mes >> anio;
			cout << "Hora (hora minuto segundo): "; cin >> hora >> minuto >> segundo;
			DTFecha fecha(dia, mes, anio);
			DTHora dh(hora, minuto, segundo);
			Venta* v = ctrl.crearVenta(rut, fecha, dh);
			cout << (v ? "Venta creada." : "Error creando venta (cliente no existe).") << endl;
		} else if (op == 7) {
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
		} else if (op == 8) {
			auto vlist = ctrl.listarVentas();
			cout << "Ventas:\n";
			for (auto v : vlist) {
				cout << "- Fecha: " << v->getFecha().getDia() << "/" << v->getFecha().getMes() << "/" << v->getFecha().getAnio();
				cout << " Hora: " << v->getHora().getHora() << ":" << v->getHora().getMinuto() << ":" << v->getHora().getSegundo();
				cout << " Total: " << v->calcularTotal() << "\n";
			}
		} else if (op == 9) {
			string rut;
			cout << "RUT cliente: "; cin >> rut;
			auto vlist = ctrl.listarVentasPorCliente(rut);
			cout << "Ventas de " << rut << ":\n";
			for (auto v : vlist) {
				cout << "- Fecha: " << v->getFecha().getDia() << "/" << v->getFecha().getMes() << "/" << v->getFecha().getAnio();
				cout << " Hora: " << v->getHora().getHora() << ":" << v->getHora().getMinuto() << ":" << v->getHora().getSegundo();
				cout << " Total: " << v->calcularTotal() << "\n";
			}
		} else {
			cout << "Opcion invalida." << endl;
		}
	}
}
