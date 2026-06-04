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