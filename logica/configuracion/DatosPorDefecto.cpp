#include "DatosPorDefecto.h"
#include "../controladores/AdminController.h"
#include "../controladores/EmpleadoController.h"

void cargarDatosPorDefecto(AdminController& admin, EmpleadoController& empleado) {
    // ====================================================================
    // 1. USUARIOS DEL SISTEMA
    // ====================================================================
    admin.crearEmpleado("Administrador", "admin", "admin", "Administrador");
    admin.crearEmpleado("Empleado1", "empleado", "empleado", "Empleado");

    // ====================================================================
    // 2. CATEGORIAS DE PRODUCTOS
    // ====================================================================
    admin.crearCategoria("Lacteos", "Productos lacteos y derivados");
    admin.crearCategoria("Bebidas", "Bebidas y refrescos");
    admin.crearCategoria("Snacks", "Snacks y golosinas");
    admin.crearCategoria("Limpieza", "Productos de limpieza para el hogar");
    admin.crearCategoria("Almacen", "Productos de almacen no perecederos");
    admin.crearCategoria("Carniceria", "Carnes y embutidos");
    admin.crearCategoria("Verduleria", "Frutas y verduras");
    admin.crearCategoria("Panaderia", "Pan, facturas y productos de panaderia");

    // ====================================================================
    // 3. PRODUCTOS
    // ====================================================================

    // -- Lacteos --
    admin.crearProducto(100, "Leche Entera", "Leche entera 1L", 45.0f, 50, 10, "Lacteos");
    admin.crearProducto(101, "Yogur Natural", "Yogur natural 200g", 25.0f, 30, 5, "Lacteos");
    admin.crearProducto(102, "Queso Colon", "Queso colon x500g", 120.0f, 20, 5, "Lacteos");
    admin.crearProducto(103, "Manteca", "Manteca 200g", 65.0f, 15, 3, "Lacteos");
    admin.crearProducto(104, "Crema de Leche", "Crema de leche 200ml", 55.0f, 25, 5, "Lacteos");

    // -- Bebidas --
    admin.crearProducto(200, "Gaseosa Cola", "Gaseosa sabor cola 1.5L", 60.0f, 100, 20, "Bebidas");
    admin.crearProducto(201, "Agua Mineral", "Agua mineral sin gas 2L", 30.0f, 80, 15, "Bebidas");
    admin.crearProducto(202, "Jugo de Naranja", "Jugo de naranja natural 1L", 50.0f, 40, 8, "Bebidas");
    admin.crearProducto(203, "Agua con Gas", "Agua mineral con gas 1.5L", 35.0f, 60, 10, "Bebidas");
    admin.crearProducto(204, "Energizante", "Bebida energetica 473ml", 45.0f, 50, 10, "Bebidas");
    admin.crearProducto(205, "Cerveza Rubia", "Cerveza rubia 1L", 80.0f, 40, 10, "Bebidas");

    // -- Snacks --
    admin.crearProducto(300, "Papas Fritas", "Papas fritas 150g", 35.0f, 20, 5, "Snacks");
    admin.crearProducto(301, "Barra de Chocolate", "Chocolate con leche 100g", 40.0f, 15, 3, "Snacks");
    admin.crearProducto(302, "Chizitos", "Snacks de maiz sabor queso 120g", 30.0f, 25, 5, "Snacks");
    admin.crearProducto(303, "Alfajor de Chocolate", "Alfajor relleno de dulce de leche", 25.0f, 30, 5, "Snacks");
    admin.crearProducto(304, "Caramelos Masticables", "Caramelos masticables sabor frutilla 80g", 20.0f, 40, 8, "Snacks");
    admin.crearProducto(305, "Galletitas de Chocolate", "Galletitas rellenas de chocolate 200g", 45.0f, 35, 7, "Snacks");

    // -- Limpieza --
    admin.crearProducto(400, "Detergente", "Detergente lavavajillas 500ml", 55.0f, 30, 5, "Limpieza");
    admin.crearProducto(401, "Lavandina", "Lavandina concentrada 1L", 40.0f, 40, 8, "Limpieza");
    admin.crearProducto(402, "Jabon en Polvo", "Jabon en polvo para ropa 800g", 90.0f, 25, 5, "Limpieza");
    admin.crearProducto(403, "Desinfectante", "Desinfectante aerosol 400ml", 65.0f, 20, 4, "Limpieza");
    admin.crearProducto(404, "Esponja", "Esponja multiuso x2", 30.0f, 50, 10, "Limpieza");

    // -- Almacen --
    admin.crearProducto(500, "Arroz Blanco", "Arroz blanco 1kg", 35.0f, 60, 10, "Almacen");
    admin.crearProducto(501, "Fideos Tallarines", "Fideos tallarines 500g", 25.0f, 70, 15, "Almacen");
    admin.crearProducto(502, "Aceite de Girasol", "Aceite de girasol 1.5L", 70.0f, 40, 8, "Almacen");
    admin.crearProducto(503, "Azucar", "Azucar refinada 1kg", 30.0f, 50, 10, "Almacen");
    admin.crearProducto(504, "Harina de Trigo", "Harina de trigo 0000 1kg", 28.0f, 45, 9, "Almacen");
    admin.crearProducto(505, "Sal Fina", "Sal fina 500g", 15.0f, 80, 15, "Almacen");
    admin.crearProducto(506, "Galletas de Agua", "Galletas de agua x4 paquetes", 55.0f, 35, 7, "Almacen");
    admin.crearProducto(507, "Dulce de Batata", "Dulce de batata 500g", 40.0f, 25, 5, "Almacen");
    admin.crearProducto(508, "Pure de Tomate", "Pure de tomate 500g", 35.0f, 30, 6, "Almacen");
    admin.crearProducto(509, "Atun en Lata", "Atun al natural 170g", 45.0f, 50, 10, "Almacen");

    // -- Carniceria --
    admin.crearProducto(600, "Carne Picada", "Carne picada especial 1kg", 250.0f, 15, 5, "Carniceria");
    admin.crearProducto(601, "Pollo Entero", "Pollo fresco 1kg aprox", 180.0f, 10, 3, "Carniceria");
    admin.crearProducto(602, "Chorizo", "Chorizo artesanal x1kg", 200.0f, 20, 5, "Carniceria");
    admin.crearProducto(603, "Milanesa de Carne", "Milanesa de carne 500g", 160.0f, 12, 4, "Carniceria");
    admin.crearProducto(604, "Pechuga de Pollo", "Pechuga de pollo 500g", 140.0f, 15, 5, "Carniceria");

    // ====================================================================
    // 4. PROVEEDORES
    // ====================================================================
    admin.crearProveedor("1000001", "Lacteos S.A.", "24000001", "Juan Perez");
    admin.crearProveedor("1000002", "Bebidas del Uruguay", "24000002", "Maria Rodriguez");
    admin.crearProveedor("1000003", "Snacks Mundial", "24000003", "Carlos Lopez");
    admin.crearProveedor("1000004", "Limpieza Total", "24000004", "Ana Martinez");
    admin.crearProveedor("1000005", "Distribuidora Almacen", "24000005", "Pedro Garcia");
    admin.crearProveedor("1000006", "Carnes del Sur", "24000006", "Laura Fernandez");

    // ====================================================================
    // 5. ASOCIACIONES PROVEEDOR-PRODUCTO
    // ====================================================================
    // Lacteos S.A. -> productos lacteos
    admin.gestionarAsociacionProveedorProducto("1000001", 100, 30, 2);
    admin.gestionarAsociacionProveedorProducto("1000001", 101, 15, 2);
    admin.gestionarAsociacionProveedorProducto("1000001", 102, 85, 3);
    admin.gestionarAsociacionProveedorProducto("1000001", 103, 42, 2);
    admin.gestionarAsociacionProveedorProducto("1000001", 104, 35, 2);

    // Bebidas del Uruguay -> bebidas
    admin.gestionarAsociacionProveedorProducto("1000002", 200, 40, 3);
    admin.gestionarAsociacionProveedorProducto("1000002", 201, 20, 3);
    admin.gestionarAsociacionProveedorProducto("1000002", 202, 32, 2);
    admin.gestionarAsociacionProveedorProducto("1000002", 203, 22, 3);
    admin.gestionarAsociacionProveedorProducto("1000002", 204, 30, 2);
    admin.gestionarAsociacionProveedorProducto("1000002", 205, 55, 3);

    // Snacks Mundial -> snacks
    admin.gestionarAsociacionProveedorProducto("1000003", 300, 22, 4);
    admin.gestionarAsociacionProveedorProducto("1000003", 301, 25, 4);
    admin.gestionarAsociacionProveedorProducto("1000003", 302, 18, 3);
    admin.gestionarAsociacionProveedorProducto("1000003", 303, 15, 2);
    admin.gestionarAsociacionProveedorProducto("1000003", 304, 12, 2);
    admin.gestionarAsociacionProveedorProducto("1000003", 305, 28, 3);

    // Limpieza Total -> limpieza
    admin.gestionarAsociacionProveedorProducto("1000004", 400, 35, 2);
    admin.gestionarAsociacionProveedorProducto("1000004", 401, 25, 2);
    admin.gestionarAsociacionProveedorProducto("1000004", 402, 60, 3);
    admin.gestionarAsociacionProveedorProducto("1000004", 403, 42, 2);
    admin.gestionarAsociacionProveedorProducto("1000004", 404, 18, 1);

    // Distribuidora Almacen -> almacen
    admin.gestionarAsociacionProveedorProducto("1000005", 500, 22, 3);
    admin.gestionarAsociacionProveedorProducto("1000005", 501, 15, 2);
    admin.gestionarAsociacionProveedorProducto("1000005", 502, 45, 3);
    admin.gestionarAsociacionProveedorProducto("1000005", 503, 18, 2);
    admin.gestionarAsociacionProveedorProducto("1000005", 504, 17, 2);
    admin.gestionarAsociacionProveedorProducto("1000005", 505, 8, 1);
    admin.gestionarAsociacionProveedorProducto("1000005", 506, 32, 2);
    admin.gestionarAsociacionProveedorProducto("1000005", 507, 25, 3);
    admin.gestionarAsociacionProveedorProducto("1000005", 508, 22, 2);
    admin.gestionarAsociacionProveedorProducto("1000005", 509, 30, 2);

    // Carnes del Sur -> carniceria
    admin.gestionarAsociacionProveedorProducto("1000006", 600, 180, 2);
    admin.gestionarAsociacionProveedorProducto("1000006", 601, 120, 2);
    admin.gestionarAsociacionProveedorProducto("1000006", 602, 140, 3);
    admin.gestionarAsociacionProveedorProducto("1000006", 603, 110, 2);
    admin.gestionarAsociacionProveedorProducto("1000006", 604, 95, 2);

    // ====================================================================
    // 6. CLIENTES REGISTRADOS
    // ====================================================================
    empleado.registrarCliente("123", "Cliente", "Test", "Direccion 123", "cliente", "cliente");
    empleado.registrarCliente("456", "Juan", "Garcia", "Av. Principal 1234", "juan@mail.com", "pass123");
    empleado.registrarCliente("789", "Maria", "Lopez", "Calle Secundaria 567", "maria@mail.com", "pass456");
    empleado.registrarCliente("111", "Pedro", "Rodriguez", "Av. Central 890", "pedro@mail.com", "pass789");
    empleado.registrarCliente("222", "Ana", "Martinez", "Boulevard Este 123", "ana@mail.com", "pass012");
    empleado.registrarCliente("333", "Carlos", "Gonzalez", "Ruta Nacional km 45", "carlos@mail.com", "pass345");
    empleado.registrarCliente("444", "Lucia", "Fernandez", "Calle Los Pinos 789", "lucia@mail.com", "pass678");
    empleado.registrarCliente("555", "Martin", "Pereira", "Av. Italia 2345", "martin@mail.com", "pass901");
}
