#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <string>
#include "Categoria.h"


using namespace std;
class Producto{
private:
    int codigo, stock, stockMinimo, cantidadCalificaciones;
    string nombre, descripcion;
    float precioVentaActual, puntajePromedio;
    Categoria* categoria;
public:
    Producto(int codigo, string nombre, string descripcion, float precioVentaActual, int stock, int stockMinimo, float puntajePromedio, Categoria* categoria);

    int getCodigo();
    string getNombre();
    string getDescripcion();
    float getPrecioVentaActual();
    int getStock();
    int getStockMinimo();
    int getCantidadCalificaciones();
    float getPuntajePromedio();
    Categoria* getCategoria();

    void setCodigo(int codigo);
    void setNombre(string nombre);
    void setDescripcion(string descripcion);
    void setPrecioVentaActual(float precioVentaActual);
    void setStock(int stock);
    void setStockMinimo(int stockMinimo);
    void setCantidadCalificaciones(int cantidad);
    void setPuntajePromedio(float puntajePromedio);
    void setCategoria(Categoria* categoria);
};
#endif
