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
    if (buscarCliente(rut) != nullptr) return nullptr; // RUT ya existe
    if (buscarClientePorCorreo(correo) != nullptr) return nullptr; // correo ya existe
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

Cliente* EmpleadoController::buscarClientePorCorreo(const string& correo) {
    for (auto c : clientes) {
        if (c != nullptr && c->getCorreo() == correo) {
            return c;
        }
    }
    return nullptr;
}

bool EmpleadoController::modificaCliente(Cliente* cliente, const string &nombre, const string &apellido,const string &direccion, const string &correo){
    if (cliente == nullptr) return false;
    // Si el correo cambia, verificar que no pertenezca a otro cliente
    if (correo != cliente->getCorreo()) {
        Cliente* otro = buscarClientePorCorreo(correo);
        if (otro != nullptr && otro != cliente) return false;
    }
    cliente->setNombre(nombre);
    cliente->setApellido(apellido);
    cliente->setDireccion(direccion);
    cliente->setCorreo(correo);
    return true;
}

vector<Cliente*> EmpleadoController::listarClientes() const {
    return clientes;
}

OrdenDeCompra* EmpleadoController::crearOrdenDeCompra(DTFecha fechaEmision, Estado estado, DTFecha fechaRecepcion, Proveedor* proveedor){
    OrdenDeCompra* o = new OrdenDeCompra(fechaEmision, estado, fechaRecepcion, proveedor);
    return o;
}

void EmpleadoController::agregarOrdenDeCompra(OrdenDeCompra* orden){
    if (orden != nullptr) {
        ordenesDeCompra.push_back(orden);
    }
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

bool EmpleadoController::agregarLineaDetalleCompra(OrdenDeCompra* orden, int cantidad, Producto* producto){
    if (adminCtrl == nullptr) return false;
    LineaDetalleCompra* linea= new LineaDetalleCompra(cantidad, producto);
    orden->addLineaDetalleCompra(linea);
    return true;
}

vector<OrdenDeCompra*> EmpleadoController::listarOrdenesPendientes() const {
    vector<OrdenDeCompra*> pendientes;
    for (OrdenDeCompra* o : ordenesDeCompra) {
        if (o != nullptr && o->getEstado() == Estado::Pendiente) {
            pendientes.push_back(o);
        }
    }
    return pendientes;
}

bool EmpleadoController::cancelarOrdenDeCompra(OrdenDeCompra* orden) {
    if (orden == nullptr || orden->getEstado() != Estado::Pendiente) return false;
    orden->setEstado(Estado::Cancelada);
    return true;
}

bool EmpleadoController::registrarRecepcionOrden(OrdenDeCompra* orden, const vector<int>& cantidadesRecibidas, DTFecha fechaRecepcion) {
    if (orden == nullptr || orden->getEstado() != Estado::Pendiente) return false;

    auto& detalle = orden->getDetalleCompra();
    if (cantidadesRecibidas.size() != detalle.size()) return false;

    orden->setEstado(Estado::Recibida);
    orden->setFechaRecepcion(fechaRecepcion);

    // Incrementar stock según cantidades recibidas
    for (size_t i = 0; i < detalle.size(); ++i) {
        if (detalle[i] != nullptr && detalle[i]->getProducto() != nullptr) {
            detalle[i]->getProducto()->setStock(
                detalle[i]->getProducto()->getStock() + cantidadesRecibidas[i]
            );
        }
    }
    return true;
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
    return v;
}

Venta* EmpleadoController::crearVentaOcasional(DTFecha fecha, DTHora hora) {
    Venta* v = new Venta(fecha, hora, nullptr);
    return v;
}

void EmpleadoController::confirmarVenta(Venta* venta) {
    if (venta == nullptr) return;
    // Descontar stock de cada producto
    for (auto linea : venta->getDetalle()) {
        Producto* p = linea->getProducto();
        p->setStock(p->getStock() - linea->getCantidad());
    }
    ventas.push_back(venta);
}

bool EmpleadoController::agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad) {
    if (venta == nullptr) return false;
    if (adminCtrl == nullptr) return false;
    Producto* p = adminCtrl->buscarProducto(codigoProducto);
    if (p == nullptr) return false;
    if (p->getStock() < cantidad) return false;
    LineaDetalleVenta* linea = new LineaDetalleVenta(cantidad, p->getPrecioVentaActual(), p);
    venta->addLinea(linea);
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

ResultadoFacturacion EmpleadoController::consultarMontoFacturado(const string& rutCliente, DTFecha inicio, DTFecha fin) const {
    ResultadoFacturacion res;
    Cliente* cliente = buscarCliente(rutCliente);
    if (cliente == nullptr) {
        res.clienteExiste = false;
        res.montoTotal = 0;
        res.cantidadVentas = 0;
        res.montoPromedio = 0;
        return res;
    }

    res.clienteExiste = true;
    res.montoTotal = 0;
    res.cantidadVentas = 0;

    vector<Venta*> ventasCliente = listarVentasPorCliente(rutCliente);
    for (Venta* v : ventasCliente) {
        if (v == nullptr) continue;
        DTFecha fechaVenta = v->getFecha();
        if (fechaVenta >= inicio && fechaVenta <= fin) {
            res.montoTotal += v->calcularTotal();
            res.cantidadVentas++;
        }
    }

    res.montoPromedio = (res.cantidadVentas > 0) ? (res.montoTotal / res.cantidadVentas) : 0;
    return res;
}

ResultadoUnidadesVendidas EmpleadoController::consultarUnidadesVendidas(int codigoProducto, DTFecha inicio, DTFecha fin) const {
    ResultadoUnidadesVendidas res;
    res.totalUnidadesVendidas = 0;
    res.cantidadVentas = 0;
    res.precioUnitarioPromedio = 0;

    float sumaPrecios = 0;

    for (Venta* v : ventas) {
        if (v == nullptr) continue;
        DTFecha fechaVenta = v->getFecha();
        if (!(fechaVenta >= inicio && fechaVenta <= fin)) continue;

        bool productoEnVenta = false;
        for (LineaDetalleVenta* linea : v->getDetalle()) {
            if (linea == nullptr || linea->getProducto() == nullptr) continue;
            if (linea->getProducto()->getCodigo() == codigoProducto) {
                res.totalUnidadesVendidas += linea->getCantidad();
                sumaPrecios += linea->getPrecioUnitario() * linea->getCantidad();
                productoEnVenta = true;
            }
        }
        if (productoEnVenta) {
            res.cantidadVentas++;
        }
    }

    res.precioUnitarioPromedio = (res.totalUnidadesVendidas > 0) ? (sumaPrecios / res.totalUnidadesVendidas) : 0;
    return res;
}
