#ifndef PALABORATORIO2026_MENUADMINISTRADOR_H
#define PALABORATORIO2026_MENUADMINISTRADOR_H

#include <string>
#include <vector>
#include <iostream>
#include "../logica/controladores/AdminController.h"
#include "../logica/controladores/AuthController.h"
#include "../logica/controladores/EmpleadoController.h"

class MenuAdministrador {
private:
	AdminController& ctrl;
	AuthController* authCtrl;
	EmpleadoController* empleadoCtrl;

	void menuProductos();
	void menuCategorias();
	void menuEmpleados();
	void menuProveedores();

public:
	MenuAdministrador(AdminController& controller, AuthController* auth,
					  EmpleadoController* empleados = nullptr);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUADMINISTRADOR_H
