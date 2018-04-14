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
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $@.c $(OBJECTS) $(LDLIBS)

$(EJGEN) : $(LIBS)
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $@.c $(OBJECTS) $(LDLIBS)


$(LIBS) ejercicio2_lib:
	$(CC) $(CFLAGS) -o $(LIB_DIR)/$@.o -c $@.c $(LDLIBS)


.PHONY: dist
dist:
	zip -r $(ZIP_NAME).zip $(DIST_PATH)

.PHONY: memtest
memtest: $(EXE)
	valgrind --leak-check=full ./build/ejercicio2 2
	valgrind --leak-check=full ./build/ejercicio2_solved 2
	valgrind --leak-check=full ./build/ejercicio3
	valgrind --leak-check=full ./build/ejercicio4 "comma_separated.txt"
	valgrind --leak.check=full ./build/ejercicio5 "sample.txt" "out.txt"

.PHONY: test
test: $(EXE)
	./build/ejercicio2 2
	./build/ejercicio2_solved 2
	./build/ejercicio3
	./build/ejercicio4 "comma_separated.txt"
	./build/ejercicio5 "sample.txt" "out.txt"

.PHONY: loremipsum
loremipsum: $(EJGEN)
	./build/ejercicio5 "lorem_ipsum.txt" "out.txt"
