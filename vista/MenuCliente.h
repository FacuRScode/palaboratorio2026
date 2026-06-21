#ifndef PALABORATORIO2026_MENUCLIENTE_H
#define PALABORATORIO2026_MENUCLIENTE_H

#pragma once
#include "../logica/controladores/VentaController.h"
#include "../logica/controladores/AuthController.h"
#include "../logica/controladores/EmpleadoController.h"

class MenuCliente {
private:
	VentaController& ctrl;
	AuthController* authCtrl;
	EmpleadoController* empleadoCtrl;

	void calificarProducto();
	void consultarInfoDetalladaProducto();
public:
	MenuCliente(VentaController& controller, AuthController* auth, EmpleadoController* empleado);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUCLIENTE_H
