#include "Venta.h"

using namespace std;
Venta::Venta(DTFecha fecha, DTHora hora) : fecha(fecha), hora(hora) {}
DTFecha Venta::getFecha() {
    return this->fecha;
}
DTHora Venta::getHora() {
    return this->hora;
}
void Venta::setFecha(DTFecha fecha) {
    this->fecha = fecha;
}
void Venta::setHora(DTHora hora) {
    this->hora = hora;
}

void Venta::addLinea(LineaDetalleVenta* linea) {
    if (linea != nullptr) this->detalleVenta.push_back(linea);
}

const vector<LineaDetalleVenta*>& Venta::getDetalle() const {
    return this->detalleVenta;
}

float Venta::calcularTotal() const {
    float total = 0.0f;
    for (auto l : detalleVenta) {
        if (l != nullptr) total += l->getCantidad() * l->getPrecioUnitario();
    }
    return total;
}

Venta::~Venta() {
    for (auto l : detalleVenta) delete l;
}

