
//
// Created by facun on 2/6/2026.
//

#include "VentaController.h"
#include <algorithm>

using namespace std;

VentaController::VentaController(AdminController* admin) : adminCtrl(admin) {}

VentaController::~VentaController() {
	for (Cliente* c : clientes) delete c;
	for (Venta* v : ventas) delete v;
}

// Clientes
Cliente* VentaController::registrarCliente(const string& rut, const string& nombre, const string& apellido,
											const string& direccion, const string& correo) {
	if (buscarCliente(rut) != nullptr) return nullptr; // ya existe
	Cliente* c = new Cliente(rut, nombre, apellido, direccion, correo);
	clientes.push_back(c);
	return c;
}

Cliente* VentaController::buscarCliente(const string& rut) const {
	for (Cliente* c : clientes) {
		if (c != nullptr && c->getRut() == rut) return c;
	}
	return nullptr;
}

vector<Cliente*> VentaController::listarClientes() const {
	return clientes;
}

// Ventas
Venta* VentaController::crearVenta(const string& rutCliente, DTFecha fecha, DTHora hora) {
	Cliente* c = buscarCliente(rutCliente);
	if (c == nullptr) return nullptr;
	Venta* v = new Venta(fecha, hora);
	ventas.push_back(v);
	c->addVenta(v);
	return v;
}

bool VentaController::agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad) {
	if (venta == nullptr) return false;
	if (adminCtrl == nullptr) return false;
	Producto* p = adminCtrl->buscarProducto(codigoProducto);
	if (p == nullptr) return false;
	if (p->getStock() < cantidad) return false;
	// crear linea con precio actual del producto
	LineaDetalleVenta* linea = new LineaDetalleVenta(cantidad, p->getPrecioVentaActual());
	venta->addLinea(linea);
	// disminuir stock
	p->setStock(p->getStock() - cantidad);
	return true;
}

vector<Venta*> VentaController::listarVentas() const {
	return ventas;
}

vector<Venta*> VentaController::listarVentasPorCliente(const string& rut) const {
	Cliente* c = buscarCliente(rut);
	if (c == nullptr) return {};
	return c->getVentas();
}

// Calificaciones
bool VentaController::calificarProducto(int codigoProducto, Puntaje puntaje, const string& comentario, DTFecha fecha) {
	if (adminCtrl == nullptr) return false;
	Producto* p = adminCtrl->buscarProducto(codigoProducto);
	if (p == nullptr) return false;
	Calificacion* cal = new Calificacion(puntaje, comentario, fecha);
	p->addCalificacion(cal);
	return true;
}
