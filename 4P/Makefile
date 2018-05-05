#-----------------------
# IMPORTANTE: Los valores de los parametros de los ejercicio?_test deben ajustarse.
# Se asume que los ficheros fuente se llaman ejercicio1.c, ejercicio2.c,...,ordenar.h
#-----------------------
CC = gcc
CFLAGS = -g
LDLIBS = -lm -pthread
EXE = $(EJGEN)
EJGEN = simular_carreras
LIBS = mylib semaforos rutina_apostador rutina_tirada rutina_monitor rutina_gestor apostador apuesta caballo sim_carr_lib
OBJECTS = $(patsubst %, $(LIB_DIR)/%.o, $^)
LIB_DIR = build/lib
BUILD_DIR = build
BUILDED = $(patsubst %, $(BUILD_DIR)/%, $(EXE))
ZIP_NAME=G2202_P6_4
DIST_PATH=./*

all : $(EXE)

.PHONY : clean
clean :
	rm -f build/**/*.o core $(BUILDED)
	rm -f text/*.txt dat/*.dat *.zip
	rm -rf html/ latex/
.PHONY : doc
doc :
	doxygen Doxyfile

$(EJGEN) : $(LIBS)
	@echo "\033[97m#---------------------------\033[0m"
	@echo "\033[90m# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<\033[0m"
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $@.c $(OBJECTS) $(LDLIBS)

$(LIBS):
	$(CC) $(CFLAGS) -o $(LIB_DIR)/$@.o -c $@.c $(LDLIBS)

.PHONY: dist
dist:
	zip -r $(ZIP_NAME).zip $(DIST_PATH)
