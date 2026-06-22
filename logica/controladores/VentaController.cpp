//
// Created by facun on 2/6/2026.
//

#include "VentaController.h"
#include "EmpleadoController.h"
#include "../dominio/Cliente.h"
#include <ctime>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Inicializar el miembro estático
VentaController* VentaController::instanciaVenta = nullptr;

VentaController::VentaController(AdminController* admin, EmpleadoController* empleado)
	: adminCtrl(admin), empleadoCtrl(empleado) {}

// Constructor privado usado por el singleton
VentaController::VentaController() : adminCtrl(nullptr), empleadoCtrl(nullptr) {}

VentaController* VentaController::getInstanciaVenta(){
	if (instanciaVenta == nullptr) {
		instanciaVenta = new VentaController();
	}
	return instanciaVenta;
}

VentaController::~VentaController() {}

// Obtener la fecha y hora actual del sistema
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

// Listar productos que el cliente ha comprado al menos una vez
vector<Producto*> VentaController::listarProductosComprados(const string& correoCliente) {
	vector<Producto*> productosComprados;
	if (adminCtrl == nullptr || empleadoCtrl == nullptr) return productosComprados;

	Cliente* cliente = empleadoCtrl->buscarClientePorCorreo(correoCliente);
	if (cliente == nullptr) return productosComprados;

	vector<Venta*> ventasCliente = empleadoCtrl->listarVentasPorCliente(cliente->getRut());

	// Extraer productos únicos de las líneas de detalle
	for (Venta* v : ventasCliente) {
		if (v == nullptr) continue;
		for (LineaDetalleVenta* linea : v->getDetalle()) {
			if (linea != nullptr && linea->getProducto() != nullptr) {
				Producto* p = linea->getProducto();
				// Evitar duplicados
				bool yaAgregado = false;
				for (Producto* existente : productosComprados) {
					if (existente->getCodigo() == p->getCodigo()) {
						yaAgregado = true;
						break;
					}
				}
				if (!yaAgregado) {
					productosComprados.push_back(p);
				}
			}
		}
	}
	return productosComprados;
}

// Calificaciones
bool VentaController::calificarProducto(int codigoProducto, Puntaje puntaje, const string& comentario, const string& correoCliente) {
	if (adminCtrl == nullptr || empleadoCtrl == nullptr) {
		throw logic_error("No hay controladores disponibles para registrar la calificacion.");
	}
	int puntajeNumerico = static_cast<int>(puntaje);
	if (puntajeNumerico < 1 || puntajeNumerico > 5) {
		throw invalid_argument("Puntaje invalido: debe estar entre 1 y 5.");
	}

	Producto* p = adminCtrl->buscarProducto(codigoProducto);
	if (p == nullptr) {
		throw invalid_argument("Producto invalido: no existe un producto con ese codigo.");
	}

	Cliente* cliente = empleadoCtrl->buscarClientePorCorreo(correoCliente);
	if (cliente == nullptr) {
		throw invalid_argument("Cliente invalido: no existe un cliente registrado con ese correo.");
	}

	bool productoComprado = false;
	vector<Producto*> comprados = listarProductosComprados(correoCliente);
	for (Producto* comprado : comprados) {
		if (comprado != nullptr && comprado->getCodigo() == codigoProducto) {
			productoComprado = true;
			break;
		}
	}
	if (!productoComprado) {
		throw logic_error("Operacion invalida: solo puede calificar un cliente que haya comprado el producto.");
	}

	// Usar fecha y hora actual del sistema
	DTFecha fechaActual = obtenerFechaActual();

	// Crear la calificación (ahora con referencia al cliente)
	Calificacion* cal = new Calificacion(puntaje, comentario, fechaActual, p, cliente);

	// Asociar la calificación al cliente
	cliente->addCalificacion(cal);


	vector<Calificacion*> historico = listarCalificacionesDeProducto(codigoProducto);
	int suma = 0;
	int cantidad = 0;
	for (Calificacion* c : historico) {
		if (c == nullptr) continue;
		suma += static_cast<int>(c->getPuntaje());
		cantidad++;
	}
	float nuevoPromedio = (cantidad > 0) ? (static_cast<float>(suma) / static_cast<float>(cantidad)) : 0.0f;
	p->setCantidadCalificaciones(cantidad);
	p->setPuntajePromedio(nuevoPromedio);

	return true;
}

// Listar todas las calificaciones de un producto, ordenadas por fecha descendente
vector<Calificacion*> VentaController::listarCalificacionesDeProducto(int codigoProducto) {
	vector<Calificacion*> resultado;
	if (empleadoCtrl == nullptr) return resultado;

	vector<Cliente*> todosClientes = empleadoCtrl->listarClientes();

	// Recolectar todas las calificaciones del producto
	for (Cliente* c : todosClientes) {
		if (c == nullptr) continue;
		for (Calificacion* cal : c->getCalificaciones()) {
			if (cal != nullptr && cal->getProductoCalificado() != nullptr &&
				cal->getProductoCalificado()->getCodigo() == codigoProducto) {
				resultado.push_back(cal);
			}
		}
	}

	// Ordenar por fecha descendente (más reciente primero)
	sort(resultado.begin(), resultado.end(), [](Calificacion* a, Calificacion* b) {
		DTFecha fa = a->getFecha();
		DTFecha fb = b->getFecha();
		if (fa.getAnio() != fb.getAnio()) return fa.getAnio() > fb.getAnio();
		if (fa.getMes() != fb.getMes()) return fa.getMes() > fb.getMes();
		return fa.getDia() > fb.getDia();
	});

	return resultado;
}

vector<ProductoClienteInfo> VentaController::listarProductosCompradosCliente(const string& correoCliente) {
	vector<ProductoClienteInfo> resultado;
	vector<Producto*> comprados = listarProductosComprados(correoCliente);
	for (Producto* p : comprados) {
		if (p == nullptr) continue;
		ProductoClienteInfo info;
		info.codigo = p->getCodigo();
		info.nombre = p->getNombre();
		info.descripcion = p->getDescripcion();
		resultado.push_back(info);
	}
	return resultado;
}

vector<ProductoClienteInfo> VentaController::listarProductosCatalogoCliente() const {
	vector<ProductoClienteInfo> resultado;
	if (adminCtrl == nullptr) return resultado;
	for (Producto* p : adminCtrl->listarProductos()) {
		if (p == nullptr) continue;
		ProductoClienteInfo info;
		info.codigo = p->getCodigo();
		info.nombre = p->getNombre();
		info.descripcion = p->getDescripcion();
		resultado.push_back(info);
	}
	return resultado;
}

ResultadoCalificacionCliente VentaController::registrarCalificacionCliente(const string& correoCliente, int codigoProducto, int puntaje, const string& comentario) {
	ResultadoCalificacionCliente res;
	res.exito = false;
	res.productoExiste = false;
	res.puntajeValido = (puntaje >= 1 && puntaje <= 5);
	res.productoComprado = false;
	res.nombreProducto = "";
	res.puntajePromedioActualizado = 0;

	if (adminCtrl == nullptr || empleadoCtrl == nullptr) {
		throw logic_error("No hay controladores disponibles para registrar la calificacion.");
	}
	Producto* producto = adminCtrl->buscarProducto(codigoProducto);
	if (producto == nullptr) {
		throw invalid_argument("Producto invalido: no existe un producto con ese codigo.");
	}

	res.productoExiste = true;
	res.nombreProducto = producto->getNombre();
	if (!res.puntajeValido) {
		throw invalid_argument("Puntaje invalido: debe estar entre 1 y 5.");
	}

	vector<Producto*> comprados = listarProductosComprados(correoCliente);
	for (Producto* p : comprados) {
		if (p != nullptr && p->getCodigo() == codigoProducto) {
			res.productoComprado = true;
			break;
		}
	}
	if (!res.productoComprado) {
		throw logic_error("Operacion invalida: solo puede calificar un cliente que haya comprado el producto.");
	}

	calificarProducto(codigoProducto, static_cast<Puntaje>(puntaje), comentario, correoCliente);

	res.exito = true;
	res.puntajePromedioActualizado = producto->getPuntajePromedio();
	return res;
}

ResultadoDetalleProductoCliente VentaController::obtenerDetalleProductoCliente(int codigoProducto) {
	ResultadoDetalleProductoCliente res;
	res.productoExiste = false;
	res.codigo = codigoProducto;
	res.nombre = "";
	res.descripcion = "";
	res.categoria = "";
	res.precioVentaUnitario = 0;
	res.stockActual = 0;
	res.puntajePromedio = 0;
	res.cantidadCalificaciones = 0;
	res.ultimasCalificaciones.clear();

	if (adminCtrl == nullptr) return res;
	Producto* producto = adminCtrl->buscarProducto(codigoProducto);
	if (producto == nullptr) return res;

	res.productoExiste = true;
	res.codigo = producto->getCodigo();
	res.nombre = producto->getNombre();
	res.descripcion = producto->getDescripcion();
	res.categoria = (producto->getCategoria() != nullptr) ? producto->getCategoria()->getNombre() : "(sin categoria)";
	res.precioVentaUnitario = producto->getPrecioVentaActual();
	res.stockActual = producto->getStock();
	res.puntajePromedio = producto->getPuntajePromedio();
	res.cantidadCalificaciones = producto->getCantidadCalificaciones();

	vector<Calificacion*> calificaciones = listarCalificacionesDeProducto(codigoProducto);
	int mostrar = min(5, static_cast<int>(calificaciones.size()));
	for (int i = 0; i < mostrar; ++i) {
		Calificacion* cal = calificaciones[i];
		if (cal == nullptr) continue;
		CalificacionClienteInfo info{
			cal->getFecha(),
			static_cast<int>(cal->getPuntaje()),
			cal->getComentario()
		};
		res.ultimasCalificaciones.push_back(info);
	}

	return res;
}

ResultadoCalificacionesProductoVista VentaController::obtenerCalificacionesProductoVista(int codigoProducto) {
	ResultadoCalificacionesProductoVista res;
	res.productoExiste = false;
	res.nombreProducto = "";
	res.puntajePromedio = 0;
	res.cantidadCalificaciones = 0;
	res.calificaciones.clear();

	if (adminCtrl == nullptr) return res;
	Producto* producto = adminCtrl->buscarProducto(codigoProducto);
	if (producto == nullptr) return res;

	res.productoExiste = true;
	res.nombreProducto = producto->getNombre();
	res.puntajePromedio = producto->getPuntajePromedio();
	res.cantidadCalificaciones = producto->getCantidadCalificaciones();

	vector<Calificacion*> calificaciones = listarCalificacionesDeProducto(codigoProducto);
	for (Calificacion* cal : calificaciones) {
		if (cal == nullptr) continue;
		CalificacionEmpleadoVistaInfo info{
			cal->getFecha(),
			static_cast<int>(cal->getPuntaje()),
			(cal->getClienteCalificador() != nullptr) ? cal->getClienteCalificador()->getRut() : "N/A",
			cal->getComentario()
		};
		res.calificaciones.push_back(info);
	}

	return res;
}
