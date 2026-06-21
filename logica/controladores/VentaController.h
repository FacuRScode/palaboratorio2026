//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_VENTACONTROLLER_H
#define PALABORATORIO2026_VENTACONTROLLER_H

#pragma once
#include <vector>
#include <string>
#include "../dominio/Calificacion.h"
#include "../dominio/Puntaje.h"
#include "../dominio/DTFecha.h"
#include "AdminController.h"

using namespace std;

class EmpleadoController; // forward declaration

class VentaController {
private:
	static VentaController* instanciaVenta;
	AdminController* adminCtrl; // para acceder a productos
	EmpleadoController* empleadoCtrl; // para acceder a clientes y ventas
	VentaController();
public:
	VentaController(AdminController* admin, EmpleadoController* empleado);
	~VentaController();

	static VentaController* getInstanciaVenta();

	// Calificaciones
	vector<Producto*> listarProductosComprados(const string& correoCliente);
	bool calificarProducto(int codigoProducto, Puntaje puntaje, const string& comentario, const string& correoCliente);
	vector<Calificacion*> listarCalificacionesDeProducto(int codigoProducto);
};

#endif //PALABORATORIO2026_VENTACONTROLLER_H
