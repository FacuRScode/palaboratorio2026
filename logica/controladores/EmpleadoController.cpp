#include "EmpleadoController.h"

using namespace std;
void EmpleadoController::agregarCliente(Cliente* cliente){
    clientes.push_back(cliente);
}
void EmpleadoController::eliminarCliente(Cliente* cliente){
    for (auto it = clientes.begin(); it != clientes.end(); ++it) {
        if (*it == cliente) {
            clientes.erase(it);
            break;
        }
    }
}
Cliente* EmpleadoController::buscarCliente(string rut){
    for (int i = 0; i < clientes.size(); i++) {
        if (clientes[i]->getRut() == rut) { // cambie id por rut
            return clientes[i];
        }
    }
    return nullptr;
}
vector<Cliente*> EmpleadoController::listarClientes(){
    return this->clientes;
}
void EmpleadoController::agregarOrdenDeCompra(OrdenDeCompra* orden){
    ordenesDeCompra.push_back(orden);
}
void EmpleadoController::eliminarOrdenDeCompra(OrdenDeCompra* orden){
    for (int i = 0; i < ordenesDeCompra.size(); i++) {
        if (ordenesDeCompra[i] == orden) {
            ordenesDeCompra.erase(ordenesDeCompra.begin() + i);
            break;
        }
    }
}
EmpleadoController::EmpleadoController(AdminController* adminController) : adminCtrl(adminController) {}

EmpleadoController* EmpleadoController::getInstanciaEmpleado(){
    if(instanciaEmpleado != nullptr){
        return instanciaEmpleado;
    } else {
        instanciaEmpleado = new EmpleadoController();
        return instanciaEmpleado;
    }
}
EmpleadoController::~EmpleadoController() {
    for (Cliente* c : clientes) delete c;
    for (OrdenDeCompra* o : ordenesDeCompra) delete o;
}