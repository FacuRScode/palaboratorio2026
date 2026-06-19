#include "MenuEmpleado.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

MenuEmpleado::MenuEmpleado(EmpleadoController& controller, AuthController* auth) : ctrl(controller), authCtrl(auth) {}

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
			while(true){
				cout << "RUT: "; cin >> rut;
				if(empleadoCtrl->buscarCliente(rut) != nullptr){
					cout<<"Error: Ya existe un cliente con ese RUT."<<endl;
					cout<<"¿Desea intentarlo con otro RUT? (s/n): "; cin >> respuesta;
					if (respuesta == 'n') {
						cout << "Registro cancelado." << endl;
						return;
					}
					cout << endl;
				} else {
					break;
				}
			}
			cout << "Nombre: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre);
			cout << "Apellido: "; getline(cin, apellido);
			cout << "Direccion: "; getline(cin, direccion);

			while(true){
				cout << "Correo: "; getline(cin, correo);
				if(empleadoCtrl->buscarClientePorCorreo(correo) != nullptr){
					cout<<"Error: Ya existe un cliente con ese correo."<<endl;
					cout<<"¿Desea intentarlo con otro correo? (s/n): "; cin >> respuesta;
					if (respuesta == 'n') {
						cout << "Registro cancelado." << endl;
						return;
					}
					cout << endl;
				} else {
					break;
				}
			}
			cout << "Contrasena: "; getline(cin, contrasena);
			cout << "\n Resumen del registro: " << endl;
			cout << "RUT: " << rut << endl;
			cout << "Nombre Completo: " << nombre << " " << apellido << endl;
			cout << "Direccion: " << direccion << endl;
			cout << "Correo: " << correo << endl;
			cout << "Contrasena: " << contrasena <<endl;
			cout << "¿Desea confiirmar el registro? (s/n): "; cin >> respuesta;
			if(respuesta == 's'){
				empleadoCtrl->registrarCliente(rut, nombre, apellido, direccion, correo, contrasena);
				cout << "Cliente registrado con exito."<<endl;
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
			while(true){
				cout << "Correo: "; getline(cin, nuevoCorreo);
				if(empleadoCtrl->buscarClientePorCorreo(nuevoCorreo) != nullptr){
					cout<<"Error: Ya existe un cliente con ese correo."<<endl;
					cout<<"¿Desea intentarlo con otro correo? (s/n): "; cin >> respuesta;
					if (respuesta == 'n') {
						cout << "\n Resumen de la edicion: " << endl;
						cout << "Nombre Completo: " << nuevoNombre << " " << nuevoApellido << endl;
						cout << "Direccion: " << nuevaDireccion << endl;
						cout << "Correo: " << cliente->getCorreo() << endl;
						cout << "¿Desea confirmar la modificacion? (s/n): "; cin >> respuesta;
            			if(respuesta == 's'){
                			empleadoCtrl->modificaCliente(cliente, nuevoNombre, nuevoApellido, nuevaDireccion, cliente->getCorreo());
                			cout << "Cliente modificado con exito." << endl;
							return;
            			} else {
               				 cout << "Modificacion cancelada. Se conservan los datos originales." << endl;
							 return;
            			}
					}
   				} else {
      				break;
				}
			}
			cout << "\n Resumen de la edicion: " << endl;
			cout << "Nombre Completo: " << nuevoNombre << " " << nuevoApellido << endl;
			cout << "Direccion: " << nuevaDireccion << endl;
			cout << "Correo: " << nuevoCorreo << endl;
			cout << "¿Desea confirmar la modificacion? (s/n): "; cin >> respuesta;
            if(respuesta == 's'){
                empleadoCtrl->modificaCliente(cliente, nuevoNombre, nuevoApellido, nuevaDireccion, nuevoCorreo);
                cout << "Cliente modificado con exito." << endl;
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
            cout << "¿El cliente esta registrado? (s/n): "; cin >> respuesta;
            if (respuesta == 's') {
                while (true) {
                    cout << "RUT: "; cin >> rut;
                    cliente = empleadoCtrl->buscarCliente(rut);
                    if (cliente == nullptr) {
                        cout << "Error: No existe un cliente con ese RUT." << endl;
                        cout << "¿Desea intentarlo con otro RUT? (s/n): "; cin >> respuesta;
                        if (respuesta == 'n') {
                            cout << "Registro cancelado." << endl;
                            return;
                        }
                        cout << endl;
                    } else {
                        break;
                    }
                }
            DTFecha fechaActual(18, 6, 2026);
	        DTHora horaActual(11, 0, 0);
            venta = empleadoCtrl->crearVenta(rut, fechaActual, horaActual);
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

                if (cantidad > producto->getStock()) {
                    cout << "Error: No hay stock suficiente, linea no agregada." << endl;
                } else {
                    empleadoCtrl->agregarLineaAVenta(venta, codigoProducto, cantidad);
                    cout << "Linea agregada con exito." << endl;
                }

                cout << "¿Desea seguir ingresando lineas de detalle? (s/n): "; cin >> respuesta;
            } while (respuesta == 's');

            cout << "Resumen de venta: " << endl;
            cout << " Cliente: " << rut << endl;
            cout << " Total: $" << venta->calcularTotal() << endl;
            cout << " Fecha/Hora: " << venta->getFecha().getDia() << "/" << venta->getFecha().getMes() << "/" << venta->getFecha().getAnio()
			<< venta->getHora().getHora() << ":" << venta->getHora().getMinuto() << ":" << venta->getHora().getSegundo() << endl;

            cout << "¿Desea confirmar esta venta? (s/n): "; cin >> respuesta;

            if (respuesta == 's' || respuesta == 'S') {
                for (auto linea : venta->getDetalle()) {
                    Producto* p = linea->getProducto();
                    p->setStock(p->getStock() - linea->getCantidad());
                }
                cout << "Venta registrada con exito." << endl;
            } else {
                cout << "Venta cancelada." << endl;
                delete venta;
            }
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

		    if (fa.getAnio() != fb.getAnio()) {
    		    return fa.getAnio() > fb.getAnio();
    		}

    		if (fa.getMes() != fb.getMes()) {
        		return fa.getMes() > fb.getMes();
    		}

		    return fa.getDia() > fb.getDia();
		});

        cout << "\nHistorial de compras: " << cliente->getNombre() << " " << cliente->getApellido() << endl;
		cout << "------------------------------------------------" << endl;

		for (size_t i = 0; i < ventas.size(); ++i) {
    		cout << (i + 1) << ") ";
    		cout <<"Fecha y hora: " << (ventas[i]->getFecha().getDia()) << "/"
        		 << (ventas[i]->getFecha().getMes()) << "/"
        		 << ventas[i]->getFecha().getAnio();
    		cout << " - " << ventas[i]->getDetalle().size() << " lineas - Total: $"
        		 << ventas[i]->calcularTotal() << endl;
		}
        int seleccion;
        cout << "\nSeleccione el numero de una venta para ver su detalle (0 para salir): ";
        cin >> seleccion;

        if (seleccion > 0 && seleccion <= static_cast<int>(ventas.size())) {
            Venta* ventaSeleccionada = ventas[seleccion - 1];

            cout << "\nDetalle de la venta N° " << seleccion << endl;
            cout << "Fecha/Hora: " << ventaSeleccionada->getFecha().getMes() << "/"
			<< (ventaSeleccionada->getFecha().getDia()) << "/"
         	<< ventaSeleccionada->getFecha().getAnio() << endl;

            for (auto linea : ventaSeleccionada->getDetalle()) {
				cout<<"Producto: ";
                Producto* prod = linea->getProducto();
                cout << "\nP. Unitario: ";
                double precioAplicado = linea->getPrecioUnitario();
				cout << "\nCantidad: ";
                int cant = linea->getCantidad();
				cout << "\nSubtotal: ";
                double subtotal = cant * precioAplicado;

                string nombreProd = prod->getNombre();
                if (nombreProd.length() < 20) {
                    nombreProd.append(20 - nombreProd.length(), ' ');
                }

                cout << nombreProd << " | "
                     << cant << "        | $"
                     << precioAplicado << "        | $"
                     << subtotal << endl;
            }
            cout << "Total compra:                                      $" << ventaSeleccionada->calcularTotal() << endl;
        } else {
            cout << "Regresando al menú de opciones..." << endl;
        }
		} else if (op == 5) {
			int cantidad,codigoProducto;
			string rutProveedor;
			char respuesta;
			Producto* producto = nullptr;
			Proveedor* proveedor = nullptr;
			OrdenDeCompra* orden = nullptr;
			ProveedorProducto* pp = nullptr;
			adminCtrl->listarProveedores();
			while(true){
			cout << "Elije un proveedor por su RUT: "; cin >> rutProveedor;
			proveedor = adminCtrl->buscarProveedor(rutProveedor);
			if(proveedor == nullptr){
				cout << "Error: No existe un proveedor con ese RUT." <<endl;
				cout << "¿Desea intentar con otro RUT? (s/n): "; cin >> respuesta;
				if(respuesta == 'n'){
					cout << "Operacion cancelada." <<endl;
					return;
				}
			}
			}
			DTFecha fechaActual(1, 1, 2026);
			orden = empleadoCtrl->crearOrdenDeCompra(fechaActual, Estado::Pendiente, DTFecha(1, 2, 2027), nullptr);
			do{
				while(true){
					cout << "Seleccion un producto: "; cin >> codigoProducto;
					producto = adminCtrl->buscarProducto(codigoProducto);
					if(producto == nullptr){
						cout << "Error: El producto no existe." << endl;
						cout << "¿Desea intentar con otro codigo? (s/n): "; cin >> respuesta;
						if (respuesta == 'n') {
                        	break;
                   		}
                    	continue;
					}
					pp = adminCtrl->buscarAsociacion(rutProveedor, codigoProducto);
					if(pp == nullptr){
						cout << "Error: El proveedor no tiene este producto." << endl;
						cout << "¿Desea intentar con otro codigo? (s/n): "; cin >> respuesta;							cout << "¿Desea intentar con otro codigo? (s/n): "; cin >> respuesta;
						if (respuesta == 'n') {
                        break;
                    }
                    	continue;
                	}
					break;
				}
				cout << "Indique la cantidad pedida: "; cin >> cantidad;
				cout << "Precio: " << producto->getPrecioVentaActual() << endl;
				cout << "Tiempo de entrega estimado: " << pp->getTiempoEntregaEstimadoEnDias();
				empleadoCtrl->agregarLineaDetalleCompra(orden, cantidad, producto);
				cout << "Linea agregada con exito a la orden." << endl;
	    		cout << "\n¿Desea seguir ingresando lineas de detalle a la orden? (s/n): "; cin >> respuesta;
        	} while (respuesta == 's');
        	cout << "Resumen de orden de compra: " << endl;
        	cout << " Proveedor: " << rutProveedor << endl;
        	cout << " Fecha Emision: ";
        	cout << fechaActual.getDia()
             	 << fechaActual.getMes()
             	 << fechaActual.getAnio() << endl;
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
		} else if (op == 5) {
		}
	}
}
