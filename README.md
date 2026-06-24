# Punto de Venta - Palaboratorio 2026

Sistema de gestión de punto de venta (POS) desarrollado en **C++20** como proyecto de laboratorio. Implementa un sistema integral para la administración de inventario, ventas, clientes, proveedores y empleados con una interfaz de consola.

---

## Descripción del Proyecto

**Palaboratorio 2026** es un sistema de gestión comercial que permite:

- **Administrar** productos, categorías, empleados, proveedores y la relación proveedor-producto.
- **Gestionar ventas** tanto a clientes registrados como ocasionales, con control de stock y registro detallado.
- **Emitir, cancelar y recibir órdenes de compra** a proveedores, actualizando automáticamente el inventario.
- **Calificar productos** por parte de los clientes, con puntajes del 1 al 5.
- **Consultar reportes** como historial de compras por cliente, monto facturado en un período, unidades vendidas de un producto y stock bajo.
- **Autenticar usuarios** con distintos roles: Administrador, Empleado y Cliente, cada uno con menús y operaciones específicas.

### Roles del Sistema

| Rol | Acceso |
|-----|--------|
| **Administrador** | Gestión completa de productos, categorías, empleados, proveedores. Consultas de calificaciones, stock, facturación y unidades vendidas. |
| **Empleado** | Registro y modificación de clientes, registro de ventas, emisión/cancelación/recepción de órdenes de compra, consultas de historial, stock y facturación. |
| **Cliente** | Visualización del catálogo, detalle de productos, calificación de productos comprados. |

---

## Estructura del Proyecto

```
palaboratorio2026/
├── main.cpp                                  # Punto de entrada del sistema
├── CMakeLists.txt                            # Configuración de compilación (CMake)
├── README.md                                 # Este archivo
│
├── logica/                                   # Capa de lógica de negocio
│   ├── configuracion/
│   │   ├── DatosPorDefecto.h                 # Carga de datos iniciales del sistema
│   │   └── DatosPorDefecto.cpp
│   │
│   ├── controladores/                        # Controladores (orquestan la lógica)
│   │   ├── AdminController.h / .cpp          # CRUD de productos, categorías, empleados, proveedores
│   │   ├── EmpleadoController.h / .cpp       # Gestión de clientes, ventas, órdenes de compra
│   │   ├── VentaController.h / .cpp          # Calificaciones, catálogo para clientes
│   │   └── AuthController.h / .cpp           # Autenticación y manejo de sesiones
│   │
│   └── dominio/                              # Modelos/entidades del dominio
│       ├── Producto.h / .cpp                 # Producto (código, nombre, stock, precio, categoría)
│       ├── Categoria.h / .cpp                # Categoría de producto
│       ├── Empleado.h / .cpp                 # Empleado (nombre, correo, rol)
│       ├── Cliente.h / .cpp                  # Cliente (RUT, datos personales, calificaciones)
│       ├── Proveedor.h / .cpp                # Proveedor (RUT, empresa, productos ofrecidos)
│       ├── ProveedorProducto.h / .cpp        # Asociación proveedor-producto (precio, tiempo entrega)
│       ├── Venta.h / .cpp                    # Venta (fecha, hora, cliente, líneas de detalle)
│       ├── LineaDetalleVenta.h / .cpp        # Línea de detalle de venta (producto, cantidad, precio)
│       ├── OrdenDeCompra.h / .cpp            # Orden de compra (fechas, estado, proveedor, líneas)
│       ├── LineaDetalleCompra.h / .cpp       # Línea de detalle de orden de compra (producto, cantidad)
│       ├── Calificacion.h / .cpp             # Calificación de producto (puntaje, comentario, fecha)
│       ├── DTFecha.h / .cpp                  # Tipo fecha (día, mes, año) con operadores de comparación
│       ├── DTHora.h / .cpp                   # Tipo hora (hora, minuto, segundo)
│       ├── Estado.h                          # Enum: Pendiente, Recibida, Cancelada
│       └── Puntaje.h                         # Enum: UNO..CINCO (1 a 5)
│
├── vista/                                    # Capa de presentación (menús de consola)
│   ├── MenuPrincipal.h / .cpp               # Menú principal con login y redirección por rol
│   ├── MenuAdministrador.h / .cpp           # Menú del administrador
│   ├── MenuEmpleado.h / .cpp                # Menú del empleado
│   └── MenuCliente.h / .cpp                 # Menú del cliente
│
└── tests/
    └── SistemaTest.cpp                       # Pruebas de integración con 43+ escenarios
```

---

## Casos de Uso y Flujo

### 1. Inicialización del Sistema

**Descripción:** Al iniciar la aplicación, se cargan datos por defecto (administrador, empleado y cliente de prueba).

**Flujo:**
1. Se crean las instancias de los controladores (`AdminController`, `EmpleadoController`, `VentaController`).
2. Se inyectan las dependencias circulares entre controladores.
3. Se llama a `cargarDatosPorDefecto()` que crea:
   - Administrador: `admin / admin`
   - Empleado: `empleado / empleado`
   - Cliente: `cliente / cliente` (RUT `123`)
4. Se muestra el menú principal con la pantalla de login.

---

### 2. Autenticación (AuthController)

**Descripción:** Permite iniciar sesión como Administrador, Empleado o Cliente.

**Flujo:**
1. El usuario ingresa su correo y contraseña.
2. Se busca primero en empleados (administradores y empleados), luego en clientes.
3. Si las credenciales son válidas, se activa la sesión con el rol correspondiente.
4. Según el rol, se redirige al menú correspondiente.

**Casos de error:**
- Credenciales incorrectas.
- Correo o contraseña vacíos.
- Sesión cerrada y reabierta con otro usuario.

---

### 3. Gestión de Productos (Administrador)

**Descripción:** El administrador puede crear, modificar, eliminar y consultar productos.

**Flujo:**
1. El administrador accede al menú de productos.
2. Puede seleccionar entre:
   - **Crear producto:** Ingresa código, nombre, descripción, precio, stock, stock mínimo y categoría.
   - **Modificar producto:** Cambia nombre, descripción, precio, categoría y stock mínimo.
   - **Eliminar producto:** Elimina el producto y opcionalmente sus asociaciones con proveedores.
   - **Listar productos:** Muestra todos los productos o filtra por categoría.
   - **Consultar stock bajo:** Lista productos donde stock ≤ stock mínimo, opcionalmente ordenados por criticidad.

**Casos de error:**
- Código de producto duplicado.
- Categoría inexistente.
- Precio negativo (depende de la implementación).
- Producto asociado a proveedores (no se puede eliminar sin flag).

---

### 4. Gestión de Categorías (Administrador)

**Descripción:** El administrador gestiona las categorías de productos.

**Flujo:**
1. Accede al menú de categorías.
2. Opciones:
   - **Crear categoría:** Nombre y descripción.
   - **Modificar categoría:** Cambia nombre y/o descripción.
   - **Eliminar categoría:** Solo si no tiene productos asociados.
   - **Listar categorías.**

**Casos de error:**
- Categoría duplicada.
- Categoría inexistente.
- Nombre extremadamente largo.

---

### 5. Gestión de Empleados (Administrador)

**Descripción:** El administrador da de alta o baja empleados.

**Flujo:**
1. El administrador selecciona la opción de empleados.
2. Opciones:
   - **Crear empleado:** Ingresa nombre, correo, contraseña y selecciona rol (1 = Empleado, 2 = Administrador).
   - **Eliminar empleado.**
   - **Listar empleados.**

**Casos de error:**
- Correo electrónico ya registrado.
- Opción de rol inválida.

---

### 6. Gestión de Proveedores (Administrador)

**Descripción:** El administrador gestiona proveedores y sus productos asociados.

**Flujo:**
1. Accede al menú de proveedores.
2. Opciones:
   - **Crear proveedor:** RUT, nombre de empresa, teléfono, contacto comercial.
   - **Modificar proveedor.**
   - **Eliminar proveedor.**
   - **Asociar producto a proveedor:** Selecciona un producto y define precio de compra pactado y tiempo de entrega estimado.
   - **Actualizar asociación:** Modifica precio y/o tiempo de entrega de una asociación existente.
   - **Listar proveedores.**

**Casos de error:**
- Proveedor con RUT inexistente.
- Producto inexistente.
- Asociación duplicada (se actualiza en lugar de crear duplicado).

---

### 7. Registro de Clientes (Empleado)

**Descripción:** El empleado registra nuevos clientes en el sistema.

**Flujo:**
1. El empleado inicia sesión y accede al menú de cliente.
2. Selecciona "Registrar cliente".
3. Ingresa RUT, nombre, apellido, dirección, correo y contraseña.
4. El sistema valida que el RUT y correo no estén duplicados.

**Casos de error:**
- RUT duplicado (lanza `invalid_argument`).
- Correo duplicado.

---

### 8. Registro de Ventas (Empleado)

**Descripción:** El empleado registra una venta, que puede ser para un cliente registrado u ocasional.

**Flujo:**
1. El empleado selecciona "Registrar venta".
2. Ingresa RUT del cliente (o vacío para cliente ocasional) y si está registrado.
3. Agrega líneas de venta: código de producto y cantidad.
4. El sistema valida:
   - Que el producto exista.
   - Que haya stock suficiente.
   - Que el cliente exista si `clienteRegistrado = true`.
   - Que haya al menos una línea de detalle.
5. Se descuenta el stock de cada producto.
6. Se muestra el total de la venta.

**Casos de error:**
- Stock insuficiente.
- Producto inexistente.
- Venta sin líneas.
- Inconsistencia RUT/flag de cliente registrado.

---

### 9. Historial de Compras y Facturación (Empleado)

**Descripción:** Consultas sobre el historial de compras de un cliente y montos facturados.

**Flujo:**
1. El empleado ingresa el RUT del cliente.
2. Opcionales: rango de fechas (inicio y fin).
3. El sistema devuelve:
   - **Historial de compras:** Fechas, productos, cantidades, subtotales y total de cada venta.
   - **Monto facturado:** Total facturado, cantidad de ventas y monto promedio.
   - **Unidades vendidas:** Total de unidades, cantidad de ventas y precio unitario promedio de un producto.

**Casos de error:**
- Cliente inexistente.
- Rango de fechas inválido (inicio > fin).
- Producto inexistente en consulta de unidades vendidas.

---

### 10. Órdenes de Compra (Empleado)

**Descripción:** El empleado gestiona órdenes de compra a proveedores.

**Subflujos:**

#### 10.1 Emitir Orden de Compra
1. Selecciona proveedor por RUT.
2. Visualiza los productos disponibles de ese proveedor.
3. Agrega líneas con producto y cantidad.
4. El sistema valida que el proveedor exista, los productos estén asociados y haya al menos una línea.
5. Se crea la orden con estado `Pendiente`.

#### 10.2 Cancelar Orden de Compra
1. Lista las órdenes pendientes.
2. Selecciona una por ID.
3. Cambia el estado a `Cancelada`.

#### 10.3 Registrar Recepción de Orden
1. Lista las órdenes pendientes.
2. Selecciona una por ID y visualiza su detalle.
3. Ingresa las cantidades recibidas para cada línea.
4. El sistema actualiza el stock de cada producto y cambia el estado a `Recibida`.

**Casos de error:**
- Proveedor inexistente.
- Producto no asociado al proveedor.
- Orden sin líneas.
- ID de orden inválido.
- Cantidades negativas en recepción.
- Cantidad incorrecta de registros en recepción.

---

### 11. Calificación de Productos (Cliente)

**Descripción:** Los clientes califican los productos que han comprado utilizando el tipo de dominio `Puntaje` (enum class con valores `UNO = 1`, `DOS = 2`, `TRES = 3`, `CUATRO = 4`, `CINCO = 5`).

**Flujo:**
1. El cliente inicia sesión.
2. Visualiza la lista de productos que ha comprado.
3. Selecciona un producto e ingresa:
   - **Puntaje:** del 1 al 5 (se mapea al enum `Puntaje`).
   - **Comentario:** texto opcional.
4. El sistema valida que el producto haya sido comprado por el cliente.
5. Se crea una instancia de `Calificacion` con el `Puntaje`, comentario, fecha/hora actual, el producto y el cliente.
6. Se registra la calificación y se actualiza el puntaje promedio del producto.

**Casos de error:**
- Producto no comprado por el cliente.
- Puntaje fuera de rango (< 1 o > 5, no se puede mapear al enum `Puntaje`).

---

### 12. Consultas del Catálogo (Cliente)

**Descripción:** Los clientes pueden ver el catálogo de productos y su detalle.

**Flujo:**
1. El cliente inicia sesión.
2. Visualiza el listado del catálogo completo.
3. Selecciona un producto para ver detalle: nombre, descripción, categoría, precio, stock, puntaje promedio, cantidad de calificaciones y últimas calificaciones.

**Casos de error:**
- Producto inexistente.

---

### 13. Consultas del Administrador

**Descripción:** Reportes y consultas disponibles para el administrador.

**Consultas disponibles:**
- **Calificaciones de un producto:** Muestra puntaje promedio, cantidad de calificaciones y detalle de cada una (fecha, hora, puntaje, cliente, comentario).
- **Detalle completo de producto:** Incluye datos del producto y lista de proveedores asociados con precio y tiempo de entrega.
- **Stock de productos:** Listado completo o filtrado por categoría.
- **Productos con stock bajo:** Ordenados por criticidad.
- **Monto facturado por cliente en un período.**
- **Unidades vendidas de un producto en un período.**

---

### 14. Datos por Defecto

Al iniciar el sistema, se cargan automáticamente:

| Tipo | Correo | Contraseña | Rol |
|------|--------|------------|-----|
| Administrador | `admin` | `admin` | Administrador |
| Empleado | `empleado` | `empleado` | Empleado |
| Cliente | `cliente` | `cliente` | Cliente |

---

## Compilación y Ejecución

### Requisitos
- CMake ≥ 4.2
- Compilador compatible con C++20

### Compilar
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Ejecutar
```bash
./palaboratorio2026
```

### Ejecutar pruebas
```bash
./sistema_test
```

---

## Tecnologías

- **Lenguaje:** C++20
- **Build system:** CMake
- **Testing:** Sistema de pruebas integrado con asserts y reportes coloreados en consola
- **IDE:** CLion
