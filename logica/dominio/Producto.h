#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <string>
#include <vector>
#include "Calificacion.h"
#include "LineaDetalleVenta.h"
#include "LineaDetalleCompra.h"


using namespace std;
class Producto{
    private:
        int codigo, stock;
        string nombre, descripcion;
        float precioVentaActual, puntajePromedio;
        vector<Calificacion*> calificaciones;
        vector<LineaDetalleVenta*> detalleVenta;
        vector<LineaDetalleCompra*> detalleCompra;
    public:
    Producto(int codigo, string nombre, string descripcion, float precioVentaActual, int stock, float puntajePromedio);

    int getCodigo();
    string getNombre();
    string getDescripcion();
    float getPrecioVentaActual();
    int getStock();
    float puntajePromedio();

    void setCodigo(int codigo);
    void setNombre(string nombre);
    void setDescripcion(string descripcion);
    void setPrecioVentaActual(float precioVentaActual);
    void setStock(int stock);
    void setPuntajePromedio(float puntajePromedio);
};
#endif