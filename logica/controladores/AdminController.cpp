//
// Created by facun on 2/6/2026.
//

#include "AdminController.h"
#include <algorithm>

using namespace std;

// Inicializar el miembro estático
AdminController* AdminController::instanciaAdmin = nullptr;

// Constructor privado usado por el singleton
AdminController::AdminController() {}

AdminController* AdminController::getInstanciaAdmin(){
	if (instanciaAdmin == nullptr) {
		instanciaAdmin = new AdminController();
	}
	return instanciaAdmin;
}

AdminController::~AdminController() {
	for (Producto* p : productos) delete p;
	for (Categoria* c : categorias) delete c;
	for (Empleado* e : empleados) delete e;
	for (Proveedor* pr : proveedores) delete pr;
}

// -- Productos -------------------------------------------------
Producto* AdminController::crearProducto(int codigo, const string& nombre, const string& descripcion,
										 float precioVentaActual, int stock, float puntajePromedio) {
	if (buscarProducto(codigo) != nullptr) return nullptr; // ya existe
	Producto* p = new Producto(codigo, nombre, descripcion, precioVentaActual, stock, puntajePromedio, nullptr);
	productos.push_back(p);
	return p;
}

Producto* AdminController::buscarProducto(int codigo) const {
	for (Producto* p : productos) {
		if (p != nullptr && p->getCodigo() == codigo) return p;
	}
	return nullptr;
}

bool AdminController::eliminarProducto(int codigo) {
	for (auto it = productos.begin(); it != productos.end(); ++it) {
		Producto* p = *it;
		if (p != nullptr && p->getCodigo() == codigo) {
			delete p;
			productos.erase(it);
			return true;
		}
	}
	return false;
}

vector<Producto*> AdminController::listarProductos() const {
	return productos;
}

// -- Categorías -----------------------------------------------
Categoria* AdminController::crearCategoria(const string& nombre, const string& descripcion) {
	if (buscarCategoria(nombre) != nullptr) return nullptr;
	Categoria* c = new Categoria(nombre, descripcion);
	categorias.push_back(c);
	return c;
}

Categoria* AdminController::buscarCategoria(const string& nombre) const {
	for (Categoria* c : categorias) {
		if (c != nullptr && c->getNombre() == nombre) return c;
	}
	return nullptr;
}

bool AdminController::eliminarCategoria(const string& nombre) {
	for (auto it = categorias.begin(); it != categorias.end(); ++it) {
		Categoria* c = *it;
		if (c != nullptr && c->getNombre() == nombre) {
			delete c;
			categorias.erase(it);
			return true;
		}
	}
	return false;
}

vector<Categoria*> AdminController::listarCategorias() const {
	return categorias;
}

bool AdminController::asignarProductoACategoria(int codigoProducto, const string& nombreCategoria) {
	Producto* p = buscarProducto(codigoProducto);
	Categoria* c = buscarCategoria(nombreCategoria);
	if (p == nullptr || c == nullptr) return false;
	p->setCategoria(c);
	return true;
}

bool AdminController::modificarCategoria(const string& nombreActual, const string& nuevoNombre, const string& nuevaDescripcion) {
	Categoria* c = buscarCategoria(nombreActual);
	if (c == nullptr) return false;

	// Si se cambio el nombre, verificar que el nuevo no exista ya en otra categoria
	if (nombreActual != nuevoNombre && buscarCategoria(nuevoNombre) != nullptr) return false;

	c->setNombre(nuevoNombre);
	c->setDescripcion(nuevaDescripcion);
	return true;
}

// -- Empleados -----------------------------------------------
Empleado* AdminController::crearEmpleado(const string& nombre, const string& correo,
										 const string& contrasena, const string& rol) {
	if (buscarEmpleado(correo) != nullptr) return nullptr;
	Empleado* e = new Empleado(nombre, correo, contrasena, rol);
	empleados.push_back(e);
	return e;
}

Empleado* AdminController::buscarEmpleado(const string& correo) const {
	for (Empleado* e : empleados) {
		if (e != nullptr && e->getCorreo() == correo) return e;
	}
	return nullptr;
}

bool AdminController::eliminarEmpleado(const string& correo) {
	for (auto it = empleados.begin(); it != empleados.end(); ++it) {
		Empleado* e = *it;
		if (e != nullptr && e->getCorreo() == correo) {
			delete e;
			empleados.erase(it);
			return true;
		}
	}
	return false;
}

vector<Empleado*> AdminController::listarEmpleados() const {
	return empleados;
}

// -- Proveedores ---------------------------------------------
Proveedor* AdminController::crearProveedor(const string& rut, const string& empresa,
										  const string& telefono, const string& contactoComercial) {
	if (buscarProveedor(rut) != nullptr) return nullptr;
	Proveedor* pr = new Proveedor(rut, empresa, telefono, contactoComercial);
	proveedores.push_back(pr);
	return pr;
}

Proveedor* AdminController::buscarProveedor(const string& rut) const {
	for (Proveedor* pr : proveedores) {
		if (pr != nullptr && pr->getRut() == rut) return pr;
	}
	return nullptr;
}

bool AdminController::eliminarProveedor(const string& rut) {
	for (auto it = proveedores.begin(); it != proveedores.end(); ++it) {
		Proveedor* pr = *it;
		if (pr != nullptr && pr->getRut() == rut) {
			delete pr;
			proveedores.erase(it);
			return true;
		}
	}
	return false;
}

vector<Proveedor*> AdminController::listarProveedores() const {
	return proveedores;
}

