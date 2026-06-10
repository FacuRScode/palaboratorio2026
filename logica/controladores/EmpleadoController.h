#ifndef EMPLEADOCONTROLLER_H
#define EMPLEADOCONTROLLER_H
#include "../dominio/Cliente.h"
#include "../dominio/OrdenDeCompra.h"
#include "AdminController.h"

using namespace std;
class EmpleadoController{
private:
    static EmpleadoController* instanciaEmpleado;
    EmpleadoController();
    vector<Cliente*> clientes;
    vector<OrdenDeCompra*> ordenesDeCompra;
    AdminController* adminCtrl;
public:
    void agregarCliente(Cliente* cliente);
    void eliminarCliente(Cliente* cliente);
    Cliente* buscarCliente(string nombre);
    vector<Cliente*> listarClientes();
    void agregarOrdenDeCompra(OrdenDeCompra* orden);
    void eliminarOrdenDeCompra(OrdenDeCompra* orden);
    EmpleadoController(AdminController* adminController);

    static EmpleadoController* getInstanciaEmpleado();
    ~EmpleadoController();
};
#endif