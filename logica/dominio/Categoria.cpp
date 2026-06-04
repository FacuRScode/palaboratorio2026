#include "Categoria.h"
#include <algorithm>

Categoria::Categoria(string nombre, string descripcion)
	: nombre(nombre), descripcion(descripcion) {}

string Categoria::getNombre(){
	return this->nombre;
}

string Categoria::getDescripcion(){
	return this->descripcion;
}

void Categoria::setNombre(string nombre){
	this->nombre = nombre;
}

void Categoria::setDescripcion(string descripcion){
	this->descripcion = descripcion;
}

void Categoria::addProducto(Producto* producto){
	if(producto != nullptr){
		this->productos.push_back(producto);
	}
}

bool Categoria::removeProducto(Producto* producto){
	auto it = std::find(this->productos.begin(), this->productos.end(), producto);
	if(it != this->productos.end()){
		this->productos.erase(it);
		return true;
	}
	return false;
}

const vector<Producto*>& Categoria::getProductos() const{
	return this->productos;
}


