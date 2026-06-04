#include "LineaDetalleVenta.h"
#include <string>

using namespace std;
LineaDetalleVenta::LineaDetalleVenta(int nuevaCantidad, float nuevoPrecioUnitario) {
    this->cantidad = nuevaCantidad;
    this->precioUnitario = nuevoPrecioUnitario;
}
int LineaDetalleVenta::getCantidad() {
    return this->cantidad;
}
float LineaDetalleVenta::getPrecioUnitario() {
    return this->precioUnitario;
}
void LineaDetalleVenta::setCantidad(int cantidad) {
    this->cantidad = cantidad;
}
void LineaDetalleVenta::setPrecioUnitario(float precioUnitario) {
    this->precioUnitario = precioUnitario;
}