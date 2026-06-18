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

class VentaController {
private:
	static VentaController* instanciaVenta;
	AdminController* adminCtrl; // para acceder a productos
	VentaController();
public:
	VentaController(AdminController* admin);
	~VentaController();

	static VentaController* getInstanciaVenta();

	// Calificaciones
	bool calificarProducto(int codigoProducto, Puntaje puntaje, const string& comentario, DTFecha fecha);
};

#endif //PALABORATORIO2026_VENTACONTROLLER_H
