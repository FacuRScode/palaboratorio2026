#include "EmpleadoController.h"
#include <algorithm>

using namespace std;

// Inicializar el miembro estático
EmpleadoController* EmpleadoController::instanciaEmpleado = nullptr;

EmpleadoController::EmpleadoController(AdminController* adminController) : adminCtrl(adminController) {}

// Constructor privado usado por el singleton
EmpleadoController::EmpleadoController() : adminCtrl(nullptr) {}

Cliente* EmpleadoController::registrarCliente(const string& rut, const string& nombre, const string& apellido,
                                            const string& direccion, const string& correo, const string& contrasena) {
    if (buscarCliente(rut) != nullptr) return nullptr; // ya existe
    Cliente* c = new Cliente(rut, nombre, apellido, direccion, correo, contrasena);
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

bool EmpleadoController::productoEstaEnOrdenesPendientes(int codigoProducto) const {
	for (OrdenDeCompra* orden : ordenesDeCompra) {
		if (orden == nullptr) continue;
		if (orden->getEstado() != Estado::Pendiente) continue;
		for (LineaDetalleCompra* linea : orden->getDetalleCompra()) {
			if (linea != nullptr && linea->getProducto() != nullptr && linea->getProducto()->getCodigo() == codigoProducto) {
				return true;
			}
		}
	}
	return false;
}

EmpleadoController* EmpleadoController::getInstanciaEmpleado(){
    if (instanciaEmpleado == nullptr) {
        instanciaEmpleado = new EmpleadoController();
    }
    return instanciaEmpleado;
}

EmpleadoController::~EmpleadoController() {
    for (Cliente* c : clientes) delete c;
    for (OrdenDeCompra* o : ordenesDeCompra) delete o;
}
