/*
 * livre.c
 * Implementation du module de gestion des livres
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 *
 * Ce module gere toutes les operations liees aux livres :
 * - Ajout, modification, suppression
 * - Recherche par ID, titre, auteur, ISBN
 * - Affichage des livres (tous ou disponibles uniquement)
 * - Sauvegarde et chargement depuis fichier binaire
 * - Validation des donnees (ISBN, annee, etc.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "livre.h"
#include "utils.h"

/*
 * Genere un nouvel identifiant unique pour un livre.
 * Parcourt le tableau pour trouver le plus grand ID existant.
 * Retourne le plus grand ID + 1, ou 1 si aucun livre n'existe.
 */
int generer_id_livre(const Livre *livres, int nb_livres) {
    int max_id = 0;
    for (int i = 0; i < nb_livres; i++) {
        if (livres[i].id > max_id) {
            max_id = livres[i].id;
        }
    }
    return max_id + 1;
}

/*
 * Valide un numero ISBN (format basique : 10 ou 13 chiffres).
 * Accepte aussi les ISBN avec tirets ou espaces.
 * Un ISBN valide contient exactement 10 ou 13 chiffres.
 * Retourne 1 si valide, 0 sinon.
 */
int valider_isbn(const char *isbn) {
    int len = (int)strlen(isbn);
    int chiffres = 0;

    if (len < 10 || len > 17) {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (isdigit((unsigned char)isbn[i])) {
            chiffres++;
        } else if (isbn[i] != '-' && isbn[i] != ' ') {
            return 0;
        }
    }

    return (chiffres == 10 || chiffres == 13);
}

/*
 * Affiche les details d'un seul livre sous forme de ligne formatee.
 * Utilisee dans les tableaux d'affichage.
 */
void afficher_livre(const Livre *livre) {
    const char *statut = (livre->nb_disponibles > 0) ? "DISPONIBLE" : "INDISPONIBLE";
    printf("  | %-4d | %-25s | %-20s | %-13s | %4d | %2d/%2d | %-12s |\n",
           livre->id,
           livre->titre,
           livre->auteur,
           livre->isbn,
           livre->annee,
           livre->nb_disponibles,
           livre->nb_exemplaires,
           statut);
}

/*
 * Affiche la liste complete de tous les livres avec en-tete de tableau.
 * Affiche un message si aucun livre n'est enregistre.
 */
void afficher_tous_les_livres(const Livre *livres, int nb_livres) {
    if (nb_livres == 0) {
        afficher_info("Aucun livre enregistre dans la bibliotheque.");
        return;
    }

    printf("\n");
    afficher_titre("LISTE DES LIVRES");
    printf("  +------+---------------------------+----------------------+---------------+------+------+--------------+\n");
    printf("  | ID   | Titre                     | Auteur               | ISBN          | Annee| Disp.| Statut       |\n");
    printf("  +------+---------------------------+----------------------+---------------+------+------+--------------+\n");

    for (int i = 0; i < nb_livres; i++) {
        afficher_livre(&livres[i]);
    }

    printf("  +------+---------------------------+----------------------+---------------+------+------+--------------+\n");
    printf("  Total : %d livre(s)\n", nb_livres);
}

/*
 * Affiche uniquement les livres disponibles (nb_disponibles > 0).
 * Utile pour savoir quels livres peuvent etre empruntes.
 */
void afficher_livres_disponibles(const Livre *livres, int nb_livres) {
    int count = 0;

    printf("\n");
    afficher_titre("LIVRES DISPONIBLES");
    printf("  +------+---------------------------+----------------------+---------------+------+\n");
    printf("  | ID   | Titre                     | Auteur               | ISBN          | Disp.|\n");
    printf("  +------+---------------------------+----------------------+---------------+------+\n");

    for (int i = 0; i < nb_livres; i++) {
        if (livres[i].nb_disponibles > 0) {
            printf("  | %-4d | %-25s | %-20s | %-13s | %4d |\n",
                   livres[i].id, livres[i].titre, livres[i].auteur,
                   livres[i].isbn, livres[i].nb_disponibles);
            count++;
        }
    }

    printf("  +------+---------------------------+----------------------+---------------+------+\n");
    if (count == 0) {
        afficher_info("Aucun livre disponible actuellement.");
    } else {
        printf("  Total : %d livre(s) disponible(s)\n", count);
    }
}

/*
 * Recherche un livre par son identifiant.
 * Parcours lineaire du tableau O(n).
 * Retourne l'index dans le tableau, ou -1 si non trouve.
 */
int rechercher_livre_par_id(const Livre *livres, int nb_livres, int id) {
    for (int i = 0; i < nb_livres; i++) {
        if (livres[i].id == id) {
            return i;
        }
    }
    return -1;
}

/*
 * Recherche des livres par titre (recherche partielle, insensible a la casse).
 * Utilise la fonction contient_sous_chaine() de utils.c.
 * Affiche les resultats trouves sous forme de tableau.
 */
void rechercher_livre_par_titre(const Livre *livres, int nb_livres, const char *titre) {
    int count = 0;

    printf("\n");
    afficher_titre("RESULTATS DE RECHERCHE PAR TITRE");

    for (int i = 0; i < nb_livres; i++) {
        if (contient_sous_chaine(livres[i].titre, titre)) {
            if (count == 0) {
                printf("  +------+---------------------------+----------------------+---------------+------+\n");
                printf("  | ID   | Titre                     | Auteur               | ISBN          | Disp.|\n");
                printf("  +------+---------------------------+----------------------+---------------+------+\n");
            }
            printf("  | %-4d | %-25s | %-20s | %-13s | %4d |\n",
                   livres[i].id, livres[i].titre, livres[i].auteur,
                   livres[i].isbn, livres[i].nb_disponibles);
            count++;
        }
    }

    if (count > 0) {
        printf("  +------+---------------------------+----------------------+---------------+------+\n");
        printf("  %d resultat(s) trouve(s)\n", count);
    } else {
        afficher_info("Aucun livre trouve avec ce titre.");
    }
}

/*
 * Recherche des livres par auteur (recherche partielle, insensible a la casse).
 * Affiche les resultats trouves sous forme de tableau.
 */
void rechercher_livre_par_auteur(const Livre *livres, int nb_livres, const char *auteur) {
    int count = 0;

    printf("\n");
    afficher_titre("RESULTATS DE RECHERCHE PAR AUTEUR");

    for (int i = 0; i < nb_livres; i++) {
        if (contient_sous_chaine(livres[i].auteur, auteur)) {
            if (count == 0) {
                printf("  +------+---------------------------+----------------------+---------------+------+\n");
                printf("  | ID   | Titre                     | Auteur               | ISBN          | Disp.|\n");
                printf("  +------+---------------------------+----------------------+---------------+------+\n");
            }
            printf("  | %-4d | %-25s | %-20s | %-13s | %4d |\n",
                   livres[i].id, livres[i].titre, livres[i].auteur,
                   livres[i].isbn, livres[i].nb_disponibles);
            count++;
        }
    }

    if (count > 0) {
        printf("  +------+---------------------------+----------------------+---------------+------+\n");
        printf("  %d resultat(s) trouve(s)\n", count);
    } else {
        afficher_info("Aucun livre trouve pour cet auteur.");
    }
}

/*
 * Recherche un livre par son ISBN exact (comparaison stricte).
 * Affiche le resultat trouve avec tous les details.
 */
void rechercher_livre_par_isbn(const Livre *livres, int nb_livres, const char *isbn) {
    printf("\n");
    afficher_titre("RESULTAT DE RECHERCHE PAR ISBN");

    for (int i = 0; i < nb_livres; i++) {
        if (strcmp(livres[i].isbn, isbn) == 0) {
            printf("  +------+---------------------------+----------------------+---------------+------+\n");
            printf("  | ID   | Titre                     | Auteur               | ISBN          | Disp.|\n");
            printf("  +------+---------------------------+----------------------+---------------+------+\n");
            printf("  | %-4d | %-25s | %-20s | %-13s | %4d |\n",
                   livres[i].id, livres[i].titre, livres[i].auteur,
                   livres[i].isbn, livres[i].nb_disponibles);
            printf("  +------+---------------------------+----------------------+---------------+------+\n");
            return;
        }
    }

    afficher_info("Aucun livre trouve avec cet ISBN.");
}

/*
 * Ajoute un nouveau livre dans le tableau dynamique.
 * Alloue de la memoire si la capacite est insuffisante (realloc).
 * Valide les donnees saisies (ISBN, annee, nombre d'exemplaires).
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int ajouter_livre(Livre **livres, int *nb_livres, int *capacite) {
    char buffer[TAILLE_BUFFER];
    Livre nouveau;

    printf("\n");
    afficher_titre("AJOUT D'UN LIVRE");

    /* Generation automatique de l'ID unique */
    nouveau.id = generer_id_livre(*livres, *nb_livres);

    /* Saisie du titre */
    printf("  Titre du livre : ");
    if (!lire_chaine(buffer, sizeof(buffer))) {
        afficher_erreur("Le titre ne peut pas etre vide.");
        return 0;
    }
    strncpy(nouveau.titre, buffer, TITRE_MAX - 1);
    nouveau.titre[TITRE_MAX - 1] = '\0';

    /* Saisie de l'auteur */
    printf("  Auteur : ");
    if (!lire_chaine(buffer, sizeof(buffer))) {
        afficher_erreur("L'auteur ne peut pas etre vide.");
        return 0;
    }
    strncpy(nouveau.auteur, buffer, AUTEUR_MAX - 1);
    nouveau.auteur[AUTEUR_MAX - 1] = '\0';

    /* Saisie et validation de l'ISBN */
    printf("  ISBN (10 ou 13 chiffres) : ");
    if (!lire_chaine(buffer, sizeof(buffer)) || !valider_isbn(buffer)) {
        afficher_erreur("ISBN invalide. Format attendu : 10 ou 13 chiffres.");
        return 0;
    }
    strncpy(nouveau.isbn, buffer, ISBN_MAX - 1);
    nouveau.isbn[ISBN_MAX - 1] = '\0';

    /* Saisie de l'editeur */
    printf("  Editeur : ");
    if (!lire_chaine(buffer, sizeof(buffer))) {
        afficher_erreur("L'editeur ne peut pas etre vide.");
        return 0;
    }
    strncpy(nouveau.editeur, buffer, EDITEUR_MAX - 1);
    nouveau.editeur[EDITEUR_MAX - 1] = '\0';

    /* Saisie et validation de l'annee */
    nouveau.annee = lire_entier("  Annee de publication : ");
    if (nouveau.annee < 1000 || nouveau.annee > 2100) {
        afficher_erreur("Annee invalide. Doit etre entre 1000 et 2100.");
        return 0;
    }

    /* Saisie du nombre d'exemplaires */
    nouveau.nb_exemplaires = lire_entier_positif("  Nombre d'exemplaires : ");
    nouveau.nb_disponibles = nouveau.nb_exemplaires;

    /* Reallocation dynamique si le tableau est plein */
    if (*nb_livres >= *capacite) {
        int nouvelle_capacite = *capacite * 2;
        Livre *temp = (Livre *)realloc(*livres, nouvelle_capacite * sizeof(Livre));
        if (temp == NULL) {
            afficher_erreur("Erreur d'allocation memoire. Impossible d'ajouter le livre.");
            return 0;
        }
        *livres = temp;
        *capacite = nouvelle_capacite;
    }

    /* Ajout du livre a la fin du tableau */
    (*livres)[*nb_livres] = nouveau;
    (*nb_livres)++;

    afficher_succes("Livre ajoute avec succes !");
    printf("  ID attribue : %d\n", nouveau.id);

    return 1;
}

/*
 * Modifie les informations d'un livre existant.
 * L'utilisateur peut laisser vide pour conserver la valeur actuelle.
 * Retourne 1 en cas de succes, 0 si le livre n'est pas trouve.
 */
int modifier_livre(Livre *livres, int nb_livres) {
    char buffer[TAILLE_BUFFER];
    int id, index;

    printf("\n");
    afficher_titre("MODIFICATION D'UN LIVRE");

    id = lire_entier("  ID du livre a modifier : ");
    index = rechercher_livre_par_id(livres, nb_livres, id);

    if (index == -1) {
        afficher_erreur("Livre non trouve.");
        return 0;
    }

    printf("\n  Livre actuel :\n");
    afficher_livre(&livres[index]);
    printf("\n  Laissez vide pour conserver la valeur actuelle.\n\n");

    /* Modification du titre */
    printf("  Nouveau titre [%s] : ", livres[index].titre);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        strncpy(livres[index].titre, buffer, TITRE_MAX - 1);
        livres[index].titre[TITRE_MAX - 1] = '\0';
    }

    /* Modification de l'auteur */
    printf("  Nouvel auteur [%s] : ", livres[index].auteur);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        strncpy(livres[index].auteur, buffer, AUTEUR_MAX - 1);
        livres[index].auteur[AUTEUR_MAX - 1] = '\0';
    }

    /* Modification de l'ISBN avec validation */
    printf("  Nouvel ISBN [%s] : ", livres[index].isbn);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        if (valider_isbn(buffer)) {
            strncpy(livres[index].isbn, buffer, ISBN_MAX - 1);
            livres[index].isbn[ISBN_MAX - 1] = '\0';
        } else {
            afficher_erreur("ISBN invalide, modification ignoree.");
        }
    }

    /* Modification de l'editeur */
    printf("  Nouvel editeur [%s] : ", livres[index].editeur);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        strncpy(livres[index].editeur, buffer, EDITEUR_MAX - 1);
        livres[index].editeur[EDITEUR_MAX - 1] = '\0';
    }

    /* Modification de l'annee */
    printf("  Nouvelle annee [%d] : ", livres[index].annee);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        int annee;
        if (sscanf(buffer, "%d", &annee) == 1 && annee >= 1000 && annee <= 2100) {
            livres[index].annee = annee;
        }
    }

    /* Modification du nombre d'exemplaires */
    printf("  Nouveau nombre d'exemplaires [%d] : ", livres[index].nb_exemplaires);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        int nb;
        if (sscanf(buffer, "%d", &nb) == 1 && nb > 0) {
            int empruntes = livres[index].nb_exemplaires - livres[index].nb_disponibles;
            livres[index].nb_exemplaires = nb;
            livres[index].nb_disponibles = nb - empruntes;
            if (livres[index].nb_disponibles < 0) {
                livres[index].nb_disponibles = 0;
            }
        }
    }

    afficher_succes("Livre modifie avec succes !");
    return 1;
}

/*
 * Supprime un livre du tableau dynamique.
 * Verifie qu'aucun exemplaire n'est emprunte avant suppression.
 * Decale les elements suivants pour combler le trou.
 * Retourne 1 en cas de succes, 0 si le livre n'est pas trouve ou a des emprunts.
 */
int supprimer_livre(Livre **livres, int *nb_livres) {
    int id, index;

    printf("\n");
    afficher_titre("SUPPRESSION D'UN LIVRE");

    id = lire_entier("  ID du livre a supprimer : ");
    index = rechercher_livre_par_id(*livres, *nb_livres, id);

    if (index == -1) {
        afficher_erreur("Livre non trouve.");
        return 0;
    }

    printf("\n  Livre a supprimer :\n");
    afficher_livre(&(*livres)[index]);

    /* Verification : impossible de supprimer si des exemplaires sont empruntes */
    if ((*livres)[index].nb_disponibles < (*livres)[index].nb_exemplaires) {
        afficher_erreur("Ce livre a des exemplaires empruntes. Impossible de le supprimer.");
        return 0;
    }

    /* Confirmation de l'utilisateur */
    if (!demander_confirmation("\n  Confirmer la suppression ?")) {
        afficher_info("Suppression annulee.");
        return 0;
    }

    /* Decalage des elements pour combler le trou */
    for (int i = index; i < *nb_livres - 1; i++) {
        (*livres)[i] = (*livres)[i + 1];
    }

    (*nb_livres)--;

    /* Liberation de memoire si le tableau est trop grand (optimisation) */
    if (*nb_livres > 0 && *nb_livres < (*nb_livres + CAPACITE_INITiale) / 4) {
        int nouvelle_capacite = (*nb_livres + CAPACITE_INITiale) / 2;
        if (nouvelle_capacite < CAPACITE_INITiale) {
            nouvelle_capacite = CAPACITE_INITiale;
        }
        Livre *temp = (Livre *)realloc(*livres, nouvelle_capacite * sizeof(Livre));
        if (temp != NULL) {
            *livres = temp;
        }
    }

    afficher_succes("Livre supprime avec succes !");
    return 1;
}

/*
 * Sauvegarde tous les livres dans un fichier binaire.
 * Format du fichier : [int nb_livres][Livre livre1][Livre livre2]...
 * Gestion des erreurs : fichier inaccessible, erreur d'ecriture.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int sauvegarder_livres(const Livre *livres, int nb_livres) {
    FILE *fichier = fopen(FICHIER_LIVRES, "wb");
    if (fichier == NULL) {
        afficher_erreur("Impossible d'ouvrir le fichier de sauvegarde des livres.");
        afficher_erreur(strerror(errno));
        return 0;
    }

    /* Ecrit d'abord le nombre de livres (pour le chargement) */
    if (fwrite(&nb_livres, sizeof(int), 1, fichier) != 1) {
        fclose(fichier);
        afficher_erreur("Erreur lors de l'ecriture du nombre de livres.");
        return 0;
    }

    /* Ecrit chaque livre un par un */
    if (nb_livres > 0) {
        if (fwrite(livres, sizeof(Livre), (size_t)nb_livres, fichier) != (size_t)nb_livres) {
            fclose(fichier);
            afficher_erreur("Erreur lors de l'ecriture des livres.");
            return 0;
        }
    }

    fclose(fichier);
    return 1;
}

/*
 * Charge les livres depuis un fichier binaire.
 * Alloue dynamiquement le tableau avec la capacite initiale.
 * Gestion des erreurs : fichier inexistant (premier demarrage), corruption, memoire insuffisante.
 * Retourne le nombre de livres charges, ou 0 si le fichier n'existe pas.
 */
int charger_livres(Livre **livres, int *capacite) {
    FILE *fichier = fopen(FICHIER_LIVRES, "rb");
    int nb_livres = 0;

    if (fichier == NULL) {
        /* Le fichier n'existe pas encore : c'est normal au premier demarrage */
        *capacite = CAPACITE_INITiale;
        *livres = (Livre *)malloc(*capacite * sizeof(Livre));
        if (*livres == NULL) {
            afficher_erreur("Erreur d'allocation memoire pour les livres.");
            return 0;
        }
        return 0;
    }

    /* Lecture du nombre de livres */
    if (fread(&nb_livres, sizeof(int), 1, fichier) != 1) {
        fclose(fichier);
        afficher_erreur("Erreur lors de la lecture du fichier de livres (fichier corrompu ?).");
        *capacite = CAPACITE_INITiale;
        *livres = (Livre *)malloc(*capacite * sizeof(Livre));
        return 0;
    }

    /* Allocation du tableau avec une marge pour de futurs ajouts */
    *capacite = nb_livres + CAPACITE_INITiale;
    *livres = (Livre *)malloc(*capacite * sizeof(Livre));
    if (*livres == NULL) {
        fclose(fichier);
        afficher_erreur("Erreur d'allocation memoire pour les livres.");
        *capacite = 0;
        return 0;
    }

    /* Lecture des livres */
    if (nb_livres > 0) {
        if (fread(*livres, sizeof(Livre), (size_t)nb_livres, fichier) != (size_t)nb_livres) {
            fclose(fichier);
            free(*livres);
            *livres = NULL;
            *capacite = 0;
            afficher_erreur("Erreur lors de la lecture des livres (fichier incomplet ?).");
            return 0;
        }
    }

    fclose(fichier);
    return nb_livres;
}

/*
 * Libere la memoire allouee pour le tableau de livres.
 * Met le pointeur a NULL apres liberation pour eviter les dangling pointers.
 */
void liberer_livres(Livre **livres) {
    if (*livres != NULL) {
        free(*livres);
        *livres = NULL;
    }
}
