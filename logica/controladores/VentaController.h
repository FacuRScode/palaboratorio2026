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

struct ProductoClienteInfo {
	int codigo;
	string nombre;
	string descripcion;
};

struct CalificacionClienteInfo {
	DTFecha fecha;
	int puntaje;
	string comentario;
};

struct ResultadoCalificacionCliente {
	bool exito;
	bool productoExiste;
	bool puntajeValido;
	bool productoComprado;
	string nombreProducto;
	float puntajePromedioActualizado;
};

struct ResultadoDetalleProductoCliente {
	bool productoExiste;
	int codigo;
	string nombre;
	string descripcion;
	string categoria;
	float precioVentaUnitario;
	int stockActual;
	float puntajePromedio;
	int cantidadCalificaciones;
	vector<CalificacionClienteInfo> ultimasCalificaciones;
};

struct CalificacionEmpleadoVistaInfo {
	DTFecha fecha;
	int puntaje;
	string rutCliente;
	string comentario;
};

struct ResultadoCalificacionesProductoVista {
	bool productoExiste;
	string nombreProducto;
	float puntajePromedio;
	int cantidadCalificaciones;
	vector<CalificacionEmpleadoVistaInfo> calificaciones;
};

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
	vector<ProductoClienteInfo> listarProductosCompradosCliente(const string& correoCliente);
	vector<ProductoClienteInfo> listarProductosCatalogoCliente() const;
	ResultadoCalificacionCliente registrarCalificacionCliente(const string& correoCliente, int codigoProducto, int puntaje, const string& comentario);
	ResultadoDetalleProductoCliente obtenerDetalleProductoCliente(int codigoProducto);
	ResultadoCalificacionesProductoVista obtenerCalificacionesProductoVista(int codigoProducto);
};

#endif //PALABORATORIO2026_VENTACONTROLLER_H
