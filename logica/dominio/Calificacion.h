#ifndef CALIFICACION_H
#define CALIFICACION_H

#include <string>
#include "DTFecha.h"
#include "Puntaje.h"
#include "Producto.h"

using namespace std;

class Cliente; // forward declaration

class Calificacion{
    private:
        Puntaje puntaje;
        string comentario;
        DTFecha fecha;
        Producto* productoCalificado;
        Cliente* clienteCalificador;
    public:
        Calificacion(Puntaje puntaje, string comentario, DTFecha fecha, Producto* productoCalificado, Cliente* clienteCalificador);

        Puntaje getPuntaje();
        string getComentario();
        DTFecha getFecha();
        Producto* getProductoCalificado();
        Cliente* getClienteCalificador();

        void setPuntaje(Puntaje puntaje);
        void setComentario(string comentario);
        void setFecha(DTFecha fecha);
        void setProductoCalificado(Producto* productoCalificado);
        void setClienteCalificador(Cliente* clienteCalificador);
};

#endif