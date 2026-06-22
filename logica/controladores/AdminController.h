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

// Forward declaration para evitar dependencia circular
class EmpleadoController;

struct ProveedorProductoInfo {
	string rutProveedor;
	string empresaProveedor;
	int precioCompraPactado;
	int tiempoEntregaDias;
};

struct DetalleProductoAdmin {
	bool productoExiste;
	Producto* producto;
	vector<ProveedorProductoInfo> proveedores;
};

struct ResultadoGestionProveedorProducto {
	bool exito;
	bool proveedorExiste;
	bool productoExiste;
	bool actualizada;
};

struct ResultadoAltaEmpleado {
	bool exito;
	bool rolValido;
	bool correoDisponible;
	string rolAsignado;
};

class AdminController {
private:
	static AdminController* instanciaAdmin;
	vector<Producto*> productos;
	vector<Categoria*> categorias;
	vector<Empleado*> empleados;
	vector<Proveedor*> proveedores;
	EmpleadoController* empleadoCtrlRef;

public:
	AdminController();
	~AdminController();

	static AdminController* getInstanciaAdmin();
	void setEmpleadoController(EmpleadoController* empleadoController);

	// -- Productos -------------------------------------------------
	Producto* crearProducto(int codigo, const string& nombre, const string& descripcion,
							 float precioVentaActual, int stock, int stockMinimo, const string& nombreCategoria);
	Producto* buscarProducto(int codigo) const;
	Producto* buscarProductoPorNombre(const string& nombre) const;
	bool modificarProducto(int codigo, const string& nuevoNombre, const string& nuevaDescripcion,
						   float nuevoPrecio, const string& nombreCategoria, int nuevoStockMinimo);
	bool eliminarProducto(int codigo, bool eliminarAsociaciones = false);
	bool productoAsociadoAProveedor(int codigoProducto) const;
	vector<Producto*> listarProductos() const;
	vector<Producto*> listarProductosPorCategoria(const string& nombreCategoria) const;
	vector<Producto*> listarProductosConStockBajo(bool ordenarPorCriticidad) const;
	DetalleProductoAdmin obtenerDetalleProductoAdmin(int codigoProducto) const;

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
	ResultadoAltaEmpleado crearEmpleadoConRolOpcion(const string& nombre, const string& correo,
													const string& contrasena, int opcionRol);
	Empleado* buscarEmpleado(const string& correo) const;
	bool eliminarEmpleado(const string& correo);
	vector<Empleado*> listarEmpleados() const;

	// -- Proveedores ---------------------------------------------
	Proveedor* crearProveedor(const string& rut, const string& empresa,
							  const string& telefono, const string& contactoComercial);
	Proveedor* buscarProveedor(const string& rut) const;
	bool eliminarProveedor(const string& rut);
	vector<Proveedor*> listarProveedores() const;
	bool modificarProveedor(const string& rut, const string& empresa,
							 const string& telefono, const string& contactoComercial);

	// -- Proveedor-Producto --------------------------------------
	vector<Producto*> listarProductosNoAsociadosAProveedor(const string& rutProveedor) const;
	ProveedorProducto* buscarAsociacion(const string& rutProveedor, int codigoProducto) const;
	bool asociarProveedorProducto(const string& rutProveedor, int codigoProducto,
								  int precioCompra, int tiempoEntrega);
	bool actualizarAsociacionProveedorProducto(const string& rutProveedor, int codigoProducto,
											   int nuevoPrecioCompra, int nuevoTiempoEntrega);
	ResultadoGestionProveedorProducto gestionarAsociacionProveedorProducto(const string& rutProveedor, int codigoProducto,
																		  int precioCompra, int tiempoEntrega);
	bool existeProveedorProducto(const string& rutProveedor, int codigoProducto) const;
	vector<ProveedorProducto*> listarAsociacionesDeProducto(int codigoProducto) const;
};

#endif //PALABORATORIO2026_ADMINCONTROLLER_H
