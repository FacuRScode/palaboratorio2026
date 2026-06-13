#ifndef PROVEEDOR_H
#define PROVEEDOR_H
#include <string>
#include <vector>
#include "ProveedorProducto.h"

using namespace std;
class Proveedor{
private:
    string rut, empresa, telefono, contactoComercial;
    vector<ProveedorProducto*> productosOfrecidos;
public:
    Proveedor(string rut, string empresa, string telefono, string contactoComercial);

    string getRut();
    string getEmpresa();
    string getTelefono();
    string getContactoComercial();
    const vector<ProveedorProducto*>& getProductosOfrecidos() const;

    void setRut(string rut);
    void setEmpresa(string empresa);
    void setTelefono(string telefono);
    void setContactoComercial(string contactoComercial);
    void addProveedorProducto(ProveedorProducto* productosOfrecidos);
};
#endif