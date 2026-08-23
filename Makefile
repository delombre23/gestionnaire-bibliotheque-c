# Makefile
# Projet : Gestionnaire de Bibliotheque - Langage C
# Prof. Patrick Mukala - UPN L2 Informatique 2025-2026

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS =

EXEC = bibliotheque

SRC = main.c livre.c emprunteur.c emprunt.c utils.c
OBJ = $(SRC:.c=.o)

HEADERS = livre.h emprunteur.h emprunt.h utils.h

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

distclean: clean
	rm -f livres.dat emprunteurs.dat emprunts.dat

rebuild: clean all

run: $(EXEC)
	./$(EXEC)

help:
	@echo "Makefile - Gestionnaire de Bibliotheque"
	@echo ""
	@echo "Commandes disponibles :"
	@echo "  make         : Compile le projet"
	@echo "  make clean   : Supprime les fichiers objets et l'executable"
	@echo "  make rebuild : Recompile tout depuis zero"
	@echo "  make run     : Compile et execute le programme"
	@echo "  make help    : Affiche cette aide"
