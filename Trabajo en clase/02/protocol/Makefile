# Compilador
CXX := g++
CXXFLAGS := -Wall -g -c -MMD -MP -I.

# Archivos fuente
SOURCES := $(wildcard *.cc)
OBJECTS := $(SOURCES:.cc=.o)

# Nombre del ejecutable
TARGET := programa.out

# Regla por defecto
all: $(TARGET)

# Linkeo final
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

# Compilación de cada .cc -> .o con dependencias
%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Incluir dependencias de headers generadas con -MMD
-include $(OBJECTS:.o=.d)

# Limpieza
clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d) $(TARGET)

.PHONY: all clean