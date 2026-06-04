#include "ProveedorProducto.h"

using namespace std;
ProveedorProducto::ProveedorProducto(int precioCompraPactado, int tiempoEntregaEstimadoEnDias) {
    this->precioCompraPactado = precioCompraPactado;
    this->tiempoEntregaEstimadoEnDias = tiempoEntregaEstimadoEnDias;
}
int ProveedorProducto::getPrecioCompraPactado() {
    return this->precioCompraPactado;
}
int ProveedorProducto::getTiempoEntregaEstimadoEnDias() {
    return this->tiempoEntregaEstimadoEnDias;
}
void ProveedorProducto::setPrecioCompraPactado(int precioCompraPactado) {
    this->precioCompraPactado = precioCompraPactado;
}

void ProveedorProducto::setTiempoEntregaEstimadoEnDias(int tiempoEntregaEstimadoEnDias) {
    this->tiempoEntregaEstimadoEnDias = tiempoEntregaEstimadoEnDias;
}
