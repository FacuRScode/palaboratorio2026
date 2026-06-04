#include "Calificacion.h"

Calificacion::Calificacion(Puntaje puntaje, string comentario, DTFecha fecha)
	: puntaje(puntaje), comentario(comentario), fecha(fecha) {}

Puntaje Calificacion::getPuntaje(){
	return this->puntaje;
}

string Calificacion::getComentario(){
	return this->comentario;
}

DTFecha Calificacion::getFecha(){
	return this->fecha;
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

