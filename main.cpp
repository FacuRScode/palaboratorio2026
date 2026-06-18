#include <iostream>
#include "logica/controladores/AdminController.h"
#include "logica/controladores/EmpleadoController.h"
#include "logica/controladores/VentaController.h"
#include "vista/MenuPrincipal.h"

int main() {
    AdminController admin;
    EmpleadoController empleado(&admin);
    VentaController ventas(&admin);

    // cuentas por defecto
    admin.crearEmpleado("Administrador", "admin", "admin", "Administrador");
    admin.crearEmpleado("Empleado1", "empleado", "empleado", "Empleado");
    empleado.registrarCliente("123", "Cliente", "Test", "Direccion 123", "cliente", "cliente");

    /* quitar el comentario para dejar de usar los usuarios hardcodeados
    if (admin.listarEmpleados().empty()) {
        // Asistente de primer inicio
        cout << "--- Primer inicio - Crear administrador ---" << endl;
        string nombre, correo, contrasena;
        cout << "Nombre: "; getline(cin, nombre);
        cout << "Correo: "; cin >> correo;
        cout << "Contrasena: "; cin >> contrasena;
        admin.crearEmpleado(nombre, correo, contrasena, "Administrador");
        cout << "Administrador creado exitosamente." << endl;
    }
    */

    MenuPrincipal menu(admin, empleado, ventas);
    menu.mostrar();

    return 0;
}
