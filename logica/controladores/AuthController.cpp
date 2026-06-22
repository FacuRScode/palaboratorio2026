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
	if (empleadoCtrl != nullptr) {
		Cliente* cliente = empleadoCtrl->buscarClientePorCorreo(correo);
		if (cliente != nullptr && cliente->getContrasena() == contrasena) {
			sesionActual = {true, cliente->getCorreo(), cliente->getNombre() + " " + cliente->getApellido(), "Cliente"};
			return true;
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
