//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_MENUEMPLEADO_H
#define PALABORATORIO2026_MENUEMPLEADO_H

#pragma once
#include "../logica/controladores/EmpleadoController.h"

class MenuEmpleado {
private:
	EmpleadoController& ctrl;
public:
	explicit MenuEmpleado(EmpleadoController& controller);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUEMPLEADO_H
