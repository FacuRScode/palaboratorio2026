//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_MENUADMINISTRADOR_H
#define PALABORATORIO2026_MENUADMINISTRADOR_H

#include <string>
#include <vector>
#include <iostream>
#include "../logica/controladores/AdminController.h"

class MenuAdministrador {
private:
	AdminController& ctrl;

	void menuProductos();
	void menuCategorias();
	void menuEmpleados();
	void menuProveedores();

public:
	explicit MenuAdministrador(AdminController& controller);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUADMINISTRADOR_H


