#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>
#include <vector>
#include "Calificacion.h"
#include "Venta.h"

using namespace std;
class Cliente{
    private:
        string rut, nombre, apellido, direccion, correo;
        vector<Calificacion*> calificaciones;
        vector<Venta*> ventas;
    public:
        Cliente(string rut, string nombre, string apellido, string direccion, string correo);

        string getRut();
        string getNombre();
        string getApellido();
        string getDireccion();
        string getCorreo();

        void setRut(string rut);
        void setNombre(string nombre);
        void setApellido(string apellido);
        void setDireccion(string direccion);
        void setCorreo(string correo);
};
#endif