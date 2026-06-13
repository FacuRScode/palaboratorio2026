#include "LineaDetalleVenta.h"
#include <string>

using namespace std;
LineaDetalleVenta::LineaDetalleVenta(int nuevaCantidad, float nuevoPrecioUnitario, Producto* producto) {
    this->cantidad = nuevaCantidad;
    this->precioUnitario = nuevoPrecioUnitario;
    this->producto = producto;
}
int LineaDetalleVenta::getCantidad() {
    return this->cantidad;
}
float LineaDetalleVenta::getPrecioUnitario() {
    return this->precioUnitario;
}
Producto* LineaDetalleVenta::getProducto() {
    return this->producto;
}
void LineaDetalleVenta::setCantidad(int cantidad) {
    this->cantidad = cantidad;
}
void LineaDetalleVenta::setPrecioUnitario(float precioUnitario) {
    this->precioUnitario = precioUnitario;
}
void LineaDetalleVenta::setProducto(Producto* producto) {
    this->producto = producto;
}