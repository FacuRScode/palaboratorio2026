#ifndef PALABORATORIO2026_MENUEMPLEADO_H
#define PALABORATORIO2026_MENUEMPLEADO_H

#pragma once
#include "../logica/controladores/EmpleadoController.h"
#include "../logica/controladores/AuthController.h"
#include "../logica/controladores/VentaController.h"

class MenuEmpleado {
private:
	EmpleadoController& ctrl;
	AuthController* authCtrl;
	VentaController* ventaCtrl;

	void consultarCalificacionesProducto();
	void consultarStockProductos();
	void consultarProductosStockBajo();
	void consultarMontoFacturadoCliente();
	void consultarUnidadesVendidasProducto();
	void consultarInfoDetalladaProducto();
public:
	MenuEmpleado(EmpleadoController& controller, AuthController* auth, VentaController* ventas);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUEMPLEADO_H
