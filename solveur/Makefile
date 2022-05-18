# definition du compilateur
CC = clang
# definition des options de compilation pour obtenir un fichier .o
PRODUCTFLAGS =  -Wall -Wextra -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -I src/dict/dict.h
COMPIL = $(CC) $(PRODUCTFLAGS)

# Deplacement des fichiers dans le bon dossier
MOV = mv *.o obj/

O = obj/
S = src/solver/
IO = src/io/
B = bin/
D = src/dict/
T = src/test/
TEST = -DSNOW_ENABLED

# Pour utiliser faire : 'make exe1'
exe1: main.o
	$(COMPIL) -o $(B)executable_1 main.o interface.o solver.o && $(MOV)
main.o: $(S)main.c interface.o solver.o
	$(COMPIL) -c $(S)main.c
interface.o: $(IO)interface.c $(IO)interface.h
	$(COMPIL) -c $(IO)interface.c
solver.o: $(S)solver.c $(S)solver.h $(D)dict.h
	$(COMPIL) -c $(S)solver.c


test: main_test.o
	$(COMPIL) $(TEST) -o $(B)test main.o && $(MOV)
main_test.o: $(S)main.c
	$(COMPIL) $(TEST) -c $(S)main.c
test_test.o: $(T)test_test.c
	$(COMPIL) $(TEST) -c $(T)test_test.c

# Pour nettoyer les fichiers sources
clean:
	rm -f ./bin/*
	rm -rf ./*.o