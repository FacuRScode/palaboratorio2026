#ifndef EMPLEADOCONTROLLER_H
#define EMPLEADOCONTROLLER_H
#include "Cliente.h"
#include "OrdenDeCompra.h"
#include "AdminController.h"

using namespace std;
class EmpleadoController{
private:
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
};
#endif