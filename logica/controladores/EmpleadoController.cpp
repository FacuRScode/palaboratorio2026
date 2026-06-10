#include "EmpleadoController.h"
#include <algorithm>

using namespace std;

EmpleadoController::EmpleadoController(AdminController* adminController) : adminCtrl(adminController) {}

Cliente* EmpleadoController::registrarCliente(const string& rut, const string& nombre, const string& apellido,
                                            const string& direccion, const string& correo) {
    if (buscarCliente(rut) != nullptr) return nullptr; // ya existe
    Cliente* c = new Cliente(rut, nombre, apellido, direccion, correo);
    clientes.push_back(c);
    return c;
}

Cliente* EmpleadoController::buscarCliente(const string& rut) const {
    for (Cliente* c : clientes) {
        if (c != nullptr && c->getRut() == rut) return c;
    }
    return nullptr;
}

vector<Cliente*> EmpleadoController::listarClientes() const {
    return clientes;
}

void EmpleadoController::agregarOrdenDeCompra(OrdenDeCompra* orden){
    ordenesDeCompra.push_back(orden);
}

void EmpleadoController::eliminarOrdenDeCompra(OrdenDeCompra* orden){
    for (auto it = ordenesDeCompra.begin(); it != ordenesDeCompra.end(); ++it) {
        if (*it == orden) {
            ordenesDeCompra.erase(it);
            break;
        }
    }
}

vector<OrdenDeCompra*> EmpleadoController::listarOrdenesDeCompra() const {
    return ordenesDeCompra;
}
