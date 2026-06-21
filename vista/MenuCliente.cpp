#include "MenuCliente.h"
#include <iostream>
#include <limits>

using namespace std;

MenuCliente::MenuCliente(VentaController& controller, AuthController* auth, EmpleadoController* empleado)
	: ctrl(controller), authCtrl(auth), empleadoCtrl(empleado) {}

void MenuCliente::mostrar() {
	while (true) {
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
			if (authCtrl) authCtrl->cerrarSesion();
			cout << "Sesion cerrada." << endl;
			return;
		}
		if (op == 1) {
			calificarProducto();
		} else if (op == 2) {
			consultarInfoDetalladaProducto();
		} else {
			cout << "Opcion invalida." << endl;
		}
	}
}

void MenuCliente::calificarProducto() {
	if (!authCtrl || !authCtrl->haySesionActiva()) {
		cout << "Debe iniciar sesion primero." << endl;
		return;
	}

	string correoCliente = authCtrl->getSesionActual().correo;

	// 1. Listar productos comprados por el cliente
	vector<Producto*> productosComprados = ctrl.listarProductosComprados(correoCliente);

	if (productosComprados.empty()) {
		cout << "\nNo ha comprado ningun producto aun. No puede calificar." << endl;
		return;
	}

	cout << "\n--- Productos que ha comprado ---\n";
	for (Producto* p : productosComprados) {
		if (p != nullptr) {
			cout << "  Codigo: " << p->getCodigo()
				 << " | Nombre: " << p->getNombre()
				 << " | Descripcion: " << p->getDescripcion() << endl;
		}
	}

	// 2. Seleccionar producto
	int codigo;
	cout << "\nSeleccione el codigo del producto a calificar: ";
	cin >> codigo;

	// Validar que el producto esté en la lista de comprados
	Producto* productoSeleccionado = nullptr;
	for (Producto* p : productosComprados) {
		if (p != nullptr && p->getCodigo() == codigo) {
			productoSeleccionado = p;
			break;
		}
	}

	if (productoSeleccionado == nullptr) {
		cout << "El codigo ingresado no corresponde a un producto comprado." << endl;
		return;
	}

	// 3. Ingresar puntaje
	int punt;
	cout << "Ingrese puntaje (1-5): ";
	cin >> punt;

	if (punt < 1 || punt > 5) {
		cout << "Puntaje invalido. Debe ser un numero entre 1 y 5." << endl;
		return;
	}

	Puntaje p = static_cast<Puntaje>(punt);

	// 4. Ingresar comentario opcional
	string comentario;
	cout << "Ingrese comentario (opcional, presione Enter para omitir): ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, comentario);

	// 5. Mostrar resumen y confirmar
	cout << "\n--- Resumen de calificacion ---\n";
	cout << "Producto: " << productoSeleccionado->getNombre() << " (codigo " << productoSeleccionado->getCodigo() << ")\n";
	cout << "Puntaje: " << punt << "/5\n";
	cout << "Comentario: " << (comentario.empty() ? "(sin comentario)" : comentario) << "\n";

	char confirmacion;
	cout << "\n¿Confirmar calificacion? (s/n): ";
	cin >> confirmacion;

	if (confirmacion != 's' && confirmacion != 'S') {
		cout << "Calificacion cancelada." << endl;
		return;
	}

	// 6. Ejecutar calificación
	bool ok = ctrl.calificarProducto(codigo, p, comentario, correoCliente);

	if (ok) {
		cout << "\n¡Producto calificado exitosamente!" << endl;
		cout << "Nuevo puntaje promedio de " << productoSeleccionado->getNombre() << ": "
			 << productoSeleccionado->getPuntajePromedio() << "/5" << endl;
	} else {
		cout << "\nNo se pudo calificar el producto. Ocurrio un error interno." << endl;
	}
}

void MenuCliente::consultarInfoDetalladaProducto() {
	AdminController* adminCtrl = AdminController::getInstanciaAdmin();
	if (adminCtrl == nullptr) return;

	auto productos = adminCtrl->listarProductos();
	if (productos.empty()) {
		cout << "\nNo hay productos registrados en el catalogo." << endl;
		return;
	}

	cout << "\n--- Productos disponibles ---\n";
	for (Producto* p : productos) {
		if (p != nullptr) {
			cout << "  Codigo: " << p->getCodigo()
				 << " | Nombre: " << p->getNombre() << endl;
		}
	}

	int codigo;
	cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
	cin >> codigo;

	if (codigo == 0) {
		cout << "Operacion cancelada." << endl;
		return;
	}

	Producto* producto = adminCtrl->buscarProducto(codigo);
	if (producto == nullptr) {
		cout << "Error: No existe un producto con el codigo ingresado." << endl;
		return;
	}

	// Mostrar informacion basica
	cout << "\n========== INFORMACION DETALLADA DEL PRODUCTO ==========\n";
	cout << "Codigo: " << producto->getCodigo() << endl;
	cout << "Nombre: " << producto->getNombre() << endl;
	cout << "Descripcion: " << producto->getDescripcion() << endl;

	string nombreCat = producto->getCategoria() != nullptr ? producto->getCategoria()->getNombre() : "(sin categoria)";
	cout << "Categoria: " << nombreCat << endl;

	cout << "Precio de venta unitario: $" << producto->getPrecioVentaActual() << endl;
	cout << "Stock actual: " << producto->getStock() << endl;
	cout << "Puntaje promedio: " << producto->getPuntajePromedio() << "/5"
		 << " (" << producto->getCantidadCalificaciones() << " calificaciones)" << endl;

	// Ultimas 5 calificaciones (solo para Cliente)
	vector<Calificacion*> calificaciones = ctrl.listarCalificacionesDeProducto(codigo);
	if (!calificaciones.empty()) {
		cout << "\n--- Ultimas calificaciones ---\n";
		int mostrar = min(5, (int)calificaciones.size());
		for (int i = 0; i < mostrar; ++i) {
			Calificacion* cal = calificaciones[i];
			if (cal == nullptr) continue;

			DTFecha f = cal->getFecha();
			string comentario = cal->getComentario();

			cout << (i + 1) << ") Fecha: " << f.getDia() << "/" << f.getMes() << "/" << f.getAnio();
			cout << " - Puntaje: " << static_cast<int>(cal->getPuntaje()) << "/5";
			cout << " - Comentario: " << (comentario.empty() ? "(sin comentario)" : comentario) << endl;
		}
	} else {
		cout << "\nNo hay calificaciones registradas para este producto." << endl;
	}
	cout << "========================================================\n";
}
