//
// Created by facun on 2/6/2026.
//


#include "MenuAdministrador.h"
#include <limits>
#include <algorithm>
#include <exception>

MenuAdministrador::MenuAdministrador(AdminController& controller, AuthController* auth,
									 EmpleadoController* empleados,
									 VentaController* ventas)
	: ctrl(controller), authCtrl(auth), empleadoCtrl(empleados), ventaCtrl(ventas) {}

void MenuAdministrador::mostrar() {
	while (true) {
		try {
			std::cout << "\n--- Menu Administrador ---\n";
			if (authCtrl && authCtrl->haySesionActiva()) {
				Sesion s = authCtrl->getSesionActual();
				std::cout << "Usuario: " << s.nombre << " (" << s.rol << ")\n";
			}
			std::cout << "1. Productos\n";
			std::cout << "2. Categorias\n";
			std::cout << "3. Empleados\n";
			std::cout << "4. Proveedores\n";
			std::cout << "5. Consultar monto facturado a cliente\n";
			std::cout << "0. Cerrar sesion\n";
			int op;
			std::cout << "Seleccione una opcion: ";
			std::cin >> op;
			switch (op) {
				case 1: menuProductos(); break;
				case 2: menuCategorias(); break;
				case 3: menuEmpleados(); break;
				case 4: menuProveedores(); break;
				case 5: consultarMontoFacturadoCliente(); break;
				case 0:
					if (authCtrl) authCtrl->cerrarSesion();
					std::cout << "Sesion cerrada." << std::endl;
					return;
				default: std::cout << "Opcion invalida." << std::endl;
			}
		} catch (const std::exception& ex) {
			std::cout << "Error inesperado: " << ex.what() << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		} catch (...) {
			std::cout << "Error inesperado en la operacion." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

void MenuAdministrador::consultarMontoFacturadoCliente() {
	try {
		if (empleadoCtrl == nullptr) {
			std::cout << "Error: Controlador de empleados no disponible." << std::endl;
			return;
		}

		std::string rut;
		std::cout << "\nIngrese el RUT del cliente: ";
		std::cin >> rut;

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;

		// Solicitar fecha de inicio
		std::cout << "\nFecha de inicio del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaInicio;
		std::cout << "Mes: "; std::cin >> mesInicio;
		std::cout << "Anio: "; std::cin >> anioInicio;
		DTFecha inicio(diaInicio, mesInicio, anioInicio);

		// Solicitar fecha de fin
		std::cout << "\nFecha de fin del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaFin;
		std::cout << "Mes: "; std::cin >> mesFin;
		std::cout << "Anio: "; std::cin >> anioFin;
		DTFecha fin(diaFin, mesFin, anioFin);
		ResultadoFacturacionConsulta consulta = empleadoCtrl->consultarMontoFacturadoValidado(rut, inicio, fin);
		if (!consulta.rangoFechasValido) {
			std::cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << std::endl;
			return;
		}
		if (!consulta.datos.clienteExiste) {
			std::cout << "Error: No existe un cliente con el RUT ingresado." << std::endl;
			return;
		}
		ResultadoFacturacion res = consulta.datos;
		Cliente* cliente = empleadoCtrl->buscarCliente(rut);

		// Mostrar resultados
		std::cout << "\n--- Resultados de facturacion ---" << std::endl;
		std::string nombreCliente = (cliente != nullptr) ? (cliente->getNombre() + " " + cliente->getApellido()) : "(desconocido)";
		std::cout << "Cliente: " << nombreCliente << " (RUT: " << rut << ")" << std::endl;
		std::cout << "Periodo: " << diaInicio << "/" << mesInicio << "/" << anioInicio
				 << " - " << diaFin << "/" << mesFin << "/" << anioFin << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
		std::cout << "Monto total facturado: $" << res.montoTotal << std::endl;
		std::cout << "Cantidad de ventas: " << res.cantidadVentas << std::endl;
		if (res.cantidadVentas > 0) {
			std::cout << "Monto promedio por venta: $" << res.montoPromedio << std::endl;
		}
		std::cout << "------------------------------------------------" << std::endl;
	} catch (const std::exception& ex) {
		std::cout << "Error en la consulta de facturacion: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en la consulta de facturacion." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::consultarCalificacionesProducto() {
	try {
		if (ventaCtrl == nullptr) {
			std::cout << "Error: Controlador de ventas no disponible." << std::endl;
			return;
		}

		std::vector<Producto*> productos = ctrl.listarProductos();
		if (productos.empty()) {
			std::cout << "\nNo hay productos registrados en el catalogo." << std::endl;
			return;
		}

		std::cout << "\n--- Catalogo de productos ---\n";
		for (Producto* p : productos) {
			if (p != nullptr) {
				std::cout << "  Codigo: " << p->getCodigo()
						 << " | Nombre: " << p->getNombre()
						 << " | Puntaje promedio: " << p->getPuntajePromedio() << "/5"
						 << " (" << p->getCantidadCalificaciones() << " calificaciones)" << std::endl;
			}
		}

		int codigo;
		std::cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
		std::cin >> codigo;

		if (codigo == 0) {
			std::cout << "Operacion cancelada." << std::endl;
			return;
		}

		Producto* producto = ctrl.buscarProducto(codigo);
		if (producto == nullptr) {
			std::cout << "No existe un producto con el codigo ingresado." << std::endl;
			return;
		}

		std::vector<Calificacion*> calificaciones = ventaCtrl->listarCalificacionesDeProducto(codigo);

		if (calificaciones.empty()) {
			std::cout << "\nEl producto \"" << producto->getNombre() << "\" no tiene calificaciones registradas." << std::endl;
			std::cout << "Puntaje promedio: " << producto->getPuntajePromedio() << "/5" << std::endl;
			return;
		}

		std::cout << "\n--- Calificaciones de \"" << producto->getNombre() << "\" ---\n";
		std::cout << "Puntaje promedio general: " << producto->getPuntajePromedio() << "/5"
				 << " (basado en " << calificaciones.size() << " calificaciones)" << std::endl;
		std::cout << "--------------------------------------------------------\n";

		for (size_t i = 0; i < calificaciones.size(); ++i) {
			Calificacion* cal = calificaciones[i];
			if (cal == nullptr) continue;

			DTFecha f = cal->getFecha();
			std::string comentario = cal->getComentario();
			Cliente* cli = cal->getClienteCalificador();

			std::cout << (i + 1) << ") Fecha: " << f.getDia() << "/" << f.getMes() << "/" << f.getAnio()
					 << " | Puntaje: " << static_cast<int>(cal->getPuntaje()) << "/5"
					 << " | Cliente RUT: " << (cli != nullptr ? cli->getRut() : "N/A")
					 << " | Comentario: " << (comentario.empty() ? "(sin comentario)" : comentario) << std::endl;
		}
		std::cout << "--------------------------------------------------------\n";
	} catch (const std::exception& ex) {
		std::cout << "Error en la consulta de calificaciones: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en la consulta de calificaciones." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::consultarStockProductos() {
	try {
		char filtrar;
		std::cout << "\n¿Desea filtrar por categoria? (s/n): ";
		std::cin >> filtrar;

		std::vector<Producto*> productos;
		if (filtrar == 's' || filtrar == 'S') {
			auto categorias = ctrl.listarCategorias();
			if (categorias.empty()) {
				std::cout << "\nNo hay categorias registradas en el sistema." << std::endl;
				return;
			}

			std::cout << "\n--- Categorias disponibles ---\n";
			for (size_t i = 0; i < categorias.size(); ++i) {
				if (categorias[i] != nullptr) {
					std::cout << "  " << (i + 1) << ". " << categorias[i]->getNombre()
						 << ": " << categorias[i]->getDescripcion() << std::endl;
				}
			}

			int opcion;
			std::cout << "\nSeleccione una categoria (1-" << categorias.size() << "): ";
			std::cin >> opcion;

			if (opcion < 1 || opcion > static_cast<int>(categorias.size())) {
				std::cout << "Opcion invalida." << std::endl;
				return;
			}

			std::string nombreCategoria = categorias[opcion - 1]->getNombre();
			productos = ctrl.listarProductosPorCategoria(nombreCategoria);

			if (productos.empty()) {
				std::cout << "\nNo hay productos en la categoria \"" << nombreCategoria << "\"." << std::endl;
				return;
			}
		} else {
			productos = ctrl.listarProductos();
			if (productos.empty()) {
				std::cout << "\nNo hay productos registrados en el catalogo." << std::endl;
				return;
			}
		}

		std::cout << "\n--- Stock actual de productos ---\n";
		std::cout << "Codigo  | Nombre                          | Categoria               | Stock Actual | Stock Minimo\n";
		std::cout << "--------|---------------------------------|-------------------------|--------------|--------------\n";

		for (Producto* p : productos) {
			if (p == nullptr) continue;

			std::string nombreCat = p->getCategoria() != nullptr ? p->getCategoria()->getNombre() : "(sin categoria)";
			std::string strStock = std::to_string(p->getStock());
			std::string strStockMin = std::to_string(p->getStockMinimo());
			std::string strCodigo = std::to_string(p->getCodigo());

			std::cout << strCodigo;
			for (int i = static_cast<int>(strCodigo.length()); i < 8; ++i) std::cout << " ";
			std::cout << "| " << p->getNombre();
			for (int i = static_cast<int>(p->getNombre().length()); i < 31; ++i) std::cout << " ";
			std::cout << "| " << nombreCat;
			for (int i = static_cast<int>(nombreCat.length()); i < 23; ++i) std::cout << " ";
			std::cout << "| " << strStock;
			for (int i = static_cast<int>(strStock.length()); i < 12; ++i) std::cout << " ";
			std::cout << "| " << strStockMin << std::endl;
		}
		std::cout << "---------------------------------------------------------------------\n";
	} catch (const std::exception& ex) {
		std::cout << "Error en la consulta de stock: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en la consulta de stock." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::consultarProductosStockBajo() {
	try {
		auto todos = ctrl.listarProductos();
		if (todos.empty()) {
			std::cout << "\nNo hay productos registrados en el catalogo." << std::endl;
			return;
		}

		char ordenar;
		std::cout << "\n¿Ordenar por diferencia ascendente (mas criticos primero)? (s/n): ";
		std::cin >> ordenar;
		std::vector<Producto*> productosBajos = ctrl.listarProductosConStockBajo(ordenar == 's' || ordenar == 'S');
		if (productosBajos.empty()) {
			std::cout << "\nNo hay productos con stock por debajo del minimo configurado." << std::endl;
			return;
		}

		std::cout << "\n--- Productos con stock bajo minimo ---\n";
		std::cout << "Codigo  | Nombre                          | Categoria               | Stock Actual | Stock Minimo | Diferencia\n";
		std::cout << "--------|---------------------------------|-------------------------|--------------|--------------|------------\n";

		for (Producto* p : productosBajos) {
			if (p == nullptr) continue;

			std::string nombreCat = p->getCategoria() != nullptr ? p->getCategoria()->getNombre() : "(sin categoria)";
			std::string strCodigo = std::to_string(p->getCodigo());
			std::string strStock = std::to_string(p->getStock());
			std::string strStockMin = std::to_string(p->getStockMinimo());
			int diferencia = p->getStockMinimo() - p->getStock();
			std::string strDiff = std::to_string(diferencia);

			std::cout << strCodigo;
			for (int i = static_cast<int>(strCodigo.length()); i < 8; ++i) std::cout << " ";
			std::cout << "| " << p->getNombre();
			for (int i = static_cast<int>(p->getNombre().length()); i < 31; ++i) std::cout << " ";
			std::cout << "| " << nombreCat;
			for (int i = static_cast<int>(nombreCat.length()); i < 23; ++i) std::cout << " ";
			std::cout << "| " << strStock;
			for (int i = static_cast<int>(strStock.length()); i < 12; ++i) std::cout << " ";
			std::cout << "| " << strStockMin;
			for (int i = static_cast<int>(strStockMin.length()); i < 12; ++i) std::cout << " ";
			std::cout << "| " << strDiff;
			std::cout << " (faltan " << diferencia << ")" << std::endl;
		}
		std::cout << "----------------------------------------------------------------------------\n";
	} catch (const std::exception& ex) {
		std::cout << "Error en la consulta de stock bajo: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en la consulta de stock bajo." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::menuProductos() {
	try {
		while (true) {
			std::cout << "\n--- Productos ---\n";
			std::cout << "1. Crear producto\n";
			std::cout << "2. Listar productos\n";
			std::cout << "3. Buscar producto por codigo\n";
			std::cout << "4. Eliminar producto por codigo\n";
			std::cout << "5. Modificar producto\n";
			std::cout << "6. Consultar calificaciones de producto\n";
			std::cout << "7. Consultar stock actual de productos\n";
			std::cout << "8. Consultar productos con stock bajo minimo\n";
			std::cout << "9. Consultar unidades vendidas de un producto\n";
			std::cout << "10. Consultar informacion detallada de un producto\n";
			std::cout << "0. Volver\n";
			int op;
			std::cout << "Seleccione una opcion: ";
			std::cin >> op;
			if (op == 0) return;
			if (op == 1) {
				int codigo;
				std::string nombre;
				std::string descripcion;
				float precio;
				int stock;
				int stockMinimo;
				std::string categoria;
				std::cout << "Codigo: "; std::cin >> codigo;
				std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
				std::cout << "Descripcion: "; std::getline(std::cin, descripcion);
				std::cout << "Precio de venta unitario: "; std::cin >> precio;
				std::cout << "Stock inicial: "; std::cin >> stock;
				std::cout << "Stock minimo: "; std::cin >> stockMinimo;
				std::cout << "Nombre categoria: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, categoria);
				Producto* p = ctrl.crearProducto(codigo, nombre, descripcion, precio, stock, stockMinimo, categoria);
				std::cout << (p ? "Producto creado exitosamente." : "No se pudo crear el producto.") << std::endl;
			} else if (op == 2) {
				auto lista = ctrl.listarProductos();
				std::cout << "Productos:\n";
				for (auto p : lista) {
					if (p) std::cout << "- codigo=" << p->getCodigo() << " nombre=" << p->getNombre() << " precio=" << p->getPrecioVentaActual() << " stock=" << p->getStock() << '\n';
				}
			} else if (op == 3) {
				int codigo;
				std::cout << "Codigo: "; std::cin >> codigo;
				Producto* p = ctrl.buscarProducto(codigo);
				if (p) std::cout << "Encontrado: " << p->getNombre() << " (codigo=" << p->getCodigo() << ")\n";
				else std::cout << "Producto no encontrado." << std::endl;
			} else if (op == 4) {
				int codigo;
				std::cout << "Codigo: "; std::cin >> codigo;
				bool ok = ctrl.eliminarProducto(codigo, true);
				std::cout << (ok ? "Producto eliminado exitosamente." : "No se pudo eliminar el producto.") << std::endl;
			} else if (op == 6) {
				consultarCalificacionesProducto();
			} else if (op == 7) {
				consultarStockProductos();
			} else if (op == 8) {
				consultarProductosStockBajo();
			} else if (op == 9) {
				consultarUnidadesVendidasProducto();
			} else if (op == 10) {
				consultarInfoDetalladaProducto();
			} else if (op == 5) {
				int codigo;
				std::string nuevoNombre;
				std::string nuevaDescripcion;
				float nuevoPrecio;
				int nuevoStockMinimo;
				std::string categoria;
				std::cout << "Codigo: "; std::cin >> codigo;
				std::cout << "Nuevo nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nuevoNombre);
				std::cout << "Nueva descripcion: "; std::getline(std::cin, nuevaDescripcion);
				std::cout << "Nuevo precio de venta unitario: "; std::cin >> nuevoPrecio;
				std::cout << "Nuevo stock minimo: "; std::cin >> nuevoStockMinimo;
				std::cout << "Nombre categoria: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, categoria);
				bool ok = ctrl.modificarProducto(codigo, nuevoNombre, nuevaDescripcion, nuevoPrecio, categoria, nuevoStockMinimo);
				std::cout << (ok ? "Producto modificado exitosamente." : "No se pudo modificar el producto.") << std::endl;
			} else {
				std::cout << "Opcion invalida." << std::endl;
			}
		}
	} catch (const std::exception& ex) {
		std::cout << "Error en menu de productos: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en menu de productos." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::consultarInfoDetalladaProducto() {
	try {
		auto productos = ctrl.listarProductos();
		if (productos.empty()) {
			std::cout << "\nNo hay productos registrados en el catalogo." << std::endl;
			return;
		}

		std::cout << "\n--- Productos disponibles ---\n";
		for (Producto* p : productos) {
			if (p != nullptr) {
				std::cout << "  Codigo: " << p->getCodigo()
						 << " | Nombre: " << p->getNombre() << std::endl;
			}
		}

		int codigo;
		std::cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
		std::cin >> codigo;

		if (codigo == 0) {
			std::cout << "Operacion cancelada." << std::endl;
			return;
		}

		DetalleProductoAdmin detalle = ctrl.obtenerDetalleProductoAdmin(codigo);
		if (!detalle.productoExiste || detalle.producto == nullptr) {
			std::cout << "Error: No existe un producto con el codigo ingresado." << std::endl;
			return;
		}
		Producto* producto = detalle.producto;

		std::cout << "\n========== INFORMACION DETALLADA DEL PRODUCTO ==========\n";
		std::cout << "Codigo: " << producto->getCodigo() << std::endl;
		std::cout << "Nombre: " << producto->getNombre() << std::endl;
		std::cout << "Descripcion: " << producto->getDescripcion() << std::endl;

		std::string nombreCat = producto->getCategoria() != nullptr ? producto->getCategoria()->getNombre() : "(sin categoria)";
		std::cout << "Categoria: " << nombreCat << std::endl;

		std::cout << "Precio de venta unitario: $" << producto->getPrecioVentaActual() << std::endl;
		std::cout << "Stock actual: " << producto->getStock() << std::endl;
		std::cout << "Puntaje promedio: " << producto->getPuntajePromedio() << "/5"
				 << " (" << producto->getCantidadCalificaciones() << " calificaciones)" << std::endl;

		std::cout << "\n--- Datos de administracion ---\n";
		std::cout << "Stock minimo: " << producto->getStockMinimo() << std::endl;

		if (detalle.proveedores.empty()) {
			std::cout << "Proveedores: Ninguno (el producto no esta asociado a ningun proveedor)" << std::endl;
		} else {
			std::cout << "\nProveedores que abastecen este producto:" << std::endl;
			for (size_t i = 0; i < detalle.proveedores.size(); ++i) {
				const ProveedorProductoInfo& info = detalle.proveedores[i];
				std::cout << "  " << (i + 1) << ". Proveedor: " << info.empresaProveedor
						 << " (RUT: " << info.rutProveedor << ")" << std::endl;
				std::cout << "     Precio de compra vigente: $" << info.precioCompraPactado << std::endl;
				std::cout << "     Tiempo de entrega estimado: " << info.tiempoEntregaDias << " dias" << std::endl;
			}
		}
		std::cout << "========================================================\n";
	} catch (const std::exception& ex) {
		std::cout << "Error en la consulta de detalle de producto: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en la consulta de detalle de producto." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::consultarUnidadesVendidasProducto() {
	try {
		if (empleadoCtrl == nullptr) {
			std::cout << "Error: Controlador de empleados no disponible." << std::endl;
			return;
		}

		auto productos = ctrl.listarProductos();
		if (productos.empty()) {
			std::cout << "\nNo hay productos registrados en el catalogo." << std::endl;
			return;
		}

		std::cout << "\n--- Productos disponibles ---\n";
		for (Producto* p : productos) {
			if (p != nullptr) {
				std::cout << "  Codigo: " << p->getCodigo()
						 << " | Nombre: " << p->getNombre() << std::endl;
			}
		}

		int codigoProducto;
		std::cout << "\nIngrese el codigo del producto (0 para cancelar): ";
		std::cin >> codigoProducto;

		if (codigoProducto == 0) {
			std::cout << "Operacion cancelada." << std::endl;
			return;
		}

		Producto* producto = ctrl.buscarProducto(codigoProducto);

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;

		std::cout << "\nFecha de inicio del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaInicio;
		std::cout << "Mes: "; std::cin >> mesInicio;
		std::cout << "Anio: "; std::cin >> anioInicio;
		DTFecha inicio(diaInicio, mesInicio, anioInicio);

		std::cout << "\nFecha de fin del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaFin;
		std::cout << "Mes: "; std::cin >> mesFin;
		std::cout << "Anio: "; std::cin >> anioFin;
		DTFecha fin(diaFin, mesFin, anioFin);

		ResultadoUnidadesVendidasConsulta consulta = empleadoCtrl->consultarUnidadesVendidasValidado(codigoProducto, inicio, fin);
		if (!consulta.productoExiste) {
			std::cout << "Error: No existe un producto con el codigo ingresado." << std::endl;
			return;
		}
		if (!consulta.rangoFechasValido) {
			std::cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << std::endl;
			return;
		}
		ResultadoUnidadesVendidas res = consulta.datos;
		if (producto == nullptr) {
			producto = ctrl.buscarProducto(codigoProducto);
		}

		std::cout << "\n--- Resultados de unidades vendidas ---" << std::endl;
		std::cout << "Producto: " << producto->getNombre() << " (codigo=" << codigoProducto << ")" << std::endl;
		std::cout << "Periodo: " << diaInicio << "/" << mesInicio << "/" << anioInicio
				 << " - " << diaFin << "/" << mesFin << "/" << anioFin << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
		std::cout << "Total de unidades vendidas: " << res.totalUnidadesVendidas << std::endl;
		std::cout << "Cantidad de ventas en las que participo: " << res.cantidadVentas << std::endl;
		if (res.totalUnidadesVendidas > 0) {
			std::cout << "Precio unitario promedio: $" << res.precioUnitarioPromedio << std::endl;
		}
		std::cout << "------------------------------------------------" << std::endl;
	} catch (const std::exception& ex) {
		std::cout << "Error en la consulta de unidades vendidas: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en la consulta de unidades vendidas." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::menuCategorias() {
	try {
		while (true) {
			std::cout << "\n--- Categorias ---\n";
			std::cout << "1. Crear categoria\n";
			std::cout << "2. Listar categorias\n";
			std::cout << "3. Buscar categoria por nombre\n";
			std::cout << "4. Eliminar categoria\n";
			std::cout << "5. Asignar producto a categoria\n";
			std::cout << "6. Modificar categoria\n";
			std::cout << "0. Volver\n";
			int op;
			std::cout << "Seleccione una opcion: ";
			std::cin >> op;
			if (op == 0) return;
			if (op == 1) {
				std::string nombre;
				std::string descripcion;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nombre: "; std::getline(std::cin, nombre);
				std::cout << "Descripcion: "; std::getline(std::cin, descripcion);
				auto c = ctrl.crearCategoria(nombre, descripcion);
				std::cout << (c ? "Categoria creada exitosamente." : "No se pudo crear la categoria.") << std::endl;
			} else if (op == 2) {
				auto lista = ctrl.listarCategorias();
				std::cout << "Categorias:\n";
				for (auto c : lista) {
					if (c) std::cout << "- " << c->getNombre() << ": " << c->getDescripcion() << '\n';
				}
			} else if (op == 3) {
				std::string nombre;
				std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
				auto c = ctrl.buscarCategoria(nombre);
				if (c) std::cout << "Encontrada: " << c->getNombre() << " - " << c->getDescripcion() << '\n';
				else std::cout << "Categoria no encontrada." << std::endl;
			} else if (op == 4) {
				std::string nombre;
				std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
				bool ok = ctrl.eliminarCategoria(nombre);
				std::cout << (ok ? "Categoria eliminada." : "No se pudo eliminar.") << std::endl;
			} else if (op == 5) {
				int codigo;
				std::string nombreCategoria;
				std::cout << "Codigo producto: "; std::cin >> codigo;
				std::cout << "Nombre categoria: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombreCategoria);
				bool ok = ctrl.asignarProductoACategoria(codigo, nombreCategoria);
				std::cout << (ok ? "Producto asignado." : "No se pudo asignar.") << std::endl;
			} else if (op == 6) {
				std::string nombreActual;
				std::string nuevoNombre;
				std::string nuevaDescripcion;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nombre actual: "; std::getline(std::cin, nombreActual);
				std::cout << "Nuevo nombre: "; std::getline(std::cin, nuevoNombre);
				std::cout << "Nueva descripcion: "; std::getline(std::cin, nuevaDescripcion);
				bool ok = ctrl.modificarCategoria(nombreActual, nuevoNombre, nuevaDescripcion);
				std::cout << (ok ? "Categoria modificada exitosamente." : "No se pudo modificar la categoria.") << std::endl;
			} else {
				std::cout << "Opcion invalida." << std::endl;
			}
		}
	} catch (const std::exception& ex) {
		std::cout << "Error en menu de categorias: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en menu de categorias." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::menuEmpleados() {
	try {
		while (true) {
			std::cout << "\n--- Empleados ---\n";
			std::cout << "1. Alta de empleado\n";
			std::cout << "2. Listar empleados\n";
			std::cout << "3. Buscar empleado por correo\n";
			std::cout << "4. Eliminar empleado por correo\n";
			std::cout << "0. Volver\n";
			int op;
			std::cout << "Seleccione una opcion: ";
			std::cin >> op;
			if (op == 0) return;
			if (op == 1) {
				std::string nombre;
				std::string correo;
				std::string contrasena;
				int opcionRol;
				std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
				std::cout << "Correo: "; std::cin >> correo;
				std::cout << "Contrasena: "; std::cin >> contrasena;
				std::cout << "Rol (1=Empleado, 2=Administrador): "; std::cin >> opcionRol;
				ResultadoAltaEmpleado alta = ctrl.crearEmpleadoConRolOpcion(nombre, correo, contrasena, opcionRol);
				if (!alta.rolValido) std::cout << "No se pudo crear el empleado. Rol invalido." << std::endl;
				else if (!alta.correoDisponible) std::cout << "No se pudo crear el empleado. Correo ya registrado." << std::endl;
				else std::cout << (alta.exito ? "Empleado creado exitosamente." : "No se pudo crear el empleado.") << std::endl;
			} else if (op == 2) {
				auto lista = ctrl.listarEmpleados();
				std::cout << "Empleados:\n";
				for (auto e : lista) {
					if (e) std::cout << "- " << e->getNombre() << " (" << e->getCorreo() << ") - rol=" << e->getRol() << '\n';
				}
			} else if (op == 3) {
				std::string correo;
				std::cout << "Correo: "; std::cin >> correo;
				auto e = ctrl.buscarEmpleado(correo);
				if (e) std::cout << "Encontrado: " << e->getNombre() << " - " << e->getCorreo() << '\n';
				else std::cout << "Empleado no encontrado." << std::endl;
			} else if (op == 4) {
				std::string correo;
				std::cout << "Correo: "; std::cin >> correo;
				bool ok = ctrl.eliminarEmpleado(correo);
				std::cout << (ok ? "Empleado eliminado." : "No se pudo eliminar.") << std::endl;
			} else {
				std::cout << "Opcion invalida." << std::endl;
			}
		}
	} catch (const std::exception& ex) {
		std::cout << "Error en menu de empleados: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en menu de empleados." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void MenuAdministrador::menuProveedores() {
	try {
		while (true) {
			std::cout << "\n--- Proveedores ---\n";
			std::cout << "1. Crear proveedor\n";
			std::cout << "2. Listar proveedores\n";
			std::cout << "3. Buscar proveedor por RUT\n";
			std::cout << "4. Eliminar proveedor por RUT\n";
			std::cout << "5. Modificar proveedor\n";
			std::cout << "6. Asociar producto a proveedor\n";
			std::cout << "0. Volver\n";
			int op;
			std::cout << "Seleccione una opcion: ";
			std::cin >> op;
			if (op == 0) return;
			if (op == 1) {
				std::string rut;
				std::string empresa;
				std::string telefono;
				std::string contacto;
				std::cout << "RUT: "; std::cin >> rut;
				std::cout << "Empresa: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, empresa);
				std::cout << "Telefono: "; std::getline(std::cin, telefono);
				std::cout << "Contacto comercial: "; std::getline(std::cin, contacto);
				auto p = ctrl.crearProveedor(rut, empresa, telefono, contacto);
				std::cout << (p ? "Proveedor creado exitosamente." : "No se pudo crear el proveedor.") << std::endl;
			} else if (op == 2) {
				auto lista = ctrl.listarProveedores();
				std::cout << "Proveedores:\n";
				for (auto p : lista) {
					if (p) std::cout << "- " << p->getRut() << " - " << p->getEmpresa() << " contacto=" << p->getContactoComercial() << '\n';
				}
			} else if (op == 3) {
				std::string rut;
				std::cout << "RUT: "; std::cin >> rut;
				auto p = ctrl.buscarProveedor(rut);
				if (p) std::cout << "Encontrado: " << p->getEmpresa() << " (" << p->getRut() << ")\n";
				else std::cout << "Proveedor no encontrado." << std::endl;
			} else if (op == 4) {
				std::string rut;
				std::cout << "RUT: "; std::cin >> rut;
				bool ok = ctrl.eliminarProveedor(rut);
				std::cout << (ok ? "Proveedor eliminado." : "No se pudo eliminar.") << std::endl;
			} else if (op == 5) {
				std::string rut;
				std::string empresa;
				std::string telefono;
				std::string contacto;
				std::cout << "RUT: "; std::cin >> rut;
				std::cout << "Nueva empresa: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, empresa);
				std::cout << "Nuevo telefono: "; std::getline(std::cin, telefono);
				std::cout << "Nuevo contacto comercial: "; std::getline(std::cin, contacto);
				bool ok = ctrl.modificarProveedor(rut, empresa, telefono, contacto);
				std::cout << (ok ? "Proveedor modificado exitosamente." : "No se pudo modificar el proveedor.") << std::endl;
			} else if (op == 6) {
				std::string rut;
				int codigoProducto;
				int precioCompra;
				int tiempoEntrega;
				std::cout << "RUT proveedor: "; std::cin >> rut;
				std::cout << "Codigo producto: "; std::cin >> codigoProducto;
				std::cout << "Precio compra pactado: "; std::cin >> precioCompra;
				std::cout << "Tiempo entrega (dias): "; std::cin >> tiempoEntrega;
				ResultadoGestionProveedorProducto gestion = ctrl.gestionarAsociacionProveedorProducto(rut, codigoProducto, precioCompra, tiempoEntrega);
				if (!gestion.proveedorExiste) std::cout << "No se pudo registrar la asociacion. Proveedor inexistente." << std::endl;
				else if (!gestion.productoExiste) std::cout << "No se pudo registrar la asociacion. Producto inexistente." << std::endl;
				else if (!gestion.exito) std::cout << "No se pudo registrar la asociacion." << std::endl;
				else std::cout << (gestion.actualizada ? "Asociacion actualizada exitosamente." : "Asociacion registrada exitosamente.") << std::endl;
			} else {
				std::cout << "Opcion invalida." << std::endl;
			}
		}
	} catch (const std::exception& ex) {
		std::cout << "Error en menu de proveedores: " << ex.what() << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} catch (...) {
		std::cout << "Error en menu de proveedores." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}
