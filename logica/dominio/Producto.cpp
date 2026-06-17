#include "Producto.h"

using namespace std;
Producto::Producto(int codigo, string nombre, string descripcion, float precioVentaActual, int stock, int stockMinimo, float puntajePromedio, Categoria* categoria) {
    this->codigo = codigo;
    this->nombre = nombre;
    this->descripcion = descripcion;
    this->precioVentaActual = precioVentaActual;
    this->stock = stock;
    this->stockMinimo = stockMinimo;
    this->categoria = categoria;
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
int Producto::getStockMinimo() {
    return this->stockMinimo;
}
float Producto::getPuntajePromedio() {
    return this->puntajePromedio;
}
Categoria* Producto::getCategoria() {
    return this->categoria;
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
void Producto::setStockMinimo(int stockMinimo) {
    this->stockMinimo = stockMinimo;
}
void Producto::setPuntajePromedio(float puntajePromedio) {
    this->puntajePromedio = puntajePromedio;
}
void Producto::setCategoria(Categoria* categoria) {
    this->categoria = categoria;
}
