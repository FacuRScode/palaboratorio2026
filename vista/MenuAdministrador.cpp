//
// Created by facun on 2/6/2026.
//


#include "MenuAdministrador.h"
#include <limits>
#include <algorithm>

MenuAdministrador::MenuAdministrador(AdminController& controller, AuthController* auth,
									 EmpleadoController* empleados,
									 VentaController* ventas)
	: ctrl(controller), authCtrl(auth), empleadoCtrl(empleados), ventaCtrl(ventas) {}

void MenuAdministrador::mostrar() {
	while (true) {
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
	}
}

void MenuAdministrador::consultarMontoFacturadoCliente() {
	if (empleadoCtrl == nullptr) {
		std::cout << "Error: Controlador de empleados no disponible." << std::endl;
		return;
	}

	std::string rut;
	std::cout << "\nIngrese el RUT del cliente: ";
	std::cin >> rut;

	Cliente* cliente = empleadoCtrl->buscarCliente(rut);
	if (cliente == nullptr) {
		std::cout << "Error: No existe un cliente con el RUT ingresado." << std::endl;
		return;
	}

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

	// Validar que inicio <= fin
	while (!(inicio <= fin)) {
		std::cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << std::endl;
		std::cout << "1. Reingresar las fechas" << std::endl;
		std::cout << "2. Cancelar" << std::endl;
		int opcion;
		std::cout << "Seleccione una opcion: ";
		std::cin >> opcion;
		if (opcion == 2) {
			std::cout << "Operacion cancelada." << std::endl;
			return;
		}

		std::cout << "\nFecha de inicio del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaInicio;
		std::cout << "Mes: "; std::cin >> mesInicio;
		std::cout << "Anio: "; std::cin >> anioInicio;
		inicio = DTFecha(diaInicio, mesInicio, anioInicio);

		std::cout << "\nFecha de fin del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaFin;
		std::cout << "Mes: "; std::cin >> mesFin;
		std::cout << "Anio: "; std::cin >> anioFin;
		fin = DTFecha(diaFin, mesFin, anioFin);
	}

	// Consultar facturacion
	ResultadoFacturacion res = empleadoCtrl->consultarMontoFacturado(rut, inicio, fin);

	// Mostrar resultados
	std::cout << "\n--- Resultados de facturacion ---" << std::endl;
	std::cout << "Cliente: " << cliente->getNombre() << " " << cliente->getApellido()
			 << " (RUT: " << rut << ")" << std::endl;
	std::cout << "Periodo: " << diaInicio << "/" << mesInicio << "/" << anioInicio
			 << " - " << diaFin << "/" << mesFin << "/" << anioFin << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << "Monto total facturado: $" << res.montoTotal << std::endl;
	std::cout << "Cantidad de ventas: " << res.cantidadVentas << std::endl;
	if (res.cantidadVentas > 0) {
		std::cout << "Monto promedio por venta: $" << res.montoPromedio << std::endl;
	}
	std::cout << "------------------------------------------------" << std::endl;
}

void MenuAdministrador::consultarCalificacionesProducto() {
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
}

void MenuAdministrador::consultarStockProductos() {
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
		auto todos = ctrl.listarProductos();

		for (Producto* p : todos) {
			if (p != nullptr && p->getCategoria() != nullptr &&
				p->getCategoria()->getNombre() == nombreCategoria) {
				productos.push_back(p);
			}
		}

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
		for (int i = strCodigo.length(); i < 8; ++i) std::cout << " ";
		std::cout << "| " << p->getNombre();
		for (int i = p->getNombre().length(); i < 31; ++i) std::cout << " ";
		std::cout << "| " << nombreCat;
		for (int i = nombreCat.length(); i < 23; ++i) std::cout << " ";
		std::cout << "| " << strStock;
		for (int i = strStock.length(); i < 12; ++i) std::cout << " ";
		std::cout << "| " << strStockMin << std::endl;
	}
	std::cout << "---------------------------------------------------------------------\n";
}

void MenuAdministrador::consultarProductosStockBajo() {
	auto todos = ctrl.listarProductos();
	if (todos.empty()) {
		std::cout << "\nNo hay productos registrados en el catalogo." << std::endl;
		return;
	}

	// Filtrar productos con stock < stock minimo
	std::vector<Producto*> productosBajos;
	for (Producto* p : todos) {
		if (p != nullptr && p->getStock() < p->getStockMinimo()) {
			productosBajos.push_back(p);
		}
	}

	if (productosBajos.empty()) {
		std::cout << "\nNo hay productos con stock por debajo del minimo configurado." << std::endl;
		return;
	}

	// Preguntar si desea ordenar por diferencia ascendente
	char ordenar;
	std::cout << "\n¿Ordenar por diferencia ascendente (mas criticos primero)? (s/n): ";
	std::cin >> ordenar;

	if (ordenar == 's' || ordenar == 'S') {
		std::sort(productosBajos.begin(), productosBajos.end(), [](Producto* a, Producto* b) {
			int diffA = a->getStockMinimo() - a->getStock();
			int diffB = b->getStockMinimo() - b->getStock();
			return diffA > diffB; // mayor diferencia = mas critico primero
		});
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
		for (int i = strCodigo.length(); i < 8; ++i) std::cout << " ";
		std::cout << "| " << p->getNombre();
		for (int i = p->getNombre().length(); i < 31; ++i) std::cout << " ";
		std::cout << "| " << nombreCat;
		for (int i = nombreCat.length(); i < 23; ++i) std::cout << " ";
		std::cout << "| " << strStock;
		for (int i = strStock.length(); i < 12; ++i) std::cout << " ";
		std::cout << "| " << strStockMin;
		for (int i = strStockMin.length(); i < 12; ++i) std::cout << " ";
		std::cout << "| " << strDiff;
		std::cout << " (faltan " << diferencia << ")" << std::endl;
	}
	std::cout << "----------------------------------------------------------------------------\n";
}

void MenuAdministrador::menuProductos() {
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

			std::cout << "\n--- Alta de producto ---\n";
			std::cout << "Codigo: "; std::cin >> codigo;
			std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
			std::cout << "Descripcion: "; std::getline(std::cin, descripcion);
			std::cout << "Precio de venta unitario: "; std::cin >> precio;

			// Listar categorias existentes y seleccionar una
			auto categorias = ctrl.listarCategorias();
			if (categorias.empty()) {
				std::cout << "\nError: No hay categorias registradas. Debe crear una categoria antes de agregar un producto." << std::endl;
				return;
			}

			std::cout << "\n--- Categorias disponibles ---\n";
			for (size_t i = 0; i < categorias.size(); ++i) {
				if (categorias[i]) std::cout << (i + 1) << ". " << categorias[i]->getNombre() << ": " << categorias[i]->getDescripcion() << '\n';
			}

			int opcionCategoria;
			std::cout << "Seleccione una categoria (1-" << categorias.size() << "): ";
			std::cin >> opcionCategoria;
			if (opcionCategoria < 1 || opcionCategoria > (int)categorias.size()) {
				std::cout << "Opcion invalida. Operacion cancelada." << std::endl;
				break;
			}

			std::cout << "Stock inicial: "; std::cin >> stock;
			std::cout << "Stock minimo para alertas de reposicion: "; std::cin >> stockMinimo;

			// Mostrar resumen y confirmar
			std::cout << "\n--- Resumen de datos ingresados ---\n";
			std::cout << "Codigo: " << codigo << "\n";
			std::cout << "Nombre: " << nombre << "\n";
			std::cout << "Descripcion: " << descripcion << "\n";
			std::cout << "Precio de venta unitario: " << precio << "\n";
			std::cout << "Categoria: " << categorias[opcionCategoria - 1]->getNombre() << "\n";
			std::cout << "Stock inicial: " << stock << "\n";
			std::cout << "Stock minimo: " << stockMinimo << "\n";
			std::cout << "1. Confirmar\n";
			std::cout << "2. Cancelar\n";
			int opcionConfirmar;
			std::cout << "Seleccione una opcion: ";
			std::cin >> opcionConfirmar;
			if (opcionConfirmar == 1) {
				Producto* p = ctrl.crearProducto(codigo, nombre, descripcion, precio, stock, stockMinimo, categorias[opcionCategoria - 1]->getNombre());
				if (p) {
					std::cout << "Producto creado exitosamente." << std::endl;
				} else {
					std::cout << "Error: El codigo ya existe o la categoria no es valida." << std::endl;
				}
			} else {
				std::cout << "Operacion cancelada." << std::endl;
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
			auto todos = ctrl.listarProductos();
			if (todos.empty()) {
				std::cout << "\nNo hay productos registrados.\n";
			} else {
				std::cout << "\n--- Productos existentes ---\n";
				for (auto p : todos) {
					if (p) {
						std::cout << "- codigo=" << p->getCodigo() << " nombre=" << p->getNombre() << '\n';
					}
				}

				int codigo;
				Producto* prod = nullptr;

				while (true) {
					std::cout << "\nIngrese el codigo del producto a eliminar (0 para cancelar): ";
					std::cin >> codigo;
					if (codigo == 0) {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
					prod = ctrl.buscarProducto(codigo);
					if (prod == nullptr) {
						std::cout << "Error: No existe un producto con el codigo '" << codigo << "'." << std::endl;
						std::cout << "1. Reingresar el codigo\n";
						std::cout << "2. Cancelar\n";
						int opcionError;
						std::cout << "Seleccione una opcion: ";
						std::cin >> opcionError;
						if (opcionError == 2) {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
						continue;
					}

					std::cout << "\n--- Datos del producto a eliminar ---\n";
					std::cout << "Codigo: " << prod->getCodigo() << "\n";
					std::cout << "Nombre: " << prod->getNombre() << "\n";
					std::cout << "Descripcion: " << prod->getDescripcion() << "\n";
					std::cout << "Precio de venta unitario: " << prod->getPrecioVentaActual() << "\n";
					std::cout << "Stock actual: " << prod->getStock() << "\n";
					std::string catActual = prod->getCategoria() ? prod->getCategoria()->getNombre() : "(sin categoria)";
					std::cout << "Categoria: " << catActual << "\n";

					if (ctrl.productoAsociadoAProveedor(codigo)) {
						std::cout << "(Tiene asociaciones con proveedores que seran eliminadas)\n";
					}

					std::cout << "\n1. Confirmar eliminacion\n";
					std::cout << "2. Cancelar\n";
					int opcionConfirmar;
					std::cout << "Seleccione una opcion: ";
					std::cin >> opcionConfirmar;
					if (opcionConfirmar == 1) {
						bool ok = ctrl.eliminarProducto(codigo, true);
						if (ok) {
							std::cout << "Producto eliminado exitosamente." << std::endl;
						} else {
							std::cout << "Error: El producto tiene ventas u ordenes de compra pendientes y no puede eliminarse." << std::endl;
						}
						break;
					} else {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
				}
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
			// Listar todos los productos
			auto todos = ctrl.listarProductos();
			if (todos.empty()) {
				std::cout << "\nNo hay productos registrados para modificar.\n";
			} else {
				std::cout << "\n--- Productos existentes ---\n";
				for (auto p : todos) {
					if (p) {
						std::string catNombre = p->getCategoria() ? p->getCategoria()->getNombre() : "(sin categoria)";
						std::cout << "- codigo=" << p->getCodigo() << " nombre=" << p->getNombre() << " categoria=" << catNombre << '\n';
					}
				}

				int codigo;
				Producto* prod = nullptr;

				// Bucle para seleccionar producto a modificar
				while (true) {
					std::cout << "\nIngrese el codigo del producto a modificar (0 para cancelar): ";
					std::cin >> codigo;
					if (codigo == 0) {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
					prod = ctrl.buscarProducto(codigo);
					if (prod == nullptr) {
						std::cout << "Error: No existe un producto con el codigo '" << codigo << "'." << std::endl;
						std::cout << "1. Reingresar el codigo\n";
						std::cout << "2. Cancelar\n";
						int opcionError;
						std::cout << "Seleccione una opcion: ";
						std::cin >> opcionError;
						if (opcionError == 2) {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
						continue;
					}

					// Mostrar datos actuales
					std::string catActual = prod->getCategoria() ? prod->getCategoria()->getNombre() : "(sin categoria)";
					std::cout << "\n--- Datos actuales del producto (codigo=" << prod->getCodigo() << ") ---\n";
					std::cout << "Codigo: " << prod->getCodigo() << " (no modificable)\n";
					std::cout << "Nombre: " << prod->getNombre() << "\n";
					std::cout << "Descripcion: " << prod->getDescripcion() << "\n";
					std::cout << "Precio de venta unitario: " << prod->getPrecioVentaActual() << "\n";
					std::cout << "Categoria: " << catActual << "\n";
					std::cout << "Stock minimo: " << prod->getStockMinimo() << "\n";

					std::string nuevoNombre;
					std::string nuevaDescripcion;
					float nuevoPrecio;
					int nuevoStockMinimo;

					std::cout << "\n--- Nuevos datos del producto ---\n";
					std::cout << "Nuevo nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nuevoNombre);
					std::cout << "Nueva descripcion: "; std::getline(std::cin, nuevaDescripcion);
					std::cout << "Nuevo precio de venta unitario: "; std::cin >> nuevoPrecio;

					// Listar categorias disponibles
					auto categorias = ctrl.listarCategorias();
					if (categorias.empty()) {
						std::cout << "\nError: No hay categorias registradas. No se puede modificar el producto sin una categoria." << std::endl;
						break;
					}

					std::cout << "\n--- Categorias disponibles ---\n";
					for (size_t i = 0; i < categorias.size(); ++i) {
						if (categorias[i]) std::cout << (i + 1) << ". " << categorias[i]->getNombre() << ": " << categorias[i]->getDescripcion() << '\n';
					}

					int opcionCategoria;
					std::cout << "Seleccione la nueva categoria (1-" << categorias.size() << "): ";
					std::cin >> opcionCategoria;
					if (opcionCategoria < 1 || opcionCategoria > (int)categorias.size()) {
						std::cout << "Opcion invalida. Operacion cancelada." << std::endl;
						break;
					}

					std::cout << "Nuevo stock minimo para alertas de reposicion: "; std::cin >> nuevoStockMinimo;

					// Mostrar resumen y confirmar
					std::cout << "\n--- Resumen de la modificacion ---\n";
					std::cout << "Codigo: " << codigo << " (no modificable)\n";
					std::cout << "Nombre anterior: " << prod->getNombre() << "\n";
					std::cout << "Nombre nuevo: " << nuevoNombre << "\n";
					std::cout << "Descripcion anterior: " << prod->getDescripcion() << "\n";
					std::cout << "Descripcion nueva: " << nuevaDescripcion << "\n";
					std::cout << "Precio anterior: " << prod->getPrecioVentaActual() << "\n";
					std::cout << "Precio nuevo: " << nuevoPrecio << "\n";
					std::cout << "Categoria anterior: " << catActual << "\n";
					std::cout << "Categoria nueva: " << categorias[opcionCategoria - 1]->getNombre() << "\n";
					std::cout << "Stock minimo anterior: " << prod->getStockMinimo() << "\n";
					std::cout << "Stock minimo nuevo: " << nuevoStockMinimo << "\n";
					std::cout << "1. Confirmar\n";
					std::cout << "2. Cancelar\n";
					int opcionConfirmar;
					std::cout << "Seleccione una opcion: ";
					std::cin >> opcionConfirmar;
					if (opcionConfirmar == 1) {
						bool ok = ctrl.modificarProducto(codigo, nuevoNombre, nuevaDescripcion, nuevoPrecio, categorias[opcionCategoria - 1]->getNombre(), nuevoStockMinimo);
						if (ok) {
							std::cout << "Producto modificado exitosamente." << std::endl;
						} else {
							std::cout << "Error: El nombre ingresado ya pertenece a otro producto o la categoria no es valida." << std::endl;
						}
						break;
					} else {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
					break;
				}
			}
		} else {
			std::cout << "Opcion invalida." << std::endl;
		}
	}
}

void MenuAdministrador::consultarInfoDetalladaProducto() {
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

	Producto* producto = ctrl.buscarProducto(codigo);
	if (producto == nullptr) {
		std::cout << "Error: No existe un producto con el codigo ingresado." << std::endl;
		return;
	}

	// Mostrar informacion basica (para todos los roles)
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

	// Informacion adicional para Administrador y Empleado
	std::cout << "\n--- Datos de administracion ---\n";
	std::cout << "Stock minimo: " << producto->getStockMinimo() << std::endl;

	// Proveedores que abastecen el producto
	auto asociaciones = ctrl.listarAsociacionesDeProducto(codigo);
	if (asociaciones.empty()) {
		std::cout << "Proveedores: Ninguno (el producto no esta asociado a ningun proveedor)" << std::endl;
	} else {
		std::cout << "\nProveedores que abastecen este producto:" << std::endl;
		for (size_t i = 0; i < asociaciones.size(); ++i) {
			ProveedorProducto* pp = asociaciones[i];
			if (pp == nullptr) continue;
			// Obtener el RUT del proveedor buscando en todos los proveedores
			std::string rutProveedor = "(desconocido)";
			std::string empresaProveedor = "(desconocida)";
			for (Proveedor* pr : ctrl.listarProveedores()) {
				if (pr == nullptr) continue;
				for (ProveedorProducto* pp2 : pr->getProductosOfrecidos()) {
					if (pp2 == pp) {
						rutProveedor = pr->getRut();
						empresaProveedor = pr->getEmpresa();
						break;
					}
				}
				if (rutProveedor != "(desconocido)") break;
			}
			std::cout << "  " << (i + 1) << ". Proveedor: " << empresaProveedor
					 << " (RUT: " << rutProveedor << ")" << std::endl;
			std::cout << "     Precio de compra vigente: $" << pp->getPrecioCompraPactado() << std::endl;
			std::cout << "     Tiempo de entrega estimado: " << pp->getTiempoEntregaEstimadoEnDias() << " dias" << std::endl;
		}
	}
	std::cout << "========================================================\n";
}

void MenuAdministrador::consultarUnidadesVendidasProducto() {
	if (empleadoCtrl == nullptr) {
		std::cout << "Error: Controlador de empleados no disponible." << std::endl;
		return;
	}

	// Listar todos los productos
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
	if (producto == nullptr) {
		std::cout << "Error: No existe un producto con el codigo ingresado." << std::endl;
		return;
	}

	// Solicitar fechas
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

	// Validar que inicio <= fin
	while (!(inicio <= fin)) {
		std::cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << std::endl;
		std::cout << "1. Reingresar las fechas" << std::endl;
		std::cout << "2. Cancelar" << std::endl;
		int opcion;
		std::cout << "Seleccione una opcion: ";
		std::cin >> opcion;
		if (opcion == 2) {
			std::cout << "Operacion cancelada." << std::endl;
			return;
		}

		std::cout << "\nFecha de inicio del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaInicio;
		std::cout << "Mes: "; std::cin >> mesInicio;
		std::cout << "Anio: "; std::cin >> anioInicio;
		inicio = DTFecha(diaInicio, mesInicio, anioInicio);

		std::cout << "\nFecha de fin del periodo:" << std::endl;
		std::cout << "Dia: "; std::cin >> diaFin;
		std::cout << "Mes: "; std::cin >> mesFin;
		std::cout << "Anio: "; std::cin >> anioFin;
		fin = DTFecha(diaFin, mesFin, anioFin);
	}

	// Consultar unidades vendidas
	ResultadoUnidadesVendidas res = empleadoCtrl->consultarUnidadesVendidas(codigoProducto, inicio, fin);

	// Mostrar resultados
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
}

void MenuAdministrador::menuCategorias() {
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
			auto existentes = ctrl.listarCategorias();
			std::cout << "\n--- Categorias existentes ---\n";
			if (existentes.empty()) {
				std::cout << "(No hay categorias registradas)\n";
			} else {
				for (auto c : existentes) {
					if (c) std::cout << "- " << c->getNombre() << ": " << c->getDescripcion() << '\n';
				}
			}

			std::string nombre;
			std::string descripcion;

			std::cout << "\n--- Nueva categoria ---\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nombre: "; std::getline(std::cin, nombre);
			std::cout << "Descripcion: "; std::getline(std::cin, descripcion);

			std::cout << "\n--- Resumen de datos ingresados ---\n";
			std::cout << "Nombre: " << nombre << "\n";
			std::cout << "Descripcion: " << descripcion << "\n";
			std::cout << "1. Confirmar\n";
			std::cout << "2. Cancelar\n";
			int opcionConfirmar;
			std::cout << "Seleccione una opcion: ";
			std::cin >> opcionConfirmar;
			if (opcionConfirmar == 1) {
				auto c = ctrl.crearCategoria(nombre, descripcion);
				if (c) {
					std::cout << "Categoria creada exitosamente." << std::endl;
				} else {
					std::cout << "Error: Ya existe una categoria con ese nombre." << std::endl;
				}
			} else {
				std::cout << "Operacion cancelada." << std::endl;
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
			// Listar todas las categorias
			auto todas = ctrl.listarCategorias();
			if (todas.empty()) {
				std::cout << "\nNo hay categorias registradas para modificar.\n";
			} else {
				std::cout << "\n--- Categorias existentes ---\n";
				for (auto c : todas) {
					if (c) std::cout << "- " << c->getNombre() << ": " << c->getDescripcion() << '\n';
				}

				std::string nombreActual;
				Categoria* cat = nullptr;

				// Bucle para seleccionar categoria a modificar
				while (true) {
					std::cout << "\nIngrese el nombre de la categoria a modificar (0 para cancelar): ";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::getline(std::cin, nombreActual);
					if (nombreActual == "0") {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
					cat = ctrl.buscarCategoria(nombreActual);
					if (cat == nullptr) {
						std::cout << "Error: No existe una categoria con el nombre '" << nombreActual << "'." << std::endl;
						std::cout << "1. Reingresar el nombre\n";
						std::cout << "2. Cancelar\n";
						int opcionError;
						std::cout << "Seleccione una opcion: ";
						std::cin >> opcionError;
						if (opcionError == 2) {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
						continue;
					}

					// Mostrar datos actuales
					std::cout << "\n--- Datos actuales de la categoria ---\n";
					std::cout << "Nombre: " << cat->getNombre() << "\n";
					std::cout << "Descripcion: " << cat->getDescripcion() << "\n";

					std::string nuevoNombre;
					std::string nuevaDescripcion;

					std::cout << "\n--- Nuevos datos ---\n";
					std::cout << "Nuevo nombre: "; std::getline(std::cin, nuevoNombre);
					std::cout << "Nueva descripcion: "; std::getline(std::cin, nuevaDescripcion);

					// Mostrar resumen y confirmar
					std::cout << "\n--- Resumen de la modificacion ---\n";
					std::cout << "Nombre anterior: " << nombreActual << "\n";
					std::cout << "Nombre nuevo: " << nuevoNombre << "\n";
					std::cout << "Descripcion anterior: " << cat->getDescripcion() << "\n";
					std::cout << "Descripcion nueva: " << nuevaDescripcion << "\n";
					std::cout << "1. Confirmar\n";
					std::cout << "2. Cancelar\n";
					int opcionConfirmar;
					std::cout << "Seleccione una opcion: ";
					std::cin >> opcionConfirmar;
					if (opcionConfirmar == 1) {
						bool ok = ctrl.modificarCategoria(nombreActual, nuevoNombre, nuevaDescripcion);
						if (ok) {
							std::cout << "Categoria modificada exitosamente." << std::endl;
						} else {
							std::cout << "Error: Ya existe otra categoria con ese nombre." << std::endl;
						}
						break;
					} else {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
					break;
				}
			}
		} else {
			std::cout << "Opcion invalida." << std::endl;
		}
	}
}

	void MenuAdministrador::menuEmpleados() {
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
				std::string rol;

				std::cout << "\n--- Alta de empleado ---\n";
				std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
				std::cout << "Correo: "; std::cin >> correo;
				std::cout << "Contrasena: "; std::cin >> contrasena;

				// Seleccion de rol
				while (true) {
					std::cout << "\nSeleccione el rol del nuevo usuario:\n";
					std::cout << "1. Empleado\n";
					std::cout << "2. Administrador\n";
					int opcionRol;
					std::cout << "Opcion: ";
					std::cin >> opcionRol;
					if (opcionRol == 1) {
						rol = "Empleado";
						break;
					} else if (opcionRol == 2) {
						rol = "Administrador";
						break;
					} else {
						std::cout << "Opcion invalida. Intente nuevamente." << std::endl;
					}
				}

				// Mostrar resumen y confirmar
				std::cout << "\n--- Resumen de datos ingresados ---\n";
				std::cout << "Nombre: " << nombre << "\n";
				std::cout << "Correo: " << correo << "\n";
				std::cout << "Rol: " << rol << "\n";
				std::cout << "1. Confirmar\n";
				std::cout << "2. Cancelar\n";
				int opcionConfirmar;
				std::cout << "Seleccione una opcion: ";
				std::cin >> opcionConfirmar;
				if (opcionConfirmar == 1) {
					auto e = ctrl.crearEmpleado(nombre, correo, contrasena, rol);
					if (e) {
						std::cout << "Empleado creado exitosamente." << std::endl;
					} else {
						std::cout << "Error: Ya existe un empleado con ese correo." << std::endl;
					}
				} else {
					std::cout << "Operacion cancelada." << std::endl;
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
}

void MenuAdministrador::menuProveedores() {
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

			std::cout << "\n--- Alta de proveedor ---\n";
			std::cout << "RUT: "; std::cin >> rut;
			std::cout << "Empresa: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, empresa);
			std::cout << "Telefono: "; std::getline(std::cin, telefono);
			std::cout << "Contacto comercial: "; std::getline(std::cin, contacto);

			std::cout << "\n--- Resumen de datos ingresados ---\n";
			std::cout << "RUT: " << rut << "\n";
			std::cout << "Empresa: " << empresa << "\n";
			std::cout << "Telefono: " << telefono << "\n";
			std::cout << "Contacto comercial: " << contacto << "\n";
			std::cout << "1. Confirmar\n";
			std::cout << "2. Cancelar\n";
			int opcionConfirmar;
			std::cout << "Seleccione una opcion: ";
			std::cin >> opcionConfirmar;
			if (opcionConfirmar == 1) {
				auto p = ctrl.crearProveedor(rut, empresa, telefono, contacto);
				if (p) {
					std::cout << "Proveedor creado exitosamente." << std::endl;
				} else {
					std::cout << "Error: Ya existe un proveedor con ese RUT." << std::endl;
				}
			} else {
				std::cout << "Operacion cancelada." << std::endl;
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
			// Listar todos los proveedores
			auto todos = ctrl.listarProveedores();
			if (todos.empty()) {
				std::cout << "\nNo hay proveedores registrados para modificar.\n";
			} else {
				std::cout << "\n--- Proveedores existentes ---\n";
				for (auto pr : todos) {
					if (pr) std::cout << "- " << pr->getRut() << " - " << pr->getEmpresa() << '\n';
				}

				std::string rut;
				Proveedor* prov = nullptr;

				// Bucle para seleccionar proveedor a modificar
				while (true) {
					std::cout << "\nIngrese el RUT del proveedor a modificar (0 para cancelar): ";
					std::cin >> rut;
					if (rut == "0") {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
					prov = ctrl.buscarProveedor(rut);
					if (prov == nullptr) {
						std::cout << "Error: No existe un proveedor con el RUT '" << rut << "'." << std::endl;
						std::cout << "1. Reingresar el RUT\n";
						std::cout << "2. Cancelar\n";
						int opcionError;
						std::cout << "Seleccione una opcion: ";
						std::cin >> opcionError;
						if (opcionError == 2) {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
						continue;
					}

					// Mostrar datos actuales
					std::cout << "\n--- Datos actuales del proveedor ---\n";
					std::cout << "RUT: " << prov->getRut() << " (no modificable)\n";
					std::cout << "Empresa: " << prov->getEmpresa() << "\n";
					std::cout << "Telefono: " << prov->getTelefono() << "\n";
					std::cout << "Contacto comercial: " << prov->getContactoComercial() << "\n";

					std::string nuevaEmpresa;
					std::string nuevoTelefono;
					std::string nuevoContacto;

					// Ingreso de nuevos datos
					std::cout << "\n--- Nuevos datos del proveedor ---\n";
					std::cout << "Nueva empresa: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nuevaEmpresa);
					std::cout << "Nuevo telefono: "; std::getline(std::cin, nuevoTelefono);
					std::cout << "Nuevo contacto comercial: "; std::getline(std::cin, nuevoContacto);

					// Mostrar resumen y confirmar
					std::cout << "\n--- Resumen de la modificacion ---\n";
					std::cout << "RUT: " << rut << " (no modificable)\n";
					std::cout << "Empresa anterior: " << prov->getEmpresa() << "\n";
					std::cout << "Empresa nueva: " << nuevaEmpresa << "\n";
					std::cout << "Telefono anterior: " << prov->getTelefono() << "\n";
					std::cout << "Telefono nuevo: " << nuevoTelefono << "\n";
					std::cout << "Contacto anterior: " << prov->getContactoComercial() << "\n";
					std::cout << "Contacto nuevo: " << nuevoContacto << "\n";
					std::cout << "1. Confirmar\n";
					std::cout << "2. Cancelar\n";
					int opcionConfirmar;
					std::cout << "Seleccione una opcion: ";
					std::cin >> opcionConfirmar;
					if (opcionConfirmar == 1) {
						bool ok = ctrl.modificarProveedor(rut, nuevaEmpresa, nuevoTelefono, nuevoContacto);
						std::cout << (ok ? "Proveedor modificado exitosamente." : "Error al modificar el proveedor.") << std::endl;
						break;
					} else {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
				}
			}
		} else if (op == 6) {
			// Asociar producto a proveedor
			auto todosProv = ctrl.listarProveedores();
			if (todosProv.empty()) {
				std::cout << "\nNo hay proveedores registrados. Debe crear un proveedor primero.\n";
			} else {
				std::cout << "\n--- Proveedores registrados ---\n";
				for (auto pr : todosProv) {
					if (pr) std::cout << "- " << pr->getRut() << " - " << pr->getEmpresa() << '\n';
				}

				std::string rut;
				Proveedor* prov = nullptr;

				// Bucle para seleccionar proveedor
				while (true) {
					std::cout << "\nIngrese el RUT del proveedor (0 para cancelar): ";
					std::cin >> rut;
					if (rut == "0") {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
					prov = ctrl.buscarProveedor(rut);
					if (prov == nullptr) {
						std::cout << "Error: No existe un proveedor con el RUT '" << rut << "'." << std::endl;
						std::cout << "1. Reingresar el RUT\n";
						std::cout << "2. Cancelar\n";
						int opcionError;
						std::cout << "Seleccione una opcion: ";
						std::cin >> opcionError;
						if (opcionError == 2) {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
						continue;
					}

					// Listar productos no asociados a este proveedor
					auto noAsociados = ctrl.listarProductosNoAsociadosAProveedor(rut);
					if (noAsociados.empty()) {
						std::cout << "\nTodos los productos del catalogo ya estan asociados a este proveedor.\n";
						break;
					}

					std::cout << "\n--- Productos disponibles (no asociados a " << prov->getEmpresa() << ") ---\n";
					for (auto p : noAsociados) {
						if (p) std::cout << "- codigo=" << p->getCodigo() << " nombre=" << p->getNombre() << '\n';
					}

					int codigoProducto;
					Producto* prod = nullptr;

					// Bucle para seleccionar producto
					while (true) {
						std::cout << "\nIngrese el codigo del producto (0 para cancelar): ";
						std::cin >> codigoProducto;
						if (codigoProducto == 0) {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
						prod = ctrl.buscarProducto(codigoProducto);
						if (prod == nullptr) {
							std::cout << "Error: No existe un producto con el codigo '" << codigoProducto << "'." << std::endl;
							std::cout << "1. Reingresar el codigo\n";
							std::cout << "2. Cancelar\n";
							int opcionError;
							std::cout << "Seleccione una opcion: ";
							std::cin >> opcionError;
							if (opcionError == 2) {
								std::cout << "Operacion cancelada." << std::endl;
								break;
							}
							continue;
						}

						// Verificar si el producto está en la lista de no asociados
						bool encontrado = false;
						for (auto p : noAsociados) {
							if (p != nullptr && p->getCodigo() == codigoProducto) {
								encontrado = true;
								break;
							}
						}
						if (!encontrado) {
							std::cout << "\nError: El producto con codigo '" << codigoProducto << "' ya esta asociado a este proveedor.\n";
							break;
						}

						// Verificar si ya existe la asociacion (doble comprobacion)
						ProveedorProducto* existente = ctrl.buscarAsociacion(rut, codigoProducto);
						if (existente != nullptr) {
							std::cout << "\nLa combinacion proveedor-producto ya existe.\n";
							std::cout << "Precio de compra actual: " << existente->getPrecioCompraPactado() << "\n";
							std::cout << "Tiempo de entrega actual: " << existente->getTiempoEntregaEstimadoEnDias() << " dias\n";
							std::cout << "1. Actualizar precio y tiempo de entrega\n";
							std::cout << "2. Cancelar\n";
							int opcionExistente;
							std::cout << "Seleccione una opcion: ";
							std::cin >> opcionExistente;
							if (opcionExistente == 2) {
								std::cout << "Operacion cancelada." << std::endl;
								break;
							}
							// Actualizar datos existentes
							int nuevoPrecio;
							int nuevoTiempo;
							std::cout << "Nuevo precio de compra pactado: ";
							std::cin >> nuevoPrecio;
							std::cout << "Nuevo tiempo de entrega estimado (dias): ";
							std::cin >> nuevoTiempo;

							std::cout << "\n--- Resumen de la actualizacion ---\n";
							std::cout << "Proveedor: " << prov->getEmpresa() << " (" << rut << ")\n";
							std::cout << "Producto: " << prod->getNombre() << " (codigo=" << codigoProducto << ")\n";
							std::cout << "Precio anterior: " << existente->getPrecioCompraPactado() << "\n";
							std::cout << "Precio nuevo: " << nuevoPrecio << "\n";
							std::cout << "Tiempo anterior: " << existente->getTiempoEntregaEstimadoEnDias() << " dias\n";
							std::cout << "Tiempo nuevo: " << nuevoTiempo << " dias\n";
							std::cout << "1. Confirmar\n";
							std::cout << "2. Cancelar\n";
							int opcionConfirmar;
							std::cout << "Seleccione una opcion: ";
							std::cin >> opcionConfirmar;
							if (opcionConfirmar == 1) {
								existente->setPrecioCompraPactado(nuevoPrecio);
								existente->setTiempoEntregaEstimadoEnDias(nuevoTiempo);
								std::cout << "Asociacion actualizada exitosamente." << std::endl;
							} else {
								std::cout << "Operacion cancelada." << std::endl;
							}
							break;
						}

						// No existe la asociacion, solicitar datos para crearla
						int precioCompra;
						int tiempoEntrega;
						std::cout << "\nPrecio de compra pactado: ";
						std::cin >> precioCompra;
						std::cout << "Tiempo de entrega estimado (dias): ";
						std::cin >> tiempoEntrega;

						// Mostrar resumen y confirmar
						std::cout << "\n--- Resumen de la asociacion ---\n";
						std::cout << "Proveedor: " << prov->getEmpresa() << " (" << rut << ")\n";
						std::cout << "Producto: " << prod->getNombre() << " (codigo=" << codigoProducto << ")\n";
						std::cout << "Precio de compra pactado: " << precioCompra << "\n";
						std::cout << "Tiempo de entrega estimado: " << tiempoEntrega << " dias\n";
						std::cout << "1. Confirmar\n";
						std::cout << "2. Cancelar\n";
						int opcionConfirmar;
						std::cout << "Seleccione una opcion: ";
						std::cin >> opcionConfirmar;
						if (opcionConfirmar == 1) {
							bool ok = ctrl.asociarProveedorProducto(rut, codigoProducto, precioCompra, tiempoEntrega);
							std::cout << (ok ? "Asociacion registrada exitosamente." : "Error al registrar la asociacion.") << std::endl;
						} else {
							std::cout << "Operacion cancelada." << std::endl;
						}
						break;
					}
					break;
				}
			}
		} else {
			std::cout << "Opcion invalida." << std::endl;
		}
	}
}
