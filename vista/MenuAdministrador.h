#ifndef PALABORATORIO2026_MENUADMINISTRADOR_H
#define PALABORATORIO2026_MENUADMINISTRADOR_H

#include <string>
#include <vector>
#include <iostream>
#include "../logica/controladores/AdminController.h"
#include "../logica/controladores/AuthController.h"

class MenuAdministrador {
private:
	AdminController& ctrl;
	AuthController* authCtrl;

	void menuProductos();
	void menuCategorias();
	void menuEmpleados();
	void menuProveedores();

public:
	MenuAdministrador(AdminController& controller, AuthController* auth);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUADMINISTRADOR_H
