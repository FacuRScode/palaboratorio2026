#include "Producto.h"

using namespace std;
Producto::Producto(int codigo, string nombre, string descripcion, float precioVentaActual, int stock, float puntajePromedio) {
    this->codigo = codigo;
    this->nombre = nombre;
    this->descripcion = descripcion;
    this->precioVentaActual = precioVentaActual;
    this->stock = stock;
    this->puntajePromedio = puntajePromedio;
}
int Producto::getCodigo() {
    return this->codigo;
}
string Producto::getNombre() {
    return this->nombre;
}
string Producto::getDescripcion() {
    return this->descripcion;
}
float Producto::getPrecioVentaActual() {
 return this->precioVentaActual;
}
int Producto::getStock() {
    return this->stock;
}
float Producto::getPuntajePromedio() {
    return this->puntajePromedio;
}
void Producto::setCodigo(int codigo) {
    this->codigo = codigo;
}
void Producto::setNombre(string nombre) {
    this->nombre = nombre;
}
void Producto::setDescripcion(string descripcion) {
    this->descripcion = descripcion;
}
void Producto::setPrecioVentaActual(float precioVentaActual) {
    this->precioVentaActual = precioVentaActual;
}
void Producto::setStock(int stock) {
    this->stock = stock;
}
void Producto::setPuntajePromedio(float puntajePromedio) {
    this->puntajePromedio = puntajePromedio;
}

void Producto::addCalificacion(Calificacion* cal) {
    if (cal != nullptr) this->calificaciones.push_back(cal);
}

const vector<Calificacion*>& Producto::getCalificaciones() const {
    return this->calificaciones;
}

void Producto::addLineaVenta(LineaDetalleVenta* linea) {
    if (linea != nullptr) this->detalleVenta.push_back(linea);
}

const vector<LineaDetalleVenta*>& Producto::getDetalleVenta() const {
    return this->detalleVenta;
}
