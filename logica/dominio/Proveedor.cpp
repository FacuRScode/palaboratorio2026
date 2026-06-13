#include "Proveedor.h"

using namespace std;
Proveedor::Proveedor(string rut, string empresa, string telefono, string contactoComercial) {
    this->rut = rut;
    this->empresa = empresa;
    this->telefono = telefono;
    this->contactoComercial = contactoComercial;
}
string Proveedor::getRut() {
    return this->rut;
}
string Proveedor::getEmpresa() {
    return this->empresa;
}
string Proveedor::getTelefono() {
    return this->telefono;
}
string Proveedor::getContactoComercial() {
    return this->contactoComercial;
}
const vector<ProveedorProducto*>& Proveedor::getProductosOfrecidos() const {
    return this->productosOfrecidos;
}
void Proveedor::setRut(string rut) {
    this->rut = rut;
}
void Proveedor::setEmpresa(string empresa) {
    this->empresa = empresa;
}
void Proveedor::setTelefono(string telefono) {
    this->telefono = telefono;
}
void Proveedor::setContactoComercial(string contactoComercial) {
    this->contactoComercial = contactoComercial;
}
void Proveedor::addProveedorProducto(ProveedorProducto* productosOfrecidos) {
    this->productosOfrecidos.push_back(productosOfrecidos);
}