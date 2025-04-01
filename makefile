# Nombre del compilador 
CXX = g++

# Opciones de compilación
CXXFLAGS = -std=c++17 

# Archivos fuente
SRC = main.cpp mainMemory.cpp usos.cpp WriteBackCache.cpp WriteThroughCache.cpp 

# Archivos objeto (reemplaza .cpp por .o)
OBJ = $(SRC:.cpp=.o)

# Nombre del ejecutable
TARGET = main_program

# Regla por defecto: compilar el programa
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

# Regla para compilar archivos fuente a objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET) $(OBJ)