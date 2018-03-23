#-----------------------
# IMPORTANTE: Los valores de los parametros de los ejercicio?_test deben ajustarse.
# Se asume que los ficheros fuente se llaman ejercicio1.c, ejercicio2.c,...,ordenar.h
#-----------------------
CC = gcc
CFLAGS = -g
LDLIBS = -lm -pthread
EXE = ejercicio4a ejercicio4b ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9 ejercicio12a ejercicio12b ejercicio13a ejercicio13b
LIBS = ejercicio12lib ejercicio13lib
LIB_DIR = build/lib
BUILD_DIR = build
ZIP_NAME=G2202_P6?_1
DIST_PATH=./*

all : $(EXE)

.PHONY : clean
clean :
	rm -f build/*.o core $(EXE)
	rm -rf html/ latex/
.PHONY : doc
doc :
	doxygen Doxyfile

$(EXE) : $(LIBS)
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $@.c $(LIB_DIR)/* $(LDLIBS)

$(LIBS) :
	$(CC) $(CFLAGS) -o $(LIB_DIR)/$@.o -c $@.c $(LDLIBS)

.PHONY: dist
dist:
	zip -r $(ZIP_NAME).zip $(DIST_PATH)
