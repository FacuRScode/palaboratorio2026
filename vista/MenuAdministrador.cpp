//
// Created by facun on 2/6/2026.
//


#include "MenuAdministrador.h"
#include <limits>

MenuAdministrador::MenuAdministrador(AdminController& controller) : ctrl(controller) {}

void MenuAdministrador::mostrar() {
	while (true) {
		std::cout << "\n--- Menu Administrador ---\n";
		std::cout << "1. Productos\n";
		std::cout << "2. Categorias\n";
		std::cout << "3. Empleados\n";
		std::cout << "4. Proveedores\n";
		std::cout << "0. Volver\n";
		int op;
		std::cout << "Seleccione una opcion: ";
		std::cin >> op;
		switch (op) {
			case 1: menuProductos(); break;
			case 2: menuCategorias(); break;
			case 3: menuEmpleados(); break;
			case 4: menuProveedores(); break;
			case 0: return;
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
			float puntaje;
			std::cout << "Codigo: "; std::cin >> codigo;
			std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
			std::cout << "Descripcion: "; std::getline(std::cin, descripcion);
			std::cout << "Precio venta actual: "; std::cin >> precio;
			std::cout << "Stock: "; std::cin >> stock;
			std::cout << "Puntaje promedio: "; std::cin >> puntaje;
			Producto* p = ctrl.crearProducto(codigo, nombre, descripcion, precio, stock, puntaje);
			if (p) std::cout << "Producto creado." << std::endl; else std::cout << "Error creando producto." << std::endl;
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
			bool ok = ctrl.eliminarProducto(codigo);
			std::cout << (ok ? "Producto eliminado." : "No se pudo eliminar.") << std::endl;
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
		std::cout << "0. Volver\n";
		int op;
		std::cout << "Seleccione una opcion: ";
		std::cin >> op;
		if (op == 0) return;
		if (op == 1) {
			std::string nombre;
			std::string descripcion;
			std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
			std::cout << "Descripcion: "; std::getline(std::cin, descripcion);
			auto c = ctrl.crearCategoria(nombre, descripcion);
			std::cout << (c ? "Categoria creada." : "Error creando categoria.") << std::endl;
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
		} else {
			std::cout << "Opcion invalida." << std::endl;
		}
	}
}

void MenuAdministrador::menuEmpleados() {
	while (true) {
		std::cout << "\n--- Empleados ---\n";
		std::cout << "1. Crear empleado\n";
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
			std::cout << "Nombre: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, nombre);
			std::cout << "Correo: "; std::cin >> correo;
			std::cout << "Contrasena: "; std::cin >> contrasena;
			std::cout << "Rol: "; std::cin >> rol;
			auto e = ctrl.crearEmpleado(nombre, correo, contrasena, rol);
			std::cout << (e ? "Empleado creado." : "Error creando empleado.") << std::endl;
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
			std::cout << (p ? "Proveedor creado." : "Error creando proveedor.") << std::endl;
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
		} else {
			std::cout << "Opcion invalida." << std::endl;
		}
	}
}
