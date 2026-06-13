#ifndef VENTA_H
#define VENTA_H
#include <vector>
#include "LineaDetalleVenta.h"
#include "Cliente.h"
#include "DTFecha.h"
#include "DTHora.h"

using namespace std;
class Venta{
private:
    DTFecha fecha;
    DTHora hora;
    vector<LineaDetalleVenta*> detalleVenta;
    Cliente* cliente;
public:
    Venta(DTFecha fecha, DTHora hora);
    Venta(DTFecha fecha, DTHora hora, Cliente* cliente);

    DTFecha getFecha();
    DTHora getHora();
    Cliente* getCliente();

    void setFecha(DTFecha fecha);
    void setHora(DTHora hora);
    void setCliente(Cliente* cliente);


    // Detalle de la venta
    void addLinea(LineaDetalleVenta* linea);
    const vector<LineaDetalleVenta*>& getDetalle() const;
    float calcularTotal() const;

    ~Venta();

};
#endif