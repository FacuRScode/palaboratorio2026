//
// Created by facun on 2/6/2026.
//

#include "VentaController.h"
#include "EmpleadoController.h"
#include "../dominio/Cliente.h"
#include <ctime>
#include <algorithm>

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
	if (adminCtrl == nullptr || empleadoCtrl == nullptr) return false;

	Producto* p = adminCtrl->buscarProducto(codigoProducto);
	if (p == nullptr) return false;

	Cliente* cliente = empleadoCtrl->buscarClientePorCorreo(correoCliente);
	if (cliente == nullptr) return false;

	// Usar fecha y hora actual del sistema
	DTFecha fechaActual = obtenerFechaActual();

	// Crear la calificación (ahora con referencia al cliente)
	Calificacion* cal = new Calificacion(puntaje, comentario, fechaActual, p, cliente);

	// Asociar la calificación al cliente
	cliente->addCalificacion(cal);

	// Recalcular el puntaje promedio del producto
	float promedioActual = p->getPuntajePromedio();
	int cantidadActual = p->getCantidadCalificaciones();
	float nuevoPromedio = (promedioActual * cantidadActual + static_cast<int>(puntaje)) / (cantidadActual + 1);

	p->setPuntajePromedio(nuevoPromedio);
	p->setCantidadCalificaciones(cantidadActual + 1);

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
