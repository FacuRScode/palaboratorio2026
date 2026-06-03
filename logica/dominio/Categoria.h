#ifndef CATEGORIA_h
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
};
#endif