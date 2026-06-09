
//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_VENTACONTROLLER_H
#define PALABORATORIO2026_VENTACONTROLLER_H

#pragma once
#include <vector>
#include <string>
#include "../dominio/Cliente.h"
#include "../dominio/Venta.h"
#include "../dominio/LineaDetalleVenta.h"
#include "../dominio/Calificacion.h"
#include "../dominio/Puntaje.h"
#include "AdminController.h"

using namespace std;

class VentaController {
private:
	vector<Cliente*> clientes;
	vector<Venta*> ventas;
	AdminController* adminCtrl; // para acceder a productos
public:
	VentaController(AdminController* admin = nullptr);
	~VentaController();

	// Clientes
	Cliente* registrarCliente(const string& rut, const string& nombre, const string& apellido,
							  const string& direccion, const string& correo);
	Cliente* buscarCliente(const string& rut) const;
	vector<Cliente*> listarClientes() const;

	// Ventas
	Venta* crearVenta(const string& rutCliente, DTFecha fecha, DTHora hora);
	bool agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad);
	vector<Venta*> listarVentas() const;
	vector<Venta*> listarVentasPorCliente(const string& rut) const;

	// Calificaciones
	bool calificarProducto(int codigoProducto, Puntaje puntaje, const string& comentario, DTFecha fecha);
};

#endif //PALABORATORIO2026_VENTACONTROLLER_H
