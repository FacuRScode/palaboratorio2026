#ifndef CATEGORIA_H
#define CATEGORIA_H

#include <string>
#include <vector>
#include "Producto.h"

using namespace std;
class Categoria{
    private:
        string nombre, descripcion;
        vector<Producto*> productos;
    public:
        Categoria(string nombre, string descripcion);

        string getNombre();
        string getDescripcion();

        void setNombre(string nombre);
        void setDescripcion(string descripcion);

        void addProducto(Producto* producto);
        bool removeProducto(Producto* producto);

        const vector<Producto*>& getProductos() const;
};
#endif