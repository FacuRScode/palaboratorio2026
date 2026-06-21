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

struct ResultadoFacturacion {
    bool clienteExiste;
    float montoTotal;
    int cantidadVentas;
    float montoPromedio;
};

struct ResultadoUnidadesVendidas {
    int totalUnidadesVendidas;
    int cantidadVentas;
    float precioUnitarioPromedio;
};

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
    Cliente* buscarClientePorCorreo(const string& correo);
    bool modificaCliente(Cliente* cliente, const string &nombre, const string &apellido,const string &direccion, const string &correo);
    vector<Cliente*> listarClientes() const;

    OrdenDeCompra* crearOrdenDeCompra(DTFecha fechaEmision, Estado estado, DTFecha fechaRecepcion, Proveedor* proveedor);
    void agregarOrdenDeCompra(OrdenDeCompra* orden);
    void eliminarOrdenDeCompra(OrdenDeCompra* orden);
    bool agregarLineaDetalleCompra(OrdenDeCompra* orden, int cantidad, Producto* producto);
    vector<OrdenDeCompra*> listarOrdenesDeCompra() const;
    vector<OrdenDeCompra*> listarOrdenesPendientes() const;
    bool cancelarOrdenDeCompra(OrdenDeCompra* orden);
    bool registrarRecepcionOrden(OrdenDeCompra* orden, const vector<int>& cantidadesRecibidas, DTFecha fechaRecepcion);
    
    EmpleadoController(AdminController* adminController);

    static EmpleadoController* getInstanciaEmpleado();
    ~EmpleadoController();

    bool productoEstaEnOrdenesPendientes(int codigoProducto) const;

    // Ventas
    Venta* crearVenta(const string& rutCliente, DTFecha fecha, DTHora hora);
    Venta* crearVentaOcasional(DTFecha fecha, DTHora hora);
    void confirmarVenta(Venta* venta);
    bool agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad);
    vector<Venta*> listarVentas() const;
    vector<Venta*> listarVentasPorCliente(const string& rut) const;
    bool productoEstaEnVentas(int codigoProducto) const;

    // Facturacion
    ResultadoFacturacion consultarMontoFacturado(const string& rutCliente, DTFecha inicio, DTFecha fin) const;

    // Unidades vendidas
    ResultadoUnidadesVendidas consultarUnidadesVendidas(int codigoProducto, DTFecha inicio, DTFecha fin) const;
};
#endif
