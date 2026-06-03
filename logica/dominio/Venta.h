#ifndef VENTA_H
#define VENTA_H
#include <vector>
#include "LineaDetalleVenta.h"
#include "DTFecha.h"
#include "DTHora.h"

using namespace std;
class Venta{
    public:
        DTFecha fecha;
        DTHora hora;
        vector<LineaDetalleVenta*> detalleVenta;
    private:

};
#endif