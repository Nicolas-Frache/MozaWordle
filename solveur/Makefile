# definition du compilateur
CC = clang
# definition des options de compilation pour obtenir un fichier .o
PRODUCTFLAGS = -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -I src/dict/dict.h
COMPIL = $(CC) $(PRODUCTFLAGS)

# Deplacement des fichiers dans le bon dossier
MOV = mv *.o obj/
O = obj/
SOLVER = src/solver/
IO = src/io/
B = bin/
DICT = src/dict/

# Pour utiliser faire : 'make exe1'


# Ex1 - 1 - Tests manuels
exe1: main.o interface.o solver.o
	$(COMPIL) -o $(B)executable_1 main.o interface.o solver.o && $(MOV)
main.o: $(SOLVER)main.c
	$(COMPIL) -c $(SOLVER)main.c
interface.o: $(IO)interface.c $(IO)interface.h
	$(COMPIL) -c $(IO)interface.c
solver.o: $(SOLVER)solver.c $(SOLVER)solver.h $(DICT)dict.h
	$(COMPIL) -c $(SOLVER)solver.c



# Pour nettoyer les fichiers sources
clean:
	rm -f ./bin/*