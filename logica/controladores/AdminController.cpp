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
										 float precioVentaActual, int stock, int stockMinimo, const string& nombreCategoria) {
	if (buscarProducto(codigo) != nullptr) return nullptr; // ya existe
	Categoria* cat = buscarCategoria(nombreCategoria);
	if (cat == nullptr) return nullptr; // categoria no existe
	Producto* p = new Producto(codigo, nombre, descripcion, precioVentaActual, stock, stockMinimo, 0, cat);
	productos.push_back(p);
	return p;
}

Producto* AdminController::buscarProducto(int codigo) const {
	for (Producto* p : productos) {
		if (p != nullptr && p->getCodigo() == codigo) return p;
	}
	return nullptr;
}

Producto* AdminController::buscarProductoPorNombre(const string& nombre) const {
	for (Producto* p : productos) {
		if (p != nullptr && p->getNombre() == nombre) return p;
	}
	return nullptr;
}

bool AdminController::modificarProducto(int codigo, const string& nuevoNombre, const string& nuevaDescripcion,
									   float nuevoPrecio, const string& nombreCategoria, int nuevoStockMinimo) {
	Producto* p = buscarProducto(codigo);
	if (p == nullptr) return false;

	// Si el nombre cambia, verificar que no exista otro producto con ese nombre
	if (p->getNombre() != nuevoNombre && buscarProductoPorNombre(nuevoNombre) != nullptr) return false;

	Categoria* cat = buscarCategoria(nombreCategoria);
	if (cat == nullptr) return false;

	p->setNombre(nuevoNombre);
	p->setDescripcion(nuevaDescripcion);
	p->setPrecioVentaActual(nuevoPrecio);
	p->setCategoria(cat);
	p->setStockMinimo(nuevoStockMinimo);
	return true;
}

bool AdminController::productoAsociadoAProveedor(int codigoProducto) const {
	for (Proveedor* pr : proveedores) {
		if (pr == nullptr) continue;
		for (ProveedorProducto* pp : pr->getProductosOfrecidos()) {
			if (pp != nullptr && pp->getProducto() != nullptr && pp->getProducto()->getCodigo() == codigoProducto) {
				return true;
			}
		}
	}
	return false;
}

bool AdminController::eliminarProducto(int codigo, bool eliminarAsociaciones) {
	for (auto it = productos.begin(); it != productos.end(); ++it) {
		Producto* p = *it;
		if (p != nullptr && p->getCodigo() == codigo) {
			// Si se solicita, eliminar las asociaciones con proveedores
			if (eliminarAsociaciones) {
				for (Proveedor* pr : proveedores) {
					if (pr == nullptr) continue;
					auto& ofrecidos = const_cast<vector<ProveedorProducto*>&>(pr->getProductosOfrecidos());
					for (auto itPr = ofrecidos.begin(); itPr != ofrecidos.end(); ) {
						if ((*itPr) != nullptr && (*itPr)->getProducto() != nullptr && (*itPr)->getProducto()->getCodigo() == codigo) {
							delete *itPr;
							itPr = ofrecidos.erase(itPr);
						} else {
							++itPr;
						}
					}
				}
			}
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

bool AdminController::modificarProveedor(const string& rut, const string& empresa,
										const string& telefono, const string& contactoComercial) {
	Proveedor* pr = buscarProveedor(rut);
	if (pr == nullptr) return false;
	pr->setEmpresa(empresa);
	pr->setTelefono(telefono);
	pr->setContactoComercial(contactoComercial);
	return true;
}

vector<Proveedor*> AdminController::listarProveedores() const {
	return proveedores;
}

// -- Proveedor-Producto ------------------------------------------
vector<Producto*> AdminController::listarProductosNoAsociadosAProveedor(const string& rutProveedor) const {
	Proveedor* pr = buscarProveedor(rutProveedor);
	if (pr == nullptr) return {};

	vector<Producto*> noAsociados;
	for (Producto* p : productos) {
		if (p == nullptr) continue;
		bool asociado = false;
		for (ProveedorProducto* pp : pr->getProductosOfrecidos()) {
			if (pp != nullptr && pp->getProducto() != nullptr && pp->getProducto()->getCodigo() == p->getCodigo()) {
				asociado = true;
				break;
			}
		}
		if (!asociado) {
			noAsociados.push_back(p);
		}
	}
	return noAsociados;
}

ProveedorProducto* AdminController::buscarAsociacion(const string& rutProveedor, int codigoProducto) const {
	Proveedor* pr = buscarProveedor(rutProveedor);
	if (pr == nullptr) return nullptr;
	for (ProveedorProducto* pp : pr->getProductosOfrecidos()) {
		if (pp != nullptr && pp->getProducto() != nullptr && pp->getProducto()->getCodigo() == codigoProducto) {
			return pp;
		}
	}
	return nullptr;
}

bool AdminController::asociarProveedorProducto(const string& rutProveedor, int codigoProducto,
											   int precioCompra, int tiempoEntrega) {
	Proveedor* pr = buscarProveedor(rutProveedor);
	Producto* p = buscarProducto(codigoProducto);
	if (pr == nullptr || p == nullptr) return false;

	// Si ya existe, no se crea de nuevo (la vista debe verificar antes)
	if (buscarAsociacion(rutProveedor, codigoProducto) != nullptr) return false;

	ProveedorProducto* pp = new ProveedorProducto(precioCompra, tiempoEntrega, p);
	pr->addProveedorProducto(pp);
	return true;
}

