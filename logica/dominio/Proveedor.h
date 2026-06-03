#ifndef PROVEEDOR_H
#define PROVEEDOR_H
#include <string>
#include <vector>
#include "OrdenDeCompra.h"

using namespace std;
class Proveedor{
    private:
        string rut, empresa, telefono, contactoComercial;
        vector<OrdenDeCompra*> ordenDeCompra;
    public:
    Proveedor(string rut, string empresa, string telefono, string contactoComercial);

    string getNombre();
    string getEmpresa();
    string getTelefono();
    string contactoComercial();

    void setNombre(string nombre);
    void setEmpresa(string empresa);
    void setTelefono(string telefono);
    void setContactoComercial(string contactoComercial);
};
#endif