#ifndef CALIFICACION_H
#define CALIFICACION_H

#include <string>
#include "DTFecha.h"
#include "Puntaje.h"
#include "Producto.h"

using namespace std;

class Calificacion{
    private:
        Puntaje puntaje;
        string comentario;
        DTFecha fecha;
        Producto* productoCalificado;
    public:
        Calificacion(Puntaje puntaje, string comentario, DTFecha fecha, Producto* productoCalificado);

        Puntaje getPuntaje();
        string getComentario();
        DTFecha getFecha();
        Producto* getProductoCalificado();

        void setPuntaje(Puntaje puntaje);
        void setComentario(string comentario);
        void setFecha(DTFecha fecha);
        void setProductoCalificado(Producto* productoCalificado);
};

#endif