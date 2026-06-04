#ifndef CALIFICACION_H
#define CALIFICACION_H

#include <string>
#include "DTFecha.h"
#include "Puntaje.h"

using namespace std;

class Calificacion{
    private:
        Puntaje puntaje;
        string comentario;
        DTFecha fecha;
    public:
        Calificacion(Puntaje puntaje, string comentario, DTFecha fecha);

        Puntaje getPuntaje();
        string getComentario();
        DTFecha getFecha();

        void setPuntaje(Puntaje puntaje);
        void setComentario(string comentario);
        void setFecha(DTFecha fecha);
};

#endif