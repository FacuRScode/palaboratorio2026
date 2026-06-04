#ifndef LINEADETALLEVENTA_H
#define LINEADETALLEVENTA_H

using namespace std;
class LineaDetalleVenta{
    private:
        int cantidad;
        float precioUnitario;
    public:
        LineaDetalleVenta(int cantidad, float precioUnitario);

        int getCantidad();
        float getPrecioUnitario();

        void setCantidad(int cantidad);
        void setPrecioUnitario(float precioUnitario);
};
#endif