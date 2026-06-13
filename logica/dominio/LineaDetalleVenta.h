#ifndef LINEADETALLEVENTA_H
#define LINEADETALLEVENTA_H
#include "Producto.h"

using namespace std;
class LineaDetalleVenta{
private:
    int cantidad;
    float precioUnitario;
    Producto* producto;
public:
    LineaDetalleVenta(int cantidad, float precioUnitario, Producto* producto);

    int getCantidad();
    float getPrecioUnitario();
    Producto* getProducto();

    void setCantidad(int cantidad);
    void setPrecioUnitario(float precioUnitario);
    void setProducto(Producto* producto);
};
#endif