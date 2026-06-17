#ifndef PALABORATORIO2026_MENU_H
#define PALABORATORIO2026_MENU_H

#include "../logica/controladores/AdminController.h"
#include "../logica/controladores/EmpleadoController.h"
#include "../logica/controladores/VentaController.h"
#include "../logica/controladores/AuthController.h"
#include "MenuAdministrador.h"
#include "MenuEmpleado.h"
#include "MenuCliente.h"

class MenuPrincipal {
private:
	AdminController& adminCtrl;
	EmpleadoController& empleadoCtrl;
	VentaController& ventaCtrl;
	AuthController authCtrl;
	MenuAdministrador menuAdmin;
	MenuEmpleado menuEmpleado;
	MenuCliente menuCliente;

	void mostrarLogin();
	void mostrarMenuSegunRol();

public:
	MenuPrincipal(AdminController& admin, EmpleadoController& empleado, VentaController& ventas);
	void mostrar();
};

#endif //PALABORATORIO2026_MENU_H
