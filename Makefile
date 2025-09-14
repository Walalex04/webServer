CC = gcc
CFLAGS = -Wall -g -Iincludes

# Directorios
SRC_DIR = src
TARGET_DIR = target
TARGET = $(TARGET_DIR)/programa

# Archivos fuente (ajusta estos nombres)
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/util.c

# Regla principal
$(TARGET): $(SRC_FILES)
	@mkdir -p $(TARGET_DIR)  # Crea la carpeta target si no existe
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES)
	@echo "Programa compilado en: $(TARGET)"

# Limpiar
clean:
	rm -rf $(TARGET_DIR)
	@echo "Carpeta target limpiada"

# Ayuda
help:
	@echo "Targets disponibles:"
	@echo "  make       - Compila el programa en target/"
	@echo "  make clean - Limpia la carpeta target"
	@echo "  make help  - Muestra esta ayuda"