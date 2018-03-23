#-----------------------
# IMPORTANTE: Los valores de los parametros de los ejercicio?_test deben ajustarse.
# Se asume que los ficheros fuente se llaman ejercicio1.c, ejercicio2.c,...,ordenar.h
#-----------------------
CC = gcc
CFLAGS = -g
LDLIBS = -lm -pthread
EXE = ejercicio2 ejercicio4 ejercicio6a ejercicio6b
BUILD_DIR = build

all : $(EXE)

.PHONY : clean
clean :
	rm -f build/*.o core $(EXE)
	rm -rf html/ latex/
.PHONY : doc
doc :
	doxygen Doxyfile

$(EXE) :
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $@.c $(LDLIBS)
