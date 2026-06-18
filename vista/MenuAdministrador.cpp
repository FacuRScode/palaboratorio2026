//
// Created by facun on 2/6/2026.
//


#include "MenuAdministrador.h"
#include <limits>

MenuAdministrador::MenuAdministrador(AdminController& controller, AuthController* auth,
									 EmpleadoController* empleados)
	: ctrl(controller), authCtrl(auth), empleadoCtrl(empleados) {}

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
		std::cout << "0. Cerrar sesion\n";
		int op;
		std::cout << "Seleccione una opcion: ";
		std::cin >> op;
		switch (op) {
			case 1: menuProductos(); break;
			case 2: menuCategorias(); break;
			case 3: menuEmpleados(); break;
			case 4: menuProveedores(); break;
			case 0:
				if (authCtrl) authCtrl->cerrarSesion();
				std::cout << "Sesion cerrada." << std::endl;
				return;
			default: std::cout << "Opcion invalida." << std::endl;
		}
	}
}

void MenuAdministrador::menuProductos() {
	while (true) {
		std::cout << "\n--- Productos ---\n";
		std::cout << "1. Crear producto\n";
		std::cout << "2. Listar productos\n";
		std::cout << "3. Buscar producto por codigo\n";
		std::cout << "4. Eliminar producto por codigo\n";
		std::cout << "5. Modificar producto\n";
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
			bool datosValidos = false;

			while (!datosValidos) {
				std::cout << "\n--- Alta de producto ---\n";
				std::cout << "Codigo: "; std::cin >> codigo;
				std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
				std::cout << "Descripcion: "; std::getline(std::cin, descripcion);
				std::cout << "Precio de venta unitario: "; std::cin >> precio;

				// Verificar si el codigo ya existe
				if (ctrl.buscarProducto(codigo) != nullptr) {
					std::cout << "\nError: Ya existe un producto con el codigo '" << codigo << "'." << std::endl;
					std::cout << "1. Reingresar los datos\n";
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

				// Listar categorias existentes y seleccionar una
				auto categorias = ctrl.listarCategorias();
				if (categorias.empty()) {
					std::cout << "\nError: No hay categorias registradas. Debe crear una categoria antes de agregar un producto." << std::endl;
					break;
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
						std::cout << "Error al crear el producto." << std::endl;
					}
					datosValidos = true;
				} else {
					std::cout << "Operacion cancelada." << std::endl;
					datosValidos = true;
				}
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
			// Listar productos que no tienen ventas ni ordenes de compra pendientes
			auto todos = ctrl.listarProductos();
			vector<Producto*> eliminables;
			for (auto p : todos) {
				if (p == nullptr) continue;
	bool tieneVentas = empleadoCtrl ? empleadoCtrl->productoEstaEnVentas(p->getCodigo()) : false;
	bool tieneOrdenesPendientes = empleadoCtrl ? empleadoCtrl->productoEstaEnOrdenesPendientes(p->getCodigo()) : false;
				if (!tieneVentas && !tieneOrdenesPendientes) {
					eliminables.push_back(p);
				}
			}

			if (eliminables.empty()) {
				std::cout << "\nNo hay productos que puedan eliminarse. Todos tienen ventas u ordenes de compra pendientes asociadas.\n";
			} else {
				std::cout << "\n--- Productos que pueden eliminarse (sin ventas ni ordenes pendientes) ---\n";
				for (auto p : eliminables) {
					if (p) {
						std::cout << "- codigo=" << p->getCodigo() << " nombre=" << p->getNombre() << '\n';
					}
				}

				int codigo;
				Producto* prod = nullptr;

				// Bucle para seleccionar producto a eliminar
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

					// Verificar que el producto siga siendo eliminable
	bool tieneVentas = empleadoCtrl ? empleadoCtrl->productoEstaEnVentas(codigo) : false;
					bool tieneOrdenesPendientes = empleadoCtrl ? empleadoCtrl->productoEstaEnOrdenesPendientes(codigo) : false;
					if (tieneVentas || tieneOrdenesPendientes) {
						std::cout << "\nError: El producto tiene ventas u ordenes de compra pendientes asociadas y no puede eliminarse.\n";
						break;
					}

					// Mostrar datos del producto seleccionado
					std::cout << "\n--- Datos del producto a eliminar ---\n";
					std::cout << "Codigo: " << prod->getCodigo() << "\n";
					std::cout << "Nombre: " << prod->getNombre() << "\n";
					std::cout << "Descripcion: " << prod->getDescripcion() << "\n";
					std::cout << "Precio de venta unitario: " << prod->getPrecioVentaActual() << "\n";
					std::cout << "Stock actual: " << prod->getStock() << "\n";
					std::string catActual = prod->getCategoria() ? prod->getCategoria()->getNombre() : "(sin categoria)";
					std::cout << "Categoria: " << catActual << "\n";

					// Verificar si tiene asociaciones con proveedores
					bool asociadoProveedor = ctrl.productoAsociadoAProveedor(codigo);
					if (asociadoProveedor) {
						std::cout << "(Tiene asociaciones con proveedores que seran eliminadas)\n";
					}

					std::cout << "\n1. Confirmar eliminacion\n";
					std::cout << "2. Cancelar\n";
					int opcionConfirmar;
					std::cout << "Seleccione una opcion: ";
					std::cin >> opcionConfirmar;
					if (opcionConfirmar == 1) {
						bool ok = ctrl.eliminarProducto(codigo, true);
						std::cout << (ok ? "Producto eliminado exitosamente junto con sus asociaciones." : "Error al eliminar el producto.") << std::endl;
						break;
					} else {
						std::cout << "Operacion cancelada." << std::endl;
						break;
					}
				}
			}
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

					// Bucle para ingreso de nuevos datos con validacion
					while (true) {
						std::cout << "\n--- Nuevos datos del producto ---\n";
						std::cout << "Nuevo nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nuevoNombre);
						std::cout << "Nueva descripcion: "; std::getline(std::cin, nuevaDescripcion);
						std::cout << "Nuevo precio de venta unitario: "; std::cin >> nuevoPrecio;

						// Verificar si el nuevo nombre ya pertenece a otro producto
						Producto* existente = ctrl.buscarProductoPorNombre(nuevoNombre);
						if (existente != nullptr && existente->getCodigo() != codigo) {
							std::cout << "\nError: Ya existe otro producto con el nombre '" << nuevoNombre << "'." << std::endl;
							std::cout << "1. Reingresar los datos\n";
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
							std::cout << (ok ? "Producto modificado exitosamente." : "Error al modificar el producto.") << std::endl;
							break;
						} else {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
					}
					break;
				}
			}
		} else {
			std::cout << "Opcion invalida." << std::endl;
		}
	}
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
			// Listar categorias existentes
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

			// Bucle para ingreso de datos con validacion de nombre unico
			while (true) {
				std::cout << "\n--- Nueva categoria ---\n";
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nombre: "; std::getline(std::cin, nombre);
				std::cout << "Descripcion: "; std::getline(std::cin, descripcion);

				// Verificar si ya existe una categoria con ese nombre
				if (ctrl.buscarCategoria(nombre) != nullptr) {
					std::cout << "\nError: Ya existe una categoria con el nombre '" << nombre << "'." << std::endl;
					std::cout << "1. Reingresar los datos\n";
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

				// Mostrar resumen y confirmar
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
						std::cout << "Error al crear la categoria." << std::endl;
					}
					break;
				} else {
					std::cout << "Operacion cancelada." << std::endl;
					break;
				}
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

					// Bucle para ingreso de nuevos datos con validacion
					while (true) {
						std::cout << "\n--- Nuevos datos ---\n";
						std::cout << "Nuevo nombre: "; std::getline(std::cin, nuevoNombre);
						std::cout << "Nueva descripcion: "; std::getline(std::cin, nuevaDescripcion);

						// Verificar si el nuevo nombre ya existe en otra categoria
						if (nuevoNombre != nombreActual && ctrl.buscarCategoria(nuevoNombre) != nullptr) {
							std::cout << "\nError: Ya existe otra categoria con el nombre '" << nuevoNombre << "'." << std::endl;
							std::cout << "1. Reingresar los datos\n";
							std::cout << "2. Cancelar\n";
							int opcionError;
							std::cout << "Seleccione una opcion: ";
							std::cin >> opcionError;
							std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							if (opcionError == 2) {
								std::cout << "Operacion cancelada." << std::endl;
								break;
							}
							continue;
						}

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
							std::cout << (ok ? "Categoria modificada exitosamente." : "Error al modificar la categoria.") << std::endl;
							break;
						} else {
							std::cout << "Operacion cancelada." << std::endl;
							break;
						}
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
				bool datosValidos = false;
				while (!datosValidos) {
					std::cout << "\n--- Alta de empleado ---\n";
					std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
					std::cout << "Correo: "; std::cin >> correo;
					std::cout << "Contrasena: "; std::cin >> contrasena;

					// Verificar si el correo ya esta registrado
					if (ctrl.buscarEmpleado(correo) != nullptr) {
						std::cout << "\nError: Ya existe un empleado registrado con el correo '" << correo << "'." << std::endl;
						std::cout << "1. Reingresar los datos\n";
						std::cout << "2. Cancelar\n";
						int opcionError;
						std::cout << "Seleccione una opcion: ";
						std::cin >> opcionError;
						if (opcionError == 2) {
							std::cout << "Operacion cancelada." << std::endl;
							break; // Sale del bucle de ingreso, vuelve al menu de empleados
						}
						continue; // Reintentar ingreso de datos
					}

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
							std::cout << "Error al crear el empleado." << std::endl;
						}
						datosValidos = true;
					} else {
						std::cout << "Operacion cancelada." << std::endl;
						datosValidos = true;
					}
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
			bool datosValidos = false;
			while (!datosValidos) {
				std::cout << "\n--- Alta de proveedor ---\n";
				std::cout << "RUT: "; std::cin >> rut;
				std::cout << "Empresa: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, empresa);
				std::cout << "Telefono: "; std::getline(std::cin, telefono);
				std::cout << "Contacto comercial: "; std::getline(std::cin, contacto);

				// Verificar si el RUT ya esta registrado
				if (ctrl.buscarProveedor(rut) != nullptr) {
					std::cout << "\nError: Ya existe un proveedor registrado con el RUT '" << rut << "'." << std::endl;
					std::cout << "1. Reingresar los datos\n";
					std::cout << "2. Cancelar\n";
					int opcionError;
					std::cout << "Seleccione una opcion: ";
					std::cin >> opcionError;
					if (opcionError == 2) {
						std::cout << "Operacion cancelada." << std::endl;
						break; // Sale del bucle de ingreso, vuelve al menu de proveedores
					}
					continue; // Reintentar ingreso de datos
				}

				// Mostrar resumen y confirmar
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
						std::cout << "Error al crear el proveedor." << std::endl;
					}
					datosValidos = true;
				} else {
					std::cout << "Operacion cancelada." << std::endl;
					datosValidos = true;
				}
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
