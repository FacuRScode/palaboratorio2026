#ifndef VENTA_H
#define VENTA_H
#include <vector>
#include "LineaDetalleVenta.h"
#include "DTFecha.h"
#include "DTHora.h"

using namespace std;
class Venta{
    private:
        DTFecha fecha;
        DTHora hora;
        vector<LineaDetalleVenta*> detalleVenta;
    public:
    Venta(DTFecha fecha, DTHora hora);

    DTFecha getFecha();
    DTHora getHora();

    void setFecha(DTFecha fecha);
    void setHora(DTHora hora);

    // Detalle de la venta
    void addLinea(LineaDetalleVenta* linea);
    const vector<LineaDetalleVenta*>& getDetalle() const;
    float calcularTotal() const;

    ~Venta();

};
#endif