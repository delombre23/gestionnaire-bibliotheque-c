/*
 * utils.c
 * Implementation des utilitaires communs
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

void effacer_ecran(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void ligne_separation(void) {
    printf("+------------------------------------------------------------------+\n");
}

void afficher_titre(const char *titre) {
    int len = (int)strlen(titre);
    int padding = (66 - len) / 2;
    ligne_separation();
    printf("|");
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", titre);
    for (int i = 0; i < (66 - len - padding); i++) printf(" ");
    printf("|\n");
    ligne_separation();
}

void afficher_erreur(const char *message) {
    printf("  [ERREUR] %s\n", message);
}

void afficher_succes(const char *message) {
    printf("  [OK] %s\n", message);
}

void afficher_info(const char *message) {
    printf("  [INFO] %s\n", message);
}

void pause_console(void) {
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
}

void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lire_chaine(char *buffer, int taille) {
    if (fgets(buffer, taille, stdin) == NULL) {
        return 0;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        vider_buffer();
    }
    if (strlen(buffer) == 0) {
        return 0;
    }
    return 1;
}

int lire_entier(const char *invite) {
    int valeur;
    char buffer[TAILLE_BUFFER];
    while (1) {
        printf("%s", invite);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &valeur) == 1) {
                return valeur;
            }
        }
        afficher_erreur("Veuillez entrer un nombre entier valide.");
    }
}

int lire_entier_positif(const char *invite) {
    int valeur;
    while (1) {
        valeur = lire_entier(invite);
        if (valeur > 0) {
            return valeur;
        }
        afficher_erreur("Veuillez entrer un nombre entier positif.");
    }
}

int demander_confirmation(const char *message) {
    char reponse[TAILLE_BUFFER];
    printf("%s (O/N) : ", message);
    if (lire_chaine(reponse, sizeof(reponse))) {
        if (reponse[0] == 'O' || reponse[0] == 'o') {
            return 1;
        }
    }
    return 0;
}

void minuscules(char *chaine) {
    for (int i = 0; chaine[i]; i++) {
        chaine[i] = (char)tolower((unsigned char)chaine[i]);
    }
}

int contient_sous_chaine(const char *texte, const char *recherche) {
    char t[TAILLE_BUFFER];
    char r[TAILLE_BUFFER];
    strncpy(t, texte, sizeof(t) - 1);
    t[sizeof(t) - 1] = '\0';
    strncpy(r, recherche, sizeof(r) - 1);
    r[sizeof(r) - 1] = '\0';
    minuscules(t);
    minuscules(r);
    return (strstr(t, r) != NULL);
}

void afficher_menu_principal(void) {
    printf("\n");
    afficher_titre("MENU PRINCIPAL");
    printf("|  1. Gestion des livres                                           |\n");
    printf("|  2. Gestion des emprunteurs                                      |\n");
    printf("|  3. Gestion des emprunts                                         |\n");
    printf("|  4. Rapports et statistiques                                     |\n");
    printf("|  5. Sauvegarder toutes les donnees                               |\n");
    printf("|  0. Quitter l'application                                        |\n");
    ligne_separation();
}

void afficher_menu_livres(void) {
    printf("\n");
    afficher_titre("GESTION DES LIVRES");
    printf("|  1. Ajouter un livre                                             |\n");
    printf("|  2. Modifier un livre                                            |\n");
    printf("|  3. Supprimer un livre                                           |\n");
    printf("|  4. Rechercher un livre par titre                                |\n");
    printf("|  5. Rechercher un livre par auteur                               |\n");
    printf("|  6. Rechercher un livre par ISBN                                 |\n");
    printf("|  7. Afficher tous les livres                                     |\n");
    printf("|  8. Afficher les livres disponibles                              |\n");
    printf("|  0. Retour au menu principal                                     |\n");
    ligne_separation();
}

void afficher_menu_emprunteurs(void) {
    printf("\n");
    afficher_titre("GESTION DES EMPRUNTEURS");
    printf("|  1. Ajouter un emprunteur                                        |\n");
    printf("|  2. Modifier un emprunteur                                       |\n");
    printf("|  3. Supprimer un emprunteur                                      |\n");
    printf("|  4. Rechercher un emprunteur par nom                             |\n");
    printf("|  5. Rechercher un emprunteur par numero de carte                 |\n");
    printf("|  6. Afficher tous les emprunteurs                                |\n");
    printf("|  0. Retour au menu principal                                     |\n");
    ligne_separation();
}

void afficher_menu_emprunts(void) {
    printf("\n");
    afficher_titre("GESTION DES EMPRUNTS");
    printf("|  1. Emprunter un livre                                           |\n");
    printf("|  2. Retourner un livre                                           |\n");
    printf("|  3. Afficher les emprunts en cours                               |\n");
    printf("|  4. Afficher les emprunts en retard                              |\n");
    printf("|  5. Historique des emprunts par emprunteur                       |\n");
    printf("|  6. Historique complet des emprunts                              |\n");
    printf("|  0. Retour au menu principal                                     |\n");
    ligne_separation();
}

void afficher_menu_rapports(void) {
    printf("\n");
    afficher_titre("RAPPORTS ET STATISTIQUES");
    printf("|  1. Statistiques generales                                       |\n");
    printf("|  2. Livres les plus empruntes                                    |\n");
    printf("|  3. Emprunteurs les plus actifs                                  |\n");
    printf("|  4. Total des amendes collectees                                 |\n");
    printf("|  0. Retour au menu principal                                     |\n");
    ligne_separation();
}

void afficher_en_tete(void) {
    printf("\n");
    ligne_separation();
    printf("|                                                                  |\n");
    printf("|         GESTIONNAIRE DE BIBLIOTHEQUE v1.0                        |\n");
    printf("|                                                                  |\n");
    printf("|         UPN - L2 Informatique - Prof. Patrick Mukala             |\n");
    printf("|                                                                  |\n");
    ligne_separation();
    printf("\n");
}
