//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_MENU_H
#define PALABORATORIO2026_MENU_H

#include "../logica/controladores/AdminController.h"
#include "../logica/controladores/EmpleadoController.h"
#include "../logica/controladores/VentaController.h"
#include "MenuAdministrador.h"
#include "MenuEmpleado.h"
#include "MenuCliente.h"

class MenuPrincipal {
private:
	MenuAdministrador menuAdmin;
	MenuEmpleado menuEmpleado;
	MenuCliente menuCliente;
public:
	MenuPrincipal(AdminController& admin, EmpleadoController& empleado, VentaController& ventas);
	void mostrar();
};

#endif //PALABORATORIO2026_MENU_H
