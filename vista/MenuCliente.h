//
// Created by facun on 2/6/2026.
//

#ifndef PALABORATORIO2026_MENUCLIENTE_H
#define PALABORATORIO2026_MENUCLIENTE_H

#pragma once
#include "../logica/controladores/VentaController.h"

class MenuCliente {
private:
	VentaController& ctrl;
public:
	explicit MenuCliente(VentaController& controller);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUCLIENTE_H
