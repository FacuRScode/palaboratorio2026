#include "ProveedorProducto.h"

using namespace std;
ProveedorProducto::ProveedorProducto(int precioCompraPactado, int tiempoEntregaEstimadoEnDias, Producto* producto) {
    this->precioCompraPactado = precioCompraPactado;
    this->tiempoEntregaEstimadoEnDias = tiempoEntregaEstimadoEnDias;
    this->producto = producto;
}
int ProveedorProducto::getPrecioCompraPactado() {
    return this->precioCompraPactado;
}
int ProveedorProducto::getTiempoEntregaEstimadoEnDias() {
    return this->tiempoEntregaEstimadoEnDias;
}
Producto* ProveedorProducto::getProducto() {
    return this->producto;
}
void ProveedorProducto::setPrecioCompraPactado(int precioCompraPactado) {
    this->precioCompraPactado = precioCompraPactado;
}

void ProveedorProducto::setTiempoEntregaEstimadoEnDias(int tiempoEntregaEstimadoEnDias) {
    this->tiempoEntregaEstimadoEnDias = tiempoEntregaEstimadoEnDias;
}
void ProveedorProducto::setProducto(Producto* producto) {
    this->producto = producto;
}
