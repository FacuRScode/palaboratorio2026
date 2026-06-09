#ifndef EMPLEADO_H
#define EMPLEADO_H

#include <string>

using namespace std;

class Empleado {
private:
    string nombre;
    string correo;
    string contrasena;
    string rol; // "Empleado" o "Administrador"
public:
    Empleado(const string& nombre, const string& correo, const string& contrasena, const string& rol);

    string getNombre();
    string getCorreo();
    string getContrasena();
    string getRol();

    void setNombre(const string& nombre);
    void setCorreo(const string& correo);
    void setContrasena(const string& contrasena);
    void setRol(const string& rol);
};

#endif

