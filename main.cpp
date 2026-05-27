/**
 * @file main.cpp
 * @brief Sistema de administración de Pollos Asados con diseño de terminal CCCaster y navegación fluida.
 * @author Esteban
 * @date 2026
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <mysql.h>
#include <windows.h>
#include "EloquentORM.h"
#include "MySQLConexion.h"

using namespace std;

// =========================================================================
// ENUMERACIÓN DE COLORES Y CONFIGURACIÓN DE VISUALIZACIÓN
// =========================================================================
enum Colors {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5, BROWN = 6, LGREY = 7,
    DGREY = 8, LBLUE = 9, LGREEN = 10, LCYAN = 11, LRED = 12, LMAGENTA = 13, YELLOW = 14, WHITE = 15
};

void Color(int Background, int Text) {
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
    int New_Color = Text + (Background * 16);
    SetConsoleTextAttribute(Console, New_Color);
}

// Mueve el cursor a una posición específica (X, Y) en el CMD
void gotoxy(int x, int y) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

// Dibuja una línea completa de asteriscos
void imprimir_marco_linea(int ancho = 55) {
    cout << string(ancho, '*') << endl;
}

// Centra o alinea un texto dentro del marco de asteriscos
void imprimir_marco_texto(string texto, int ancho = 55, bool centrado = false) {
    cout << "* ";
    int espacio_disponible = ancho - 4; // Restamos los bordes "* " y " *"
    
    if (texto.length() > espacio_disponible) {
        texto = texto.substr(0, espacio_disponible);
    }

    int espacios_izq = 0;
    int espacios_der = espacio_disponible - texto.length();

    if (centrado) {
        espacios_izq = espacios_der / 2;
        espacios_der = espacios_der - espacios_izq;
    }

    cout << string(espacios_izq, ' ') << texto << string(espacios_der, ' ') << " *" << endl;
}

// =========================================================================
// VERIFICACIONES DE BASE DE DATOS
// =========================================================================
bool id_existe(MySQLConexion &db, string tabla, int id) {
    string query = "SELECT id FROM " + tabla + " WHERE id = " + to_string(id);
    mysql_query(db.getConnection(), query.c_str());
    MYSQL_RES *res = mysql_store_result(db.getConnection());
    bool existe = (mysql_num_rows(res) > 0);
    mysql_free_result(res);
    return existe;
}

// =========================================================================
// MODELOS ORM
// =========================================================================
class Cliente : public EloquentORM {
public:
    Cliente(MySQLConexion &db) : EloquentORM(db, "clientes", {"id", "nombre", "telefono"}) {}
};

class MenuCombo : public EloquentORM {
public:
    MenuCombo(MySQLConexion &db) : EloquentORM(db, "menu", {"id", "nombre_combo", "precio"}) {}
};

class Orden : public EloquentORM {
public:
    Orden(MySQLConexion &db) : EloquentORM(db, "ordenes", {"id", "fecha", "cantidad", "total", "cliente_id", "menu_id"}) {}
};

// =========================================================================
// VISTAS Y LOGICA CRUD (GESTIÓN DE CLIENTES)
// =========================================================================
void listar_clientes(MySQLConexion &db) {
    system("cls"); // Limpia el rastro del menú anterior
    gotoxy(0, 0);  // Asegura comenzar arriba a la izquierda
    
    Cliente c(db);
    vector<map<string, string>> regs = c.getAll();
    
    Color(BLACK, LCYAN);
    imprimir_marco_linea();
    imprimir_marco_texto("LISTADO DE CLIENTES", 55, true);
    imprimir_marco_linea();
    
    Color(BLACK, WHITE);
    if (regs.empty()) {
        imprimir_marco_texto("No hay clientes registrados en el sistema.");
    } else {
        for (const auto &f : regs) {
            string info = "ID: " + f.at("id") + " | " + f.at("nombre") + " | Tel: " + f.at("telefono");
            imprimir_marco_texto(info);
        }
    }
    imprimir_marco_linea();
    
    Color(BLACK, LGREY);
    cout << "\nPresione cualquier tecla para regresar al menu de clientes...";
    cin.ignore(); cin.get();
}

void gestionar_clientes(MySQLConexion &db) {
    int sub = 0;
    while (sub != 5) {
        system("cls"); // Limpia la pantalla antes de dibujar el menú
        gotoxy(0, 0);  // Reinicia las coordenadas de renderizado
        
        Color(BLACK, YELLOW);
        imprimir_marco_linea();
        imprimir_marco_texto("GESTION DE CLIENTES", 55, true);
        imprimir_marco_linea();
        
        Color(BLACK, LGREEN);
        imprimir_marco_texto(" * [1] Listar Clientes");
        imprimir_marco_texto(" * [2] Registrar Nuevo Cliente");
        imprimir_marco_texto(" * [3] Actualizar Datos de Cliente");
        imprimir_marco_texto(" * [4] Eliminar Cliente");
        Color(BLACK, LRED);
        imprimir_marco_texto(" * [5] Regresar al Menu Principal");
        Color(BLACK, YELLOW);
        imprimir_marco_linea();
        
        Color(BLACK, WHITE);
        cout << "Seleccione una opcion: "; cin >> sub;

        if (sub == 1) {
            listar_clientes(db);
        } 
        else if (sub == 2) {
            system("cls"); gotoxy(0,0);
            int id; string nombre, telefono;
            Color(BLACK, LCYAN);
            imprimir_marco_linea();
            imprimir_marco_texto("REGISTRAR NUEVO CLIENTE", 55, true);
            imprimir_marco_linea();
            
            cout << "\n>>> Ingrese Nuevo ID: "; cin >> id;
            if (id_existe(db, "clientes", id)) {
                Color(BLACK, LRED); cout << "\n[!] Error: El ID ingresado ya existe.\n";
            } else {
                cout << ">>> Nombre: "; cin.ignore(); getline(cin, nombre);
                cout << ">>> Telefono: "; getline(cin, telefono);
                string q = "INSERT INTO clientes (id, nombre, telefono) VALUES (" + to_string(id) + ", '" + nombre + "', '" + telefono + "')";
                if (mysql_query(db.getConnection(), q.c_str()) == 0) {
                    Color(BLACK, LGREEN); cout << "\n[+] Cliente registrado con exito.\n";
                }
            }
            cout << "\nPresione cualquier tecla para continuar..."; cin.get();
        } 
        else if (sub == 3) {
            system("cls"); gotoxy(0,0);
            int id; 
            Color(BLACK, LCYAN);
            imprimir_marco_linea();
            imprimir_marco_texto("ACTUALIZAR CLIENTE", 55, true);
            imprimir_marco_linea();
            
            cout << "\n>>> ID a actualizar: "; cin >> id;
            Cliente c(db);
            if (c.find(id)) {
                int n_id; string nombre, telefono;
                cout << ">>> Nuevo ID: "; cin >> n_id;
                cout << ">>> Nuevo Nombre: "; cin.ignore(); getline(cin, nombre);
                cout << ">>> Nuevo Telefono: "; getline(cin, telefono);
                string q = "UPDATE clientes SET id = " + to_string(n_id) + ", nombre = '" + nombre + "', telefono = '" + telefono + "' WHERE id = " + to_string(id);
                if (mysql_query(db.getConnection(), q.c_str()) == 0) {
                    Color(BLACK, LGREEN); cout << "\n[+] Registro actualizado con exito.\n";
                }
            } else { Color(BLACK, LRED); cout << "\n[!] Error: ID no encontrado.\n"; }
            cout << "\nPresione cualquier tecla para continuar..."; cin.ignore(); cin.get();
        } 
        else if (sub == 4) {
            system("cls"); gotoxy(0,0);
            int id; 
            Color(BLACK, LRED);
            imprimir_marco_linea();
            imprimir_marco_texto("ELIMINAR CLIENTE DEL SISTEMA", 55, true);
            imprimir_marco_linea();
            
            cout << "\n>>> ID de cliente a eliminar: "; cin >> id;
            Cliente c(db);
            if (c.find(id) && c.remove()) {
                Color(BLACK, LGREEN); cout << "\n[+] Cliente eliminado del sistema.\n";
            } else { Color(BLACK, LRED); cout << "\n[!] Error: ID no encontrado o invalido.\n"; }
            cout << "\nPresione cualquier tecla para continuar..."; cin.ignore(); cin.get();
        }
    }
}

// =========================================================================
// VISTAS Y LOGICA CRUD (GESTIÓN DE ÓRDENES)
// =========================================================================
void listar_ordenes(MySQLConexion &db) {
    system("cls"); gotoxy(0,0);
    Orden o(db);
    vector<map<string, string>> regs = o.getAll();
    
    Color(BLACK, LCYAN);
    imprimir_marco_linea();
    imprimir_marco_texto("HISTORIAL DE ORDENES", 55, true);
    imprimir_marco_linea();
    
    Color(BLACK, WHITE);
    if (regs.empty()) {
        imprimir_marco_texto("No se registran ordenes pendientes.");
    } else {
        for (const auto &f : regs) {
            string info = "ID: " + f.at("id") + " | Cant: " + f.at("cantidad") + " | Tot: Q" + f.at("total") + " | C_ID: " + f.at("cliente_id");
            imprimir_marco_texto(info);
        }
    }
    imprimir_marco_linea();
    
    Color(BLACK, LGREY);
    cout << "\nPresione cualquier tecla para regresar al menu de ordenes...";
    cin.ignore(); cin.get();
}

void gestionar_ordenes(MySQLConexion &db) {
    int sub = 0;
    while (sub != 5) {
        system("cls"); gotoxy(0,0);
        
        Color(BLACK, YELLOW);
        imprimir_marco_linea();
        imprimir_marco_texto("GESTION DE ORDENES", 55, true);
        imprimir_marco_linea();
        
        Color(BLACK, LGREEN);
        imprimir_marco_texto(" * [1] Ver Historial");
        imprimir_marco_texto(" * [2] Generar Nueva Orden");
        imprimir_marco_texto(" * [3] Modificar Orden");
        imprimir_marco_texto(" * [4] Cancelar Orden");
        Color(BLACK, LRED);
        imprimir_marco_texto(" * [5] Regresar al Menu Principal");
        Color(BLACK, YELLOW);
        imprimir_marco_linea();
        
        Color(BLACK, WHITE);
        cout << "Seleccione una opcion: "; cin >> sub;

        if (sub == 1) {
            listar_ordenes(db);
        } 
        else if (sub == 2) {
            system("cls"); gotoxy(0,0);
            int id, idCliente, idCombo, cantidad; string fecha;
            Color(BLACK, LCYAN);
            imprimir_marco_linea();
            imprimir_marco_texto("GENERAR NUEVA ORDEN", 55, true);
            imprimir_marco_linea();
            
            cout << "\n>>> ID de Orden: "; cin >> id;
            if (id_existe(db, "ordenes", id)) {
                Color(BLACK, LRED); cout << "[!] Error: La orden ya existe.\n"; 
                cout << "\nPresione cualquier tecla para continuar..."; cin.ignore(); cin.get(); continue;
            }
            cout << ">>> ID Cliente: "; cin >> idCliente;
            if (!id_existe(db, "clientes", idCliente)) {
                Color(BLACK, LRED); cout << "[!] Error: Cliente no existe.\n"; 
                cout << "\nPresione cualquier tecla para continuar..."; cin.ignore(); cin.get(); continue;
            }
            cout << ">>> ID Combo: "; cin >> idCombo;
            if (!id_existe(db, "menu", idCombo)) {
                Color(BLACK, LRED); cout << "[!] Error: Combo inexistente.\n"; 
                cout << "\nPresione cualquier tecla para continuar..."; cin.ignore(); cin.get(); continue;
            }
            cout << ">>> Cantidad: "; cin >> cantidad;
            cout << ">>> Fecha (AAAA-MM-DD): "; cin.ignore(); getline(cin, fecha);

            MenuCombo m(db); double precio = 0.0;
            vector<map<string, string>> combos = m.getAll();
            for (const auto &c : combos) {
                if (stoi(c.at("id")) == idCombo) { precio = stod(c.at("precio")); break; }
            }
            double totalCalculado = precio * cantidad;

            string q = "INSERT INTO ordenes (id, fecha, cantidad, total, cliente_id, menu_id) VALUES (" 
                       + to_string(id) + ", '" + fecha + "', " + to_string(cantidad) + ", " 
                       + to_string(totalCalculado) + ", " + to_string(idCliente) + ", " + to_string(idCombo) + ")";
            
            if (mysql_query(db.getConnection(), q.c_str()) == 0) {
                Color(BLACK, LGREEN); cout << "\n[+] Orden Procesada. Total: Q" << totalCalculado << "\n";
            }
            cout << "\nPresione cualquier tecla para continuar..."; cin.get();
        }
        else if (sub == 3) {
            system("cls"); gotoxy(0,0);
            int id; 
            Color(BLACK, LCYAN);
            imprimir_marco_linea();
            imprimir_marco_texto("MODIFICAR ORDEN EXISTENTE", 55, true);
            imprimir_marco_linea();
            
            cout << "\n>>> ID de orden a modificar: "; cin >> id;
            Orden o(db);
            if (o.find(id)) {
                int n_id, idCliente, idCombo, cantidad; string fecha;
                cout << ">>> Nuevo ID Orden: "; cin >> n_id;
                cout << ">>> Nuevo ID Cliente: "; cin >> idCliente;
                cout << ">>> Nuevo ID Combo: "; cin >> idCombo;
                cout << ">>> Nueva Cantidad: "; cin >> cantidad;
                cout << ">>> Nueva Fecha (AAAA-MM-DD): "; cin.ignore(); getline(cin, fecha);

                MenuCombo m(db); double precio = 0.0;
                vector<map<string, string>> combos = m.getAll();
                for (const auto &c : combos) {
                    if (stoi(c.at("id")) == idCombo) { precio = stod(c.at("precio")); break; }
                }
                double totalCalculado = precio * cantidad;

                string q = "UPDATE ordenes SET id = " + to_string(n_id) + ", fecha = '" + fecha + "', cantidad = " 
                           + to_string(cantidad) + ", total = " + to_string(totalCalculado) + ", cliente_id = " 
                           + to_string(idCliente) + ", menu_id = " + to_string(idCombo) + " WHERE id = " + to_string(id);
                
                if (mysql_query(db.getConnection(), q.c_str()) == 0) {
                    Color(BLACK, LGREEN); cout << "\n[+] Orden actualizada con exito.\n";
                }
            } else { Color(BLACK, LRED); cout << "\n[!] Error: Orden no encontrada.\n"; }
            cout << "\nPresione cualquier tecla para continuar..."; cin.ignore(); cin.get();
        }
        else if (sub == 4) {
            system("cls"); gotoxy(0,0);
            int id; 
            Color(BLACK, LRED);
            imprimir_marco_linea();
            imprimir_marco_texto("CANCELAR ORDEN", 55, true);
            imprimir_marco_linea();
            
            cout << "\n>>> ID de orden a cancelar: "; cin >> id;
            Orden o(db);
            if (o.find(id) && o.remove()) {
                Color(BLACK, LGREEN); cout << "\n[+] Orden cancelada y eliminada.\n";
            } else { Color(BLACK, LRED); cout << "\n[!] Error: No se encontro la orden.\n"; }
            cout << "\nPresione cualquier tecla para continuar..."; cin.ignore(); cin.get();
        }
    }
}

// =========================================================================
// MENU PRINCIPAL
// =========================================================================
int main() {
    MySQLConexion db("root", "admin", "pollos_asados_db", "localhost", 3306);
    if (!db.open()) return 1;

    int opcion = 0;
    while (opcion != 3) {
        system("cls"); // Mantiene el menú principal limpio y fijo en el tope
        gotoxy(0, 0);  // Dibuja desde la esquina superior izquierda
        
        Color(BLACK, LMAGENTA);
        imprimir_marco_linea();
        imprimir_marco_texto("SISTEMA DE CONTROL ACADEMICO (POLLOS ASADOS)", 55, true);
        imprimir_marco_linea();
        
        Color(BLACK, LBLUE);
        imprimir_marco_texto(" * [1] Gestionar Clientes");
        imprimir_marco_texto(" * [2] Gestionar Ordenes");
        Color(BLACK, LRED);
        imprimir_marco_texto(" * [3] Quit / Salir del Programa");
        Color(BLACK, LMAGENTA);
        imprimir_marco_linea();
        
        Color(BLACK, WHITE);
        cout << "Seleccione una opcion: "; cin >> opcion;

        switch (opcion) {
            case 1: gestionar_clientes(db); break;
            case 2: gestionar_ordenes(db); break;
            case 3: 
                system("cls"); gotoxy(0,0);
                Color(BLACK, LRED);
                cout << "*******************************************************\n";
                cout << "* Cerrando modulos de persistencia...              *\n";
                cout << "* Servidor Desconectado con Exito.                 *\n";
                cout << "*******************************************************\n";
                cout << "\nPresione ENTER para cerrar la ventana...";
                cin.ignore(); cin.get(); 
                break;
        }
    }
    db.close();
    return 0;
}