//
// Created by facun on 2/6/2026.
//

#include "VentaController.h"
#include "EmpleadoController.h"
#include <algorithm>

using namespace std;

// Inicializar el miembro estático
VentaController* VentaController::instanciaVenta = nullptr;

VentaController::VentaController(AdminController* admin, EmpleadoController* empleado) : adminCtrl(admin), empleadoCtrl(empleado) {}

// Constructor privado usado por el singleton
VentaController::VentaController() : adminCtrl(nullptr), empleadoCtrl(nullptr) {}

VentaController* VentaController::getInstanciaVenta(){
	if (instanciaVenta == nullptr) {
		instanciaVenta = new VentaController();
	}
	return instanciaVenta;
}

VentaController::~VentaController() {
	for (Venta* v : ventas) delete v;
}

// Clientes (delega en EmpleadoController)
Cliente* VentaController::registrarCliente(const string& rut, const string& nombre, const string& apellido,
                                            const string& direccion, const string& correo) {
    if (empleadoCtrl == nullptr) return nullptr;
    return empleadoCtrl->registrarCliente(rut, nombre, apellido, direccion, correo);
}

// Ventas
Venta* VentaController::crearVenta(const string& rutCliente, DTFecha fecha, DTHora hora) {
	if (empleadoCtrl == nullptr) return nullptr;
	Cliente* c = empleadoCtrl->buscarCliente(rutCliente);
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
	if (empleadoCtrl == nullptr) return {};
	Cliente* c = empleadoCtrl->buscarCliente(rut);
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