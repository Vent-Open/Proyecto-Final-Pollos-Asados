/**
 * @file main.cpp
 * @brief Sistema de administración de venta de Pollos Asados.
 * @details Utiliza EloquentORM para la persistencia en MySQL y la API de Windows 
 * para el diseño estético de interfaz de usuario en consola.
 * @author Jorge Esteban Fausto Martinez 1690_25-697
 * @date 27/05/2026
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

/**
 * @brief Enumeración de colores estándar de la consola de Windows.
 */
enum Colors {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5, BROWN = 6, LGREY = 7,
    DGREY = 8, LBLUE = 9, LGREEN = 10, LCYAN = 11, LRED = 12, LMAGENTA = 13, YELLOW = 14, WHITE = 15
};

/**
 * @brief Cambia el color del texto y del fondo de la consola de comandos.
 * @param Background Código de color para el fondo de la pantalla.
 * @param Text Código de color para los caracteres del texto.
 */
void Color(int Background, int Text) {
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
    int New_Color = Text + (Background * 16);
    SetConsoleTextAttribute(Console, New_Color);
}

/**
 * @brief Mueve el cursor de la consola a una coordenada (X, Y) específica.
 * @param x Posición en el eje horizontal (columnas).
 * @param y Posición en el eje vertical (filas).
 */
void gotoxy(int x, int y) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

/**
 * @brief Imprime una línea horizontal compuesta por asteriscos para delimitar marcos.
 * @param ancho Cantidad de caracteres de largo que tendrá la línea (por defecto 55).
 */
void imprimir_marco_linea(int ancho = 55) {
    cout << string(ancho, '*') << endl;
}

/**
 * @brief Ajusta, alinea e imprime una cadena de texto dentro de un marco de asteriscos.
 * @param texto Cadena de caracteres que se desea mostrar.
 * @param ancho Ancho total de la ventana o del recuadro (por defecto 55).
 * @param centrado Define si el texto debe posicionarse al centro del marco.
 */
void imprimir_marco_texto(string texto, int ancho = 55, bool centrado = false) {
    cout << "* ";
    int espacio_disponible = ancho - 4; 
    
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

/**
 * @brief Realiza una consulta directa a MySQL para verificar la existencia de una llave primaria.
 * @param db Instancia de la conexión activa a la base de datos.
 * @param tabla Nombre de la tabla en la que se realizará la búsqueda.
 * @param id Llave primaria (ID) numérica que se desea validar.
 * @return true Si el ID ya existe en la base de datos.
 * @return false Si el ID se encuentra disponible.
 */
bool id_existe(MySQLConexion &db, string tabla, int id) {
    string query = "SELECT id FROM " + tabla + " WHERE id = " + to_string(id);
    mysql_query(db.getConnection(), query.c_str());
    MYSQL_RES *res = mysql_store_result(db.getConnection());
    bool existe = (mysql_num_rows(res) > 0);
    mysql_free_result(res);
    return existe;
}


/**
 * @class Cliente
 * @brief Modelo encargado de gestionar el mapeo y persistencia de la tabla 'clientes'.
 */
class Cliente : public EloquentORM {
public:
    /**
     * @brief Constructor que inicializa el modelo vinculando la tabla y sus columnas.
     * @param db Conexión a MySQL.
     */
    Cliente(MySQLConexion &db) : EloquentORM(db, "clientes", {"id", "nombre", "telefono"}) {}
};

/**
 * @class MenuCombo
 * @brief Modelo encargado de gestionar el mapeo y persistencia de la tabla 'menu'.
 */
class MenuCombo : public EloquentORM {
public:
    /**
     * @brief Constructor que inicializa el modelo vinculando la tabla y sus columnas de costos.
     * @param db Conexión a MySQL.
     */
    MenuCombo(MySQLConexion &db) : EloquentORM(db, "menu", {"id", "nombre_combo", "precio"}) {}
};

/**
 * @class Orden
 * @brief Modelo encargado de gestionar el mapeo y persistencia de la tabla 'ordenes'.
 */
class Orden : public EloquentORM {
public:
    /**
     * @brief Constructor que inicializa el modelo vinculando la tabla y sus llaves foráneas.
     * @param db Conexión a MySQL.
     */
    Orden(MySQLConexion &db) : EloquentORM(db, "ordenes", {"id", "fecha", "cantidad", "total", "cliente_id", "menu_id"}) {}
};


/**
 * @brief Recupera todos los registros de los clientes y los renderiza en un formato de lista enmarcada.
 * @param db Conexión activa a la base de datos.
 */
void listar_clientes(MySQLConexion &db) {
    system("cls"); 
    gotoxy(0, 0);  
    
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

/**
 * @brief Despliega el submenú de gestión de clientes (CRUD) y captura las entradas del usuario.
 * @param db Conexión activa a la base de datos.
 */
void gestionar_clientes(MySQLConexion &db) {
    int sub = 0;
    while (sub != 5) {
        system("cls"); 
        gotoxy(0, 0);  
        
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


/**
 * @brief Muestra el historial completo de las ventas procesadas en la base de datos.
 * @param db Conexión activa a la base de datos.
 */
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

/**
 * @brief Gestiona el flujo del CRUD de órdenes calculando montos automáticamente según el menú de combos.
 * @param db Conexión activa a la base de datos.
 */
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
            int id, idCliente, idCombo, quantity; string fecha;
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
            cout << ">>> Cantidad: "; cin >> quantity;
            cout << ">>> Fecha (AAAA-MM-DD): "; cin.ignore(); getline(cin, fecha);

            MenuCombo m(db); double precio = 0.0;
            vector<map<string, string>> combos = m.getAll();
            for (const auto &c : combos) {
                if (stoi(c.at("id")) == idCombo) { precio = stod(c.at("precio")); break; }
            }
            double totalCalculado = precio * quantity;

            string q = "INSERT INTO ordenes (id, fecha, cantidad, total, cliente_id, menu_id) VALUES (" 
                       + to_string(id) + ", '" + fecha + "', " + to_string(quantity) + ", " 
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
                int n_id, idCliente, idCombo, quantity; string fecha;
                cout << ">>> Nuevo ID Orden: "; cin >> n_id;
                cout << ">>> Nuevo ID Cliente: "; cin >> idCliente;
                cout << ">>> Nuevo ID Combo: "; cin >> idCombo;
                cout << ">>> Nueva Cantidad: "; cin >> quantity;
                cout << ">>> Nueva Fecha (AAAA-MM-DD): "; cin.ignore(); getline(cin, fecha);

                MenuCombo m(db); double precio = 0.0;
                vector<map<string, string>> combos = m.getAll();
                for (const auto &c : combos) {
                    if (stoi(c.at("id")) == idCombo) { precio = stod(c.at("precio")); break; }
                }
                double totalCalculado = precio * quantity;

                string q = "UPDATE ordenes SET id = " + to_string(n_id) + ", fecha = '" + fecha + "', cantidad = " 
                           + to_string(quantity) + ", total = " + to_string(totalCalculado) + ", cliente_id = " 
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

/**
 * @brief Función principal que inicializa la conexión a la DB y controla el bucle del menú maestro.
 * @return int Código de estado de salida (0 para finalización exitosa, 1 para fallos de conexión).
 */
int main() {
    MySQLConexion db("root", "admin", "pollos_asados_db", "localhost", 3306);
    if (!db.open()) return 1;

    int opcion = 0;
    while (opcion != 3) {
        system("cls"); 
        gotoxy(0, 0);  
        
        Color(BLACK, YELLOW);
        imprimir_marco_linea();
        imprimir_marco_texto("Control De Ventas de los Pollos Asados)", 55, true);
        imprimir_marco_linea();
        
        Color(BLACK, LBLUE);
        imprimir_marco_texto(" * [1] Gestionar Clientes");
        imprimir_marco_texto(" * [2] Gestionar Ordenes");
        Color(BLACK, LRED);
        imprimir_marco_texto(" * [3] Quit / Salir del Programa");
        Color(BLACK, YELLOW);
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
