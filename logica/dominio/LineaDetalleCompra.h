#ifndef LINEADETALLECOMPRA_H
#define LINEADETALLECOMPRA_H

using namespace std;
class LineaDetalleCompra{
    private:
        int cantidad;
    public:
    LineaDetalleCompra(int cantidad);

    int getCantidad();

    void setCantidad(int cantidad);
};
#endif