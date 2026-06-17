//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_ADMINCONTROLLER_H
#define PALABORATORIO2026_ADMINCONTROLLER_H

#pragma once
#include <vector>
#include <string>
#include "../dominio/Producto.h"
#include "../dominio/Categoria.h"
#include "../dominio/Empleado.h"
#include "../dominio/Proveedor.h"

using namespace std;

class AdminController {
private:
	static AdminController* instanciaAdmin;
	vector<Producto*> productos;
	vector<Categoria*> categorias;
	vector<Empleado*> empleados;
	vector<Proveedor*> proveedores;

public:
	AdminController();
	~AdminController();

	static AdminController* getInstanciaAdmin();

	// -- Productos -------------------------------------------------
	Producto* crearProducto(int codigo, const string& nombre, const string& descripcion,
							 float precioVentaActual, int stock, float puntajePromedio);
	Producto* buscarProducto(int codigo) const;
	bool eliminarProducto(int codigo);
	vector<Producto*> listarProductos() const;

	// -- Categorías -----------------------------------------------
	Categoria* crearCategoria(const string& nombre, const string& descripcion);
	Categoria* buscarCategoria(const string& nombre) const;
	bool eliminarCategoria(const string& nombre);
	vector<Categoria*> listarCategorias() const;
	bool asignarProductoACategoria(int codigoProducto, const string& nombreCategoria);
	bool modificarCategoria(const string& nombreActual, const string& nuevoNombre, const string& nuevaDescripcion);

	// -- Empleados -----------------------------------------------
	Empleado* crearEmpleado(const string& nombre, const string& correo,
							 const string& contrasena, const string& rol);
	Empleado* buscarEmpleado(const string& correo) const;
	bool eliminarEmpleado(const string& correo);
	vector<Empleado*> listarEmpleados() const;

	// -- Proveedores ---------------------------------------------
	Proveedor* crearProveedor(const string& rut, const string& empresa,
							  const string& telefono, const string& contactoComercial);
	Proveedor* buscarProveedor(const string& rut) const;
	bool eliminarProveedor(const string& rut);
	vector<Proveedor*> listarProveedores() const;
};

#endif //PALABORATORIO2026_ADMINCONTROLLER_H


