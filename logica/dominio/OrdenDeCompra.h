#ifndef ORDENDECOMPRA_H
#define ORDENDECOMPRA_H
#include <vector>
#include "LineaDetalleCompra.h"
#include "DTFecha.h"
#include "Estado.h"

using namespace std;
class OrdenDeCompra{
    private:
        DTFecha fechaEmision, fechaRecepcion;
        Estado estado;
        vector<LineaDetalleCompra*> detalleCompra;
    public:
    OrdenDeCompra(DTFecha DTfechaEmision, Estado estado, DTFecha fechaRecepcion);

    DTFecha getFechaEmision();
    DTFecha getFechaRecepcion();
    Estado getEstado();

    void setFechaEmision(DTFecha fechaEmision);
    void setFechaRecepcion(DTFecha fechaRecepcion);
    void setEstado(Estado estado);
};
#endif