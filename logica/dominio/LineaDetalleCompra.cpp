
#include "LineaDetalleCompra.h"

LineaDetalleCompra::LineaDetalleCompra(int cantidad) : cantidad(cantidad) {}

int LineaDetalleCompra::getCantidad() {
	return this->cantidad;
}

void LineaDetalleCompra::setCantidad(int cantidad) {
	this->cantidad = cantidad;
}


