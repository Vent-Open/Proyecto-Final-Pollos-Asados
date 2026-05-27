-- 1. Desactivamos la verificación de llaves foráneas para que nos deje borrar todo sin trabas
SET FOREIGN_KEY_CHECKS = 0;

-- 2. Borramos las tablas viejas por completo
DROP TABLE IF EXISTS ordenes;
DROP TABLE IF EXISTS clientes;
DROP TABLE IF EXISTS menu;

-- 3. Volvemos a activar las verificaciones para crear todo con seguridad
SET FOREIGN_KEY_CHECKS = 1;

-- 4. Creamos la tabla de Clientes limpia
CREATE TABLE clientes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    telefono VARCHAR(20)
);

-- 5. Creamos la tabla del Menú de comida
CREATE TABLE menu (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre_combo VARCHAR(100) NOT NULL,
    precio DECIMAL(10, 2) NOT NULL
);

-- 6. Creamos la tabla Órdenes con sus conexiones correctas
CREATE TABLE ordenes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    fecha DATE NOT NULL,
    cantidad INT NOT NULL,
    total DECIMAL(10, 2) NOT NULL,
    cliente_id INT NOT NULL,
    menu_id INT NOT NULL,
    FOREIGN KEY (cliente_id) REFERENCES clientes(id) ON DELETE CASCADE,
    FOREIGN KEY (menu_id) REFERENCES menu(id) ON DELETE CASCADE
);

-- 7. Insertamos los combos de pollos asados
INSERT INTO menu (nombre_combo, precio) VALUES 
('Combo Unitario', 45.00),
('Combo Doble', 85.00),
('Combo Familiar', 145.00);

-- 8. Dejamos un cliente de prueba listo
INSERT INTO clientes (nombre, telefono) VALUES ('Luis', '5555-1234');