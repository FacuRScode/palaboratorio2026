#include "MenuEmpleado.h"
#include <iostream>
#include <limits>
#include <exception>

using namespace std;

MenuEmpleado::MenuEmpleado(EmpleadoController& controller, AuthController* auth, VentaController* ventas)
	: ctrl(controller), authCtrl(auth), ventaCtrl(ventas) {}

void MenuEmpleado::mostrar() {
	while (true) {
		try {
			cout << "\n--- Menu Empleado ---\n";
			if (authCtrl && authCtrl->haySesionActiva()) {
				Sesion s = authCtrl->getSesionActual();
				cout << "Usuario: " << s.nombre << " (" << s.rol << ")\n";
			}
			cout << "1. Registrar cliente\n";
			cout << "2. Modificar cliente\n";
			cout << "3. Registrar venta\n";
			cout << "4. Historial de compras de cliente\n";
			cout << "5. Emitir orden de compra\n";
			cout << "6. Cancelar orden de compra\n";
			cout << "7. Registrar recepcion de orden de compra\n";
			cout << "8. Consultar calificaciones de producto\n";
			cout << "9. Consultar stock actual de productos\n";
			cout << "10. Consultar productos con stock bajo minimo\n";
			cout << "11. Consultar monto facturado a cliente\n";
			cout << "12. Consultar unidades vendidas de un producto\n";
			cout << "13. Consultar informacion detallada de un producto\n";
			cout << "0. Cerrar sesion\n";

			int op;
			cout << "Seleccione una opcion: ";
			cin >> op;

			if (op == 0) {
				if (authCtrl) authCtrl->cerrarSesion();
				cout << "Sesion cerrada." << endl;
				return;
			}
			if (op == 1) registrarCliente();
			else if (op == 2) modificarCliente();
			else if (op == 3) registrarVenta();
			else if (op == 4) historialComprasCliente();
			else if (op == 5) emitirOrdenCompra();
			else if (op == 6) cancelarOrdenCompra();
			else if (op == 7) registrarRecepcionOrdenCompra();
			else if (op == 8) consultarCalificacionesProducto();
			else if (op == 9) consultarStockProductos();
			else if (op == 10) consultarProductosStockBajo();
			else if (op == 11) consultarMontoFacturadoCliente();
			else if (op == 12) consultarUnidadesVendidasProducto();
			else if (op == 13) consultarInfoDetalladaProducto();
			else cout << "Opcion invalida." << endl;
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

void MenuEmpleado::registrarCliente() {
	try {
		string rut, nombre, apellido, direccion, correo, contrasena;
		char respuesta;
		cout << "RUT: "; cin >> rut;
		cout << "Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre);
		cout << "Apellido: "; getline(cin, apellido);
		cout << "Direccion: "; getline(cin, direccion);
		cout << "Correo: "; getline(cin, correo);
		cout << "Contrasena: "; getline(cin, contrasena);

		cout << "\nResumen del registro:\n";
		cout << "RUT: " << rut << endl;
		cout << "Nombre Completo: " << nombre << " " << apellido << endl;
		cout << "Direccion: " << direccion << endl;
		cout << "Correo: " << correo << endl;
		cout << "Contrasena: " << contrasena << endl;
		cout << "¿Desea confirmar el registro? (s/n): "; cin >> respuesta;
		if (respuesta != 's' && respuesta != 'S') {
			cout << "Registro cancelado." << endl;
			return;
		}

		Cliente* nuevo = ctrl.registrarCliente(rut, nombre, apellido, direccion, correo, contrasena);
		if (nuevo != nullptr) cout << "Cliente registrado con exito." << endl;
		else cout << "Error: El RUT o el correo ya estan registrados en el sistema." << endl;
	} catch (const exception& ex) {
		cout << "Error al registrar cliente: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al registrar cliente." << endl;
	}
}

void MenuEmpleado::modificarCliente() {
	try {
		string rut, nuevoNombre, nuevoApellido, nuevaDireccion, nuevoCorreo;
		char respuesta;

		cout << "RUT: "; cin >> rut;
		Cliente* cliente = ctrl.buscarCliente(rut);
		if (cliente == nullptr) {
			cout << "Error: No existe un cliente con ese RUT." << endl;
			return;
		}

		cout << "\nDatos actuales del cliente:" << endl;
		cout << "Nombre Completo: " << cliente->getNombre() << " " << cliente->getApellido() << endl;
		cout << "Direccion: " << cliente->getDireccion() << endl;
		cout << "Correo: " << cliente->getCorreo() << endl;

		cout << "Nuevo Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nuevoNombre);
		cout << "Nuevo Apellido: "; getline(cin, nuevoApellido);
		cout << "Nueva Direccion: "; getline(cin, nuevaDireccion);
		cout << "Nuevo Correo: "; getline(cin, nuevoCorreo);

		cout << "\nResumen de la edicion:\n";
		cout << "Nombre Completo: " << nuevoNombre << " " << nuevoApellido << endl;
		cout << "Direccion: " << nuevaDireccion << endl;
		cout << "Correo: " << nuevoCorreo << endl;
		cout << "¿Desea confirmar la modificacion? (s/n): "; cin >> respuesta;
		if (respuesta != 's' && respuesta != 'S') {
			cout << "Modificacion cancelada. Se conservan los datos originales." << endl;
			return;
		}

		if (ctrl.modificaCliente(cliente, nuevoNombre, nuevoApellido, nuevaDireccion, nuevoCorreo)) {
			cout << "Cliente modificado con exito." << endl;
		} else {
			cout << "Error: El correo ya pertenece a otro cliente. Modificacion cancelada." << endl;
		}
	} catch (const exception& ex) {
		cout << "Error al modificar cliente: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al modificar cliente." << endl;
	}
}

void MenuEmpleado::registrarVenta() {
	try {
		char esRegistrado;
		cout << "¿El cliente esta registrado? (s/n): ";
		cin >> esRegistrado;

		bool clienteRegistrado = (esRegistrado == 's' || esRegistrado == 'S');
		string rutCliente;
		if (clienteRegistrado) {
			cout << "RUT del cliente: ";
			cin >> rutCliente;
		}

		vector<pair<int, int>> lineas;
		while (true) {
			int codigoProducto;
			int cantidad;
			cout << "\nIngrese codigo del producto (0 para finalizar): ";
			cin >> codigoProducto;
			if (codigoProducto == 0) break;
			cout << "Ingrese la cantidad: ";
			cin >> cantidad;
			lineas.push_back({codigoProducto, cantidad});
		}

		cout << "¿Desea confirmar esta venta? (s/n): ";
		char confirmar;
		cin >> confirmar;
		if (confirmar != 's' && confirmar != 'S') {
			cout << "Venta cancelada." << endl;
			return;
		}

		ResultadoRegistroVenta res = ctrl.registrarVentaCompleta(rutCliente, clienteRegistrado, lineas);
		if (!res.exito) {
			if (!res.clienteValido) cout << "Error: No existe un cliente con el RUT ingresado." << endl;
			else if (!res.lineasValidas) cout << "Error: Debe ingresar al menos una linea valida." << endl;
			else if (!res.productoValido) cout << "Error: Uno o mas productos no existen." << endl;
			else if (!res.stockSuficiente) cout << "Error: No hay stock suficiente para uno o mas productos." << endl;
			else cout << "Error al registrar la venta." << endl;
			return;
		}

		cout << "\nResumen de venta:" << endl;
		cout << "Cliente: " << (res.clienteRegistrado ? res.rutCliente : "Ocasional")
			 << " (" << res.nombreCliente << ")" << endl;
		for (const auto& linea : res.lineas) {
			cout << "  Producto: " << linea.nombreProducto << " (codigo=" << linea.codigoProducto << ")"
				 << " | Cantidad: " << linea.cantidad
				 << " | P. Unitario: $" << linea.precioUnitario
				 << " | Subtotal: $" << linea.subtotal << endl;
		}
		cout << "Total: $" << res.total << endl;
		cout << "Fecha/Hora: " << res.fecha.getDia() << "/" << res.fecha.getMes() << "/" << res.fecha.getAnio()
			 << " " << res.hora.getHora() << ":" << res.hora.getMinuto() << ":" << res.hora.getSegundo() << endl;
		cout << "Venta registrada con exito." << endl;
	} catch (const exception& ex) {
		cout << "Error al registrar venta: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al registrar venta." << endl;
	}
}

void MenuEmpleado::historialComprasCliente() {
	try {
		string rut;
		cout << "RUT: ";
		cin >> rut;

		ResultadoHistorialCliente res = ctrl.obtenerHistorialComprasCliente(rut);
		if (!res.clienteExiste) {
			cout << "Error: No existe un cliente con ese RUT." << endl;
			return;
		}
		if (res.ventas.empty()) {
			cout << "El cliente " << res.nombreCliente << " no registra compras en el sistema." << endl;
			return;
		}

		cout << "\nHistorial de compras: " << res.nombreCliente << endl;
		cout << "------------------------------------------------" << endl;
		for (size_t i = 0; i < res.ventas.size(); ++i) {
			const auto& v = res.ventas[i];
			cout << (i + 1) << ") Fecha: " << v.fecha.getDia() << "/" << v.fecha.getMes() << "/" << v.fecha.getAnio()
				 << " Hora: " << v.hora.getHora() << ":" << v.hora.getMinuto() << ":" << v.hora.getSegundo()
				 << " - " << v.lineas.size() << " lineas - Total: $" << v.total << endl;
		}

		int seleccion;
		cout << "\nSeleccione el numero de una venta para ver su detalle (0 para salir): ";
		cin >> seleccion;
		if (seleccion <= 0 || seleccion > static_cast<int>(res.ventas.size())) {
			cout << "Regresando al menu de opciones..." << endl;
			return;
		}

		const auto& venta = res.ventas[seleccion - 1];
		cout << "\nDetalle de la venta N° " << seleccion << endl;
		cout << "Fecha: " << venta.fecha.getDia() << "/" << venta.fecha.getMes() << "/" << venta.fecha.getAnio()
			 << " Hora: " << venta.hora.getHora() << ":" << venta.hora.getMinuto() << ":" << venta.hora.getSegundo() << endl;
		for (const auto& linea : venta.lineas) {
			cout << "  Producto: " << linea.nombreProducto << " (codigo=" << linea.codigoProducto << ")" << endl;
			cout << "  Cantidad: " << linea.cantidad << " | P. Unitario: $" << linea.precioUnitario
				 << " | Subtotal: $" << linea.subtotal << endl;
		}
		cout << "  -----------------------------------------" << endl;
		cout << "  Total compra: $" << venta.total << endl;
	} catch (const exception& ex) {
		cout << "Error al consultar historial: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al consultar historial." << endl;
	}
}

void MenuEmpleado::emitirOrdenCompra() {
	try {
		string rutProveedor;
		cout << "RUT del proveedor: ";
		cin >> rutProveedor;

		ResultadoProductosProveedor productosProveedor = ctrl.obtenerProductosProveedor(rutProveedor);
		if (!productosProveedor.proveedorExiste) {
			cout << "Error: No existe un proveedor con ese RUT." << endl;
			return;
		}
		if (productosProveedor.productos.empty()) {
			cout << "El proveedor no tiene productos asociados." << endl;
			return;
		}

		cout << "\nProductos que abastece " << productosProveedor.empresaProveedor << ":" << endl;
		for (const auto& p : productosProveedor.productos) {
			cout << "  Codigo: " << p.codigoProducto
				 << " | " << p.nombreProducto
				 << " | Precio: $" << p.precioCompraPactado
				 << " | Entrega: " << p.tiempoEntregaDias << " dias" << endl;
		}

		vector<pair<int, int>> lineas;
		while (true) {
			int codigoProducto;
			int cantidad;
			cout << "\nSeleccione el codigo del producto (0 para finalizar): ";
			cin >> codigoProducto;
			if (codigoProducto == 0) break;
			cout << "Cantidad pedida: ";
			cin >> cantidad;
			lineas.push_back({codigoProducto, cantidad});
		}

		cout << "¿Desea confirmar esta orden de compra? (s/n): ";
		char confirmar;
		cin >> confirmar;
		if (confirmar != 's' && confirmar != 'S') {
			cout << "Operacion cancelada." << endl;
			return;
		}

		ResultadoOrdenCompra res = ctrl.emitirOrdenCompra(rutProveedor, lineas);
		if (!res.exito) {
			if (!res.proveedorExiste) cout << "Error: Proveedor invalido." << endl;
			else if (!res.lineasValidas) cout << "Error: Debe ingresar lineas validas para la orden." << endl;
			else if (!res.productoValido) cout << "Error: Uno o mas productos no existen." << endl;
			else if (!res.productoAsociadoProveedor) cout << "Error: Uno o mas productos no estan asociados al proveedor." << endl;
			else cout << "Error al registrar la orden de compra." << endl;
			return;
		}

		cout << "\nResumen de orden de compra:" << endl;
		cout << "Proveedor: " << res.empresaProveedor << " (RUT: " << res.rutProveedor << ")" << endl;
		cout << "Fecha Emision: " << res.fechaEmision.getDia() << "/" << res.fechaEmision.getMes()
			 << "/" << res.fechaEmision.getAnio() << endl;
		cout << "Cant. Lineas: " << res.cantidadLineas << endl;
		cout << "Total Estimado: $" << res.totalEstimado << endl;
		cout << "La orden de compra ha sido registrada con estado 'Pendiente'." << endl;
	} catch (const exception& ex) {
		cout << "Error al emitir orden de compra: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al emitir orden de compra." << endl;
	}
}

void MenuEmpleado::cancelarOrdenCompra() {
	try {
		vector<OrdenPendienteInfo> pendientes = ctrl.listarOrdenesPendientesInfo();
		if (pendientes.empty()) {
			cout << "\nNo hay ordenes de compra en estado 'Pendiente'." << endl;
			return;
		}

		cout << "\n--- Ordenes de compra pendientes ---\n";
		for (const auto& orden : pendientes) {
			cout << "  ID " << orden.id
				 << " | Proveedor: " << orden.empresaProveedor
				 << " | Fecha Emision: " << orden.fechaEmision.getDia() << "/" << orden.fechaEmision.getMes() << "/" << orden.fechaEmision.getAnio()
				 << " | Lineas: " << orden.cantidadLineas << endl;
		}

		int id;
		cout << "\nIngrese el ID de la orden a cancelar: ";
		cin >> id;

		DetalleOrdenPendiente detalle = ctrl.obtenerDetalleOrdenPendiente(id);
		if (!detalle.idValido || !detalle.exito) {
			cout << "Error: ID invalido." << endl;
			return;
		}

		cout << "\n--- Detalle de la orden de compra ---\n";
		cout << "Proveedor: " << detalle.resumen.empresaProveedor << endl;
		cout << "Fecha de emision: " << detalle.resumen.fechaEmision.getDia() << "/"
			 << detalle.resumen.fechaEmision.getMes() << "/"
			 << detalle.resumen.fechaEmision.getAnio() << endl;
		cout << "Estado: Pendiente" << endl;
		cout << "Lineas de la orden:\n";
		for (size_t i = 0; i < detalle.lineas.size(); ++i) {
			cout << "  " << (i + 1) << ". Producto: " << detalle.lineas[i].nombreProducto
				 << " | Cantidad: " << detalle.lineas[i].cantidad << endl;
		}
		cout << "Total estimado: $" << detalle.resumen.totalEstimado << endl;

		char confirmar;
		cout << "\n¿Esta seguro de cancelar esta orden de compra? (s/n): ";
		cin >> confirmar;
		if (confirmar != 's' && confirmar != 'S') {
			cout << "Operacion cancelada. No se realizaron cambios." << endl;
			return;
		}

		ResultadoCancelarOrden cancelacion = ctrl.cancelarOrdenPendiente(id);
		if (cancelacion.exito) {
			cout << "La orden de compra ha sido cancelada." << endl;
			cout << "Nota: Una orden cancelada no generara movimiento de stock." << endl;
		} else {
			cout << "Error al cancelar la orden." << endl;
		}
	} catch (const exception& ex) {
		cout << "Error al cancelar orden: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al cancelar orden." << endl;
	}
}

void MenuEmpleado::registrarRecepcionOrdenCompra() {
	try {
		vector<OrdenPendienteInfo> pendientes = ctrl.listarOrdenesPendientesInfo();
		if (pendientes.empty()) {
			cout << "\nNo hay ordenes de compra en estado 'Pendiente'." << endl;
			return;
		}

		cout << "\n--- Ordenes de compra pendientes ---\n";
		for (const auto& orden : pendientes) {
			cout << "  ID " << orden.id
				 << " | Proveedor: " << orden.empresaProveedor
				 << " | Fecha Emision: " << orden.fechaEmision.getDia() << "/" << orden.fechaEmision.getMes() << "/" << orden.fechaEmision.getAnio() << endl;
		}

		int id;
		cout << "\nIngrese el ID de la orden recibida: ";
		cin >> id;

		DetalleOrdenPendiente detalle = ctrl.obtenerDetalleOrdenPendiente(id);
		if (!detalle.idValido || !detalle.exito) {
			cout << "Error: ID invalido." << endl;
			return;
		}

		cout << "\n--- Detalle de la orden de compra ---\n";
		cout << "Proveedor: " << detalle.resumen.empresaProveedor << endl;
		cout << "Fecha de emision: " << detalle.resumen.fechaEmision.getDia() << "/"
			 << detalle.resumen.fechaEmision.getMes() << "/"
			 << detalle.resumen.fechaEmision.getAnio() << endl;
		cout << "\nProductos y cantidades pedidas:\n";
		for (size_t i = 0; i < detalle.lineas.size(); ++i) {
			cout << "  " << (i + 1) << ". " << detalle.lineas[i].nombreProducto
				 << " - Cantidad pedida: " << detalle.lineas[i].cantidad << endl;
		}

		vector<int> cantidadesRecibidas;
		cout << "\n--- Ingrese las cantidades recibidas ---\n";
		for (const auto& linea : detalle.lineas) {
			int cant;
			cout << "  " << linea.nombreProducto << " (pedido: " << linea.cantidad << "): ";
			cin >> cant;
			cantidadesRecibidas.push_back(cant);
		}

		cout << "\n--- Resumen de recepcion ---\n";
		cout << "Proveedor: " << detalle.resumen.empresaProveedor << endl;
		for (size_t i = 0; i < detalle.lineas.size(); ++i) {
			cout << "  " << detalle.lineas[i].nombreProducto
				 << ": pedido " << detalle.lineas[i].cantidad
				 << " | recibido " << cantidadesRecibidas[i] << endl;
		}

		char confirmar;
		cout << "\n¿Desea confirmar la recepcion de esta orden? (s/n): ";
		cin >> confirmar;
		if (confirmar != 's' && confirmar != 'S') {
			cout << "Operacion cancelada. No se realizaron cambios." << endl;
			return;
		}

		ResultadoRegistrarRecepcionOrden recepcion = ctrl.registrarRecepcionOrdenPendiente(id, cantidadesRecibidas);
		if (!recepcion.idValido) {
			cout << "Error: ID invalido." << endl;
		} else if (!recepcion.cantidadesValidas) {
			cout << "Error: Cantidades recibidas invalidas." << endl;
		} else if (recepcion.exito) {
			cout << "\nRecepcion registrada con exito." << endl;
			cout << "La orden ha pasado al estado 'Recibida'." << endl;
			cout << "El stock de los productos ha sido actualizado." << endl;
		} else {
			cout << "\nError al registrar la recepcion." << endl;
		}
	} catch (const exception& ex) {
		cout << "Error al registrar recepcion: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al registrar recepcion." << endl;
	}
}

void MenuEmpleado::consultarStockProductos() {
	try {
		char filtrar;
		cout << "\n¿Desea filtrar por categoria? (s/n): ";
		cin >> filtrar;

		vector<StockProductoEmpleadoInfo> productos;
		if (filtrar == 's' || filtrar == 'S') {
			vector<CategoriaEmpleadoInfo> categorias = ctrl.listarCategoriasEmpleado();
			if (categorias.empty()) {
				cout << "\nNo hay categorias registradas en el sistema." << endl;
				return;
			}

			cout << "\n--- Categorias disponibles ---\n";
			for (size_t i = 0; i < categorias.size(); ++i) {
				cout << "  " << (i + 1) << ". " << categorias[i].nombre
					 << ": " << categorias[i].descripcion << endl;
			}

			int opcion;
			cout << "\nSeleccione una categoria (1-" << categorias.size() << "): ";
			cin >> opcion;
			if (opcion < 1 || opcion > static_cast<int>(categorias.size())) {
				cout << "Opcion invalida." << endl;
				return;
			}

			productos = ctrl.listarStockProductosVista(categorias[opcion - 1].nombre);
			if (productos.empty()) {
				cout << "\nNo hay productos en la categoria seleccionada." << endl;
				return;
			}
		} else {
			productos = ctrl.listarStockProductosVista();
			if (productos.empty()) {
				cout << "\nNo hay productos registrados en el catalogo." << endl;
				return;
			}
		}

		cout << "\n--- Stock actual de productos ---\n";
		cout << "Codigo  | Nombre                          | Categoria               | Stock Actual | Stock Minimo\n";
		cout << "--------|---------------------------------|-------------------------|--------------|--------------\n";
		for (const StockProductoEmpleadoInfo& p : productos) {
			cout << p.codigo << "\t| " << p.nombre;
			for (int i = static_cast<int>(p.nombre.length()); i < 31; ++i) cout << " ";
			cout << "| " << p.categoria;
			for (int i = static_cast<int>(p.categoria.length()); i < 23; ++i) cout << " ";
			cout << "| " << p.stockActual;
			for (int i = static_cast<int>(to_string(p.stockActual).length()); i < 12; ++i) cout << " ";
			cout << "| " << p.stockMinimo << endl;
		}
		cout << "---------------------------------------------------------------------\n";
	} catch (const exception& ex) {
		cout << "Error al consultar stock: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al consultar stock." << endl;
	}
}

void MenuEmpleado::consultarProductosStockBajo() {
	try {
		char ordenar;
		cout << "\n¿Ordenar por diferencia ascendente (mas criticos primero)? (s/n): ";
		cin >> ordenar;

		vector<StockProductoEmpleadoInfo> productosBajos = ctrl.listarProductosStockBajoVista(ordenar == 's' || ordenar == 'S');
		if (productosBajos.empty()) {
			cout << "\nNo hay productos con stock por debajo del minimo configurado." << endl;
			return;
		}

		cout << "\n--- Productos con stock bajo minimo ---\n";
		cout << "Codigo  | Nombre                          | Categoria               | Stock Actual | Stock Minimo | Diferencia\n";
		cout << "--------|---------------------------------|-------------------------|--------------|--------------|------------\n";
		for (const StockProductoEmpleadoInfo& p : productosBajos) {
			int diferencia = p.stockMinimo - p.stockActual;
			string strCodigo = to_string(p.codigo);
			string strStock = to_string(p.stockActual);
			string strStockMin = to_string(p.stockMinimo);
			string strDiff = to_string(diferencia);

			cout << strCodigo;
			for (int i = static_cast<int>(strCodigo.length()); i < 8; ++i) cout << " ";
			cout << "| " << p.nombre;
			for (int i = static_cast<int>(p.nombre.length()); i < 31; ++i) cout << " ";
			cout << "| " << p.categoria;
			for (int i = static_cast<int>(p.categoria.length()); i < 23; ++i) cout << " ";
			cout << "| " << strStock;
			for (int i = static_cast<int>(strStock.length()); i < 12; ++i) cout << " ";
			cout << "| " << strStockMin;
			for (int i = static_cast<int>(strStockMin.length()); i < 12; ++i) cout << " ";
			cout << "| " << strDiff << " (faltan " << diferencia << ")" << endl;
		}
		cout << "----------------------------------------------------------------------------\n";
	} catch (const exception& ex) {
		cout << "Error al consultar stock bajo: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al consultar stock bajo." << endl;
	}
}

void MenuEmpleado::consultarCalificacionesProducto() {
	try {
		if (ventaCtrl == nullptr) {
			cout << "Error: Controlador de ventas no disponible." << endl;
			return;
		}

		vector<ProductoClienteInfo> productos = ventaCtrl->listarProductosCatalogoCliente();
		if (productos.empty()) {
			cout << "\nNo hay productos registrados en el catalogo." << endl;
			return;
		}

		cout << "\n--- Catalogo de productos ---\n";
		for (const ProductoClienteInfo& p : productos) {
			cout << "  Codigo: " << p.codigo
				 << " | Nombre: " << p.nombre << endl;
		}

		int codigo;
		cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
		cin >> codigo;
		if (codigo == 0) {
			cout << "Operacion cancelada." << endl;
			return;
		}

		ResultadoCalificacionesProductoVista resultado = ventaCtrl->obtenerCalificacionesProductoVista(codigo);
		if (!resultado.productoExiste) {
			cout << "No existe un producto con el codigo ingresado." << endl;
			return;
		}

		if (resultado.calificaciones.empty()) {
			cout << "\nEl producto \"" << resultado.nombreProducto << "\" no tiene calificaciones registradas." << endl;
			cout << "Puntaje promedio: " << resultado.puntajePromedio << "/5" << endl;
			return;
		}

		cout << "\n--- Calificaciones de \"" << resultado.nombreProducto << "\" ---\n";
		cout << "Puntaje promedio general: " << resultado.puntajePromedio << "/5"
			 << " (basado en " << resultado.calificaciones.size() << " calificaciones)" << endl;
		cout << "--------------------------------------------------------\n";
		for (size_t i = 0; i < resultado.calificaciones.size(); ++i) {
			const CalificacionEmpleadoVistaInfo& cal = resultado.calificaciones[i];
			cout << (i + 1) << ") Fecha: " << cal.fecha.getDia() << "/" << cal.fecha.getMes() << "/" << cal.fecha.getAnio()
				 << " | Puntaje: " << cal.puntaje << "/5"
				 << " | Cliente RUT: " << cal.rutCliente
				 << " | Comentario: " << (cal.comentario.empty() ? "(sin comentario)" : cal.comentario) << endl;
		}
		cout << "--------------------------------------------------------\n";
	} catch (const exception& ex) {
		cout << "Error al consultar calificaciones: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al consultar calificaciones." << endl;
	}
}

void MenuEmpleado::consultarMontoFacturadoCliente() {
	try {
		string rut;
		cout << "\nIngrese el RUT del cliente: ";
		cin >> rut;

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;
		cout << "\nFecha de inicio del periodo:" << endl;
		cout << "Dia: "; cin >> diaInicio;
		cout << "Mes: "; cin >> mesInicio;
		cout << "Anio: "; cin >> anioInicio;
		DTFecha inicio(diaInicio, mesInicio, anioInicio);

		cout << "\nFecha de fin del periodo:" << endl;
		cout << "Dia: "; cin >> diaFin;
		cout << "Mes: "; cin >> mesFin;
		cout << "Anio: "; cin >> anioFin;
		DTFecha fin(diaFin, mesFin, anioFin);

		ResultadoFacturacionConsulta consulta = ctrl.consultarMontoFacturadoValidado(rut, inicio, fin);
		if (!consulta.rangoFechasValido) {
			cout << "Error: La fecha de inicio debe ser anterior o igual a la fecha de fin." << endl;
			return;
		}
		if (!consulta.datos.clienteExiste) {
			cout << "Error: No existe un cliente con el RUT ingresado." << endl;
			return;
		}

		string nombreCliente = ctrl.obtenerNombreClientePorRut(rut);
		cout << "\n--- Resultados de facturacion ---" << endl;
		cout << "Cliente: " << (!nombreCliente.empty() ? nombreCliente : rut)
			 << " (RUT: " << rut << ")" << endl;
		cout << "Periodo: " << diaInicio << "/" << mesInicio << "/" << anioInicio
			 << " - " << diaFin << "/" << mesFin << "/" << anioFin << endl;
		cout << "------------------------------------------------" << endl;
		cout << "Monto total facturado: $" << consulta.datos.montoTotal << endl;
		cout << "Cantidad de ventas: " << consulta.datos.cantidadVentas << endl;
		if (consulta.datos.cantidadVentas > 0) {
			cout << "Monto promedio por venta: $" << consulta.datos.montoPromedio << endl;
		}
		cout << "------------------------------------------------" << endl;
	} catch (const exception& ex) {
		cout << "Error al consultar facturacion: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al consultar facturacion." << endl;
	}
}

void MenuEmpleado::consultarUnidadesVendidasProducto() {
	try {
		vector<ProductoEmpleadoInfo> productos = ctrl.listarProductosEmpleado();
		if (productos.empty()) {
			cout << "\nNo hay productos registrados en el catalogo." << endl;
			return;
		}

		cout << "\n--- Productos disponibles ---\n";
		for (const ProductoEmpleadoInfo& p : productos) {
			cout << "  Codigo: " << p.codigo << " | Nombre: " << p.nombre << endl;
		}

		int codigoProducto;
		cout << "\nIngrese el codigo del producto (0 para cancelar): ";
		cin >> codigoProducto;
		if (codigoProducto == 0) {
			cout << "Operacion cancelada." << endl;
			return;
		}

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;
		cout << "\nFecha de inicio del periodo:" << endl;
		cout << "Dia: "; cin >> diaInicio;
		cout << "Mes: "; cin >> mesInicio;
		cout << "Anio: "; cin >> anioInicio;
		DTFecha inicio(diaInicio, mesInicio, anioInicio);

		cout << "\nFecha de fin del periodo:" << endl;
		cout << "Dia: "; cin >> diaFin;
		cout << "Mes: "; cin >> mesFin;
		cout << "Anio: "; cin >> anioFin;
		DTFecha fin(diaFin, mesFin, anioFin);

		ResultadoUnidadesVendidasConsulta consulta = ctrl.consultarUnidadesVendidasValidado(codigoProducto, inicio, fin);
		if (!consulta.rangoFechasValido) {
			cout << "Error: La fecha de inicio debe ser anterior o igual a la fecha de fin." << endl;
			return;
		}
		if (!consulta.productoExiste) {
			cout << "Error: No existe un producto con el codigo ingresado." << endl;
			return;
		}

		string nombreProducto = ctrl.obtenerNombreProductoPorCodigo(codigoProducto);
		cout << "\n--- Resultados de unidades vendidas ---" << endl;
		cout << "Producto: " << (!nombreProducto.empty() ? nombreProducto : "(desconocido)")
			 << " (codigo=" << codigoProducto << ")" << endl;
		cout << "Periodo: " << diaInicio << "/" << mesInicio << "/" << anioInicio
			 << " - " << diaFin << "/" << mesFin << "/" << anioFin << endl;
		cout << "------------------------------------------------" << endl;
		cout << "Total de unidades vendidas: " << consulta.datos.totalUnidadesVendidas << endl;
		cout << "Cantidad de ventas en las que participo: " << consulta.datos.cantidadVentas << endl;
		if (consulta.datos.totalUnidadesVendidas > 0) {
			cout << "Precio unitario promedio: $" << consulta.datos.precioUnitarioPromedio << endl;
		}
		cout << "------------------------------------------------" << endl;
	} catch (const exception& ex) {
		cout << "Error al consultar unidades vendidas: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al consultar unidades vendidas." << endl;
	}
}

void MenuEmpleado::consultarInfoDetalladaProducto() {
	try {
		vector<ProductoEmpleadoInfo> productos = ctrl.listarProductosEmpleado();
		if (productos.empty()) {
			cout << "\nNo hay productos registrados en el catalogo." << endl;
			return;
		}

		cout << "\n--- Productos disponibles ---\n";
		for (const ProductoEmpleadoInfo& p : productos) {
			cout << "  Codigo: " << p.codigo << " | Nombre: " << p.nombre << endl;
		}

		int codigo;
		cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
		cin >> codigo;
		if (codigo == 0) {
			cout << "Operacion cancelada." << endl;
			return;
		}

		DetalleProductoEmpleadoVista detalle = ctrl.obtenerDetalleProductoEmpleadoVista(codigo);
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
		cout << "\n--- Datos de administracion ---\n";
		cout << "Stock minimo: " << detalle.stockMinimo << endl;

		if (detalle.proveedores.empty()) {
			cout << "Proveedores: Ninguno (el producto no esta asociado a ningun proveedor)" << endl;
		} else {
			cout << "\nProveedores que abastecen este producto:" << endl;
			for (size_t i = 0; i < detalle.proveedores.size(); ++i) {
				const ProveedorProductoInfo& pp = detalle.proveedores[i];
				cout << "  " << (i + 1) << ". Proveedor: " << pp.empresaProveedor
					 << " (RUT: " << pp.rutProveedor << ")" << endl;
				cout << "     Precio de compra vigente: $" << pp.precioCompraPactado << endl;
				cout << "     Tiempo de entrega estimado: " << pp.tiempoEntregaDias << " dias" << endl;
			}
		}
		cout << "========================================================\n";
	} catch (const exception& ex) {
		cout << "Error al consultar detalle de producto: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al consultar detalle de producto." << endl;
	}
}
