#ifndef ORDENDECOMPRA_H
#define ORDENDECOMPRA_H
#include <vector>
#include "LineaDetalleCompra.h"
#include "Proveedor.h"
#include "DTFecha.h"
#include "Estado.h"

using namespace std;
class OrdenDeCompra{
private:
    DTFecha fechaEmision, fechaRecepcion;
    Estado estado;
    Proveedor* proveedor;
    vector<LineaDetalleCompra*> detalleCompra;
public:
    OrdenDeCompra(DTFecha DTfechaEmision, Estado estado, DTFecha fechaRecepcion, Proveedor* proveedor);

    DTFecha getFechaEmision();
    DTFecha getFechaRecepcion();
    Estado getEstado();
    Proveedor* getProveedor();

    void setFechaEmision(DTFecha fechaEmision);
    void setFechaRecepcion(DTFecha fechaRecepcion);
    void setEstado(Estado estado);
    void setProveedor(Proveedor* proveedor);

    void addLineaDetalleCompra(LineaDetalleCompra* linea);
    const vector<LineaDetalleCompra*>& getDetalleCompra() const;
    float calcularTotal() const;

    ~OrdenDeCompra();
};
#endif