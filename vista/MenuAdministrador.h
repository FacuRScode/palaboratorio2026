#ifndef PALABORATORIO2026_MENUADMINISTRADOR_H
#define PALABORATORIO2026_MENUADMINISTRADOR_H

#include <string>
#include <vector>
#include <iostream>
#include "../logica/controladores/AdminController.h"
#include "../logica/controladores/AuthController.h"
#include "../logica/controladores/EmpleadoController.h"
#include "../logica/controladores/VentaController.h"

class MenuAdministrador {
private:
	AdminController& ctrl;
	AuthController* authCtrl;
	EmpleadoController* empleadoCtrl;
	VentaController* ventaCtrl;

	void menuProductos();
	void menuCategorias();
	void menuEmpleados();
	void menuProveedores();
	void consultarCalificacionesProducto();
	void consultarStockProductos();
	void consultarProductosStockBajo();
	void consultarMontoFacturadoCliente();
	void consultarUnidadesVendidasProducto();
	void consultarInfoDetalladaProducto();

public:
	MenuAdministrador(AdminController& controller, AuthController* auth,
					  EmpleadoController* empleados = nullptr,
					  VentaController* ventas = nullptr);
	void mostrar();
};

#endif //PALABORATORIO2026_MENUADMINISTRADOR_H
