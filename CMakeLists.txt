cmake_minimum_required(VERSION 4.2)
project(palaboratorio2026)

set(CMAKE_CXX_STANDARD 14)

include_directories(logica/controladores)
include_directories(logica/dominio)
include_directories(vista)

add_executable(palaboratorio2026
        logica/controladores/AdminController.cpp
        logica/controladores/AdminController.h
        logica/controladores/EmpleadoController.cpp
        logica/controladores/EmpleadoController.h
        logica/controladores/VentaController.cpp
        logica/controladores/VentaController.h
        logica/dominio/Calificacion.cpp
        logica/dominio/Calificacion.h
        logica/dominio/Categoria.cpp
        logica/dominio/Categoria.h
        logica/dominio/Cliente.cpp
        logica/dominio/Cliente.h
        logica/dominio/DTFecha.cpp
        logica/dominio/DTFecha.h
        logica/dominio/DTHora.cpp
        logica/dominio/DTHora.h
        logica/dominio/LineaDetalleCompra.cpp
        logica/dominio/LineaDetalleCompra.h
        logica/dominio/LineaDetalleVenta.cpp
        logica/dominio/LineaDetalleVenta.h
        logica/dominio/OrdenDeCompra.cpp
        logica/dominio/OrdenDeCompra.h
        logica/dominio/Producto.cpp
        logica/dominio/Producto.h
        logica/dominio/Proveedor.cpp
        logica/dominio/Proveedor.h
        logica/dominio/ProveedorProducto.cpp
        logica/dominio/ProveedorProducto.h
        logica/dominio/Puntaje.h
        logica/dominio/Venta.cpp
        logica/dominio/Venta.h
        vista/Menu.cpp
        vista/Menu.h
        vista/MenuAdministrador.cpp
        vista/MenuAdministrador.h
        vista/MenuCliente.cpp
        vista/MenuCliente.h
        vista/MenuEmpleado.cpp
        vista/MenuEmpleado.h
        main.cpp
        logica/dominio/Categoria.h
        logica/dominio/Calificacion.h)
