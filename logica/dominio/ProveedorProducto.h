#ifndef PROVEEDORPRODUCTO_H
#define PROVEEDORPRODUCTO_H
#include <vector>
#include "Proveedor.h"
#include "Producto.h"

using namespace std;
class ProveedorProducto{
    private:
        int precioCompraPactado, tiempoEntregaEstimadoEnDias;
    public:
    ProveedorProducto(int precioCompraPactado, int tiempoEntregaEstimadoEnDias);

    int getPrecioCompraPactado();
    int getTiempoEntregaEstimadoEnDias();

    void setPrecioCompraPactado(int precioCompraPactado);
    void setTiempoEntregaEstimadoEnDias(int tiempoEntregaEstimadoEnDias);
};
#endif
