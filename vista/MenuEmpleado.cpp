#include "MenuEmpleado.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <ctime>

using namespace std;

MenuEmpleado::MenuEmpleado(EmpleadoController& controller, AuthController* auth, VentaController* ventas)
	: ctrl(controller), authCtrl(auth), ventaCtrl(ventas) {}

void MenuEmpleado::mostrar() {
	while (true) {
		auto adminCtrl = AdminController::getInstanciaAdmin();
		auto empleadoCtrl = EmpleadoController::getInstanciaEmpleado();
		cout << "\n--- Menu Empleado ---\n";
		if (authCtrl && authCtrl->haySesionActiva()) {
			Sesion s = authCtrl->getSesionActual();
			cout << "Usuario: " << s.nombre << " (" << s.rol << ")\n";
		}
		cout << "1. Registrar cliente\n";
		cout << "2. Modificar cliente\n";
		cout << "3. Registrar venta\n";
		cout << "4. Historial de compras de cliente\n";
		cout << "5. Emitir orden de compra\n";
		cout << "6. Cancelar orden de compra\n";
		cout << "7. Registrar recepcion de orden de compra\n";
		cout << "8. Consultar calificaciones de producto\n";
		cout << "9. Consultar stock actual de productos\n";
		cout << "10. Consultar productos con stock bajo minimo\n";
		cout << "11. Consultar monto facturado a cliente\n";
		cout << "12. Consultar unidades vendidas de un producto\n";
		cout << "13. Consultar informacion detallada de un producto\n";
		cout << "0. Cerrar sesion\n";
		int op;
		cout << "Seleccione una opcion: ";
		cin >> op;
		if (op == 0) {
			if (authCtrl) authCtrl->cerrarSesion();
			cout << "Sesion cerrada." << endl;
			return;
		} else if (op == 1) {
			string rut, nombre, apellido, direccion, correo, contrasena;
			char respuesta;
			cout << "RUT: "; cin >> rut;
			cout << "Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre);
			cout << "Apellido: "; getline(cin, apellido);
			cout << "Direccion: "; getline(cin, direccion);
			cout << "Correo: "; getline(cin, correo);
			cout << "Contrasena: "; getline(cin, contrasena);
			cout << "\n Resumen del registro: " << endl;
			cout << "RUT: " << rut << endl;
			cout << "Nombre Completo: " << nombre << " " << apellido << endl;
			cout << "Direccion: " << direccion << endl;
			cout << "Correo: " << correo << endl;
			cout << "Contrasena: " << contrasena <<endl;
			cout << "¿Desea confirmar el registro? (s/n): "; cin >> respuesta;
			if(respuesta == 's'){
				Cliente* nuevo = empleadoCtrl->registrarCliente(rut, nombre, apellido, direccion, correo, contrasena);
				if (nuevo != nullptr) {
					cout << "Cliente registrado con exito."<<endl;
				} else {
					cout << "Error: El RUT o el correo ya estan registrados en el sistema."<<endl;
				}
			}
			else{
				cout << "Registro cancelado."<<endl;
			}
		} else if (op == 2) {
			string rut, nuevoNombre, nuevoApellido, nuevaDireccion, nuevoCorreo;
            char respuesta;
            Cliente* cliente = nullptr;
			while(true){
				cout << "RUT: "; cin >> rut;
				cliente = empleadoCtrl->buscarCliente(rut);
				if(cliente == nullptr){
					cout<<"Error: No existe un cliente con ese RUT."<<endl;
					cout<<"¿Desea intentarlo con otro RUT? (s/n): "; cin >> respuesta;
					if (respuesta == 'n') {
            			cout << "Modificacion cancelada." << endl;
            			return;
						}
       			 cout << endl;
   				} else {
      				break;
				}
			}
			cout << "\n Datos actuales del cliente:" << endl;
   			cout << "Nombre Completo: " << cliente->getNombre() << " " << cliente->getApellido() << endl;
    		cout << "Direccion: " << cliente->getDireccion() << endl;
    		cout << "Correo: " << cliente->getCorreo() << endl;

			cout << "Nuevo Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nuevoNombre);
    		cout << "Nuevo Apellido: "; getline(cin, nuevoApellido);
			cout << "Nueva Direccion: "; getline(cin, nuevaDireccion);
			cout << "Correo: "; getline(cin, nuevoCorreo);
			cout << "\n Resumen de la edicion: " << endl;
			cout << "Nombre Completo: " << nuevoNombre << " " << nuevoApellido << endl;
			cout << "Direccion: " << nuevaDireccion << endl;
			cout << "Correo: " << nuevoCorreo << endl;
			cout << "¿Desea confirmar la modificacion? (s/n): "; cin >> respuesta;
            if(respuesta == 's'){
                if (empleadoCtrl->modificaCliente(cliente, nuevoNombre, nuevoApellido, nuevaDireccion, nuevoCorreo)) {
                    cout << "Cliente modificado con exito." << endl;
                } else {
                    cout << "Error: El correo ya pertenece a otro cliente. Modificacion cancelada." << endl;
                }
            } else {
                cout << "Modificacion cancelada. Se conservan los datos originales." << endl;
            }
		} else if (op == 3) {
		    string rut = "";
            char respuesta;
            int cantidad, codigoProducto;
            Cliente* cliente = nullptr;
            Producto* producto = nullptr;
            Venta* venta = nullptr;

            // Obtener fecha y hora actual del sistema
            time_t t = time(nullptr);
            tm* now = localtime(&t);
            DTFecha fechaSistema(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
            DTHora horaSistema(now->tm_hour, now->tm_min, now->tm_sec);

            cout << "¿El cliente esta registrado? (s/n): "; cin >> respuesta;
            if (respuesta == 's' || respuesta == 'S') {
                while (true) {
                    cout << "RUT: "; cin >> rut;
                    cliente = empleadoCtrl->buscarCliente(rut);
                    if (cliente == nullptr) {
                        cout << "Error: No existe un cliente con ese RUT." << endl;
                        cout << "¿Desea intentarlo con otro RUT? (s/n): "; cin >> respuesta;
                        if (respuesta == 'n' || respuesta == 'N') {
                            cout << "Registro cancelado." << endl;
                            return;
                        }
                        cout << endl;
                    } else {
                        break;
                    }
                }
                venta = empleadoCtrl->crearVenta(rut, fechaSistema, horaSistema);
            } else {
                // Cliente ocasional
                venta = empleadoCtrl->crearVentaOcasional(fechaSistema, horaSistema);
            }

            if (venta == nullptr) {
                cout << "Error al crear la venta." << endl;
                return;
            }

            do {
                while (true) {
                    cout << "\nIngrese codigo del producto: "; cin >> codigoProducto;
                    producto = adminCtrl->buscarProducto(codigoProducto);
                    if (producto == nullptr) {
                        cout << "Error: No existe un producto con ese codigo." << endl;
                        cout << "¿Desea intentarlo con otro codigo? (s/n): "; cin >> respuesta;
                        if (respuesta == 'n' || respuesta == 'N') {
                            cout << "Registro cancelado." << endl;
                            delete venta;
			                return;
                        }
                        cout << endl;
                    } else {
                        break;
                    }
                }

                cout << "Stock disponible: " << producto->getStock() << endl;
                cout << "Precio unitario: $" << producto->getPrecioVentaActual() << endl;

                cout << "Ingrese la cantidad: "; cin >> cantidad;

                if (empleadoCtrl->agregarLineaAVenta(venta, codigoProducto, cantidad)) {
                    cout << "Linea agregada con exito." << endl;
                } else {
                    cout << "Error: No hay stock suficiente o el producto no existe, linea no agregada." << endl;
                }

                cout << "¿Desea seguir ingresando lineas de detalle? (s/n): "; cin >> respuesta;
            } while (respuesta == 's' || respuesta == 'S');

            // Actualizar fecha/hora al momento de confirmar
            t = time(nullptr);
            now = localtime(&t);
            venta->setFecha(DTFecha(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900));
            venta->setHora(DTHora(now->tm_hour, now->tm_min, now->tm_sec));

            string nombreCliente = (cliente != nullptr) ? (cliente->getNombre() + " " + cliente->getApellido()) : "Ocasional";
            cout << "\nResumen de venta: " << endl;
            cout << " Cliente: " << (cliente != nullptr ? rut : "Ocasional") << " (" << nombreCliente << ")" << endl;
            cout << " Total: $" << venta->calcularTotal() << endl;
            cout << " Fecha/Hora: " << venta->getFecha().getDia() << "/" << venta->getFecha().getMes() << "/" << venta->getFecha().getAnio()
			<< " " << venta->getHora().getHora() << ":" << venta->getHora().getMinuto() << ":" << venta->getHora().getSegundo() << endl;

            cout << "¿Desea confirmar esta venta? (s/n): "; cin >> respuesta;

            if (respuesta == 's' || respuesta == 'S') {
                empleadoCtrl->confirmarVenta(venta);
                cout << "Venta registrada con exito." << endl;
            } else {
                cout << "Venta cancelada." << endl;
                delete venta;
            }

		}else if (op == 4){
			string rut;
			char respuesta;
			Cliente* cliente = nullptr;
			while(true){
				cout << "RUT: "; cin >> rut;
				cliente = empleadoCtrl->buscarCliente(rut);
				if(cliente == nullptr){
					cout<<"Error: No existe un cliente con ese RUT."<<endl;
					cout<<"¿Desea intentarlo con otro RUT? (s/n): "; cin >> respuesta;
					if (respuesta == 'n') {
            			cout << "Operacion cancelada." << endl;
            			return;
						}
       			 cout << endl;
   				} else {
					break;
				}
			}
			auto ventas = empleadoCtrl->listarVentasPorCliente(rut);
			if (ventas.empty()) {
            cout << "El cliente " << cliente->getNombre() << " no registra compras en el sistema." << endl;
            return;
        }

        sort(ventas.begin(), ventas.end(), [](Venta* a, Venta* b) {
   			DTFecha fa = a->getFecha();
    		DTFecha fb = b->getFecha();
    		DTHora ha = a->getHora();
    		DTHora hb = b->getHora();

		    if (fa.getAnio() != fb.getAnio()) {
    		    return fa.getAnio() > fb.getAnio();
    		}

    		if (fa.getMes() != fb.getMes()) {
        		return fa.getMes() > fb.getMes();
    		}

    		if (fa.getDia() != fb.getDia()) {
        		return fa.getDia() > fb.getDia();
    		}

    		if (ha.getHora() != hb.getHora()) {
        		return ha.getHora() > hb.getHora();
    		}

    		if (ha.getMinuto() != hb.getMinuto()) {
        		return ha.getMinuto() > hb.getMinuto();
    		}

		    return ha.getSegundo() > hb.getSegundo();
		});

        cout << "\nHistorial de compras: " << cliente->getNombre() << " " << cliente->getApellido() << endl;
		cout << "------------------------------------------------" << endl;

		for (size_t i = 0; i < ventas.size(); ++i) {
    		cout << (i + 1) << ") ";
    		cout << "Fecha: " << ventas[i]->getFecha().getDia() << "/"
        		 << ventas[i]->getFecha().getMes() << "/"
        		 << ventas[i]->getFecha().getAnio();
    		cout << " Hora: " << ventas[i]->getHora().getHora() << ":"
        		 << ventas[i]->getHora().getMinuto() << ":"
        		 << ventas[i]->getHora().getSegundo();
    		cout << " - " << ventas[i]->getDetalle().size() << " lineas - Total: $"
        		 << ventas[i]->calcularTotal() << endl;
		}
        int seleccion;
        cout << "\nSeleccione el numero de una venta para ver su detalle (0 para salir): ";
        cin >> seleccion;

        if (seleccion > 0 && seleccion <= static_cast<int>(ventas.size())) {
            Venta* ventaSeleccionada = ventas[seleccion - 1];

            cout << "\nDetalle de la venta N° " << seleccion << endl;
            cout << "Fecha: " << ventaSeleccionada->getFecha().getDia() << "/"
            	 << ventaSeleccionada->getFecha().getMes() << "/"
         	 	 << ventaSeleccionada->getFecha().getAnio();
            cout << " Hora: " << ventaSeleccionada->getHora().getHora() << ":"
            	 << ventaSeleccionada->getHora().getMinuto() << ":"
            	 << ventaSeleccionada->getHora().getSegundo() << endl;

            for (auto linea : ventaSeleccionada->getDetalle()) {
                Producto* prod = linea->getProducto();
                double precioAplicado = linea->getPrecioUnitario();
                int cant = linea->getCantidad();
                double subtotal = cant * precioAplicado;

				string nombreProd = prod != nullptr ? prod->getNombre() : "(producto eliminado)";
				string strCodigo = prod != nullptr ? to_string(prod->getCodigo()) : "-";

                cout << "  Producto: " << nombreProd << " (codigo=" << strCodigo << ")" << endl;
                cout << "  Cantidad: " << cant << " | P. Unitario: $" << precioAplicado
                     << " | Subtotal: $" << subtotal << endl;
            }
            cout << "  -----------------------------------------" << endl;
            cout << "  Total compra: $" << ventaSeleccionada->calcularTotal() << endl;
        } else {
            cout << "Regresando al menu de opciones..." << endl;
        }
		} else if (op == 5) {
			int cantidad, codigoProducto;
			string rutProveedor;
			char respuesta;
			Producto* producto = nullptr;
			Proveedor* proveedor = nullptr;
			OrdenDeCompra* orden = nullptr;
			ProveedorProducto* pp = nullptr;

			// Obtener fecha y hora actual del sistema
			time_t t = time(nullptr);
			tm* now = localtime(&t);
			DTFecha fechaSistema(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

			auto proveedores = adminCtrl->listarProveedores();
			if (proveedores.empty()) {
				cout << "No hay proveedores registrados en el sistema." << endl;
				return;
			}
			while(true){
				cout << "Elija un proveedor por su RUT: "; cin >> rutProveedor;
				proveedor = adminCtrl->buscarProveedor(rutProveedor);
				if(proveedor == nullptr){
					cout << "Error: No existe un proveedor con ese RUT." << endl;
					cout << "¿Desea intentar con otro RUT? (s/n): "; cin >> respuesta;
					if(respuesta == 'n' || respuesta == 'N'){
						cout << "Operacion cancelada." << endl;
						return;
					}
				} else {
					break;
				}
			}
			orden = empleadoCtrl->crearOrdenDeCompra(fechaSistema, Estado::Pendiente, fechaSistema, proveedor);
			do{
				bool cancelar = false;
				while(true){
					// Mostrar productos que el proveedor puede abastecer
					auto productosOfrecidos = proveedor->getProductosOfrecidos();
					cout << "\nProductos que abastece " << proveedor->getEmpresa() << ":" << endl;
					for (size_t i = 0; i < productosOfrecidos.size(); ++i) {
						ProveedorProducto* pProd = productosOfrecidos[i];
						if (pProd != nullptr && pProd->getProducto() != nullptr) {
							cout << "  Codigo: " << pProd->getProducto()->getCodigo()
								 << " | " << pProd->getProducto()->getNombre()
								 << " | Precio: $" << pProd->getPrecioCompraPactado()
								 << " | Entrega: " << pProd->getTiempoEntregaEstimadoEnDias() << " dias" << endl;
						}
					}
					cout << "Seleccione el codigo del producto (0 para cancelar): "; cin >> codigoProducto;
					if (codigoProducto == 0) {
						cancelar = true;
						break;
					}
					producto = adminCtrl->buscarProducto(codigoProducto);
					if(producto == nullptr){
						cout << "Error: El producto no existe." << endl;
						cout << "¿Desea intentar con otro codigo? (s/n): "; cin >> respuesta;
						if (respuesta == 'n' || respuesta == 'N') {
							cancelar = true;
							break;
						}
						continue;
					}
					pp = adminCtrl->buscarAsociacion(rutProveedor, codigoProducto);
					if(pp == nullptr){
						cout << "Error: El proveedor no tiene este producto." << endl;
						cout << "¿Desea intentar con otro codigo? (s/n): "; cin >> respuesta;
						if (respuesta == 'n' || respuesta == 'N') {
							cancelar = true;
							break;
						}
						continue;
					}
					break;
				}
				if (cancelar) break;

				cout << "Indique la cantidad pedida: "; cin >> cantidad;
				cout << "Precio de compra pactado: $" << pp->getPrecioCompraPactado() << endl;
				cout << "Tiempo de entrega estimado: " << pp->getTiempoEntregaEstimadoEnDias() << " dias" << endl;
				empleadoCtrl->agregarLineaDetalleCompra(orden, cantidad, producto);
				cout << "Linea agregada con exito a la orden." << endl;
	    		cout << "\n¿Desea seguir ingresando lineas de detalle a la orden? (s/n): "; cin >> respuesta;
        	} while (respuesta == 's' || respuesta == 'S');
        	cout << "\nResumen de orden de compra: " << endl;
        	cout << " Proveedor: " << proveedor->getEmpresa() << " (RUT: " << rutProveedor << ")" << endl;
        	cout << " Fecha Emision: " << fechaSistema.getDia() << "/"
             	 << fechaSistema.getMes() << "/"
             	 << fechaSistema.getAnio() << endl;
        	cout << " Cant. Lineas: " << orden->getDetalleCompra().size() << endl;
        	cout << " Total Estimado: $" << orden->calcularTotal() << endl;
        	cout << "¿Desea confirmar esta orden de compra? (s/n): "; cin >> respuesta;
        	if (respuesta == 's' || respuesta == 'S') {
            	empleadoCtrl->agregarOrdenDeCompra(orden);
            	cout << "La orden de compra ha sido registrada con estado 'Pendiente'." << endl;
        	} else {
            	cout << "Operacion cancelada. Orden descartada." << endl;
            	delete orden;
        	}
        	} else if (op == 6) {
			// Cancelar orden de compra
			auto pendientes = empleadoCtrl->listarOrdenesPendientes();
			if (pendientes.empty()) {
				cout << "\nNo hay ordenes de compra en estado 'Pendiente'." << endl;
				return;
			}

			cout << "\n--- Ordenes de compra pendientes ---\n";
			for (size_t i = 0; i < pendientes.size(); ++i) {
				OrdenDeCompra* o = pendientes[i];
				if (o == nullptr) continue;
				string nomEmpresa = o->getProveedor() != nullptr ? o->getProveedor()->getEmpresa() : "(sin proveedor)";
				DTFecha fe = o->getFechaEmision();
				cout << "  ID " << i << " | Proveedor: " << nomEmpresa
					 << " | Fecha Emision: " << fe.getDia() << "/" << fe.getMes() << "/" << fe.getAnio()
					 << " | Lineas: " << o->getDetalleCompra().size() << endl;
			}

			int id;
			cout << "\nIngrese el ID de la orden a cancelar: ";
			cin >> id;

			if (id < 0 || id >= static_cast<int>(pendientes.size())) {
				cout << "Error: ID invalido." << endl;
				return;
			}

			OrdenDeCompra* ordenSeleccionada = pendientes[id];

			// Mostrar detalle completo
			cout << "\n--- Detalle de la orden de compra ---\n";
			cout << "Proveedor: " << (ordenSeleccionada->getProveedor() != nullptr ? ordenSeleccionada->getProveedor()->getEmpresa() : "N/A") << endl;
			DTFecha fe = ordenSeleccionada->getFechaEmision();
			cout << "Fecha de emision: " << fe.getDia() << "/" << fe.getMes() << "/" << fe.getAnio() << endl;
			cout << "Estado: Pendiente" << endl;
			cout << "Lineas de la orden:\n";
			for (size_t j = 0; j < ordenSeleccionada->getDetalleCompra().size(); ++j) {
				LineaDetalleCompra* linea = ordenSeleccionada->getDetalleCompra()[j];
				if (linea == nullptr) continue;
				Producto* prod = linea->getProducto();
				string nombreProd = prod != nullptr ? prod->getNombre() : "(producto eliminado)";
				cout << "  " << (j + 1) << ". Producto: " << nombreProd
					 << " | Cantidad: " << linea->getCantidad() << endl;
			}
			cout << "Total estimado: $" << ordenSeleccionada->calcularTotal() << endl;

			char confirmar;
			cout << "\n¿Esta seguro de cancelar esta orden de compra? (s/n): ";
			cin >> confirmar;

			if (confirmar == 's' || confirmar == 'S') {
				empleadoCtrl->cancelarOrdenDeCompra(ordenSeleccionada);
				cout << "La orden de compra ha sido cancelada." << endl;
				cout << "Nota: Una orden cancelada no generara movimiento de stock." << endl;
			} else {
				cout << "Operacion cancelada. No se realizaron cambios." << endl;
			}
		} else if (op == 7) {
			// Registrar recepcion de orden de compra
			auto pendientes = empleadoCtrl->listarOrdenesPendientes();
			if (pendientes.empty()) {
				cout << "\nNo hay ordenes de compra en estado 'Pendiente'." << endl;
				return;
			}

			cout << "\n--- Ordenes de compra pendientes ---\n";
			for (size_t i = 0; i < pendientes.size(); ++i) {
				OrdenDeCompra* o = pendientes[i];
				if (o == nullptr) continue;
				string nomEmpresa = o->getProveedor() != nullptr ? o->getProveedor()->getEmpresa() : "(sin proveedor)";
				DTFecha fe = o->getFechaEmision();
				cout << "  ID " << i << " | Proveedor: " << nomEmpresa
					 << " | Fecha Emision: " << fe.getDia() << "/" << fe.getMes() << "/" << fe.getAnio() << endl;
			}

			int id;
			cout << "\nIngrese el ID de la orden recibida: ";
			cin >> id;

			if (id < 0 || id >= static_cast<int>(pendientes.size())) {
				cout << "Error: ID invalido." << endl;
				return;
			}

			OrdenDeCompra* ordenSeleccionada = pendientes[id];

			// Mostrar detalle de la orden
			auto& detalle = ordenSeleccionada->getDetalleCompra();
			cout << "\n--- Detalle de la orden de compra ---\n";
			cout << "Proveedor: " << (ordenSeleccionada->getProveedor() != nullptr ? ordenSeleccionada->getProveedor()->getEmpresa() : "N/A") << endl;
			DTFecha fe = ordenSeleccionada->getFechaEmision();
			cout << "Fecha de emision: " << fe.getDia() << "/" << fe.getMes() << "/" << fe.getAnio() << endl;
			cout << "\nProductos y cantidades pedidas:\n";
			for (size_t j = 0; j < detalle.size(); ++j) {
				if (detalle[j] == nullptr) continue;
				Producto* prod = detalle[j]->getProducto();
				string nombreProd = prod != nullptr ? prod->getNombre() : "(producto eliminado)";
				cout << "  " << (j + 1) << ". " << nombreProd
					 << " - Cantidad pedida: " << detalle[j]->getCantidad() << endl;
			}

			// Solicitar cantidades recibidas (pueden haber diferencias)
			vector<int> cantidadesRecibidas;
			cout << "\n--- Ingrese las cantidades recibidas ---\n";
			for (size_t j = 0; j < detalle.size(); ++j) {
				if (detalle[j] == nullptr) continue;
				Producto* prod = detalle[j]->getProducto();
				string nombreProd = prod != nullptr ? prod->getNombre() : "(producto eliminado)";
				int cant;
				cout << "  " << nombreProd << " (pedido: " << detalle[j]->getCantidad() << "): ";
				cin >> cant;
				if (cant < 0) cant = 0;
				cantidadesRecibidas.push_back(cant);
			}

			// Mostrar resumen
			cout << "\n--- Resumen de recepcion ---\n";
			cout << "Proveedor: " << (ordenSeleccionada->getProveedor() != nullptr ? ordenSeleccionada->getProveedor()->getEmpresa() : "N/A") << endl;
			for (size_t j = 0; j < detalle.size(); ++j) {
				if (detalle[j] == nullptr) continue;
				Producto* prod = detalle[j]->getProducto();
				string nombreProd = prod != nullptr ? prod->getNombre() : "(producto eliminado)";
				cout << "  " << nombreProd << ": pedido " << detalle[j]->getCantidad()
					 << " | recibido " << cantidadesRecibidas[j] << endl;
			}

			char confirmar;
			cout << "\n¿Desea confirmar la recepcion de esta orden? (s/n): ";
			cin >> confirmar;

			if (confirmar == 's' || confirmar == 'S') {
				// Obtener fecha y hora actual del sistema
				time_t t = time(nullptr);
				tm* now = localtime(&t);
				DTFecha fechaSistema(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

				bool ok = empleadoCtrl->registrarRecepcionOrden(ordenSeleccionada, cantidadesRecibidas, fechaSistema);
				if (ok) {
					cout << "\nRecepcion registrada con exito." << endl;
					cout << "La orden ha pasado al estado 'Recibida'." << endl;
					cout << "El stock de los productos ha sido actualizado." << endl;
				} else {
					cout << "\nError al registrar la recepcion." << endl;
				}
			} else {
				cout << "Operacion cancelada. No se realizaron cambios." << endl;
			}
		} else if (op == 8) {
			consultarCalificacionesProducto();
		} else if (op == 9) {
			consultarStockProductos();
		} else if (op == 10) {
			consultarProductosStockBajo();
		} else if (op == 11) {
			consultarMontoFacturadoCliente();
		} else if (op == 12) {
			consultarUnidadesVendidasProducto();
		} else if (op == 13) {
			consultarInfoDetalladaProducto();
		}
	}
}

void MenuEmpleado::consultarStockProductos() {
	AdminController* adminCtrl = AdminController::getInstanciaAdmin();
	if (adminCtrl == nullptr) return;

	char filtrar;
	cout << "\n¿Desea filtrar por categoria? (s/n): ";
	cin >> filtrar;

	vector<Producto*> productos;

	if (filtrar == 's' || filtrar == 'S') {
		auto categorias = adminCtrl->listarCategorias();
		if (categorias.empty()) {
			cout << "\nNo hay categorias registradas en el sistema." << endl;
			return;
		}

		cout << "\n--- Categorias disponibles ---\n";
		for (size_t i = 0; i < categorias.size(); ++i) {
			if (categorias[i] != nullptr) {
				cout << "  " << (i + 1) << ". " << categorias[i]->getNombre()
					 << ": " << categorias[i]->getDescripcion() << endl;
			}
		}

		int opcion;
		cout << "\nSeleccione una categoria (1-" << categorias.size() << "): ";
		cin >> opcion;

		if (opcion < 1 || opcion > static_cast<int>(categorias.size())) {
			cout << "Opcion invalida." << endl;
			return;
		}

		string nombreCategoria = categorias[opcion - 1]->getNombre();
		auto todos = adminCtrl->listarProductos();

		for (Producto* p : todos) {
			if (p != nullptr && p->getCategoria() != nullptr &&
				p->getCategoria()->getNombre() == nombreCategoria) {
				productos.push_back(p);
			}
		}

		if (productos.empty()) {
			cout << "\nNo hay productos en la categoria \"" << nombreCategoria << "\"." << endl;
			return;
		}
	} else {
		productos = adminCtrl->listarProductos();
		if (productos.empty()) {
			cout << "\nNo hay productos registrados en el catalogo." << endl;
			return;
		}
	}

	cout << "\n--- Stock actual de productos ---\n";
	cout << "Codigo  | Nombre                          | Categoria               | Stock Actual | Stock Minimo\n";
	cout << "--------|---------------------------------|-------------------------|--------------|--------------\n";

	for (Producto* p : productos) {
		if (p == nullptr) continue;

		string nombreCat = p->getCategoria() != nullptr ? p->getCategoria()->getNombre() : "(sin categoria)";

		// Formatear con anchors
		cout << p->getCodigo() << "\t| " << p->getNombre();
		// Padding para alinear
		for (int i = p->getNombre().length(); i < 31; ++i) cout << " ";
		cout << "| " << nombreCat;
		for (int i = nombreCat.length(); i < 23; ++i) cout << " ";
		cout << "| " << p->getStock();
		for (int i = to_string(p->getStock()).length(); i < 12; ++i) cout << " ";
		cout << "| " << p->getStockMinimo() << endl;
	}
	cout << "---------------------------------------------------------------------\n";
}

void MenuEmpleado::consultarProductosStockBajo() {
	AdminController* adminCtrl = AdminController::getInstanciaAdmin();
	if (adminCtrl == nullptr) return;

	auto todos = adminCtrl->listarProductos();
	if (todos.empty()) {
		cout << "\nNo hay productos registrados en el catalogo." << endl;
		return;
	}

	// Filtrar productos con stock < stock minimo
	vector<Producto*> productosBajos;
	for (Producto* p : todos) {
		if (p != nullptr && p->getStock() < p->getStockMinimo()) {
			productosBajos.push_back(p);
		}
	}

	if (productosBajos.empty()) {
		cout << "\nNo hay productos con stock por debajo del minimo configurado." << endl;
		return;
	}

	// Preguntar si desea ordenar por diferencia ascendente
	char ordenar;
	cout << "\n¿Ordenar por diferencia ascendente (mas criticos primero)? (s/n): ";
	cin >> ordenar;

	if (ordenar == 's' || ordenar == 'S') {
		sort(productosBajos.begin(), productosBajos.end(), [](Producto* a, Producto* b) {
			int diffA = a->getStockMinimo() - a->getStock();
			int diffB = b->getStockMinimo() - b->getStock();
			return diffA > diffB; // mayor diferencia = mas critico primero
		});
	}

	cout << "\n--- Productos con stock bajo minimo ---\n";
	cout << "Codigo  | Nombre                          | Categoria               | Stock Actual | Stock Minimo | Diferencia\n";
	cout << "--------|---------------------------------|-------------------------|--------------|--------------|------------\n";

	for (Producto* p : productosBajos) {
		if (p == nullptr) continue;

		string nombreCat = p->getCategoria() != nullptr ? p->getCategoria()->getNombre() : "(sin categoria)";
		string strCodigo = to_string(p->getCodigo());
		string strStock = to_string(p->getStock());
		string strStockMin = to_string(p->getStockMinimo());
		int diferencia = p->getStockMinimo() - p->getStock();
		string strDiff = to_string(diferencia);

		cout << strCodigo;
		for (int i = strCodigo.length(); i < 8; ++i) cout << " ";
		cout << "| " << p->getNombre();
		for (int i = p->getNombre().length(); i < 31; ++i) cout << " ";
		cout << "| " << nombreCat;
		for (int i = nombreCat.length(); i < 23; ++i) cout << " ";
		cout << "| " << strStock;
		for (int i = strStock.length(); i < 12; ++i) cout << " ";
		cout << "| " << strStockMin;
		for (int i = strStockMin.length(); i < 12; ++i) cout << " ";
		cout << "| " << strDiff;
		cout << " (faltan " << diferencia << ")" << endl;
	}
	cout << "----------------------------------------------------------------------------\n";
}

void MenuEmpleado::consultarCalificacionesProducto() {
	if (ventaCtrl == nullptr) {
		cout << "Error: Controlador de ventas no disponible." << endl;
		return;
	}

	AdminController* adminCtrl = AdminController::getInstanciaAdmin();
	if (adminCtrl == nullptr) return;

	vector<Producto*> productos = adminCtrl->listarProductos();
	if (productos.empty()) {
		cout << "\nNo hay productos registrados en el catalogo." << endl;
		return;
	}

	cout << "\n--- Catalogo de productos ---\n";
	for (Producto* p : productos) {
		if (p != nullptr) {
			cout << "  Codigo: " << p->getCodigo()
				 << " | Nombre: " << p->getNombre()
				 << " | Puntaje promedio: " << p->getPuntajePromedio() << "/5"
				 << " (" << p->getCantidadCalificaciones() << " calificaciones)" << endl;
		}
	}

	int codigo;
	cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
	cin >> codigo;

	if (codigo == 0) {
		cout << "Operacion cancelada." << endl;
		return;
	}

	Producto* producto = adminCtrl->buscarProducto(codigo);
	if (producto == nullptr) {
		cout << "No existe un producto con el codigo ingresado." << endl;
		return;
	}

	vector<Calificacion*> calificaciones = ventaCtrl->listarCalificacionesDeProducto(codigo);

	if (calificaciones.empty()) {
		cout << "\nEl producto \"" << producto->getNombre() << "\" no tiene calificaciones registradas." << endl;
		cout << "Puntaje promedio: " << producto->getPuntajePromedio() << "/5" << endl;
		return;
	}

	cout << "\n--- Calificaciones de \"" << producto->getNombre() << "\" ---\n";
	cout << "Puntaje promedio general: " << producto->getPuntajePromedio() << "/5"
		 << " (basado en " << calificaciones.size() << " calificaciones)" << endl;
	cout << "--------------------------------------------------------\n";

	for (size_t i = 0; i < calificaciones.size(); ++i) {
		Calificacion* cal = calificaciones[i];
		if (cal == nullptr) continue;

		DTFecha f = cal->getFecha();
		string comentario = cal->getComentario();
		Cliente* cli = cal->getClienteCalificador();

		cout << (i + 1) << ") Fecha: " << f.getDia() << "/" << f.getMes() << "/" << f.getAnio()
			 << " | Puntaje: " << static_cast<int>(cal->getPuntaje()) << "/5"
			 << " | Cliente RUT: " << (cli != nullptr ? cli->getRut() : "N/A")
			 << " | Comentario: " << (comentario.empty() ? "(sin comentario)" : comentario) << endl;
	}
	cout << "--------------------------------------------------------\n";
}

void MenuEmpleado::consultarMontoFacturadoCliente() {
	EmpleadoController* empleadoCtrl = EmpleadoController::getInstanciaEmpleado();
	if (empleadoCtrl == nullptr) {
		cout << "Error: Controlador de empleados no disponible." << endl;
		return;
	}

	string rut;
	cout << "\nIngrese el RUT del cliente: ";
	cin >> rut;

	Cliente* cliente = empleadoCtrl->buscarCliente(rut);
	if (cliente == nullptr) {
		cout << "Error: No existe un cliente con el RUT ingresado." << endl;
		return;
	}

	int diaInicio, mesInicio, anioInicio;
	int diaFin, mesFin, anioFin;

	// Solicitar fecha de inicio
	cout << "\nFecha de inicio del periodo:" << endl;
	cout << "Dia: "; cin >> diaInicio;
	cout << "Mes: "; cin >> mesInicio;
	cout << "Anio: "; cin >> anioInicio;
	DTFecha inicio(diaInicio, mesInicio, anioInicio);

	// Solicitar fecha de fin
	cout << "\nFecha de fin del periodo:" << endl;
	cout << "Dia: "; cin >> diaFin;
	cout << "Mes: "; cin >> mesFin;
	cout << "Anio: "; cin >> anioFin;
	DTFecha fin(diaFin, mesFin, anioFin);

	// Validar que inicio <= fin
	while (!(inicio <= fin)) {
		cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << endl;
		cout << "1. Reingresar las fechas" << endl;
		cout << "2. Cancelar" << endl;
		int opcion;
		cout << "Seleccione una opcion: ";
		cin >> opcion;
		if (opcion == 2) {
			cout << "Operacion cancelada." << endl;
			return;
		}

		cout << "\nFecha de inicio del periodo:" << endl;
		cout << "Dia: "; cin >> diaInicio;
		cout << "Mes: "; cin >> mesInicio;
		cout << "Anio: "; cin >> anioInicio;
		inicio = DTFecha(diaInicio, mesInicio, anioInicio);

		cout << "\nFecha de fin del periodo:" << endl;
		cout << "Dia: "; cin >> diaFin;
		cout << "Mes: "; cin >> mesFin;
		cout << "Anio: "; cin >> anioFin;
		fin = DTFecha(diaFin, mesFin, anioFin);
	}

	// Consultar facturacion
	ResultadoFacturacion res = empleadoCtrl->consultarMontoFacturado(rut, inicio, fin);

	// Mostrar resultados
	cout << "\n--- Resultados de facturacion ---" << endl;
	cout << "Cliente: " << cliente->getNombre() << " " << cliente->getApellido()
		 << " (RUT: " << rut << ")" << endl;
	cout << "Periodo: " << diaInicio << "/" << mesInicio << "/" << anioInicio
		 << " - " << diaFin << "/" << mesFin << "/" << anioFin << endl;
	cout << "------------------------------------------------" << endl;
	cout << "Monto total facturado: $" << res.montoTotal << endl;
	cout << "Cantidad de ventas: " << res.cantidadVentas << endl;
	if (res.cantidadVentas > 0) {
		cout << "Monto promedio por venta: $" << res.montoPromedio << endl;
	}
	cout << "------------------------------------------------" << endl;
}

void MenuEmpleado::consultarUnidadesVendidasProducto() {
	EmpleadoController* empleadoCtrl = EmpleadoController::getInstanciaEmpleado();
	if (empleadoCtrl == nullptr) {
		cout << "Error: Controlador de empleados no disponible." << endl;
		return;
	}

	AdminController* adminCtrl = AdminController::getInstanciaAdmin();
	if (adminCtrl == nullptr) return;

	// Listar todos los productos
	auto productos = adminCtrl->listarProductos();
	if (productos.empty()) {
		cout << "\nNo hay productos registrados en el catalogo." << endl;
		return;
	}

	cout << "\n--- Productos disponibles ---\n";
	for (Producto* p : productos) {
		if (p != nullptr) {
			cout << "  Codigo: " << p->getCodigo()
				 << " | Nombre: " << p->getNombre() << endl;
		}
	}

	int codigoProducto;
	cout << "\nIngrese el codigo del producto (0 para cancelar): ";
	cin >> codigoProducto;

	if (codigoProducto == 0) {
		cout << "Operacion cancelada." << endl;
		return;
	}

	Producto* producto = adminCtrl->buscarProducto(codigoProducto);
	if (producto == nullptr) {
		cout << "Error: No existe un producto con el codigo ingresado." << endl;
		return;
	}

	// Solicitar fechas
	int diaInicio, mesInicio, anioInicio;
	int diaFin, mesFin, anioFin;

	cout << "\nFecha de inicio del periodo:" << endl;
	cout << "Dia: "; cin >> diaInicio;
	cout << "Mes: "; cin >> mesInicio;
	cout << "Anio: "; cin >> anioInicio;
	DTFecha inicio(diaInicio, mesInicio, anioInicio);

	cout << "\nFecha de fin del periodo:" << endl;
	cout << "Dia: "; cin >> diaFin;
	cout << "Mes: "; cin >> mesFin;
	cout << "Anio: "; cin >> anioFin;
	DTFecha fin(diaFin, mesFin, anioFin);

	// Validar que inicio <= fin
	while (!(inicio <= fin)) {
		cout << "\nError: La fecha de inicio debe ser anterior o igual a la fecha de fin." << endl;
		cout << "1. Reingresar las fechas" << endl;
		cout << "2. Cancelar" << endl;
		int opcion;
		cout << "Seleccione una opcion: ";
		cin >> opcion;
		if (opcion == 2) {
			cout << "Operacion cancelada." << endl;
			return;
		}

		cout << "\nFecha de inicio del periodo:" << endl;
		cout << "Dia: "; cin >> diaInicio;
		cout << "Mes: "; cin >> mesInicio;
		cout << "Anio: "; cin >> anioInicio;
		inicio = DTFecha(diaInicio, mesInicio, anioInicio);

		cout << "\nFecha de fin del periodo:" << endl;
		cout << "Dia: "; cin >> diaFin;
		cout << "Mes: "; cin >> mesFin;
		cout << "Anio: "; cin >> anioFin;
		fin = DTFecha(diaFin, mesFin, anioFin);
	}

	// Consultar unidades vendidas
	ResultadoUnidadesVendidas res = empleadoCtrl->consultarUnidadesVendidas(codigoProducto, inicio, fin);

	// Mostrar resultados
	cout << "\n--- Resultados de unidades vendidas ---" << endl;
	cout << "Producto: " << producto->getNombre() << " (codigo=" << codigoProducto << ")" << endl;
	cout << "Periodo: " << diaInicio << "/" << mesInicio << "/" << anioInicio
		 << " - " << diaFin << "/" << mesFin << "/" << anioFin << endl;
	cout << "------------------------------------------------" << endl;
	cout << "Total de unidades vendidas: " << res.totalUnidadesVendidas << endl;
	cout << "Cantidad de ventas en las que participo: " << res.cantidadVentas << endl;
	if (res.totalUnidadesVendidas > 0) {
		cout << "Precio unitario promedio: $" << res.precioUnitarioPromedio << endl;
	}
	cout << "------------------------------------------------" << endl;
}

void MenuEmpleado::consultarInfoDetalladaProducto() {
	AdminController* adminCtrl = AdminController::getInstanciaAdmin();
	if (adminCtrl == nullptr) return;

	auto productos = adminCtrl->listarProductos();
	if (productos.empty()) {
		cout << "\nNo hay productos registrados en el catalogo." << endl;
		return;
	}

	cout << "\n--- Productos disponibles ---\n";
	for (Producto* p : productos) {
		if (p != nullptr) {
			cout << "  Codigo: " << p->getCodigo()
				 << " | Nombre: " << p->getNombre() << endl;
		}
	}

	int codigo;
	cout << "\nIngrese el codigo del producto a consultar (0 para cancelar): ";
	cin >> codigo;

	if (codigo == 0) {
		cout << "Operacion cancelada." << endl;
		return;
	}

	Producto* producto = adminCtrl->buscarProducto(codigo);
	if (producto == nullptr) {
		cout << "Error: No existe un producto con el codigo ingresado." << endl;
		return;
	}

	// Mostrar informacion basica (para todos los roles)
	cout << "\n========== INFORMACION DETALLADA DEL PRODUCTO ==========\n";
	cout << "Codigo: " << producto->getCodigo() << endl;
	cout << "Nombre: " << producto->getNombre() << endl;
	cout << "Descripcion: " << producto->getDescripcion() << endl;

	string nombreCat = producto->getCategoria() != nullptr ? producto->getCategoria()->getNombre() : "(sin categoria)";
	cout << "Categoria: " << nombreCat << endl;

	cout << "Precio de venta unitario: $" << producto->getPrecioVentaActual() << endl;
	cout << "Stock actual: " << producto->getStock() << endl;
	cout << "Puntaje promedio: " << producto->getPuntajePromedio() << "/5"
		 << " (" << producto->getCantidadCalificaciones() << " calificaciones)" << endl;

	// Informacion adicional para Empleado y Administrador
	cout << "\n--- Datos de administracion ---\n";
	cout << "Stock minimo: " << producto->getStockMinimo() << endl;

	// Proveedores que abastecen el producto
	auto asociaciones = adminCtrl->listarAsociacionesDeProducto(codigo);
	if (asociaciones.empty()) {
		cout << "Proveedores: Ninguno (el producto no esta asociado a ningun proveedor)" << endl;
	} else {
		cout << "\nProveedores que abastecen este producto:" << endl;
		for (size_t i = 0; i < asociaciones.size(); ++i) {
			ProveedorProducto* pp = asociaciones[i];
			if (pp == nullptr) continue;
			// Obtener datos del proveedor
			string rutProveedor = "(desconocido)";
			string empresaProveedor = "(desconocida)";
			for (Proveedor* pr : adminCtrl->listarProveedores()) {
				if (pr == nullptr) continue;
				for (ProveedorProducto* pp2 : pr->getProductosOfrecidos()) {
					if (pp2 == pp) {
						rutProveedor = pr->getRut();
						empresaProveedor = pr->getEmpresa();
						break;
					}
				}
				if (rutProveedor != "(desconocido)") break;
			}
			cout << "  " << (i + 1) << ". Proveedor: " << empresaProveedor
				 << " (RUT: " << rutProveedor << ")" << endl;
			cout << "     Precio de compra vigente: $" << pp->getPrecioCompraPactado() << endl;
			cout << "     Tiempo de entrega estimado: " << pp->getTiempoEntregaEstimadoEnDias() << " dias" << endl;
		}
	}
	cout << "========================================================\n";
}
