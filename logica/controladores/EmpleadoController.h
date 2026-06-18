#ifndef EMPLEADOCONTROLLER_H
#define EMPLEADOCONTROLLER_H
#include "../dominio/Cliente.h"
#include "../dominio/OrdenDeCompra.h"
#include "../dominio/Venta.h"
#include "../dominio/LineaDetalleVenta.h"
#include "../dominio/DTFecha.h"
#include "../dominio/DTHora.h"
#include "AdminController.h"
#pragma once

using namespace std;
class EmpleadoController{
private:
    static EmpleadoController* instanciaEmpleado;
    EmpleadoController();
    vector<Cliente*> clientes;
    vector<OrdenDeCompra*> ordenesDeCompra;
    vector<Venta*> ventas;
    AdminController* adminCtrl;
public:
    Cliente* registrarCliente(const string& rut, const string& nombre, const string& apellido,
                              const string& direccion, const string& correo, const string& contrasena = "");
    Cliente* buscarCliente(const string& rut) const;
    vector<Cliente*> listarClientes() const;
    void agregarOrdenDeCompra(OrdenDeCompra* orden);
    void eliminarOrdenDeCompra(OrdenDeCompra* orden);
    vector<OrdenDeCompra*> listarOrdenesDeCompra() const;
    EmpleadoController(AdminController* adminController);

    static EmpleadoController* getInstanciaEmpleado();
    ~EmpleadoController();

    bool productoEstaEnOrdenesPendientes(int codigoProducto) const;

    // Ventas
    Venta* crearVenta(const string& rutCliente, DTFecha fecha, DTHora hora);
    bool agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad);
    vector<Venta*> listarVentas() const;
    vector<Venta*> listarVentasPorCliente(const string& rut) const;
    bool productoEstaEnVentas(int codigoProducto) const;
};
#endif
