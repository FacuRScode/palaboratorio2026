#include "MenuCliente.h"
#include <iostream>
#include <limits>
#include <exception>

using namespace std;

MenuCliente::MenuCliente(VentaController& controller, AuthController* auth, EmpleadoController* empleado)
	: ctrl(controller), authCtrl(auth), empleadoCtrl(empleado) {}

void MenuCliente::mostrar() {
	while (true) {
		try {
			cout << "\n--- Menu Cliente ---\n";
			if (authCtrl && authCtrl->haySesionActiva()) {
				Sesion s = authCtrl->getSesionActual();
				cout << "Usuario: " << s.nombre << " (" << s.rol << ")\n";
			}
			cout << "1. Calificar producto\n";
			cout << "2. Consultar informacion detallada de un producto\n";
			cout << "0. Cerrar sesion\n";
			int op;
			cout << "Seleccione una opcion: ";
			cin >> op;
			if (op == 0) {
				char confirmar;
				cout << "¿Esta seguro de cerrar la sesion? (s/n): ";
				cin >> confirmar;
				if (confirmar == 's' || confirmar == 'S') {
					if (authCtrl) authCtrl->cerrarSesion();
					cout << "Sesion cerrada. Redirigiendo a la pantalla de inicio de sesion..." << endl;
					return;
				}
				cout << "Operacion cancelada. Sesion activa." << endl;
				continue;
			}
			if (op == 1) {
				calificarProducto();
			} else if (op == 2) {
				consultarInfoDetalladaProducto();
			} else {
				cout << "Opcion invalida." << endl;
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

void MenuCliente::calificarProducto() {
	try {
		if (!authCtrl || !authCtrl->haySesionActiva()) {
			cout << "Debe iniciar sesion primero." << endl;
			return;
		}

		string correoCliente = authCtrl->getSesionActual().correo;
		vector<ProductoClienteInfo> productosComprados = ctrl.listarProductosCompradosCliente(correoCliente);
		if (productosComprados.empty()) {
			cout << "\nNo ha comprado ningun producto aun. No puede calificar." << endl;
			return;
		}

		cout << "\n--- Productos que ha comprado ---\n";
		for (const ProductoClienteInfo& p : productosComprados) {
			cout << "  Codigo: " << p.codigo
				 << " | Nombre: " << p.nombre
				 << " | Descripcion: " << p.descripcion << endl;
		}

		int codigo;
		int puntaje;
		string comentario;
		cout << "\nSeleccione el codigo del producto a calificar: ";
		cin >> codigo;

		// Validar puntaje en la vista antes de enviar al controlador
		while (true) {
			cout << "Ingrese puntaje (1-5): ";
			if (cin >> puntaje && puntaje >= 1 && puntaje <= 5) {
				break;
			}
			cout << "Puntaje invalido. Debe ser un numero entre 1 y 5." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		cout << "Ingrese comentario (opcional, presione Enter para omitir): ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, comentario);

		cout << "\n--- Resumen de calificacion ---\n";
		cout << "Producto codigo: " << codigo << "\n";
		cout << "Puntaje: " << puntaje << "/5\n";
		cout << "Comentario: " << (comentario.empty() ? "(sin comentario)" : comentario) << "\n";

		char confirmacion;
		cout << "\n¿Confirmar calificacion? (s/n): ";
		cin >> confirmacion;
		if (confirmacion != 's' && confirmacion != 'S') {
			cout << "Calificacion cancelada." << endl;
			return;
		}

		ResultadoCalificacionCliente resultado = ctrl.registrarCalificacionCliente(correoCliente, codigo, puntaje, comentario);
		if (resultado.exito) {
			cout << "\n¡Producto calificado exitosamente!" << endl;
			cout << "Nuevo puntaje promedio de " << resultado.nombreProducto << ": "
				 << resultado.puntajePromedioActualizado << "/5" << endl;
			return;
		}
		if (!resultado.productoExiste) {
			cout << "\nNo existe un producto con el codigo ingresado." << endl;
		} else if (!resultado.puntajeValido) {
			cout << "\nPuntaje invalido. Debe ser un numero entre 1 y 5." << endl;
		} else if (!resultado.productoComprado) {
			cout << "\nNo puede calificar un producto que no haya comprado." << endl;
		} else {
			cout << "\nNo se pudo calificar el producto." << endl;
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

void MenuCliente::consultarInfoDetalladaProducto() {
	try {
		vector<ProductoClienteInfo> productos = ctrl.listarProductosCatalogoCliente();
		if (productos.empty()) {
			cout << "\nNo hay productos registrados en el catalogo." << endl;
			return;
		}

		cout << "\n--- Productos disponibles ---\n";
		for (const ProductoClienteInfo& p : productos) {
			cout << "  Codigo: " << p.codigo << " | Nombre: " << p.nombre << endl;
		}

		int codigo;
		cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
		cin >> codigo;
		if (codigo == 0) {
			cout << "Operacion cancelada." << endl;
			return;
		}

		ResultadoDetalleProductoCliente detalle = ctrl.obtenerDetalleProductoCliente(codigo);
		if (!detalle.productoExiste) {
			cout << "Error: No existe un producto con el codigo ingresado." << endl;
			return;
		}

		cout << "\n========== INFORMACION DETALLADA DEL PRODUCTO ==========\n";
		cout << "Codigo: " << detalle.codigo << endl;
		cout << "Nombre: " << detalle.nombre << endl;
		cout << "Descripcion: " << detalle.descripcion << endl;
		cout << "Categoria: " << detalle.categoria << endl;
		cout << "Precio de venta unitario: $" << detalle.precioVentaUnitario << endl;
		cout << "Stock actual: " << detalle.stockActual << endl;
		cout << "Puntaje promedio: " << detalle.puntajePromedio << "/5"
			 << " (" << detalle.cantidadCalificaciones << " calificaciones)" << endl;

		if (!detalle.ultimasCalificaciones.empty()) {
			cout << "\n--- Ultimas calificaciones ---\n";
			for (size_t i = 0; i < detalle.ultimasCalificaciones.size(); ++i) {
				const CalificacionClienteInfo& cal = detalle.ultimasCalificaciones[i];
				cout << (i + 1) << ") Fecha: " << cal.fecha.getDia() << "/" << cal.fecha.getMes() << "/" << cal.fecha.getAnio();
				cout << " - Puntaje: " << cal.puntaje << "/5";
				cout << " - Comentario: " << (cal.comentario.empty() ? "(sin comentario)" : cal.comentario) << endl;
			}
		} else {
			cout << "\nNo hay calificaciones registradas para este producto." << endl;
		}
		cout << "========================================================\n";
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
