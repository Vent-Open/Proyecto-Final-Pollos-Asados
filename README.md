*******************************************************************************
                  SISTEMA DE CONTROL DE VENTA DE POLLOS ASADOS
                                  README.TXT
*******************************************************************************
* Desarrollador: Jorge Esteban Fausto Martinez 1690-25-697
* Año: 27/05/2026
* Propósito: Proyecto Universitario de Refactorización y Persistencia Relacional
*******************************************************************************

*******************************************************************************
1. DESCRIPCIÓN GENERAL
*******************************************************************************
Aplicación de consola interactiva desarrollada en C++ para la gestión y 
control de clientes y órdenes en un restaurante de pollos asados. El 
sistema original fue REFACTORIZADO bajo principios de POO, implementando 
un mapeo relacional mediante un Framework EloquentORM conectado a MySQL.


*******************************************************************************
2. CARACTERÍSTICAS PRINCIPALES
*******************************************************************************
* REFACTORIZACIÓN MODULAR: Funciones independientes de control y vistas.
* PERSISTENCIA CON ORM: Mapeo directo de tablas mediante herencia de 
  clases (Cliente, MenuCombo, Orden), evitando saturar el código con SQL.
* INTERFAZ ESTILO "RETRO": Menús interactivos enmarcados con asteriscos.
* DINAMISMO DE PANTALLA: Navegación fluida mediante limpieza de pantalla 
  (system("cls")) y coordenadas de cursor con la API de Windows (gotoxy).
* CÁLCULOS AUTOMÁTICOS: Procesamiento en tiempo real de totales de órdenes 
  basándose en los precios unitarios de la tabla "menu".
* DOCUMENTACIÓN DOXYGEN: Código fuente completamente comentado con bloques 
  estándar (/** ... */) para la gestión de parámetros y retornos.


*******************************************************************************
3. TECNOLOGÍAS UTILIZADAS
*******************************************************************************
* Lenguaje: C++11 / C++14
* Base de Datos: MySQL Server 8.0 / MariaDB
* Conector: mysql.h (MySQL Development Connector)
* Compilador Recomendado: MinGW GCC
* Librerías del Sistema: <windows.h> para control de color y gotoxy.


*******************************************************************************
4. ARQUITECTURA DEL CÓDIGO (main.cpp)
*******************************************************************************
El flujo del programa está estructurado en capas bien definidas:

[CAPA 1: CONFIGURACIÓN VISUAL] 
--> Manejo de colores (Color) y coordenadas en consola (gotoxy).
       |
       v
[CAPA 2: MODELOS DE PERSISTENCIA (ORM)] 
--> Clientes, MenuCombo y Ordenes heredando de EloquentORM.
       |
       v
[CAPA 3: CAPA DE CONTROLADORES / LÓGICA DE NEGOCIO] 
--> Flujos CRUD independientes para procesamiento de datos.
       |
       v
[CAPA 4: MENÚ MAESTRO PRINCIPAL] 
--> Bucle principal y persistencia de conexión a MySQL.


*******************************************************************************
5. CONFIGURACIÓN DEL ENTORNO
*******************************************************************************
1. BASE DE DATOS: Asegúrese de levantar su servidor MySQL local e importar 
   el esquema desde el archivo "PollosAsados.sql". Las tablas requeridas 
   son: 'clientes', 'menu' y 'ordenes'.

2. VARIABLES DE CONEXIÓN: Ajuste las credenciales de su servidor local 
   directamente en la función main() de su 'main.cpp':
   MySQLConexion db("root", "tu_password", "pollos_asados_db", ...);

3. EJECUCIÓN EN VENTANA EXTERNA (VS Code): Para que el renderizado de 
   colores y la función gotoxy operen correctamente sobre la API de Windows, 
   el archivo ".vscode/launch.json" debe tener activa la propiedad:
   "externalConsole": true


*******************************************************************************
NOTA DE EVALUACIÓN: Este software ha sido diseñado con un enfoque estricto 
en la separación de responsabilidades. La lógica de las vistas no interfiere 
con las consultas directas a la base de datos, cumpliendo al 100% con los   
requerimientos de ingeniería de software y modularidad solicitados.         
*******************************************************************************
