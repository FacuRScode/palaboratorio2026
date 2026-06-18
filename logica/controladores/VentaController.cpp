//
// Created by facun on 2/6/2026.
//

#include "VentaController.h"

using namespace std;

// Inicializar el miembro estático
VentaController* VentaController::instanciaVenta = nullptr;

VentaController::VentaController(AdminController* admin) : adminCtrl(admin) {}

// Constructor privado usado por el singleton
VentaController::VentaController() : adminCtrl(nullptr) {}

VentaController* VentaController::getInstanciaVenta(){
	if (instanciaVenta == nullptr) {
		instanciaVenta = new VentaController();
	}
	return instanciaVenta;
}

VentaController::~VentaController() {}

// Calificaciones
bool VentaController::calificarProducto(int codigoProducto, Puntaje puntaje, const string& comentario, DTFecha fecha) {
	if (adminCtrl == nullptr) return false;
	Producto* p = adminCtrl->buscarProducto(codigoProducto);
	if (p == nullptr) return false;
	Calificacion* cal = new Calificacion(puntaje, comentario, fecha, p);
	return true;
}
