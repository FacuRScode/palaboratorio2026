#ifndef EMPLEADOCONTROLLER_H
#define EMPLEADOCONTROLLER_H
#include "../dominio/Cliente.h"
#include "../dominio/OrdenDeCompra.h"
#include "AdminController.h"
#pragma once

using namespace std;
class EmpleadoController{
private:
    static EmpleadoController* instanciaEmpleado;
    EmpleadoController();
    vector<Cliente*> clientes;
    vector<OrdenDeCompra*> ordenesDeCompra;
    AdminController* adminCtrl;
public:
    Cliente* registrarCliente(const string& rut, const string& nombre, const string& apellido,
                              const string& direccion, const string& correo);
    Cliente* buscarCliente(const string& rut) const;
    vector<Cliente*> listarClientes() const;
    void agregarOrdenDeCompra(OrdenDeCompra* orden);
    void eliminarOrdenDeCompra(OrdenDeCompra* orden);
    vector<OrdenDeCompra*> listarOrdenesDeCompra() const;
    EmpleadoController(AdminController* adminController);

    static EmpleadoController* getInstanciaEmpleado();
    ~EmpleadoController();
};
#endif