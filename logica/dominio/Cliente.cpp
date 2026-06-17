#include "Cliente.h"

Cliente::Cliente(string rut, string nombre, string apellido, string direccion, string correo, string contrasena)
	: rut(rut), nombre(nombre), apellido(apellido), direccion(direccion), correo(correo), contrasena(contrasena) {
}

string Cliente::getRut() {
	return this->rut;
}

string Cliente::getNombre() {
	return this->nombre;
}

string Cliente::getApellido() {
	return this->apellido;
}

string Cliente::getDireccion() {
	return this->direccion;
}

string Cliente::getCorreo() {
	return this->correo;
}

string Cliente::getContrasena() {
	return this->contrasena;
}

void Cliente::setRut(string rut) {
	this->rut = rut;
}

void Cliente::setNombre(string nombre) {
	this->nombre = nombre;
}

void Cliente::setApellido(string apellido) {
	this->apellido = apellido;
}

void Cliente::setDireccion(string direccion) {
	this->direccion = direccion;
}

void Cliente::setCorreo(string correo) {
	this->correo = correo;
}

void Cliente::setContrasena(string contrasena) {
	this->contrasena = contrasena;
}

void Cliente::addCalificacion(Calificacion* calificacion) {
	this->calificaciones.push_back(calificacion);
}

const vector<Calificacion*>& Cliente::getCalificaciones() const {
	return this->calificaciones;
}
