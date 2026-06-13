#ifndef PROVEEDORPRODUCTO_H
#define PROVEEDORPRODUCTO_H
#include <vector>
#include "Producto.h"

using namespace std;
class ProveedorProducto{
private:
    int precioCompraPactado, tiempoEntregaEstimadoEnDias;
    Producto* producto;
public:
    ProveedorProducto(int precioCompraPactado, int tiempoEntregaEstimadoEnDias, Producto* producto);

    int getPrecioCompraPactado();
    int getTiempoEntregaEstimadoEnDias();
    Producto* getProducto();

    void setPrecioCompraPactado(int precioCompraPactado);
    void setTiempoEntregaEstimadoEnDias(int tiempoEntregaEstimadoEnDias);
    void setProducto(Producto* producto);
};
#endif