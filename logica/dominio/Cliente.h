#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>
#include <vector>
#include "Calificacion.h"

using namespace std;
class Cliente{
    private:
        string rut, nombre, apellido, direccion, correo, contrasena;
        vector<Calificacion*> calificaciones;
    public:
        Cliente(string rut, string nombre, string apellido, string direccion, string correo, string contrasena);

        string getRut();
        string getNombre();
        string getApellido();
        string getDireccion();
        string getCorreo();
        string getContrasena();

        void setRut(string rut);
        void setNombre(string nombre);
        void setApellido(string apellido);
        void setDireccion(string direccion);
        void setCorreo(string correo);
        void setContrasena(string contrasena);

    // Calificaciones
    void addCalificacion(Calificacion* calificacion);
    const vector<Calificacion*>& getCalificaciones() const;
};
#endif
