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
			if (!(std::cin >> op)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Entrada invalida. Por favor ingrese un numero." << std::endl;
				continue;
			}
			switch (op) {
				case 1: menuProductos(); break;
				case 2: menuCategorias(); break;
				case 3: menuEmpleados(); break;
				case 4: menuProveedores(); break;
				case 5: consultarMontoFacturadoCliente(); break;
				case 0: {
					char confirmar;
					std::cout << "¿Esta seguro de cerrar la sesion? (s/n): ";
					std::cin >> confirmar;
					if (confirmar == 's' || confirmar == 'S') {
						if (authCtrl) authCtrl->cerrarSesion();
						std::cout << "Sesion cerrada. Redirigiendo a la pantalla de inicio de sesion..." << std::endl;
						return;
					}
					std::cout << "Operacion cancelada. Sesion activa." << std::endl;
					break;
				}
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
		std::cout << "\nIngrese el RUT del cliente (0 para cancelar): ";
		std::cin >> rut;
		if (rut == "0") {
			std::cout << "Operacion cancelada." << std::endl;
			return;
		}

		// Validar que el cliente exista
		{
			Cliente* cliente = empleadoCtrl->buscarCliente(rut);
			if (cliente == nullptr) {
				std::cout << "Error: No existe un cliente con el RUT ingresado." << std::endl;
				return;
			}
		}

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;

		while (true) {
			std::cout << "\nFecha de inicio del periodo:" << std::endl;
			std::cout << "Dia: "; std::cin >> diaInicio;
			std::cout << "Mes: "; std::cin >> mesInicio;
			std::cout << "Anio: "; std::cin >> anioInicio;

			std::cout << "\nFecha de fin del periodo:" << std::endl;
			std::cout << "Dia: "; std::cin >> diaFin;
			std::cout << "Mes: "; std::cin >> mesFin;
			std::cout << "Anio: "; std::cin >> anioFin;

			if (DTFecha(diaInicio, mesInicio, anioInicio) <= DTFecha(diaFin, mesFin, anioFin)) {
				break;
			}

			std::cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << std::endl;
			char reintentar;
			std::cout << "¿Desea reingresar las fechas? (s/n): ";
			std::cin >> reintentar;
			if (reintentar != 's' && reintentar != 'S') {
				std::cout << "Operacion cancelada." << std::endl;
				return;
			}
		}

		DTFecha inicio(diaInicio, mesInicio, anioInicio);
		DTFecha fin(diaFin, mesFin, anioFin);

		ResultadoFacturacionConsulta consulta = empleadoCtrl->consultarMontoFacturadoValidado(rut, inicio, fin);
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
			DTHora h = cal->getHora();
			std::string comentario = cal->getComentario();
			Cliente* cli = cal->getClienteCalificador();

			std::cout << (i + 1) << ") Fecha: " << f.getDia() << "/" << f.getMes() << "/" << f.getAnio()
					 << " " << h.getHora() << ":" << h.getMinuto() << ":" << h.getSegundo()
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
			if (!(std::cin >> op)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Entrada invalida. Por favor ingrese un numero." << std::endl;
				continue;
			}
			if (op == 0) return;
			if (op == 1) {
				// Solicitar datos basicos con validacion de codigo unico
				int codigo;
				std::string nombre;
				std::string descripcion;
				float precio;
				int stock;
				int stockMinimo;
				std::string categoriaSeleccionada;

				// Paso 1: Codigo (validar que no exista ya, con opcion de cancelar)
				while (true) {
					std::cout << "Codigo unico del producto (0 para cancelar): ";
					std::cin >> codigo;
					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Entrada invalida. Intente nuevamente.\n";
						continue;
					}
					if (codigo == 0) {
						std::cout << "Operacion cancelada.\n";
						break;
					}
					Producto* existente = ctrl.buscarProducto(codigo);
					if (existente != nullptr) {
						std::cout << "Error: Ya existe un producto con el codigo " << codigo
								 << " (\"" << existente->getNombre() << "\"). Ingrese otro codigo.\n";
					} else {
						break;
					}
				}
				if (codigo == 0) break;

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nombre: "; std::getline(std::cin, nombre);
				std::cout << "Descripcion: "; std::getline(std::cin, descripcion);

				std::cout << "Precio de venta unitario: ";
				std::cin >> precio;
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Precio invalido. Se asignara $0.\n";
					precio = 0;
				}

				// Paso 2: Listar categorias existentes y seleccionar exactamente una
				{
					auto categorias = ctrl.listarCategorias();
					if (categorias.empty()) {
						std::cout << "\nNo hay categorias registradas. El producto se creara sin categoria.\n";
						categoriaSeleccionada = "";
					} else {
						std::cout << "\n--- Categorias disponibles ---\n";
						for (size_t i = 0; i < categorias.size(); ++i) {
							if (categorias[i] != nullptr) {
								std::cout << "  " << (i + 1) << ". " << categorias[i]->getNombre()
										 << ": " << categorias[i]->getDescripcion() << std::endl;
							}
						}
						int opcCat;
						while (true) {
							std::cout << "Seleccione una categoria (1-" << categorias.size() << ", 0=sin categoria): ";
							std::cin >> opcCat;
							if (std::cin.fail()) {
								std::cin.clear();
								std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
								std::cout << "Entrada invalida.\n";
								continue;
							}
							if (opcCat == 0) {
								categoriaSeleccionada = "";
								break;
							}
							if (opcCat >= 1 && opcCat <= static_cast<int>(categorias.size())) {
								categoriaSeleccionada = categorias[opcCat - 1]->getNombre();
								break;
							}
							std::cout << "Opcion invalida.\n";
						}
					}
				}

				// Paso 3: Stock inicial y stock minimo configurable
				std::cout << "Stock inicial: "; std::cin >> stock;
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Stock invalido. Se asignara 0.\n";
					stock = 0;
				}
				std::cout << "Stock minimo (alerta de reposicion): "; std::cin >> stockMinimo;
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Stock minimo invalido. Se asignara 0.\n";
					stockMinimo = 0;
				}

				// Paso 4: Mostrar resumen y dar opcion de confirmar o cancelar
				std::cout << "\n========== RESUMEN DEL PRODUCTO ==========\n";
				std::cout << "Codigo: " << codigo << "\n";
				std::cout << "Nombre: " << nombre << "\n";
				std::cout << "Descripcion: " << descripcion << "\n";
				std::cout << "Precio unitario: $" << precio << "\n";
				std::cout << "Categoria: " << (categoriaSeleccionada.empty() ? "(sin categoria)" : categoriaSeleccionada) << "\n";
				std::cout << "Stock inicial: " << stock << "\n";
				std::cout << "Stock minimo: " << stockMinimo << "\n";
				std::cout << "==========================================\n";

				char confirmar;
				std::cout << "¿Confirma el alta del producto? (s/n): ";
				std::cin >> confirmar;

				if (confirmar == 's' || confirmar == 'S') {
					Producto* p = ctrl.crearProducto(codigo, nombre, descripcion, precio, stock, stockMinimo, categoriaSeleccionada);
					if (p != nullptr) {
						std::cout << "Producto \"" << nombre << "\" dado de alta exitosamente en el catalogo.\n";
					} else {
						std::cout << "No se pudo crear el producto. Verifique los datos e intente nuevamente.\n";
					}
				} else {
					std::cout << "Operacion cancelada. El producto no fue dado de alta.\n";
				}
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
				// 1. Listar todos los productos (codigo, nombre)
				auto todosProductos = ctrl.listarProductos();
				if (todosProductos.empty()) {
					std::cout << "\nNo hay productos registrados en el catalogo.\n";
					break;
				}
				std::cout << "\n--- Productos del catalogo ---\n";
				for (Producto* p : todosProductos) {
					if (p != nullptr) {
						std::cout << "  Codigo: " << p->getCodigo()
								 << " | Nombre: " << p->getNombre() << std::endl;
					}
				}
				std::cout << "------------------------------\n";

				// 2. Seleccionar producto por codigo
				int codigo;
				std::cout << "\nIngrese el codigo del producto a eliminar (0 para cancelar): ";
				std::cin >> codigo;
				if (codigo == 0) {
					std::cout << "Operacion cancelada.\n";
					break;
				}

				Producto* producto = ctrl.buscarProducto(codigo);
				if (producto == nullptr) {
					std::cout << "Error: No existe un producto con el codigo " << codigo << ".\n";
					break;
				}

				// 3. Mostrar datos del producto seleccionado
				std::string nombreCat = producto->getCategoria() != nullptr
					? producto->getCategoria()->getNombre() : "(sin categoria)";

				std::cout << "\n--- Datos del producto a eliminar ---\n";
				std::cout << "Codigo: " << producto->getCodigo() << "\n";
				std::cout << "Nombre: " << producto->getNombre() << "\n";
				std::cout << "Descripcion: " << producto->getDescripcion() << "\n";
				std::cout << "Precio unitario: $" << producto->getPrecioVentaActual() << "\n";
				std::cout << "Categoria: " << nombreCat << "\n";
				std::cout << "Stock actual: " << producto->getStock() << "\n";
				std::cout << "--------------------------------------\n";

				// 4. Solicitar confirmacion
				char confirmar;
				std::cout << "¿Confirma la eliminacion del producto \"" << producto->getNombre() << "\"? (s/n): ";
				std::cin >> confirmar;

				if (confirmar != 's' && confirmar != 'S') {
					std::cout << "Operacion cancelada. El producto no fue eliminado.\n";
					break;
				}

				// 5. Intentar eliminar
				try {
					bool ok = ctrl.eliminarProducto(codigo, true);
					if (ok) {
						std::cout << "Producto \"" << producto->getNombre() << "\" eliminado exitosamente del catalogo.\n";
					} else {
						std::cout << "No se pudo eliminar el producto.\n";
					}
				} catch (const std::logic_error& ex) {
					std::cout << "Error: " << ex.what() << std::endl;
				}
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
				// 1. Listar todos los productos (codigo, nombre, categoria)
				auto todosProductos = ctrl.listarProductos();
				if (todosProductos.empty()) {
					std::cout << "\nNo hay productos registrados en el catalogo.\n";
					break;
				}
				std::cout << "\n--- Productos existentes ---\n";
				for (Producto* p : todosProductos) {
					if (p != nullptr) {
						std::string nombreCat = p->getCategoria() != nullptr ? p->getCategoria()->getNombre() : "(sin categoria)";
						std::cout << "  Codigo: " << p->getCodigo()
								 << " | Nombre: " << p->getNombre()
								 << " | Categoria: " << nombreCat << std::endl;
					}
				}
				std::cout << "-----------------------------\n";

				// 2. Seleccionar producto por codigo
				int codigo;
				std::cout << "\nIngrese el codigo del producto a modificar: ";
				std::cin >> codigo;

				Producto* productoActual = ctrl.buscarProducto(codigo);
				if (productoActual == nullptr) {
					std::cout << "Error: No existe un producto con el codigo " << codigo << ".\n";
					break;
				}

				// 3. Mostrar datos actuales (el codigo no puede modificarse)
				std::string nombreCatActual = productoActual->getCategoria() != nullptr
					? productoActual->getCategoria()->getNombre() : "(sin categoria)";

				std::cout << "\n--- Datos actuales del producto (codigo: " << codigo << ") ---\n";
				std::cout << "Nombre: " << productoActual->getNombre() << "\n";
				std::cout << "Descripcion: " << productoActual->getDescripcion() << "\n";
				std::cout << "Precio unitario: $" << productoActual->getPrecioVentaActual() << "\n";
				std::cout << "Categoria: " << nombreCatActual << "\n";
				std::cout << "Stock minimo: " << productoActual->getStockMinimo() << "\n";
				std::cout << "-----------------------------------------------------\n";

				// 4. Solicitar nuevos datos (con reintento si el nombre ya existe)
				std::string nuevoNombre;
				std::string nuevaDescripcion;
				float nuevoPrecio;
				int nuevoStockMinimo;
				std::string categoria;

				while (true) {
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "\nNuevo nombre (deje vacio para mantener \"" << productoActual->getNombre() << "\"): ";
					std::getline(std::cin, nuevoNombre);
					if (nuevoNombre.empty()) {
						nuevoNombre = productoActual->getNombre();
					}

					std::cout << "Nueva descripcion (deje vacio para mantener actual): ";
					std::getline(std::cin, nuevaDescripcion);
					if (nuevaDescripcion.empty()) {
						nuevaDescripcion = productoActual->getDescripcion();
					}

					std::cout << "Nuevo precio de venta unitario: ";
					std::cin >> nuevoPrecio;
					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Precio invalido. Se mantendra $" << productoActual->getPrecioVentaActual() << ".\n";
						nuevoPrecio = productoActual->getPrecioVentaActual();
					}

					std::cout << "Nuevo stock minimo: ";
					std::cin >> nuevoStockMinimo;
					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Stock minimo invalido. Se mantendra " << productoActual->getStockMinimo() << ".\n";
						nuevoStockMinimo = productoActual->getStockMinimo();
					}

					// 5. Listar categorias y seleccionar una
					{
						auto categorias = ctrl.listarCategorias();
						if (categorias.empty()) {
							std::cout << "\nNo hay categorias registradas. El producto quedara sin categoria.\n";
							categoria = "";
						} else {
							std::cout << "\n--- Categorias disponibles ---\n";
							for (size_t i = 0; i < categorias.size(); ++i) {
								if (categorias[i] != nullptr) {
									std::cout << "  " << (i + 1) << ". " << categorias[i]->getNombre()
											 << ": " << categorias[i]->getDescripcion() << std::endl;
								}
							}
							int opcCat;
							while (true) {
								std::cout << "Seleccione una categoria (1-" << categorias.size() << ", 0=sin categoria): ";
								std::cin >> opcCat;
								if (std::cin.fail()) {
									std::cin.clear();
									std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
									std::cout << "Entrada invalida.\n";
									continue;
								}
								if (opcCat == 0) {
									categoria = "";
									break;
								}
								if (opcCat >= 1 && opcCat <= static_cast<int>(categorias.size())) {
									categoria = categorias[opcCat - 1]->getNombre();
									break;
								}
								std::cout << "Opcion invalida.\n";
							}
						}
					}

					// 6. Verificar que el nuevo nombre no pertenezca a otro producto
					if (nuevoNombre != productoActual->getNombre() && ctrl.buscarProductoPorNombre(nuevoNombre) != nullptr) {
						std::cout << "\nError: Ya existe otro producto con el nombre \"" << nuevoNombre << "\".\n";
						char reintentar;
						std::cout << "¿Desea corregir los datos? (s/n): ";
						std::cin >> reintentar;
						if (reintentar != 's' && reintentar != 'S') {
							std::cout << "Operacion cancelada. No se realizaron cambios.\n";
							break;
						}
						continue;
					}

					// 7. Mostrar resumen y confirmar
					std::string nombreCatNueva = categoria.empty() ? "(sin categoria)" : categoria;

					std::cout << "\n========== RESUMEN DE LA MODIFICACION ==========\n";
					std::cout << "Codigo: " << codigo << " (no modificable)\n";
					std::cout << "Nombre: " << productoActual->getNombre() << " -> " << nuevoNombre << "\n";
					std::cout << "Descripcion: " << productoActual->getDescripcion() << " -> " << nuevaDescripcion << "\n";
					std::cout << "Precio unitario: $" << productoActual->getPrecioVentaActual() << " -> $" << nuevoPrecio << "\n";
					std::cout << "Categoria: " << nombreCatActual << " -> " << nombreCatNueva << "\n";
					std::cout << "Stock minimo: " << productoActual->getStockMinimo() << " -> " << nuevoStockMinimo << "\n";
					std::cout << "================================================\n";

					char confirmar;
					std::cout << "¿Confirma la modificacion del producto? (s/n): ";
					std::cin >> confirmar;

					if (confirmar == 's' || confirmar == 'S') {
						bool ok = ctrl.modificarProducto(codigo, nuevoNombre, nuevaDescripcion, nuevoPrecio, categoria, nuevoStockMinimo);
						if (ok) {
							std::cout << "Producto \"" << nuevoNombre << "\" modificado exitosamente.\n";
						} else {
							std::cout << "No se pudo modificar el producto. Verifique los datos e intente nuevamente.\n";
						}
					} else {
						std::cout << "Operacion cancelada. No se realizaron cambios.\n";
					}
					break;
				}
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

		// Validar que el producto exista antes de pedir fechas
		{
			Producto* producto = ctrl.buscarProducto(codigoProducto);
			if (producto == nullptr) {
				std::cout << "Error: No existe un producto con el codigo ingresado." << std::endl;
				return;
			}
		}

		int diaInicio, mesInicio, anioInicio;
		int diaFin, mesFin, anioFin;

		while (true) {
			std::cout << "\nFecha de inicio del periodo:" << std::endl;
			std::cout << "Dia: "; std::cin >> diaInicio;
			std::cout << "Mes: "; std::cin >> mesInicio;
			std::cout << "Anio: "; std::cin >> anioInicio;

			std::cout << "\nFecha de fin del periodo:" << std::endl;
			std::cout << "Dia: "; std::cin >> diaFin;
			std::cout << "Mes: "; std::cin >> mesFin;
			std::cout << "Anio: "; std::cin >> anioFin;

			if (DTFecha(diaInicio, mesInicio, anioInicio) <= DTFecha(diaFin, mesFin, anioFin)) {
				break;
			}

			std::cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << std::endl;
			char reintentar;
			std::cout << "¿Desea reingresar las fechas? (s/n): ";
			std::cin >> reintentar;
			if (reintentar != 's' && reintentar != 'S') {
				std::cout << "Operacion cancelada." << std::endl;
				return;
			}
		}

		DTFecha inicio(diaInicio, mesInicio, anioInicio);
		DTFecha fin(diaFin, mesFin, anioFin);

		ResultadoUnidadesVendidasConsulta consulta = empleadoCtrl->consultarUnidadesVendidasValidado(codigoProducto, inicio, fin);
		ResultadoUnidadesVendidas res = consulta.datos;

		Producto* producto = ctrl.buscarProducto(codigoProducto);
		if (producto == nullptr) {
			std::cout << "Error: No existe un producto con el codigo ingresado." << std::endl;
			return;
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
		if (!(std::cin >> op)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Entrada invalida. Por favor ingrese un numero." << std::endl;
			continue;
		}
		if (op == 0) return;
			if (op == 1) {
				// 1. Listar categorias existentes
				auto categoriasExistentes = ctrl.listarCategorias();
				std::cout << "\n--- Categorias existentes ---\n";
				if (categoriasExistentes.empty()) {
					std::cout << "  (No hay categorias registradas)\n";
				} else {
					for (Categoria* cat : categoriasExistentes) {
						if (cat != nullptr) {
							std::cout << "  - " << cat->getNombre() << ": " << cat->getDescripcion() << std::endl;
						}
					}
				}
				std::cout << "-----------------------------\n";

				// 2. Solicitar datos de la nueva categoria (con reintento si el nombre ya existe)
				std::string nombre;
				std::string descripcion;

				while (true) {
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "\nNombre de la nueva categoria: ";
					std::getline(std::cin, nombre);

					std::cout << "Descripcion: ";
					std::getline(std::cin, descripcion);

					// 3. Verificar que no exista otra categoria con el mismo nombre
					if (ctrl.buscarCategoria(nombre) != nullptr) {
						std::cout << "\nError: Ya existe una categoria con el nombre \"" << nombre << "\".\n";
						char reintentar;
						std::cout << "¿Desea reingresar los datos? (s/n): ";
						std::cin >> reintentar;
						if (reintentar != 's' && reintentar != 'S') {
							std::cout << "Operacion cancelada.\n";
							break;
						}
						continue;
					}

					// 4. Mostrar resumen y confirmar
					std::cout << "\n========== RESUMEN DE LA CATEGORIA ==========\n";
					std::cout << "Nombre: " << nombre << "\n";
					std::cout << "Descripcion: " << descripcion << "\n";
					std::cout << "============================================\n";

					char confirmar;
					std::cout << "¿Confirma el alta de la categoria? (s/n): ";
					std::cin >> confirmar;

					if (confirmar == 's' || confirmar == 'S') {
						Categoria* c = ctrl.crearCategoria(nombre, descripcion);
						if (c != nullptr) {
							std::cout << "Categoria \"" << nombre << "\" creada exitosamente.\n";
						} else {
							std::cout << "No se pudo crear la categoria. Verifique los datos e intente nuevamente.\n";
						}
					} else {
						std::cout << "Operacion cancelada. La categoria no fue creada.\n";
					}
					break;
				}
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
				// 1. Listar categorias existentes
				auto categoriasExistentes = ctrl.listarCategorias();
				if (categoriasExistentes.empty()) {
					std::cout << "\nNo hay categorias registradas en el sistema.\n";
					break;
				}

				std::cout << "\n--- Categorias existentes ---\n";
				for (size_t i = 0; i < categoriasExistentes.size(); ++i) {
					if (categoriasExistentes[i] != nullptr) {
						std::cout << "  " << (i + 1) << ". " << categoriasExistentes[i]->getNombre()
								 << ": " << categoriasExistentes[i]->getDescripcion() << std::endl;
					}
				}
				std::cout << "------------------------------\n";

				// 2. Seleccionar categoria por nombre
				std::string nombreActual;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Ingrese el nombre de la categoria a modificar: ";
				std::getline(std::cin, nombreActual);

				Categoria* catActual = ctrl.buscarCategoria(nombreActual);
				if (catActual == nullptr) {
					std::cout << "\nError: No existe una categoria con el nombre \"" << nombreActual << "\".\n";
					break;
				}

				// 3. Mostrar datos actuales
				std::cout << "\n--- Datos actuales de la categoria ---\n";
				std::cout << "Nombre: " << catActual->getNombre() << "\n";
				std::cout << "Descripcion: " << catActual->getDescripcion() << "\n";
				std::cout << "---------------------------------------\n";

				// 4. Permitir edicion de nombre y descripcion (con reintento si el nombre ya existe)
				std::string nuevoNombre;
				std::string nuevaDescripcion;

				while (true) {
					std::cout << "\nNuevo nombre (deje vacio para mantener \"" << catActual->getNombre() << "\"): ";
					std::getline(std::cin, nuevoNombre);
					if (nuevoNombre.empty()) {
						nuevoNombre = catActual->getNombre();
					}

					std::cout << "Nueva descripcion (deje vacio para mantener actual): ";
					std::getline(std::cin, nuevaDescripcion);
					if (nuevaDescripcion.empty()) {
						nuevaDescripcion = catActual->getDescripcion();
					}

					// 5. Verificar que el nuevo nombre no exista en otra categoria
					if (nombreActual != nuevoNombre && ctrl.buscarCategoria(nuevoNombre) != nullptr) {
						std::cout << "\nError: Ya existe otra categoria con el nombre \"" << nuevoNombre << "\".\n";
						char reintentar;
						std::cout << "¿Desea corregir los datos? (s/n): ";
						std::cin >> reintentar;
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						if (reintentar != 's' && reintentar != 'S') {
							std::cout << "Operacion cancelada. No se realizaron cambios.\n";
							break;
						}
						continue;
					}

					// 6. Mostrar resumen y confirmar
					std::cout << "\n========== RESUMEN DE LA MODIFICACION ==========\n";
					std::cout << "Nombre actual: " << nombreActual << "\n";
					std::cout << "Nuevo nombre: " << nuevoNombre << "\n";
					std::cout << "Nueva descripcion: " << nuevaDescripcion << "\n";
					std::cout << "===============================================\n";

					char confirmar;
					std::cout << "¿Confirma la modificacion de la categoria? (s/n): ";
					std::cin >> confirmar;

					if (confirmar == 's' || confirmar == 'S') {
						bool ok = ctrl.modificarCategoria(nombreActual, nuevoNombre, nuevaDescripcion);
						if (ok) {
							std::cout << "Categoria modificada exitosamente.\n";
						} else {
							std::cout << "No se pudo modificar la categoria. Verifique los datos e intente nuevamente.\n";
						}
					} else {
						std::cout << "Operacion cancelada. No se realizaron cambios.\n";
					}
					break;
				}
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
		if (!(std::cin >> op)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Entrada invalida. Por favor ingrese un numero." << std::endl;
			continue;
		}
		if (op == 0) return;
			if (op == 1) {
				std::string nombre;
				std::string correo;
				std::string contrasena;
				int opcionRol;

				// Ciclo principal: permite reingresar datos si hay error
				while (true) {
					std::cout << "\n--- Alta de empleado ---\n";

					// 1. Solicitar datos del empleado
					std::cout << "Nombre completo: ";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::getline(std::cin, nombre);

					std::cout << "Correo electronico: ";
					std::cin >> correo;

					std::cout << "Contrasena: ";
					std::cin >> contrasena;

					// 2. Solicitar rol
					std::cout << "Rol del nuevo usuario:\n";
					std::cout << "  1. Empleado\n";
					std::cout << "  2. Administrador\n";
					std::cout << "Seleccione una opcion (1-2): ";
					std::cin >> opcionRol;

				// 3. Validar datos en el controlador (sin crear aun)
					ResultadoAltaEmpleado validacion = ctrl.validarAltaEmpleado(correo, opcionRol);

					if (!validacion.rolValido) {
						std::cout << "\nError: El rol seleccionado no es valido. Debe ser Empleado o Administrador.\n";
						char reintentar;
						std::cout << "¿Desea reingresar los datos? (s/n): ";
						std::cin >> reintentar;
						if (reintentar != 's' && reintentar != 'S') {
							std::cout << "Operacion cancelada.\n";
							break;
						}
						continue;
					}

					if (!validacion.correoDisponible) {
						std::cout << "\nError: Ya existe un usuario registrado con el correo \"" << correo << "\".\n";
						char reintentar;
						std::cout << "¿Desea reingresar los datos? (s/n): ";
						std::cin >> reintentar;
						if (reintentar != 's' && reintentar != 'S') {
							std::cout << "Operacion cancelada.\n";
							break;
						}
						continue;
					}

					// 4. Mostrar resumen de los datos ingresados
					std::cout << "\n========== RESUMEN DE DATOS ==========\n";
					std::cout << "Nombre completo: " << nombre << "\n";
					std::cout << "Correo electronico: " << correo << "\n";
					std::cout << "Contrasena: " << std::string(contrasena.length(), '*') << "\n";
					std::cout << "Rol: " << validacion.rolAsignado << "\n";
					std::cout << "=====================================\n";

					// 5. Confirmar o cancelar
					char confirmar;
					std::cout << "¿Confirma el alta del empleado? (s/n): ";
					std::cin >> confirmar;

					if (confirmar == 's' || confirmar == 'S') {
						// Crear efectivamente el empleado
						ResultadoAltaEmpleado alta = ctrl.crearEmpleadoConRolOpcion(nombre, correo, contrasena, opcionRol);
						if (alta.exito) {
							std::cout << "Empleado \"" << nombre << "\" dado de alta exitosamente con rol "
									  << alta.rolAsignado << ".\n";
						} else {
							std::cout << "No se pudo dar de alta el empleado. Verifique los datos e intente nuevamente.\n";
						}
					} else {
						std::cout << "Operacion cancelada. El empleado no fue dado de alta.\n";
					}
					break;
				}
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
		if (!(std::cin >> op)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Entrada invalida. Por favor ingrese un numero." << std::endl;
			continue;
		}
		if (op == 0) return;
			if (op == 1) {
				std::string rut;
				std::string empresa;
				std::string telefono;
				std::string contacto;

				while (true) {
					std::cout << "\n--- Alta de proveedor ---\n";
					std::cout << "RUT: "; std::cin >> rut;

					std::cout << "Empresa: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, empresa);
					std::cout << "Telefono: "; std::getline(std::cin, telefono);
					std::cout << "Contacto comercial: "; std::getline(std::cin, contacto);

					// Verificar que no exista otro proveedor con el mismo RUT
					if (ctrl.buscarProveedor(rut) != nullptr) {
						std::cout << "\nError: Ya existe un proveedor registrado con el RUT \"" << rut << "\".\n";
						char reintentar;
						std::cout << "¿Desea reingresar los datos? (s/n): ";
						std::cin >> reintentar;
						if (reintentar != 's' && reintentar != 'S') {
							std::cout << "Operacion cancelada.\n";
							break;
						}
						continue;
					}

					// Mostrar resumen y confirmar
					std::cout << "\n========== RESUMEN DEL PROVEEDOR ==========\n";
					std::cout << "RUT: " << rut << "\n";
					std::cout << "Empresa: " << empresa << "\n";
					std::cout << "Telefono: " << telefono << "\n";
					std::cout << "Contacto comercial: " << contacto << "\n";
					std::cout << "============================================\n";

					char confirmar;
					std::cout << "¿Confirma el alta del proveedor? (s/n): ";
					std::cin >> confirmar;

					if (confirmar == 's' || confirmar == 'S') {
						Proveedor* p = ctrl.crearProveedor(rut, empresa, telefono, contacto);
						if (p != nullptr) {
							std::cout << "Proveedor \"" << empresa << "\" registrado exitosamente.\n";
						} else {
							std::cout << "No se pudo registrar el proveedor. Verifique los datos e intente nuevamente.\n";
						}
					} else {
						std::cout << "Operacion cancelada. El proveedor no fue registrado.\n";
					}
					break;
				}
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
				// 1. Listar proveedores (RUT y nombre de empresa)
				auto todosProveedores = ctrl.listarProveedores();
				if (todosProveedores.empty()) {
					std::cout << "\nNo hay proveedores registrados en el sistema.\n";
					break;
				}
				std::cout << "\n--- Proveedores registrados ---\n";
				for (Proveedor* pr : todosProveedores) {
					if (pr != nullptr) {
						std::cout << "  RUT: " << pr->getRut()
								 << " | Empresa: " << pr->getEmpresa() << std::endl;
					}
				}
				std::cout << "-------------------------------\n";

				// 2. Seleccionar proveedor por RUT
				std::string rut;
				std::cout << "\nIngrese el RUT del proveedor a modificar: ";
				std::cin >> rut;

				Proveedor* proveedorActual = ctrl.buscarProveedor(rut);
				if (proveedorActual == nullptr) {
					std::cout << "Error: No existe un proveedor con el RUT \"" << rut << "\".\n";
					break;
				}

				// 3. Mostrar datos actuales (el RUT no puede modificarse)
				std::cout << "\n--- Datos actuales del proveedor (RUT: " << rut << ") ---\n";
				std::cout << "Empresa: " << proveedorActual->getEmpresa() << "\n";
				std::cout << "Telefono: " << proveedorActual->getTelefono() << "\n";
				std::cout << "Contacto comercial: " << proveedorActual->getContactoComercial() << "\n";
				std::cout << "--------------------------------------------------------\n";

				// 4. Solicitar nuevos datos
				std::string empresa;
				std::string telefono;
				std::string contacto;

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\nNueva empresa (deje vacio para mantener \"" << proveedorActual->getEmpresa() << "\"): ";
				std::getline(std::cin, empresa);
				if (empresa.empty()) empresa = proveedorActual->getEmpresa();

				std::cout << "Nuevo telefono (deje vacio para mantener \"" << proveedorActual->getTelefono() << "\"): ";
				std::getline(std::cin, telefono);
				if (telefono.empty()) telefono = proveedorActual->getTelefono();

				std::cout << "Nuevo contacto comercial (deje vacio para mantener \"" << proveedorActual->getContactoComercial() << "\"): ";
				std::getline(std::cin, contacto);
				if (contacto.empty()) contacto = proveedorActual->getContactoComercial();

				// 5. Mostrar resumen y confirmar
				std::cout << "\n========== RESUMEN DE LA MODIFICACION ==========\n";
				std::cout << "RUT: " << rut << " (no modificable)\n";
				std::cout << "Empresa: " << proveedorActual->getEmpresa() << " -> " << empresa << "\n";
				std::cout << "Telefono: " << proveedorActual->getTelefono() << " -> " << telefono << "\n";
				std::cout << "Contacto comercial: " << proveedorActual->getContactoComercial() << " -> " << contacto << "\n";
				std::cout << "================================================\n";

				char confirmar;
				std::cout << "¿Confirma la modificacion del proveedor? (s/n): ";
				std::cin >> confirmar;

				if (confirmar == 's' || confirmar == 'S') {
					bool ok = ctrl.modificarProveedor(rut, empresa, telefono, contacto);
					if (ok) {
						std::cout << "Proveedor \"" << empresa << "\" modificado exitosamente.\n";
					} else {
						std::cout << "No se pudo modificar el proveedor. Verifique los datos e intente nuevamente.\n";
					}
				} else {
					std::cout << "Operacion cancelada. No se realizaron cambios.\n";
				}
			} else if (op == 6) {
				std::string rut;
				int codigoProducto;
				int precioCompra;
				int tiempoEntrega;

				// 1. Listar proveedores y seleccionar uno
				auto todosProveedores = ctrl.listarProveedores();
				if (todosProveedores.empty()) {
					std::cout << "\nNo hay proveedores registrados en el sistema.\n";
					break;
				}
				std::cout << "\n--- Proveedores registrados ---\n";
				for (Proveedor* pr : todosProveedores) {
					if (pr != nullptr) {
						std::cout << "  RUT: " << pr->getRut()
								 << " | Empresa: " << pr->getEmpresa() << std::endl;
					}
				}
				std::cout << "-------------------------------\n";
				std::cout << "Ingrese el RUT del proveedor: ";
				std::cin >> rut;

				Proveedor* proveedor = ctrl.buscarProveedor(rut);
				if (proveedor == nullptr) {
					std::cout << "Error: No existe un proveedor con el RUT \"" << rut << "\".\n";
					break;
				}

				// 2. Listar productos del catalogo que aun NO tienen asociacion con ese proveedor
				auto productosDisponibles = ctrl.listarProductosNoAsociadosAProveedor(rut);
				if (productosDisponibles.empty()) {
					std::cout << "\nTodos los productos del catalogo ya estan asociados al proveedor \"" 
							 << proveedor->getEmpresa() << "\".\n";
					break;
				}
				std::cout << "\n--- Productos disponibles para asociar ---\n";
				for (Producto* p : productosDisponibles) {
					if (p != nullptr) {
						std::cout << "  Codigo: " << p->getCodigo()
								 << " | Nombre: " << p->getNombre() << std::endl;
					}
				}
				std::cout << "------------------------------------------\n";
				std::cout << "Ingrese el codigo del producto: ";
				std::cin >> codigoProducto;

				// Verificar que el producto seleccionado este en la lista de disponibles
				bool productoValido = false;
				for (Producto* p : productosDisponibles) {
					if (p != nullptr && p->getCodigo() == codigoProducto) {
						productoValido = true;
						break;
					}
				}
				if (!productoValido) {
					std::cout << "Error: El producto con codigo " << codigoProducto 
							 << " no esta disponible para asociar (no existe o ya esta asociado a este proveedor).\n";
					break;
				}

				Producto* producto = ctrl.buscarProducto(codigoProducto);

				// 3. Verificar si ya existe la combinacion (para informar y ofrecer actualizar)
				bool yaExiste = ctrl.existeProveedorProducto(rut, codigoProducto);
				if (yaExiste) {
					std::cout << "\nLa combinacion proveedor-producto ya existe.\n";
					std::cout << "Se actualizaran el precio de compra y el tiempo de entrega vigentes.\n";
				} else {
					std::cout << "\nSe registrara una nueva asociacion.\n";
				}

				// 4. Solicitar precio de compra y tiempo de entrega
				std::cout << "Precio de compra pactado: ";
				std::cin >> precioCompra;
				std::cout << "Tiempo de entrega estimado (dias): ";
				std::cin >> tiempoEntrega;

				// 5. Mostrar resumen y confirmar
				std::cout << "\n========== RESUMEN DE LA ASOCIACION ==========\n";
				std::cout << "Proveedor: " << proveedor->getEmpresa() << " (RUT: " << rut << ")\n";
				std::cout << "Producto: " << (producto != nullptr ? producto->getNombre() : "Codigo " + std::to_string(codigoProducto))
						 << " (Codigo: " << codigoProducto << ")\n";
				std::cout << "Precio de compra pactado: $" << precioCompra << "\n";
				std::cout << "Tiempo de entrega: " << tiempoEntrega << " dias\n";
				std::cout << "Operacion: " << (yaExiste ? "ACTUALIZAR datos existentes" : "REGISTRAR nueva asociacion") << "\n";
				std::cout << "==============================================\n";

				char confirmar;
				std::cout << "¿Confirma la operacion? (s/n): ";
				std::cin >> confirmar;

				if (confirmar == 's' || confirmar == 'S') {
					ResultadoGestionProveedorProducto gestion = ctrl.gestionarAsociacionProveedorProducto(rut, codigoProducto, precioCompra, tiempoEntrega);
					if (!gestion.proveedorExiste) {
						std::cout << "No se pudo registrar la asociacion. Proveedor inexistente." << std::endl;
					} else if (!gestion.productoExiste) {
						std::cout << "No se pudo registrar la asociacion. Producto inexistente." << std::endl;
					} else if (!gestion.exito) {
						std::cout << "No se pudo registrar la asociacion." << std::endl;
					} else {
						std::cout << (gestion.actualizada
							? "Asociacion actualizada exitosamente."
							: "Asociacion registrada exitosamente.") << std::endl;
					}
				} else {
					std::cout << "Operacion cancelada. No se realizaron cambios.\n";
				}
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
