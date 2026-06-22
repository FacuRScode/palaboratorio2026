#include "EmpleadoController.h"
#include <algorithm>
#include <ctime>
#include <stdexcept>
#include <unordered_map>

using namespace std;

static DTFecha obtenerFechaActual() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return DTFecha(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

static DTHora obtenerHoraActual() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return DTHora(now->tm_hour, now->tm_min, now->tm_sec);
}

// Inicializar el miembro estático
EmpleadoController* EmpleadoController::instanciaEmpleado = nullptr;

EmpleadoController::EmpleadoController(AdminController* adminController) : adminCtrl(adminController) {}

// Constructor privado usado por el singleton
EmpleadoController::EmpleadoController() : adminCtrl(nullptr) {}

Cliente* EmpleadoController::registrarCliente(const string& rut, const string& nombre, const string& apellido,
                                            const string& direccion, const string& correo, const string& contrasena) {
    if (buscarCliente(rut) != nullptr) {
        throw invalid_argument("RUT invalido: ya existe un cliente registrado con ese RUT.");
    }
    if (buscarClientePorCorreo(correo) != nullptr) {
        throw invalid_argument("Correo invalido: ya existe un cliente registrado con ese correo.");
    }
    if (adminCtrl != nullptr && adminCtrl->buscarEmpleado(correo) != nullptr) {
        throw invalid_argument("Correo invalido: ya existe un empleado registrado con ese correo.");
    }
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
    if (cliente == nullptr) throw invalid_argument("Cliente invalido: no existe.");
    // Si el correo cambia, verificar que no pertenezca a otro cliente
    if (correo != cliente->getCorreo()) {
        Cliente* otro = buscarClientePorCorreo(correo);
        if (otro != nullptr && otro != cliente) {
            throw invalid_argument("Correo invalido: ya pertenece a otro cliente.");
        }
        if (adminCtrl != nullptr && adminCtrl->buscarEmpleado(correo) != nullptr) {
            throw invalid_argument("Correo invalido: ya pertenece a un empleado.");
        }
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
    if (orden == nullptr) throw invalid_argument("Orden invalida: no se puede agregar linea de detalle.");
    if (producto == nullptr) throw invalid_argument("Producto invalido: no se puede agregar linea de detalle.");
    if (cantidad <= 0) throw invalid_argument("Cantidad invalida: debe ser mayor a cero.");
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
    if (orden == nullptr) throw invalid_argument("Orden invalida: no existe.");
    if (orden->getEstado() != Estado::Pendiente) {
        throw logic_error("Operacion invalida: solo se pueden cancelar ordenes en estado Pendiente.");
    }
    orden->setEstado(Estado::Cancelada);
    return true;
}

bool EmpleadoController::registrarRecepcionOrden(OrdenDeCompra* orden, const vector<int>& cantidadesRecibidas, DTFecha fechaRecepcion) {
    if (orden == nullptr) throw invalid_argument("Orden invalida: no existe.");
    if (orden->getEstado() != Estado::Pendiente) {
        throw logic_error("Operacion invalida: solo se puede recibir una orden en estado Pendiente.");
    }

    auto& detalle = orden->getDetalleCompra();
    if (cantidadesRecibidas.size() != detalle.size()) {
        throw invalid_argument("Cantidades invalidas: la cantidad de registros recibidos no coincide con las lineas de la orden.");
    }
    for (int cantidad : cantidadesRecibidas) {
        if (cantidad < 0) {
            throw invalid_argument("Cantidades invalidas: no se aceptan cantidades negativas en la recepcion.");
        }
    }

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
    if (venta == nullptr) throw invalid_argument("Venta invalida: no existe.");
    if (venta->getDetalle().empty()) {
        throw logic_error("Operacion invalida: una venta debe tener al menos una linea de detalle.");
    }
    venta->setFecha(obtenerFechaActual());
    venta->setHora(obtenerHoraActual());

    unordered_map<Producto*, int> cantidadPorProducto;
    for (LineaDetalleVenta* linea : venta->getDetalle()) {
        if (linea == nullptr || linea->getProducto() == nullptr) {
            throw invalid_argument("Linea invalida: faltan datos de producto en la venta.");
        }
        if (linea->getCantidad() <= 0) {
            throw invalid_argument("Cantidad invalida: cada linea de venta debe tener cantidad mayor a cero.");
        }
        cantidadPorProducto[linea->getProducto()] += linea->getCantidad();
    }

    for (const auto& entrada : cantidadPorProducto) {
        Producto* producto = entrada.first;
        int cantidad = entrada.second;
        if (producto->getStock() < cantidad) {
            throw logic_error("Stock insuficiente: la venta intentaria dejar stock negativo.");
        }
    }

    for (const auto& entrada : cantidadPorProducto) {
        Producto* producto = entrada.first;
        int cantidad = entrada.second;
        producto->setStock(producto->getStock() - cantidad);
    }
    ventas.push_back(venta);
}

bool EmpleadoController::agregarLineaAVenta(Venta* venta, int codigoProducto, int cantidad) {
    if (venta == nullptr) throw invalid_argument("Venta invalida: no existe.");
    if (adminCtrl == nullptr) throw logic_error("No hay controlador de administracion disponible.");
    if (cantidad <= 0) throw invalid_argument("Cantidad invalida: debe ser mayor a cero.");
    Producto* p = adminCtrl->buscarProducto(codigoProducto);
    if (p == nullptr) throw invalid_argument("Producto invalido: no existe un producto con el codigo indicado.");
    if (p->getStock() < cantidad) throw logic_error("Stock insuficiente para agregar la linea de venta.");
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

ResultadoFacturacionConsulta EmpleadoController::consultarMontoFacturadoValidado(const string& rutCliente, DTFecha inicio, DTFecha fin) const {
    ResultadoFacturacionConsulta consulta;
    consulta.exito = false;
    consulta.rangoFechasValido = (inicio <= fin);
    consulta.datos = consultarMontoFacturado(rutCliente, inicio, fin);

    if (!consulta.rangoFechasValido) return consulta;
    if (!consulta.datos.clienteExiste) return consulta;

    consulta.exito = true;
    return consulta;
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

ResultadoUnidadesVendidasConsulta EmpleadoController::consultarUnidadesVendidasValidado(int codigoProducto, DTFecha inicio, DTFecha fin) const {
    ResultadoUnidadesVendidasConsulta consulta;
    consulta.exito = false;
    consulta.rangoFechasValido = (inicio <= fin);
    consulta.productoExiste = (adminCtrl != nullptr && adminCtrl->buscarProducto(codigoProducto) != nullptr);
    consulta.datos = consultarUnidadesVendidas(codigoProducto, inicio, fin);

    if (!consulta.rangoFechasValido) return consulta;
    if (!consulta.productoExiste) return consulta;

    consulta.exito = true;
    return consulta;
}

ResultadoRegistroVenta EmpleadoController::registrarVentaCompleta(const string& rutCliente, bool clienteRegistrado, const vector<pair<int, int>>& lineasSolicitadas) {
    ResultadoRegistroVenta res{
        false,
        !clienteRegistrado,
        !lineasSolicitadas.empty(),
        true,
        true,
        clienteRegistrado,
        rutCliente,
        clienteRegistrado ? "" : "Ocasional",
        DTFecha(1, 1, 1900),
        DTHora(0, 0, 0),
        0,
        {}
    };

    if (!res.lineasValidas) {
        throw invalid_argument("Venta invalida: debe contener al menos una linea de detalle.");
    }

    Cliente* cliente = nullptr;
    if (clienteRegistrado) {
        cliente = buscarCliente(rutCliente);
        res.clienteValido = (cliente != nullptr);
        if (!res.clienteValido) {
            throw invalid_argument("Cliente invalido: no existe un cliente registrado con el RUT indicado.");
        }
        res.nombreCliente = cliente->getNombre() + " " + cliente->getApellido();
    }

    Venta* venta = clienteRegistrado ? crearVenta(rutCliente, obtenerFechaActual(), obtenerHoraActual())
                                     : crearVentaOcasional(obtenerFechaActual(), obtenerHoraActual());
    if (venta == nullptr) throw logic_error("No se pudo iniciar el registro de la venta.");

    unordered_map<int, int> cantidadSolicitadaPorCodigo;
    for (const auto& lineaSolicitada : lineasSolicitadas) {
        int codigoProducto = lineaSolicitada.first;
        int cantidad = lineaSolicitada.second;
        if (cantidad <= 0) {
            res.lineasValidas = false;
            delete venta;
            throw invalid_argument("Cantidad invalida: cada linea de venta debe tener cantidad mayor a cero.");
        }
        Producto* producto = (adminCtrl != nullptr) ? adminCtrl->buscarProducto(codigoProducto) : nullptr;
        if (producto == nullptr) {
            res.productoValido = false;
            delete venta;
            throw invalid_argument("Producto invalido: uno o mas codigos no existen.");
        }
        cantidadSolicitadaPorCodigo[codigoProducto] += cantidad;
    }

    for (const auto& entrada : cantidadSolicitadaPorCodigo) {
        Producto* producto = (adminCtrl != nullptr) ? adminCtrl->buscarProducto(entrada.first) : nullptr;
        if (producto == nullptr) {
            res.productoValido = false;
            delete venta;
            throw invalid_argument("Producto invalido: no existe un producto indicado en la venta.");
        }
        if (producto->getStock() < entrada.second) {
            res.stockSuficiente = false;
            delete venta;
            throw logic_error("Stock insuficiente: la venta intentaria dejar stock negativo.");
        }
    }

    for (const auto& lineaSolicitada : lineasSolicitadas) {
        int codigoProducto = lineaSolicitada.first;
        int cantidad = lineaSolicitada.second;
        agregarLineaAVenta(venta, codigoProducto, cantidad);
    }

    confirmarVenta(venta);

    res.exito = true;
    res.fecha = venta->getFecha();
    res.hora = venta->getHora();
    res.total = venta->calcularTotal();
    for (LineaDetalleVenta* linea : venta->getDetalle()) {
        if (linea == nullptr || linea->getProducto() == nullptr) continue;
        LineaVentaResumen info;
        info.codigoProducto = linea->getProducto()->getCodigo();
        info.nombreProducto = linea->getProducto()->getNombre();
        info.cantidad = linea->getCantidad();
        info.precioUnitario = linea->getPrecioUnitario();
        info.subtotal = linea->getCantidad() * linea->getPrecioUnitario();
        res.lineas.push_back(info);
    }

    return res;
}

ResultadoHistorialCliente EmpleadoController::obtenerHistorialComprasCliente(const string& rutCliente) const {
    ResultadoHistorialCliente res;
    res.clienteExiste = false;
    res.rutCliente = rutCliente;
    res.nombreCliente = "";
    res.ventas.clear();

    Cliente* cliente = buscarCliente(rutCliente);
    if (cliente == nullptr) return res;

    res.clienteExiste = true;
    res.nombreCliente = cliente->getNombre() + " " + cliente->getApellido();

    vector<Venta*> ventasCliente = listarVentasPorCliente(rutCliente);
    sort(ventasCliente.begin(), ventasCliente.end(), [](Venta* a, Venta* b) {
        if (a == nullptr || b == nullptr) return a != nullptr;
        DTFecha fa = a->getFecha();
        DTFecha fb = b->getFecha();
        if (fa.getAnio() != fb.getAnio()) return fa.getAnio() > fb.getAnio();
        if (fa.getMes() != fb.getMes()) return fa.getMes() > fb.getMes();
        if (fa.getDia() != fb.getDia()) return fa.getDia() > fb.getDia();
        DTHora ha = a->getHora();
        DTHora hb = b->getHora();
        if (ha.getHora() != hb.getHora()) return ha.getHora() > hb.getHora();
        if (ha.getMinuto() != hb.getMinuto()) return ha.getMinuto() > hb.getMinuto();
        return ha.getSegundo() > hb.getSegundo();
    });

    for (Venta* venta : ventasCliente) {
        if (venta == nullptr) continue;
        HistorialVentaInfo infoVenta{
            venta->getFecha(),
            venta->getHora(),
            venta->calcularTotal(),
            {}
        };

        for (LineaDetalleVenta* linea : venta->getDetalle()) {
            if (linea == nullptr) continue;
            LineaHistorialVentaInfo infoLinea;
            infoLinea.codigoProducto = (linea->getProducto() != nullptr) ? linea->getProducto()->getCodigo() : -1;
            infoLinea.nombreProducto = (linea->getProducto() != nullptr) ? linea->getProducto()->getNombre() : "(producto eliminado)";
            infoLinea.cantidad = linea->getCantidad();
            infoLinea.precioUnitario = linea->getPrecioUnitario();
            infoLinea.subtotal = linea->getCantidad() * linea->getPrecioUnitario();
            infoVenta.lineas.push_back(infoLinea);
        }

        res.ventas.push_back(infoVenta);
    }

    return res;
}

ResultadoProductosProveedor EmpleadoController::obtenerProductosProveedor(const string& rutProveedor) const {
    ResultadoProductosProveedor res;
    res.proveedorExiste = false;
    res.rutProveedor = rutProveedor;
    res.empresaProveedor = "";
    res.productos.clear();

    if (adminCtrl == nullptr) return res;
    Proveedor* proveedor = adminCtrl->buscarProveedor(rutProveedor);
    if (proveedor == nullptr) return res;

    res.proveedorExiste = true;
    res.empresaProveedor = proveedor->getEmpresa();

    for (ProveedorProducto* pp : proveedor->getProductosOfrecidos()) {
        if (pp == nullptr || pp->getProducto() == nullptr) continue;
        ProductoProveedorInfo info;
        info.codigoProducto = pp->getProducto()->getCodigo();
        info.nombreProducto = pp->getProducto()->getNombre();
        info.precioCompraPactado = pp->getPrecioCompraPactado();
        info.tiempoEntregaDias = pp->getTiempoEntregaEstimadoEnDias();
        res.productos.push_back(info);
    }
    return res;
}

ResultadoOrdenCompra EmpleadoController::emitirOrdenCompra(const string& rutProveedor, const vector<pair<int, int>>& lineasSolicitadas) {
    ResultadoOrdenCompra res{
        false,
        false,
        !lineasSolicitadas.empty(),
        true,
        true,
        rutProveedor,
        "",
        DTFecha(1, 1, 1900),
        0,
        0
    };

    if (adminCtrl == nullptr || !res.lineasValidas) return res;
    Proveedor* proveedor = adminCtrl->buscarProveedor(rutProveedor);
    if (proveedor == nullptr) return res;
    res.proveedorExiste = true;
    res.empresaProveedor = proveedor->getEmpresa();

    DTFecha fechaActual = obtenerFechaActual();
    OrdenDeCompra* orden = crearOrdenDeCompra(fechaActual, Estado::Pendiente, fechaActual, proveedor);
    if (orden == nullptr) return res;

    for (const auto& lineaSolicitada : lineasSolicitadas) {
        int codigoProducto = lineaSolicitada.first;
        int cantidad = lineaSolicitada.second;
        if (cantidad <= 0) {
            res.lineasValidas = false;
            delete orden;
            return res;
        }

        Producto* producto = adminCtrl->buscarProducto(codigoProducto);
        if (producto == nullptr) {
            res.productoValido = false;
            delete orden;
            return res;
        }
        if (!adminCtrl->existeProveedorProducto(rutProveedor, codigoProducto)) {
            res.productoAsociadoProveedor = false;
            delete orden;
            return res;
        }
        if (!agregarLineaDetalleCompra(orden, cantidad, producto)) {
            res.lineasValidas = false;
            delete orden;
            return res;
        }
    }

    agregarOrdenDeCompra(orden);
    res.exito = true;
    res.fechaEmision = orden->getFechaEmision();
    res.cantidadLineas = static_cast<int>(orden->getDetalleCompra().size());
    res.totalEstimado = orden->calcularTotal();
    return res;
}

vector<OrdenPendienteInfo> EmpleadoController::listarOrdenesPendientesInfo() const {
    vector<OrdenPendienteInfo> resultado;
    vector<OrdenDeCompra*> pendientes = listarOrdenesPendientes();
    for (size_t i = 0; i < pendientes.size(); ++i) {
        OrdenDeCompra* orden = pendientes[i];
        if (orden == nullptr) continue;
        OrdenPendienteInfo info{
            static_cast<int>(i),
            (orden->getProveedor() != nullptr) ? orden->getProveedor()->getEmpresa() : "(sin proveedor)",
            orden->getFechaEmision(),
            static_cast<int>(orden->getDetalleCompra().size()),
            orden->calcularTotal()
        };
        resultado.push_back(info);
    }
    return resultado;
}

DetalleOrdenPendiente EmpleadoController::obtenerDetalleOrdenPendiente(int idOrdenPendiente) const {
    DetalleOrdenPendiente res{
        false,
        false,
        OrdenPendienteInfo{-1, "", DTFecha(1, 1, 1900), 0, 0},
        {}
    };

    vector<OrdenDeCompra*> pendientes = listarOrdenesPendientes();
    if (idOrdenPendiente < 0 || idOrdenPendiente >= static_cast<int>(pendientes.size())) return res;

    OrdenDeCompra* orden = pendientes[idOrdenPendiente];
    if (orden == nullptr) return res;

    res.idValido = true;
    res.exito = true;
    res.resumen.id = idOrdenPendiente;
    res.resumen.empresaProveedor = (orden->getProveedor() != nullptr) ? orden->getProveedor()->getEmpresa() : "(sin proveedor)";
    res.resumen.fechaEmision = orden->getFechaEmision();
    res.resumen.cantidadLineas = static_cast<int>(orden->getDetalleCompra().size());
    res.resumen.totalEstimado = orden->calcularTotal();

    for (LineaDetalleCompra* linea : orden->getDetalleCompra()) {
        if (linea == nullptr) continue;
        LineaOrdenInfo infoLinea;
        infoLinea.nombreProducto = (linea->getProducto() != nullptr) ? linea->getProducto()->getNombre() : "(producto eliminado)";
        infoLinea.cantidad = linea->getCantidad();
        res.lineas.push_back(infoLinea);
    }

    return res;
}

ResultadoCancelarOrden EmpleadoController::cancelarOrdenPendiente(int idOrdenPendiente) {
    ResultadoCancelarOrden res;
    res.exito = false;
    res.idValido = false;

    vector<OrdenDeCompra*> pendientes = listarOrdenesPendientes();
    if (idOrdenPendiente < 0 || idOrdenPendiente >= static_cast<int>(pendientes.size())) return res;

    OrdenDeCompra* orden = pendientes[idOrdenPendiente];
    if (orden == nullptr) return res;

    res.idValido = true;
    res.exito = cancelarOrdenDeCompra(orden);
    return res;
}

ResultadoRegistrarRecepcionOrden EmpleadoController::registrarRecepcionOrdenPendiente(int idOrdenPendiente, const vector<int>& cantidadesRecibidas) {
    ResultadoRegistrarRecepcionOrden res;
    res.exito = false;
    res.idValido = false;
    res.cantidadesValidas = true;

    vector<OrdenDeCompra*> pendientes = listarOrdenesPendientes();
    if (idOrdenPendiente < 0 || idOrdenPendiente >= static_cast<int>(pendientes.size())) return res;

    OrdenDeCompra* orden = pendientes[idOrdenPendiente];
    if (orden == nullptr) return res;
    res.idValido = true;

    if (cantidadesRecibidas.size() != orden->getDetalleCompra().size()) {
        res.cantidadesValidas = false;
        return res;
    }
    for (int cantidad : cantidadesRecibidas) {
        if (cantidad < 0) {
            res.cantidadesValidas = false;
            return res;
        }
    }

    res.exito = registrarRecepcionOrden(orden, cantidadesRecibidas, obtenerFechaActual());
    return res;
}

vector<Producto*> EmpleadoController::listarStockProductos(const string& categoria) const {
    if (adminCtrl == nullptr) return {};
    if (categoria.empty()) return adminCtrl->listarProductos();
    return adminCtrl->listarProductosPorCategoria(categoria);
}

vector<Producto*> EmpleadoController::listarProductosConStockBajo(bool ordenarPorCriticidad) const {
    if (adminCtrl == nullptr) return {};
    return adminCtrl->listarProductosConStockBajo(ordenarPorCriticidad);
}

DetalleProductoAdmin EmpleadoController::obtenerDetalleProductoEmpleado(int codigoProducto) const {
    if (adminCtrl == nullptr) {
        DetalleProductoAdmin vacio;
        vacio.productoExiste = false;
        vacio.producto = nullptr;
        vacio.proveedores.clear();
        return vacio;
    }
    return adminCtrl->obtenerDetalleProductoAdmin(codigoProducto);
}

vector<CategoriaEmpleadoInfo> EmpleadoController::listarCategoriasEmpleado() const {
    vector<CategoriaEmpleadoInfo> resultado;
    if (adminCtrl == nullptr) return resultado;
    for (Categoria* categoria : adminCtrl->listarCategorias()) {
        if (categoria == nullptr) continue;
        CategoriaEmpleadoInfo info;
        info.nombre = categoria->getNombre();
        info.descripcion = categoria->getDescripcion();
        resultado.push_back(info);
    }
    return resultado;
}

vector<ProductoEmpleadoInfo> EmpleadoController::listarProductosEmpleado() const {
    vector<ProductoEmpleadoInfo> resultado;
    if (adminCtrl == nullptr) return resultado;
    for (Producto* producto : adminCtrl->listarProductos()) {
        if (producto == nullptr) continue;
        ProductoEmpleadoInfo info;
        info.codigo = producto->getCodigo();
        info.nombre = producto->getNombre();
        resultado.push_back(info);
    }
    return resultado;
}

vector<StockProductoEmpleadoInfo> EmpleadoController::listarStockProductosVista(const string& categoria) const {
    vector<StockProductoEmpleadoInfo> resultado;
    vector<Producto*> productos = listarStockProductos(categoria);
    for (Producto* producto : productos) {
        if (producto == nullptr) continue;
        StockProductoEmpleadoInfo info;
        info.codigo = producto->getCodigo();
        info.nombre = producto->getNombre();
        info.categoria = (producto->getCategoria() != nullptr) ? producto->getCategoria()->getNombre() : "(sin categoria)";
        info.stockActual = producto->getStock();
        info.stockMinimo = producto->getStockMinimo();
        resultado.push_back(info);
    }
    return resultado;
}

vector<StockProductoEmpleadoInfo> EmpleadoController::listarProductosStockBajoVista(bool ordenarPorCriticidad) const {
    vector<StockProductoEmpleadoInfo> resultado;
    vector<Producto*> productos = listarProductosConStockBajo(ordenarPorCriticidad);
    for (Producto* producto : productos) {
        if (producto == nullptr) continue;
        StockProductoEmpleadoInfo info;
        info.codigo = producto->getCodigo();
        info.nombre = producto->getNombre();
        info.categoria = (producto->getCategoria() != nullptr) ? producto->getCategoria()->getNombre() : "(sin categoria)";
        info.stockActual = producto->getStock();
        info.stockMinimo = producto->getStockMinimo();
        resultado.push_back(info);
    }
    return resultado;
}

DetalleProductoEmpleadoVista EmpleadoController::obtenerDetalleProductoEmpleadoVista(int codigoProducto) const {
    DetalleProductoEmpleadoVista res;
    res.productoExiste = false;
    res.codigo = codigoProducto;
    res.nombre = "";
    res.descripcion = "";
    res.categoria = "";
    res.precioVentaUnitario = 0;
    res.stockActual = 0;
    res.stockMinimo = 0;
    res.puntajePromedio = 0;
    res.cantidadCalificaciones = 0;
    res.proveedores.clear();

    DetalleProductoAdmin detalle = obtenerDetalleProductoEmpleado(codigoProducto);
    if (!detalle.productoExiste || detalle.producto == nullptr) return res;

    Producto* producto = detalle.producto;
    res.productoExiste = true;
    res.codigo = producto->getCodigo();
    res.nombre = producto->getNombre();
    res.descripcion = producto->getDescripcion();
    res.categoria = (producto->getCategoria() != nullptr) ? producto->getCategoria()->getNombre() : "(sin categoria)";
    res.precioVentaUnitario = producto->getPrecioVentaActual();
    res.stockActual = producto->getStock();
    res.stockMinimo = producto->getStockMinimo();
    res.puntajePromedio = producto->getPuntajePromedio();
    res.cantidadCalificaciones = producto->getCantidadCalificaciones();
    res.proveedores = detalle.proveedores;
    return res;
}

string EmpleadoController::obtenerNombreClientePorRut(const string& rutCliente) const {
    Cliente* cliente = buscarCliente(rutCliente);
    if (cliente == nullptr) return "";
    return cliente->getNombre() + " " + cliente->getApellido();
}

string EmpleadoController::obtenerNombreProductoPorCodigo(int codigoProducto) const {
    if (adminCtrl == nullptr) return "";
    Producto* producto = adminCtrl->buscarProducto(codigoProducto);
    if (producto == nullptr) return "";
    return producto->getNombre();
}
