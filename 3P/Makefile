#-----------------------
# IMPORTANTE: Los valores de los parametros de los ejercicio?_test deben ajustarse.
# Se asume que los ficheros fuente se llaman ejercicio1.c, ejercicio2.c,...,ordenar.h
#-----------------------
CC = gcc
CFLAGS = -g
LDLIBS = -lm -pthread
EXE = ejercicio2 ejercicio2_solved ejercicio3 ejercicio4
LIBS = mylib semaforos
LIB_DIR = build/lib
BUILD_DIR = build
ZIP_NAME=G2202_P6_3
DIST_PATH=./*

all : $(EXE)

.PHONY : clean
clean :
	rm -f build/**/*.o core $(BUILD_DIR)/$(EXE)
	rm -f text/*.txt dat/*.dat *.zip
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
