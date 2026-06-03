
#include "DTHora.h"

DTHora::DTHora(int hora, int minuto, int segundo) : hora(hora), minuto(minuto), segundo(segundo) {}

int DTHora::getHora() {
	return this->hora;
}

int DTHora::getMinuto() {
	return this->minuto;
}

int DTHora::getSegundo() {
	return this->segundo;
}

void DTHora::setHora(int hora) {
	this->hora = hora;
}

void DTHora::setMinuto(int minuto) {
	this->minuto = minuto;
}

void DTHora::setSegundo(int segundo) {
	this->segundo = segundo;
}


