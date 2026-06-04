#include "OrdenDeCompra.h"
#include <string>

using namespace std;
OrdenDeCompra::OrdenDeCompra(DTFecha fechaEmision, Estado estado, DTFecha fechaRecepcion) : fechaEmision(fechaEmision), estado(estado), fechaRecepcion(fechaRecepcion){}
DTFecha OrdenDeCompra::getFechaEmision() {
  return this->fechaEmision;
}DTFecha OrdenDeCompra::getFechaRecepcion() {
  return this->fechaRecepcion;
}
void OrdenDeCompra::setFechaEmision(DTFecha fechaEmision) {
  this->fechaEmision = fechaEmision;
}
void OrdenDeCompra::setFechaRecepcion(DTFecha fechaRecepcion) {
  this->fechaRecepcion = fechaRecepcion;
}
void OrdenDeCompra::setEstado(Estado estado) {
  this->estado = estado;
}