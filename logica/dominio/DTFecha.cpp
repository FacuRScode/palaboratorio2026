
#include "DTFecha.h"

DTFecha::DTFecha(int dia, int mes, int anio) : dia(dia), mes(mes), anio(anio) {}

int DTFecha::getDia() {
	return this->dia;
}

int DTFecha::getMes() {
	return this->mes;
}

int DTFecha::getAnio() {
	return this->anio;
}

void DTFecha::setDia(int dia) {
	this->dia = dia;
}

void DTFecha::setMes(int mes) {
	this->mes = mes;
}

void DTFecha::setAnio(int anio) {
	this->anio = anio;
}


