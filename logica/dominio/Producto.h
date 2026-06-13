#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <string>
#include "Categoria.h"


using namespace std;
class Producto{
private:
    int codigo, stock;
    string nombre, descripcion;
    float precioVentaActual, puntajePromedio;
    Categoria* categoria;
public:
    Producto(int codigo, string nombre, string descripcion, float precioVentaActual, int stock, float puntajePromedio, Categoria* categoria);

    int getCodigo();
    string getNombre();
    string getDescripcion();
    float getPrecioVentaActual();
    int getStock();
    float getPuntajePromedio();
    Categoria* getCategoria();

    void setCodigo(int codigo);
    void setNombre(string nombre);
    void setDescripcion(string descripcion);
    void setPrecioVentaActual(float precioVentaActual);
    void setStock(int stock);
    void setPuntajePromedio(float puntajePromedio);
    void setCategoria(Categoria* categoria);
};
#endif