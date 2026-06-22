#include "MenuEmpleado.h"
#include <iostream>
#include <limits>
#include <exception>
#include <ctime>

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

		while (true) {
			cout << "\n--- Alta de cliente registrado ---\n";

			cout << "RUT: "; cin >> rut;
			cout << "Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre);
			cout << "Apellido: "; getline(cin, apellido);
			cout << "Direccion: "; getline(cin, direccion);
			cout << "Correo: "; getline(cin, correo);

			if (ctrl.buscarCliente(rut) != nullptr) {
				cout << "\nError: Ya existe un cliente registrado con el RUT \"" << rut << "\".\n";
				char reintentar;
				cout << "¿Desea reingresar los datos? (s/n): ";
				cin >> reintentar;
				if (reintentar != 's' && reintentar != 'S') {
					cout << "Operacion cancelada. El cliente no fue registrado.\n";
					return;
				}
				continue;
			}

			if (ctrl.buscarClientePorCorreo(correo) != nullptr) {
				cout << "\nError: Ya existe un cliente registrado con el correo \"" << correo << "\".\n";
				char reintentar;
				cout << "¿Desea reingresar los datos? (s/n): ";
				cin >> reintentar;
				if (reintentar != 's' && reintentar != 'S') {
					cout << "Operacion cancelada. El cliente no fue registrado.\n";
					return;
				}
				continue;
			}

			cout << "\nContrasena (para que el cliente acceda al sistema y califique productos): ";
			cin >> contrasena;

			cout << "\n========== RESUMEN DEL REGISTRO ==========\n";
			cout << "RUT: " << rut << endl;
			cout << "Nombre Completo: " << nombre << " " << apellido << endl;
			cout << "Direccion: " << direccion << endl;
			cout << "Correo: " << correo << endl;
			cout << "Contrasena: " << string(contrasena.length(), '*') << endl;
			cout << "==========================================\n";

			char confirmar;
			cout << "¿Confirma el alta del cliente? (s/n): ";
			cin >> confirmar;

			if (confirmar != 's' && confirmar != 'S') {
				cout << "Operacion cancelada. El cliente no fue registrado.\n";
				return;
			}

			Cliente* nuevo = ctrl.registrarCliente(rut, nombre, apellido, direccion, correo, contrasena);
			if (nuevo != nullptr) {
				cout << "Cliente \"" << nombre << " " << apellido << "\" registrado con exito.\n";
			} else {
				cout << "Error: No se pudo registrar el cliente. Verifique los datos e intente nuevamente.\n";
			}
			break;
		}
	} catch (const exception& ex) {
		cout << "Error al registrar cliente: " << ex.what() << endl;
	} catch (...) {
		cout << "Error al registrar cliente." << endl;
	}
}

void MenuEmpleado::modificarCliente() {
	try {
		string rut, nuevoNombre, nuevoApellido, nuevaDireccion, nuevoCorreo;

		cout << "RUT del cliente a modificar: "; cin >> rut;
		Cliente* cliente = ctrl.buscarCliente(rut);
		if (cliente == nullptr) {
			cout << "Error: No existe un cliente con el RUT \"" << rut << "\"." << endl;
			return;
		}

		cout << "\n--- Datos actuales del cliente (RUT: " << rut << ") ---\n";
		cout << "Nombre Completo: " << cliente->getNombre() << " " << cliente->getApellido() << endl;
		cout << "Direccion: " << cliente->getDireccion() << endl;
		cout << "Correo: " << cliente->getCorreo() << endl;
		cout << "------------------------------------------------------\n";

		while (true) {
			cout << "\nNuevo Nombre (deje vacio para mantener \"" << cliente->getNombre() << "\"): ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nuevoNombre);
			if (nuevoNombre.empty()) nuevoNombre = cliente->getNombre();

			cout << "Nuevo Apellido (deje vacio para mantener \"" << cliente->getApellido() << "\"): ";
			getline(cin, nuevoApellido);
			if (nuevoApellido.empty()) nuevoApellido = cliente->getApellido();

			cout << "Nueva Direccion (deje vacio para mantener actual): ";
			getline(cin, nuevaDireccion);
			if (nuevaDireccion.empty()) nuevaDireccion = cliente->getDireccion();

			cout << "Nuevo Correo (deje vacio para mantener \"" << cliente->getCorreo() << "\"): ";
			getline(cin, nuevoCorreo);
			if (nuevoCorreo.empty()) nuevoCorreo = cliente->getCorreo();

			if (nuevoCorreo != cliente->getCorreo()) {
				Cliente* existenteCorreo = ctrl.buscarClientePorCorreo(nuevoCorreo);
				if (existenteCorreo != nullptr) {
					cout << "\nError: El correo \"" << nuevoCorreo << "\" ya pertenece a otro cliente.\n";
					char reintentar;
					cout << "¿Desea corregir los datos? (s/n): ";
					cin >> reintentar;
					if (reintentar != 's' && reintentar != 'S') {
						cout << "Operacion cancelada. No se realizaron cambios." << endl;
						return;
					}
					continue;
				}
			}

			cout << "\n========== RESUMEN DE LA MODIFICACION ==========\n";
			cout << "RUT: " << rut << " (no modificable)\n";
			cout << "Nombre Completo: " << cliente->getNombre() << " " << cliente->getApellido()
				 << " -> " << nuevoNombre << " " << nuevoApellido << "\n";
			cout << "Direccion: " << cliente->getDireccion() << " -> " << nuevaDireccion << "\n";
			cout << "Correo: " << cliente->getCorreo() << " -> " << nuevoCorreo << "\n";
			cout << "================================================\n";

			char confirmar;
			cout << "¿Confirma la modificacion del cliente? (s/n): ";
			cin >> confirmar;

			if (confirmar != 's' && confirmar != 'S') {
				cout << "Modificacion cancelada. Se conservan los datos originales." << endl;
				return;
			}

			if (ctrl.modificaCliente(cliente, nuevoNombre, nuevoApellido, nuevaDireccion, nuevoCorreo)) {
				cout << "Cliente modificado con exito." << endl;
			} else {
				cout << "Error: No se pudo modificar el cliente. Verifique los datos e intente nuevamente." << endl;
			}
			break;
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
		cout << "¿La venta es a un cliente registrado? (s/n): ";
		cin >> esRegistrado;

		bool clienteRegistrado = (esRegistrado == 's' || esRegistrado == 'S');
		string rutCliente;
		if (clienteRegistrado) {
			cout << "RUT del cliente: ";
			cin >> rutCliente;

			Cliente* cliente = ctrl.buscarCliente(rutCliente);
			if (cliente == nullptr) {
				cout << "Error: No existe un cliente con el RUT \"" << rutCliente << "\" en el sistema." << endl;
				return;
			}
		}

		struct LineaTemp {
			int codigoProducto;
			string nombreProducto;
			int cantidad;
			float precioUnitario;
		};
		vector<LineaTemp> lineasTemp;
		while (true) {
			int codigoProducto;
			int cantidad;

			cout << "\n--- Nueva linea de detalle ---\n";
			cout << "Ingrese codigo del producto (0 para finalizar): ";
			cin >> codigoProducto;
			if (codigoProducto == 0) break;

			DetalleProductoEmpleadoVista prodInfo = ctrl.obtenerDetalleProductoEmpleadoVista(codigoProducto);
			if (!prodInfo.productoExiste) {
				cout << "Error: No existe un producto con el codigo " << codigoProducto << ".\n";
				continue;
			}

			cout << "Producto: " << prodInfo.nombre << "\n";
			cout << "Stock disponible: " << prodInfo.stockActual << "\n";
			cout << "Precio unitario actual: $" << prodInfo.precioVentaUnitario << "\n";

			cout << "Cantidad deseada: ";
			cin >> cantidad;

			if (cantidad <= 0) {
				cout << "Error: La cantidad debe ser mayor a cero. No se agrego la linea.\n";
				continue;
			}

			if (cantidad > prodInfo.stockActual) {
				cout << "Error: Stock insuficiente. Stock disponible: " << prodInfo.stockActual
					 << ". No se puede agregar esta linea.\n";
				continue;
			}

			LineaTemp linea = {codigoProducto, prodInfo.nombre, cantidad, prodInfo.precioVentaUnitario};
			lineasTemp.push_back(linea);
			cout << "Linea agregada: " << linea.nombreProducto << " x" << linea.cantidad
				 << " = $" << (linea.cantidad * linea.precioUnitario) << "\n";
		}

		if (lineasTemp.empty()) {
			cout << "Error: Debe ingresar al menos una linea de detalle para registrar la venta." << endl;
			return;
		}

		float totalCalculado = 0;
		{
			Cliente* cli = clienteRegistrado ? ctrl.buscarCliente(rutCliente) : nullptr;
			string nombreCliente = (cli != nullptr) ? (cli->getNombre() + " " + cli->getApellido()) : "";
			cout << "\n========== RESUMEN DE LA VENTA ==========\n";
			cout << "Cliente: " << (clienteRegistrado ? nombreCliente : "Ocasional")
				 << (clienteRegistrado ? " (RUT: " + rutCliente + ")" : "") << "\n";
		}
		cout << "-----------------------------------------\n";
		for (const auto& linea : lineasTemp) {
			float subtotal = linea.cantidad * linea.precioUnitario;
			totalCalculado += subtotal;
			cout << "  " << linea.nombreProducto << " (codigo=" << linea.codigoProducto << ")\n";
			cout << "    Cantidad: " << linea.cantidad
				 << " | P. Unitario: $" << linea.precioUnitario
				 << " | Subtotal: $" << subtotal << "\n";
		}
		cout << "-----------------------------------------\n";
		cout << "Total calculado: $" << totalCalculado << "\n";
		cout << "=========================================\n";

		char confirmar;
		cout << "¿Confirma el registro de esta venta? (s/n): ";
		cin >> confirmar;
		if (confirmar != 's' && confirmar != 'S') {
			cout << "Venta cancelada. No se registraron cambios." << endl;
			return;
		}

		vector<pair<int, int>> lineasParaRegistro;
		for (const auto& lt : lineasTemp) {
			lineasParaRegistro.push_back({lt.codigoProducto, lt.cantidad});
		}
		ResultadoRegistroVenta res = ctrl.registrarVentaCompleta(rutCliente, clienteRegistrado, lineasParaRegistro);
		if (!res.exito) {
			if (!res.clienteValido) cout << "Error: No existe un cliente con el RUT ingresado." << endl;
			else if (!res.lineasValidas) cout << "Error: Debe ingresar al menos una linea valida." << endl;
			else if (!res.productoValido) cout << "Error: Uno o mas productos no existen." << endl;
			else if (!res.stockSuficiente) cout << "Error: No hay stock suficiente para uno o mas productos." << endl;
			else cout << "Error al registrar la venta." << endl;
			return;
		}

		cout << "\nVenta registrada con exito." << endl;
		cout << "El stock de cada producto involucrado ha sido descontado automaticamente." << endl;
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
		vector<ProveedorInfoVista> proveedores = ctrl.listarProveedoresVista();
		if (proveedores.empty()) {
			cout << "\nNo hay proveedores registrados en el sistema." << endl;
			return;
		}

		cout << "\n--- Proveedores registrados ---\n";
		for (size_t i = 0; i < proveedores.size(); ++i) {
			cout << "  " << (i + 1) << ". " << proveedores[i].empresa
				 << " (RUT: " << proveedores[i].rut << ")" << endl;
		}

		string rutProveedor;
		cout << "\nIngrese el RUT del proveedor a seleccionar (o 0 para cancelar): ";
		cin >> rutProveedor;
		if (rutProveedor == "0") {
			cout << "Operacion cancelada." << endl;
			return;
		}

		ResultadoProductosProveedor productosProveedor = ctrl.obtenerProductosProveedor(rutProveedor);
		if (!productosProveedor.proveedorExiste) {
			cout << "Error: No existe un proveedor con ese RUT." << endl;
			return;
		}
		if (productosProveedor.productos.empty()) {
			cout << "El proveedor " << productosProveedor.empresaProveedor << " no tiene productos asociados." << endl;
			return;
		}

		cout << "\nProductos que abastece " << productosProveedor.empresaProveedor << ":" << endl;
		for (const auto& p : productosProveedor.productos) {
			cout << "  Codigo: " << p.codigoProducto
				 << " | " << p.nombreProducto
				 << " | Precio de compra: $" << p.precioCompraPactado
				 << " | Tiempo de entrega: " << p.tiempoEntregaDias << " dias" << endl;
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

		if (lineas.empty()) {
			cout << "Debe ingresar al menos una linea para emitir la orden." << endl;
			return;
		}

		// 4. Calcular resumen ANTES de pedir confirmacion
		time_t t = time(nullptr);
		tm* now = localtime(&t);
		DTFecha fechaActual(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

		float totalEstimado = 0;
		cout << "\n========== RESUMEN DE LA ORDEN DE COMPRA ==========\n";
		cout << "Proveedor: " << productosProveedor.empresaProveedor << " (RUT: " << rutProveedor << ")\n";
		cout << "Fecha de emision: " << fechaActual.getDia() << "/" << fechaActual.getMes() << "/" << fechaActual.getAnio() << "\n";
		cout << "---------------------------------------------------\n";
		for (size_t i = 0; i < lineas.size(); ++i) {
			int codigo = lineas[i].first;
			int cant = lineas[i].second;
			float precio = 0;
			string nombreProd = "(desconocido)";
			for (const auto& p : productosProveedor.productos) {
				if (p.codigoProducto == codigo) {
					precio = static_cast<float>(p.precioCompraPactado);
					nombreProd = p.nombreProducto;
					break;
				}
			}
			float subtotal = precio * cant;
			totalEstimado += subtotal;
			cout << "  " << (i + 1) << ". " << nombreProd << " (cod=" << codigo << ")\n";
			cout << "     Cantidad: " << cant << " | Precio unitario: $" << precio
				 << " | Subtotal: $" << subtotal << "\n";
		}
		cout << "---------------------------------------------------\n";
		cout << "Cantidad de lineas: " << lineas.size() << "\n";
		cout << "Total estimado: $" << totalEstimado << "\n";
		cout << "===================================================\n";

		char confirmar;
		cout << "\n¿Confirma la emision de esta orden de compra? (s/n): ";
		cin >> confirmar;
		if (confirmar != 's' && confirmar != 'S') {
			cout << "Operacion cancelada. No se registro la orden." << endl;
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

		cout << "\nLa orden de compra ha sido registrada con estado 'Pendiente'." << endl;
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
				 << " | Nombre: " << p.nombre
				 << " | Puntaje promedio: " << p.puntajePromedio << "/5" << endl;
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
				 << " " << cal.hora.getHora() << ":" << cal.hora.getMinuto() << ":" << cal.hora.getSegundo()
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
		cout << "\nIngrese el RUT del cliente (0 para cancelar): ";
		cin >> rut;
		if (rut == "0") {
			cout << "Operacion cancelada." << endl;
			return;
		}

		// Validar que el cliente exista
		{
			Cliente* cliente = ctrl.buscarCliente(rut);
			if (cliente == nullptr) {
				cout << "Error: No existe un cliente con el RUT ingresado." << endl;
				return;
			}
		}

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;

		while (true) {
			cout << "\nFecha de inicio del periodo:" << endl;
			cout << "Dia: "; cin >> diaInicio;
			cout << "Mes: "; cin >> mesInicio;
			cout << "Anio: "; cin >> anioInicio;

			cout << "\nFecha de fin del periodo:" << endl;
			cout << "Dia: "; cin >> diaFin;
			cout << "Mes: "; cin >> mesFin;
			cout << "Anio: "; cin >> anioFin;

			if (DTFecha(diaInicio, mesInicio, anioInicio) <= DTFecha(diaFin, mesFin, anioFin)) {
				break;
			}

			cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << endl;
			char reintentar;
			cout << "¿Desea reingresar las fechas? (s/n): ";
			cin >> reintentar;
			if (reintentar != 's' && reintentar != 'S') {
				cout << "Operacion cancelada." << endl;
				return;
			}
		}

		DTFecha inicio(diaInicio, mesInicio, anioInicio);
		DTFecha fin(diaFin, mesFin, anioFin);

		ResultadoFacturacionConsulta consulta = ctrl.consultarMontoFacturadoValidado(rut, inicio, fin);
		ResultadoFacturacion res = consulta.datos;

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

		// Validar que el producto exista antes de pedir fechas
		{
			ProductoEmpleadoInfo prodEncontrado;
			bool existe = false;
			for (const ProductoEmpleadoInfo& p : productos) {
				if (p.codigo == codigoProducto) {
					prodEncontrado = p;
					existe = true;
					break;
				}
			}
			if (!existe) {
				cout << "Error: No existe un producto con el codigo ingresado." << endl;
				return;
			}
		}

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;

		while (true) {
			cout << "\nFecha de inicio del periodo:" << endl;
			cout << "Dia: "; cin >> diaInicio;
			cout << "Mes: "; cin >> mesInicio;
			cout << "Anio: "; cin >> anioInicio;

			cout << "\nFecha de fin del periodo:" << endl;
			cout << "Dia: "; cin >> diaFin;
			cout << "Mes: "; cin >> mesFin;
			cout << "Anio: "; cin >> anioFin;

			if (DTFecha(diaInicio, mesInicio, anioInicio) <= DTFecha(diaFin, mesFin, anioFin)) {
				break;
			}

			cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << endl;
			char reintentar;
			cout << "¿Desea reingresar las fechas? (s/n): ";
			cin >> reintentar;
			if (reintentar != 's' && reintentar != 'S') {
				cout << "Operacion cancelada." << endl;
				return;
			}
		}

		DTFecha inicio(diaInicio, mesInicio, anioInicio);
		DTFecha fin(diaFin, mesFin, anioFin);

		ResultadoUnidadesVendidasConsulta consulta = ctrl.consultarUnidadesVendidasValidado(codigoProducto, inicio, fin);

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
