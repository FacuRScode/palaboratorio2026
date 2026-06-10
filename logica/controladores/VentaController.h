//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_VENTACONTROLLER_H
#define PALABORATORIO2026_VENTACONTROLLER_H

#pragma once
#include <vector>
#include <string>
#include "../dominio/Venta.h"
#include "../dominio/LineaDetalleVenta.h"
#include "../dominio/Calificacion.h"
#include "../dominio/Puntaje.h"
#include "../dominio/Cliente.h"
#include "AdminController.h"

using namespace std;

class EmpleadoController;

class VentaController {
private:
	static VentaController* instanciaVenta;
	vector<Venta*> ventas;
	AdminController* adminCtrl; // para acceder a productos
	EmpleadoController* empleadoCtrl; // para acceder a clientes
	VentaController();
public:
	VentaController(AdminController* admin, EmpleadoController* empleado);
	~VentaController();

	static VentaController* getInstanciaVenta();

	// Clientes (delega en EmpleadoController)
	Cliente* registrarCliente(const string& rut, const string& nombre, const string& apellido,
							  const string& direccion, const string& correo);

	// Ventas
	Venta* crearVenta(const string& rutCliente, DTFecha fecha, DTHora hora);
	bool agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad);
	vector<Venta*> listarVentas() const;
	vector<Venta*> listarVentasPorCliente(const string& rut) const;

	// Calificaciones
	bool calificarProducto(int codigoProducto, Puntaje puntaje, const string& comentario, DTFecha fecha);
};

#endif //PALABORATORIO2026_VENTACONTROLLER_H