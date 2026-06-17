#ifndef PALABORATORIO2026_MENUEMPLEADO_H
#define PALABORATORIO2026_MENUEMPLEADO_H

#pragma once
#include "../logica/controladores/EmpleadoController.h"
#include "../logica/controladores/AuthController.h"

class MenuEmpleado {
private:
	EmpleadoController& ctrl;
	AuthController* authCtrl;
public:
	MenuEmpleado(EmpleadoController& controller, AuthController* auth);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUEMPLEADO_H
