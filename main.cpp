/**
 * @file main.cpp
 * @brief Proyecto Final
 * @date 2026-05-21
 * @author Jorge Esteban Fausto Martinez
 */

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include "EloquentORM.h" 

using namespace std;

/**
 * @enum Colores
 * @brief Enumerador para el manejo de colores en la consola de Windows.
 */
enum Colors { 
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5, BROWN = 6, LGREY = 7, 
    DGREY = 8, LBLUE = 9, LGREEN = 10, LCYAN = 11, LRED = 12, LMAGENTA = 13, YELLOW = 14, WHITE = 15
};

/**
 * @brief Cambia el color del texto y fondo en el CMD.
 * @param background Código de color para el fondo.
 * @param text Código de color para el texto.
 */
void cambiar_color(int background, int text) { 
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); 
    int newColor = text + (background * 16);
    SetConsoleTextAttribute(console, newColor); 
}

/**
 * @brief Muestra los encabezados de sección estéticos en la consola.
 * @param titulo Texto que se mostrará centrado/destacado en el encabezado.
 */
void mostrar_encabezado(string titulo) {
    cambiar_color(BLACK, YELLOW);
    cout << "\n========================================" << endl;
    cout << "  " << titulo << endl;
    cout << "========================================" << endl;
    cambiar_color(BLACK, WHITE);
}

/**
 * @class Estudiante
 * @brief Representa la entidad Estudiante mapeada a la tabla 'estudiantes' en la base de datos.
 * 
 * Esta clase gestiona las operaciones CRUD básicas mediante la simulación de EloquentORM.
 */
class Estudiante {
private:
    int id;                      /**< Identificador único del estudiante. */
    string nombreEstudiante;     /**< Nombre completo del estudiante. */
    string correoEstudiante;     /**< Correo electrónico institucional del estudiante. */

public:
    /**
     * @brief Constructor vacío obligatorio para inicializaciones por defecto.
     * 
     * Inicializa las variables miembro con valores nulos o vacíos.
     */
    Estudiante() {
        id = 0;
        nombreEstudiante = "";
        correoEstudiante = "";
    }

    /**
     * @brief Constructor parametrizado para la creación de nuevos registros.
     * @param nombre Nombre completo del estudiante.
     * @param correo Correo electrónico del estudiante.
     */
    Estudiante(string nombre, string correo) {
        id = 0;
        nombreEstudiante = nombre;
        correoEstudiante = correo;
    }

    /**
     * @brief Función CRUD: Inserta un nuevo estudiante usando el framework ORM.
     * @note Actualmente emite un mensaje de éxito simulado por consola.
     */
    void guardar_estudiante() {
        // Lógica de inserción adaptada al framework:
        // ORM::insert("estudiantes").values(nombreEstudiante, correoEstudiante);
        cambiar_color(BLACK, LGREEN);
        cout << "\n[OK] Estudiante guardado exitosamente." << endl;
    }

    /**
     * @brief Función CRUD estática: Muestra todos los estudiantes registrados.
     * Imprime una lista formateada en pantalla. Actualmente utiliza datos estáticos de prueba.
     */
    static void listar_estudiantes() {
        mostrar_encabezado("LISTA DE ESTUDIANTES REGISTRADOS");
        cout << "ID\tNombre\t\tCorreo" << endl;
        cout << "----------------------------------------" << endl;
        //cout << "" << endl; Ejemplo de salida estática temporal
    }

    /**
     * @brief Función CRUD estática: Modifica los datos de un estudiante existente por ID.
     * @param idBuscar ID del estudiante que se desea actualizar.
     * @param nuevoNombre Nuevo nombre que reemplazará al anterior.
     * @param nuevoCorreo Nuevo correo electrónico que reemplazará al anterior.
     */
    static void actualizar_estudiante(int idBuscar, string nuevoNombre, string nuevoCorreo) {
        // Lógica de actualización: ORM::update("estudiantes").set(...).where("id", idBuscar);
        cambiar_color(BLACK, LGREEN);
        cout << "\n[OK] Datos del estudiante actualizados." << endl;
    }

    /**
     * @brief Función CRUD estática: Elimina un registro de estudiante por su ID.
     * @param idBuscar ID del estudiante que se dará de baja.
     */
    static void eliminar_estudiante(int idBuscar) {
        // Lógica de eliminación: ORM::remove("estudiantes").where("id", idBuscar);
        cambiar_color(BLACK, LRED);
        cout << "\n[OK] Estudiante eliminado de la base de datos." << endl;
    }
};

/**
 * @brief Función auxiliar para la gestión del flujo del menú de estudiantes.
 * 
 * Despliega las opciones de la sección de alumnos (Crear, Leer, Actualizar, Eliminar)
 * y captura las entradas del usuario.
 */
void gestionar_estudiantes() {
    int opcionSubmenu = 0;
    do {
        mostrar_encabezado("MENU: GESTION DE ESTUDIANTES");
        cout << "1. Registrar Estudiante" << endl;
        cout << "2. Ver Todos los Estudiantes" << endl;
        cout << "3. Modificar Estudiante" << endl;
        cout << "4. Eliminar Estudiante" << endl;
        cout << "5. Regresar al Menu Principal" << endl;
        cout << "\nSeleccione una opcion: ";
        cin >> opcionSubmenu;

        string nombreInput, correoInput;
        int idInput;

        switch (opcionSubmenu) {
            case 1:
                cout << "Ingrese el nombre del estudiante: ";
                cin.ignore();
                getline(cin, nombreInput);
                cout << "Ingrese el correo electronico: ";
                getline(cin, correoInput);
                {
                    Estudiante nuevoEstudiante(nombreInput, correoInput);
                    nuevoEstudiante.guardar_estudiante();
                }
                break;
            case 2:
                Estudiante::listar_estudiantes();
                break;
            case 3:
                cout << "Ingrese el ID del estudiante a modificar: ";
                cin >> idInput;
                cout << "Ingrese el nuevo nombre: ";
                cin.ignore();
                getline(cin, nombreInput);
                cout << "Ingrese el nuevo correo: ";
                getline(cin, correoInput);
                Estudiante::actualizar_estudiante(idInput, nombreInput, correoInput);
                break;
            case 4:
                cout << "Ingrese el ID del estudiante a eliminar: ";
                cin >> idInput;
                Estudiante::eliminar_estudiante(idInput);
                break;
        }
    } while (opcionSubmenu != 5);
}

/**
 * @brief Punto de entrada principal de la aplicación por consola.
 * 
 * Inicializa la interfaz de usuario, simula la conexión a la base de datos MySQL
 * y controla el bucle del menú principal.
 * @return int Código de estado de finalización del programa (0 para éxito).
 */
int main() {
    int opcionPrincipal = 0;

    // Simulación de verificación de la conexión a la base de datos
    mostrar_encabezado("SISTEMA DE CONTROL ACADEMICO UMG");
    cambiar_color(BLACK, LGREEN);
    cout << "Conectando a MySQL mediante EloquentORM..." << endl;
    cout << "[CONEXION EXITOSA]" << endl;
    cambiar_color(BLACK, WHITE);

    do {
        mostrar_encabezado("MENU PRINCIPAL");
        cout << "1. Gestionar Estudiantes" << endl;
        cout << "2. Gestionar Cursos (Proximamente)" << endl;
        cout << "3. Asignar Cursos (Tabla Intermedia)" << endl;
        cout << "4. Salir del Sistema" << endl;
        cout << "\nSeleccione una opcion: ";
        cin >> opcionPrincipal;

        switch (opcionPrincipal) {
            case 1:
                gestionar_estudiantes();
                break;
            case 2:
                cambiar_color(BLACK, LBLUE);
                cout << "\n[INFO] Esta seccion seguira la misma estructura que estudiantes." << endl;
                break;
            case 3:
                cambiar_color(BLACK, LBLUE);
                cout << "\n[INFO] Aqui se guardaran las llaves foraneas (estudiante_id y curso_id)." << endl;
                break;
            case 4:
                cambiar_color(BLACK, YELLOW); // Corregido el original 'LYELLOW' que no existe en tu enum (es YELLOW)
                cout << "\nSaliendo del programa..." << endl;
                break;
            default:
                cambiar_color(BLACK, LRED);
                cout << "\n[ERROR] Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcionPrincipal != 4);

    cambiar_color(BLACK, WHITE);
    return 0;
}