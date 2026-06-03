#ifndef ORDENDECOMPRA_H
#define ORDENDECOMPRA_H
#include <vector>
#include "LineaDetalleCompra.h"
#include "DTFecha.h"

using namespace std;
class OrdenDeCompra{
    private:
        DTFecha fechaEmision, fechaRecepcion;
        //Estado estado;
        vector<LineaDetalleCompra*> detalleCompra;
    public:
    OrdenDeCompra();

};
#endif