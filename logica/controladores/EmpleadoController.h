#ifndef EMPLEADOCONTROLLER_H
#define EMPLEADOCONTROLLER_H
#include "../dominio/Cliente.h"
#include "../dominio/OrdenDeCompra.h"
#include "../dominio/Venta.h"
#include "../dominio/LineaDetalleVenta.h"
#include "../dominio/DTFecha.h"
#include "../dominio/DTHora.h"
#include "AdminController.h"
#include <utility>
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

struct ResultadoFacturacionConsulta {
    bool exito;
    bool rangoFechasValido;
    ResultadoFacturacion datos;
};

struct ResultadoUnidadesVendidasConsulta {
    bool exito;
    bool productoExiste;
    bool rangoFechasValido;
    ResultadoUnidadesVendidas datos;
};

struct LineaVentaResumen {
    int codigoProducto;
    string nombreProducto;
    int cantidad;
    float precioUnitario;
    float subtotal;
};

struct ResultadoRegistroVenta {
    bool exito;
    bool clienteValido;
    bool lineasValidas;
    bool stockSuficiente;
    bool productoValido;
    bool clienteRegistrado;
    string rutCliente;
    string nombreCliente;
    DTFecha fecha;
    DTHora hora;
    float total;
    vector<LineaVentaResumen> lineas;
};

struct LineaHistorialVentaInfo {
    int codigoProducto;
    string nombreProducto;
    int cantidad;
    float precioUnitario;
    float subtotal;
};

struct HistorialVentaInfo {
    DTFecha fecha;
    DTHora hora;
    float total;
    vector<LineaHistorialVentaInfo> lineas;
};

struct ResultadoHistorialCliente {
    bool clienteExiste;
    string rutCliente;
    string nombreCliente;
    vector<HistorialVentaInfo> ventas;
};

struct ProductoProveedorInfo {
    int codigoProducto;
    string nombreProducto;
    int precioCompraPactado;
    int tiempoEntregaDias;
};

struct ResultadoProductosProveedor {
    bool proveedorExiste;
    string rutProveedor;
    string empresaProveedor;
    vector<ProductoProveedorInfo> productos;
};

struct ResultadoOrdenCompra {
    bool exito;
    bool proveedorExiste;
    bool lineasValidas;
    bool productoValido;
    bool productoAsociadoProveedor;
    string rutProveedor;
    string empresaProveedor;
    DTFecha fechaEmision;
    int cantidadLineas;
    float totalEstimado;
};

struct LineaOrdenInfo {
    string nombreProducto;
    int cantidad;
};

struct OrdenPendienteInfo {
    int id;
    string empresaProveedor;
    DTFecha fechaEmision;
    int cantidadLineas;
    float totalEstimado;
};

struct DetalleOrdenPendiente {
    bool exito;
    bool idValido;
    OrdenPendienteInfo resumen;
    vector<LineaOrdenInfo> lineas;
};

struct ResultadoCancelarOrden {
    bool exito;
    bool idValido;
};

struct ResultadoRegistrarRecepcionOrden {
    bool exito;
    bool idValido;
    bool cantidadesValidas;
};

struct ProveedorInfoVista {
    string rut;
    string empresa;
};

struct CategoriaEmpleadoInfo {
    string nombre;
    string descripcion;
};

struct ProductoEmpleadoInfo {
    int codigo;
    string nombre;
};

struct StockProductoEmpleadoInfo {
    int codigo;
    string nombre;
    string categoria;
    int stockActual;
    int stockMinimo;
};

struct DetalleProductoEmpleadoVista {
    bool productoExiste;
    int codigo;
    string nombre;
    string descripcion;
    string categoria;
    float precioVentaUnitario;
    int stockActual;
    int stockMinimo;
    float puntajePromedio;
    int cantidadCalificaciones;
    vector<ProveedorProductoInfo> proveedores;
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
    ResultadoFacturacionConsulta consultarMontoFacturadoValidado(const string& rutCliente, DTFecha inicio, DTFecha fin) const;

    // Unidades vendidas
    ResultadoUnidadesVendidas consultarUnidadesVendidas(int codigoProducto, DTFecha inicio, DTFecha fin) const;
    ResultadoUnidadesVendidasConsulta consultarUnidadesVendidasValidado(int codigoProducto, DTFecha inicio, DTFecha fin) const;

    // Casos de uso de vista Empleado
    ResultadoRegistroVenta registrarVentaCompleta(const string& rutCliente, bool clienteRegistrado, const vector<pair<int, int>>& lineasSolicitadas);
    ResultadoHistorialCliente obtenerHistorialComprasCliente(const string& rutCliente) const;
    ResultadoProductosProveedor obtenerProductosProveedor(const string& rutProveedor) const;
    ResultadoOrdenCompra emitirOrdenCompra(const string& rutProveedor, const vector<pair<int, int>>& lineasSolicitadas);
    vector<OrdenPendienteInfo> listarOrdenesPendientesInfo() const;
    DetalleOrdenPendiente obtenerDetalleOrdenPendiente(int idOrdenPendiente) const;
    ResultadoCancelarOrden cancelarOrdenPendiente(int idOrdenPendiente);
    ResultadoRegistrarRecepcionOrden registrarRecepcionOrdenPendiente(int idOrdenPendiente, const vector<int>& cantidadesRecibidas);
    vector<Producto*> listarStockProductos(const string& categoria = "") const;
    vector<Producto*> listarProductosConStockBajo(bool ordenarPorCriticidad) const;
    DetalleProductoAdmin obtenerDetalleProductoEmpleado(int codigoProducto) const;
    vector<ProveedorInfoVista> listarProveedoresVista() const;
    vector<CategoriaEmpleadoInfo> listarCategoriasEmpleado() const;
    vector<ProductoEmpleadoInfo> listarProductosEmpleado() const;
    vector<StockProductoEmpleadoInfo> listarStockProductosVista(const string& categoria = "") const;
    vector<StockProductoEmpleadoInfo> listarProductosStockBajoVista(bool ordenarPorCriticidad) const;
    DetalleProductoEmpleadoVista obtenerDetalleProductoEmpleadoVista(int codigoProducto) const;
    string obtenerNombreClientePorRut(const string& rutCliente) const;
    string obtenerNombreProductoPorCodigo(int codigoProducto) const;
};
#endif
