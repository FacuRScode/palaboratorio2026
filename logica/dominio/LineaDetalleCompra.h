#ifndef LINEADETALLECOMPRA_H
#define LINEADETALLECOMPRA_H
#include "Producto.h"

using namespace std;
class LineaDetalleCompra{
private:
    int cantidad;
    Producto* producto;
public:
    LineaDetalleCompra(int cantidad, Producto* producto);

    int getCantidad();
    Producto* getProducto();

    void setCantidad(int cantidad);
    void setProducto(Producto* producto);
};
#endif