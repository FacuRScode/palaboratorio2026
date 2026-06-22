#include <iostream>
#include "logica/controladores/AdminController.h"
#include "logica/controladores/EmpleadoController.h"
#include "logica/controladores/VentaController.h"
#include "logica/configuracion/DatosPorDefecto.h"
#include "vista/MenuPrincipal.h"

int main() {
    AdminController admin;
    EmpleadoController empleado(&admin);
    admin.setEmpleadoController(&empleado);
    VentaController ventas(&admin, &empleado);

    cargarDatosPorDefecto(admin, empleado);

    MenuPrincipal menu(admin, empleado, ventas);
    menu.mostrar();

    return 0;
}
