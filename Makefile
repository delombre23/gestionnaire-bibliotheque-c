# Makefile
# Makefile
# Projet : Gestionnaire de Bibliotheque - Langage C
# Prof. Patrick Mukala - UPN L2 Informatique 2025-2026

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS =

# Nom de l'executable
EXEC = bibliotheque

# Fichiers sources et objets
SRC = main.c livre.c emprunteur.c emprunt.c utils.c
OBJ = $(SRC:.c=.o)

# Fichiers d'en-tete
HEADERS = livre.h emprunteur.h emprunt.h utils.h

# Regle par defaut : compilation de l'executable
all: $(EXEC)

# Compilation de l'executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compilation des fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'executable
clean:
	rm -f $(OBJ) $(EXEC)

# Nettoyage complet (incluant les fichiers de donnees)
distclean: clean
	rm -f livres.dat emprunteurs.dat emprunts.dat

# Recompilation complete
rebuild: clean all

# Execution du programme
run: $(EXEC)
	./$(EXEC)

# Aide
help:
	@echo "Makefile - Gestionnaire de Bibliotheque"
	@echo ""
	@echo "Commandes disponibles :"
	@echo "  make         : Compile le projet"
	@echo "  make clean   : Supprime les fichiers objets et l'executable"
	@echo "  make rebuild : Recompile tout depuis zero"
	@echo "  make run     : Compile et execute le programme"
	@echo "  make help    : Affiche cette aide"
