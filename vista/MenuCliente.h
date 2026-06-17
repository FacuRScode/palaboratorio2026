#ifndef PALABORATORIO2026_MENUCLIENTE_H
#define PALABORATORIO2026_MENUCLIENTE_H

#pragma once
#include "../logica/controladores/VentaController.h"
#include "../logica/controladores/AuthController.h"

class MenuCliente {
private:
	VentaController& ctrl;
	AuthController* authCtrl;
public:
	MenuCliente(VentaController& controller, AuthController* auth);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUCLIENTE_H
