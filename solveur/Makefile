# definition du compilateur
CC = clang
# definition des options de compilation pour obtenir un fichier .o
PRODUCTFLAGS =  -Wall -Wextra -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
#PRODUCTFLAGS += -I src/dict/ -I src/solver/ -I src/io/ -I src/utils/test/
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
exe1: main.o interface.o solver.o attempts_tools.o dict.o
	$(COMPIL) -o $(B)executable_1 main.o interface.o solver.o attempts_tools.o && $(MOV)
main.o:
	$(COMPIL) -c $(S)main.c

interface.o:
	$(COMPIL) -c $(IO)interface.c
solver.o:
	$(COMPIL) -c $(S)solver.c
attempts_tools.o:
	$(COMPIL) -c $(S)attempts_tools.c
dict.o:
	$(COMPIL) -c $(D)dict.c


test: main_test.o attempts_tools.o dict.o solver.o interface.o main.o
	$(COMPIL) $(TEST) -o $(B)test main_test.o dict.o attempts_tools.o solver.o && $(MOV)
main_test.o:
	$(COMPIL) $(TEST) -c $(S)main.c	&& mv ./main.o ./main_test.o

#test_test.o: $(T)test_test.c
#	$(COMPIL) $(TEST) -c $(T)test_test.c
#test_attempts_tools.o: $(T)test_attempts_tools.c
#	$(COMPIL) $(TEST) -c $(T)test_attempts_tools.c

# Pour nettoyer les fichiers sources
clean:
	rm -f ./bin/*
	rm -rf ./*.o
	rm -rf ./obj/*.o