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
    for (Venta* v : ventas) delete v;
}

// -- Ventas -------------------------------------------------------

Venta* EmpleadoController::crearVenta(const string& rutCliente, DTFecha fecha, DTHora hora) {
    if (adminCtrl == nullptr) return nullptr;
    Cliente* c = buscarCliente(rutCliente);
    if (c == nullptr) return nullptr;
    Venta* v = new Venta(fecha, hora, c);
    ventas.push_back(v);
    return v;
}

bool EmpleadoController::agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad) {
    if (venta == nullptr) return false;
    if (adminCtrl == nullptr) return false;
    Producto* p = adminCtrl->buscarProducto(codigoProducto);
    if (p == nullptr) return false;
    if (p->getStock() < cantidad) return false;
    LineaDetalleVenta* linea = new LineaDetalleVenta(cantidad, p->getPrecioVentaActual(), p);
    venta->addLinea(linea);
    p->setStock(p->getStock() - cantidad);
    return true;
}

vector<Venta*> EmpleadoController::listarVentas() const {
    return ventas;
}

vector<Venta*> EmpleadoController::listarVentasPorCliente(const string& rut) const {
    vector<Venta*> ventasCliente;
    for (Venta* v : ventas) {
        if (v->getCliente() != nullptr && v->getCliente()->getRut() == rut) {
            ventasCliente.push_back(v);
        }
    }
    return ventasCliente;
}

bool EmpleadoController::productoEstaEnVentas(int codigoProducto) const {
    for (Venta* v : ventas) {
        if (v == nullptr) continue;
        for (LineaDetalleVenta* linea : v->getDetalle()) {
            if (linea != nullptr && linea->getProducto() != nullptr && linea->getProducto()->getCodigo() == codigoProducto) {
                return true;
            }
        }
    }
    return false;
}
