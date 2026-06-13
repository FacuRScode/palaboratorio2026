#ifndef CATEGORIA_H
#define CATEGORIA_H
#include <string>

using namespace std;
class Categoria{
    private:
        string nombre, descripcion;
    public:
        Categoria(string nombre, string descripcion);
//el michael estuvo aca
        string getNombre();
        string getDescripcion();

        void setNombre(string nombre);
        void setDescripcion(string descripcion);
};
#endif