
#include "DTFecha.h"

DTFecha::DTFecha(int dia, int mes, int anio) : dia(dia), mes(mes), anio(anio) {}

int DTFecha::getDia() const {
	return this->dia;
}

int DTFecha::getMes() const {
	return this->mes;
}

int DTFecha::getAnio() const {
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

bool DTFecha::operator<=(const DTFecha& otra) const {
	if (anio != otra.anio) return anio <= otra.anio;
	if (mes != otra.mes) return mes <= otra.mes;
	return dia <= otra.dia;
}

bool DTFecha::operator>=(const DTFecha& otra) const {
	return otra <= *this;
}

bool DTFecha::operator<(const DTFecha& otra) const {
	if (anio != otra.anio) return anio < otra.anio;
	if (mes != otra.mes) return mes < otra.mes;
	return dia < otra.dia;
}


