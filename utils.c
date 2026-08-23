/*
 * utils.c
 * Implementation des utilitaires communs
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 *
 * Ce module fournit les fonctions utilitaires pour l'interface utilisateur :
 * - Affichage des menus et titres
 * - Saisie et validation des entrees utilisateur
 * - Gestion de l'ecran (portable, sans system())
 * - Fonctions de manipulation de chaines
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

/*
 * Efface l'ecran de la console de maniere portable.
 * Utilise les sequences ANSI d'echappement (compatible Linux, macOS, Windows 10+).
 * Avantage : pas d'appel a system() qui est couteux et non securise.
 */
void effacer_ecran(void) {
    /* Sequence ANSI : efface l'ecran et repositionne le curseur en haut a gauche */
    printf("\033[2J\033[H");
    fflush(stdout);
}

/*
 * Affiche une ligne de separation pour delimiter les sections.
 * Utilisee pour encadrer les titres et les tableaux.
 */
void ligne_separation(void) {
    printf("+------------------------------------------------------------------+\n");
}

/*
 * Affiche un titre centre avec encadrement decoratif.
 * Le titre est centre dans une ligne de 66 caracteres.
 */
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

/*
 * Affiche un message d'erreur formate.
 * Prefixe par [ERREUR] pour une identification rapide.
 */
void afficher_erreur(const char *message) {
    printf("  [ERREUR] %s\n", message);
}

/*
 * Affiche un message de succes formate.
 * Prefixe par [OK] pour une identification rapide.
 */
void afficher_succes(const char *message) {
    printf("  [OK] %s\n", message);
}

/*
 * Affiche un message d'information formate.
 * Prefixe par [INFO] pour une identification rapide.
 */
void afficher_info(const char *message) {
    printf("  [INFO] %s\n", message);
}

/*
 * Attend que l'utilisateur appuie sur Entree pour continuer.
 * Utilisee pour faire une pause entre les actions.
 */
void pause_console(void) {
    printf("\n  Appuyez sur Entree pour continuer...");
    getchar();
}

/*
 * Vide le buffer d'entree standard.
 * Utilisee apres un scanf ou quand fgets lit une ligne trop longue.
 * Consomme tous les caracteres jusqu'au '\n' ou EOF.
 */
void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Lit une ligne de texte depuis l'entree standard.
 * Utilise fgets pour eviter les debordements de buffer.
 * Supprime le caractere de fin de ligne '\n'.
 * Si la ligne est trop longue, vide le reste du buffer.
 * Retourne 1 en cas de succes, 0 si la ligne est vide ou en cas d'erreur.
 */
int lire_chaine(char *buffer, int taille) {
    if (fgets(buffer, taille, stdin) == NULL) {
        return 0;
    }

    /* Supprime le caractere de fin de ligne */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        /* La ligne est trop longue, vider le reste du buffer */
        vider_buffer();
    }

    /* Verifie que la chaine n'est pas vide */
    if (strlen(buffer) == 0) {
        return 0;
    }

    return 1;
}

/*
 * Lit un entier depuis l'entree standard avec validation.
 * Boucle tant que la saisie n'est pas un entier valide.
 * Utilise fgets + sscanf pour eviter les problemes de buffer.
 */
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

/*
 * Lit un entier positif depuis l'entree standard avec validation.
 * Boucle tant que la saisie n'est pas un entier strictement positif.
 */
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

/*
 * Demande une confirmation Oui/Non a l'utilisateur.
 * Accepte 'O', 'o', 'OUI', 'oui' pour Oui.
 * Accepte 'N', 'n', 'NON', 'non' pour Non.
 * Retourne 1 pour Oui, 0 pour Non.
 */
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

/*
 * Convertit une chaine en minuscules.
 * Modifie la chaine en place.
 * Utilisee pour les recherches insensibles a la casse.
 */
void minuscules(char *chaine) {
    for (int i = 0; chaine[i]; i++) {
        chaine[i] = (char)tolower((unsigned char)chaine[i]);
    }
}

/*
 * Verifie si une chaine contient une sous-chaine (insensible a la casse).
 * Convertit temporairement les deux chaines en minuscules pour la comparaison.
 * Retourne 1 si trouve, 0 sinon.
 */
int contient_sous_chaine(const char *texte, const char *recherche) {
    char t[TAILLE_BUFFER];
    char r[TAILLE_BUFFER];

    /* Copie et conversion en minuscules */
    strncpy(t, texte, sizeof(t) - 1);
    t[sizeof(t) - 1] = '\0';
    strncpy(r, recherche, sizeof(r) - 1);
    r[sizeof(r) - 1] = '\0';

    minuscules(t);
    minuscules(r);

    return (strstr(t, r) != NULL);
}

/*
 * Affiche le menu principal de l'application.
 */
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

/*
 * Affiche le sous-menu de gestion des livres.
 */
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

/*
 * Affiche le sous-menu de gestion des emprunteurs.
 */
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

/*
 * Affiche le sous-menu de gestion des emprunts.
 */
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

/*
 * Affiche le sous-menu de rapports et statistiques.
 */
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

/*
 * Affiche l'en-tete de l'application au demarrage.
 */
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
