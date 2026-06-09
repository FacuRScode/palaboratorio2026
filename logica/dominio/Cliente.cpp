#include "Cliente.h"

Cliente::Cliente(string rut, string nombre, string apellido, string direccion, string correo)
	: rut(rut), nombre(nombre), apellido(apellido), direccion(direccion), correo(correo) {
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

void Cliente::addVenta(Venta* venta) {
	if (venta != nullptr) this->ventas.push_back(venta);
}

const vector<Venta*>& Cliente::getVentas() const {
	return this->ventas;
}


