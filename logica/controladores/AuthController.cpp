#include "AuthController.h"

AuthController::AuthController(AdminController* admin, EmpleadoController* empleado)
    : adminCtrl(admin), empleadoCtrl(empleado) {
    sesionActual = {false, "", "", ""};
}

bool AuthController::iniciarSesion(const string& correo, const string& contrasena) {
    // 1. Buscar entre empleados (incluye administradores)
    Empleado* empleado = adminCtrl->buscarEmpleado(correo);
    if (empleado != nullptr && empleado->getContrasena() == contrasena) {
        sesionActual = {true, empleado->getCorreo(), empleado->getNombre(), empleado->getRol()};
        return true;
    }

    // 2. Buscar entre clientes registrados
    // Necesitamos iterar los clientes del EmpleadoController para buscar por correo
    if (empleadoCtrl != nullptr) {
        vector<Cliente*> clientes = empleadoCtrl->listarClientes();
        for (Cliente* c : clientes) {
            if (c != nullptr && c->getCorreo() == correo && c->getContrasena() == contrasena) {
                sesionActual = {true, c->getCorreo(), c->getNombre() + " " + c->getApellido(), "Cliente"};
                return true;
            }
        }
    }

    // Credenciales inválidas
    return false;
}

void AuthController::cerrarSesion() {
    sesionActual = {false, "", "", ""};
}

Sesion AuthController::getSesionActual() const {
    return sesionActual;
}

bool AuthController::haySesionActiva() const {
    return sesionActual.activa;
}
