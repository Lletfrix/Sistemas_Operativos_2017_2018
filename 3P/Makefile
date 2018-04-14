#-----------------------
# IMPORTANTE: Los valores de los parametros de los ejercicio?_test deben ajustarse.
# Se asume que los ficheros fuente se llaman ejercicio1.c, ejercicio2.c,...,ordenar.h
#-----------------------
CC = gcc
CFLAGS = -g
LDLIBS = -lm -pthread
EXE = $(EJ2) $(EJGEN)
EJ2 = ejercicio2 ejercicio2_solved
EJGEN = ejercicio3 ejercicio4 ejercicio5
LIBS = mylib semaforos
OBJECTS = $(patsubst %, $(LIB_DIR)/%.o, $^)
LIB_DIR = build/lib
BUILD_DIR = build
BUILDED = $(patsubst %, $(BUILD_DIR)/%, $(EXE))
ZIP_NAME=G2202_P6_3
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

$(EJ2) : $(LIBS) ejercicio2_lib
	@echo "\033[97m#---------------------------\033[0m"
	@echo "\033[90m# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<\033[0m"
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $@.c $(OBJECTS) $(LDLIBS)

$(EJGEN) : $(LIBS)
	@echo "\033[97m#---------------------------\033[0m"
	@echo "\033[90m# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<\033[0m"
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $@.c $(OBJECTS) $(LDLIBS)


$(LIBS) ejercicio2_lib:
	$(CC) $(CFLAGS) -o $(LIB_DIR)/$@.o -c $@.c $(LDLIBS)


.PHONY: dist
dist:
	zip -r $(ZIP_NAME).zip $(DIST_PATH)

.PHONY: memtest
memtest: $(EXE)

	@echo "\033[93mEjecutando Ejercicio 2 con Valgrind"
	@echo "#---------------------------"
	valgrind --leak-check=full ./build/ejercicio2 3
	@echo "\033[94mEjecutando Ejercicio 2_solved con Valgrind"
	@echo "#---------------------------"
	valgrind --leak-check=full ./build/ejercicio2_solved 3
	@echo "\033[95mEjecutando Ejercicio 3 con Valgrind"
	@echo "#---------------------------"
	valgrind --leak-check=full ./build/ejercicio3
	@echo "\033[96mEjecutando Ejercicio 4 con Valgrind"
	@echo "#---------------------------"
	valgrind --leak-check=full ./build/ejercicio4 "comma_separated.txt"
	@echo "\033[91mEjecutando Ejercicio 5 con Valgrind"
	@echo "#---------------------------"
	valgrind --leak-check=full ./build/ejercicio5 "sample.txt" "out.txt"
	@echo "\033[0m"

.PHONY: test
test: $(EXE)
	@echo "\033[93mEjecutando Ejercicio 2"
	@echo "#---------------------------"
	./build/ejercicio2 3
	@echo "\033[94mEjecutando Ejercicio 2_solved"
	@echo "#---------------------------"
	./build/ejercicio2_solved 3
	@echo "\033[95mEjecutando Ejercicio 3"
	@echo "#---------------------------"
	./build/ejercicio3
	@echo "\033[96mEjecutando Ejercicio 4"
	@echo "#---------------------------"
	./build/ejercicio4 "comma_separated.txt"
	@echo "\033[91mEjecutando Ejercicio 5"
	@echo "#---------------------------"
	./build/ejercicio5 "sample.txt" "out.txt"
	@echo "\033[0m"
	@echo "--> Imprimiendo sample.txt"
	@cat sample.txt
	@echo "--> Imprimiendo out.txt"
	@cat out.txt

.PHONY: loremipsum
loremipsum: $(EJGEN)
	./build/ejercicio5 "lorem_ipsum.txt" "out.txt"
