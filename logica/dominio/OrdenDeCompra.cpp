#include "OrdenDeCompra.h"
#include <string>

using namespace std;
OrdenDeCompra::OrdenDeCompra(DTFecha fechaEmision, Estado estado, DTFecha fechaRecepcion, Proveedor* proveedor) : fechaEmision(fechaEmision), estado(estado), fechaRecepcion(fechaRecepcion), proveedor(proveedor){}
DTFecha OrdenDeCompra::getFechaEmision() {
  return this->fechaEmision;
}
Estado OrdenDeCompra::getEstado() {
  return this->estado;
}
DTFecha OrdenDeCompra::getFechaRecepcion() {
  return this->fechaRecepcion;
}
Proveedor* OrdenDeCompra::getProveedor() {
  return this->proveedor;
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
void OrdenDeCompra::setProveedor(Proveedor* proveedor) {
  this->proveedor = proveedor;
}

const vector<LineaDetalleCompra*>& OrdenDeCompra::getDetalleCompra() const {
  return this->detalleCompra;
}

void OrdenDeCompra::addLineaDetalleCompra(LineaDetalleCompra* linea) {
  this->detalleCompra.push_back(linea);
}

float OrdenDeCompra::calcularTotal() const {
  float total = 0.0f;
  for (auto l : detalleCompra) {
    if (l != nullptr) total += l->getCantidad() * l->getPrecioCompra();
  }
  return total;
}

OrdenDeCompra::~OrdenDeCompra() {
  for (LineaDetalleCompra* linea : detalleCompra) {
    delete linea;
  }
  detalleCompra.clear();
}