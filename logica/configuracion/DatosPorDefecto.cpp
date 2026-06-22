#include "DatosPorDefecto.h"
#include "../controladores/AdminController.h"
#include "../controladores/EmpleadoController.h"

void cargarDatosPorDefecto(AdminController& admin, EmpleadoController& empleado) {
    admin.crearEmpleado("Administrador", "admin", "admin", "Administrador");
    admin.crearEmpleado("Empleado1", "empleado", "empleado", "Empleado");
    empleado.registrarCliente("123", "Cliente", "Test", "Direccion 123", "cliente", "cliente");
}
