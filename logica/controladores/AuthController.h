#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <string>
#include "AdminController.h"
#include "EmpleadoController.h"

using namespace std;

struct Sesion {
    bool activa;
    string correo;
    string nombre;
    string rol; // "Administrador", "Empleado", "Cliente"
};

class AuthController {
private:
    Sesion sesionActual;
    AdminController* adminCtrl;
    EmpleadoController* empleadoCtrl;

public:
    AuthController(AdminController* admin, EmpleadoController* empleado);

    // Iniciar sesión con correo y contraseña
    // Retorna true si las credenciales son correctas
    bool iniciarSesion(const string& correo, const string& contrasena);

    // Cerrar sesión
    void cerrarSesion();

    // Obtener sesión actual
    Sesion getSesionActual() const;

    // Verificar si hay sesión activa
    bool haySesionActiva() const;
};

#endif
