-- ============================================================================
-- SCRIPT DE CREACIÓN DE BASE DE DATOS Y TABLAS: POLLOS ASADOS
-- Desarrollador: Jorge Esteban Fausto Martinez 1690-25-697
-- Año: 27/05/2026
-- ============================================================================

-- Crear la base de datos si no existe y seleccionarla
CREATE DATABASE IF NOT EXISTS pollos_asados_db;
USE pollos_asados_db;

-- Eliminar tablas previas en orden inverso (por restricciones de llaves foráneas)
DROP TABLE IF EXISTS ordenes;
DROP TABLE IF EXISTS menu;
DROP TABLE IF EXISTS clientes;

-- Tabla: clientes
CREATE TABLE clientes (
    id INT NOT NULL,
    nombre VARCHAR(100) NOT NULL,
    telefono VARCHAR(20) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Tabla: menu (Combos de comida)
CREATE TABLE menu (
    id INT NOT NULL,
    nombre_combo VARCHAR(100) NOT NULL,
    precio DECIMAL(10,1) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Tabla: ordenes (Une a clientes y menu)
CREATE TABLE ordenes (
    id INT NOT NULL,
    fecha DATE NOT NULL,
    cantidad INT NOT NULL,
    total DECIMAL(10,1) NOT NULL,
    cliente_id INT NOT NULL,
    menu_id INT NOT NULL,
    PRIMARY KEY (id),
    
    -- Definición de Relaciones (Llaves Foráneas que generan las líneas del diagrama)
    CONSTRAINT fk_orden_cliente 
        FOREIGN KEY (cliente_id) REFERENCES clientes(id)
        ON DELETE CASCADE ON UPDATE CASCADE,
        
    CONSTRAINT fk_orden_menu 
        FOREIGN KEY (menu_id) REFERENCES menu(id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Datos de la tabla 'clientes'
INSERT INTO clientes (id, nombre, telefono) VALUES 
(1, 'Jorge', '4567-5670'),
(2, 'Luis Garcia', '8731-1212'),
(3, 'Alexis', '1235-1234');

-- Datos de la tabla 'menu'
INSERT INTO menu (id, nombre_combo, precio) VALUES 
(1, 'Combo Unitario', 45.0),
(2, 'Combo Doble', 85.0),
(3, 'Combo Familiar', 145.0);

-- Datos de la tabla 'ordenes'
INSERT INTO ordenes (id, fecha, cantidad, total, cliente_id, menu_id) VALUES 
(1, '2026-05-27', 2, 170.0, 1, 2), -- Jorge compró 2 Combos Dobles 
(2, '2026-05-27', 1, 145.0, 2, 3); -- Luis García compró 1 Combo Familiar 
