#include "Calificacion.h"
#include "Cliente.h"

Calificacion::Calificacion(Puntaje puntaje, string comentario, DTFecha fecha, Producto* productoCalificado, Cliente* clienteCalificador)
	: puntaje(puntaje), comentario(comentario), fecha(fecha), productoCalificado(productoCalificado), clienteCalificador(clienteCalificador) {}

Puntaje Calificacion::getPuntaje(){
	return this->puntaje;
}

string Calificacion::getComentario(){
	return this->comentario;
}

DTFecha Calificacion::getFecha(){
	return this->fecha;
}

Producto* Calificacion::getProductoCalificado(){
	return this->productoCalificado;
}

Cliente* Calificacion::getClienteCalificador(){
	return this->clienteCalificador;
}

void Calificacion::setPuntaje(Puntaje puntaje){
	this->puntaje = puntaje;
}

void Calificacion::setComentario(string comentario){
	this->comentario = comentario;
}

void Calificacion::setFecha(DTFecha fecha){
	this->fecha = fecha;
}

void Calificacion::setProductoCalificado(Producto* productoCalificado){
	this->productoCalificado = productoCalificado;
}

void Calificacion::setClienteCalificador(Cliente* clienteCalificador){
	this->clienteCalificador = clienteCalificador;
}
