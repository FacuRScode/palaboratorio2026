#include "Empleado.h"

Empleado::Empleado(const string& nombre, const string& correo, const string& contrasena, const string& rol)
    : nombre(nombre), correo(correo), contrasena(contrasena), rol(rol) {}

string Empleado::getNombre() { return nombre; }
string Empleado::getCorreo() { return correo; }
string Empleado::getContrasena() { return contrasena; }
string Empleado::getRol() { return rol; }

void Empleado::setNombre(const string& nombre) { this->nombre = nombre; }
void Empleado::setCorreo(const string& correo) { this->correo = correo; }
void Empleado::setContrasena(const string& contrasena) { this->contrasena = contrasena; }
void Empleado::setRol(const string& rol) { this->rol = rol; }

