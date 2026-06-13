#include "LineaDetalleCompra.h"

LineaDetalleCompra::LineaDetalleCompra(int cantidad, Producto* producto) : cantidad(cantidad), producto(producto) {}

int LineaDetalleCompra::getCantidad() {
	return this->cantidad;
}

Producto* LineaDetalleCompra::getProducto() {
	return this->producto;
}

void LineaDetalleCompra::setCantidad(int cantidad) {
	this->cantidad = cantidad;
}

void LineaDetalleCompra::setProducto(Producto* producto) {
	this->producto = producto;
}


