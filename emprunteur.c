/*
 * emprunteur.c
 * Implementation du module de gestion des emprunteurs
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "emprunteur.h"
#include "utils.h"

void generer_numero_carte(char *numero_carte, int id) {
    snprintf(numero_carte, NUMERO_CARTE_MAX, "CARTE-%04d", id);
}

int valider_telephone(const char *telephone) {
    int len = (int)strlen(telephone);
    int chiffres = 0;
    if (len < 8 || len > 20) {
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (isdigit((unsigned char)telephone[i])) {
            chiffres++;
        } else if (telephone[i] != ' ' && telephone[i] != '-' && telephone[i] != '+' && telephone[i] != '(' && telephone[i] != ')') {
            return 0;
        }
    }
    return (chiffres >= 8);
}

int valider_email(const char *email) {
    int len = (int)strlen(email);
    int arobase = 0;
    int point_apres_arobase = 0;
    if (len < 5 || len > EMAIL_MAX - 1) {
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            arobase++;
        }
        if (arobase == 1 && email[i] == '.') {
            point_apres_arobase = 1;
        }
    }
    return (arobase == 1 && point_apres_arobase);
}

void afficher_emprunteur(const Emprunteur *emprunteur) {
    printf("  | %-4d | %-12s | %-15s | %-15s | %-15s | %2d |\n",
           emprunteur->id, emprunteur->numero_carte, emprunteur->nom,
           emprunteur->prenom, emprunteur->telephone, emprunteur->nb_emprunts_actifs);
}

void afficher_tous_les_emprunteurs(const Emprunteur *emprunteurs, int nb_emprunteurs) {
    if (nb_emprunteurs == 0) {
        afficher_info("Aucun emprunteur enregistre.");
        return;
    }
    printf("\n");
    afficher_titre("LISTE DES EMPRUNTEURS");
    printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
    printf("  | ID   | No Carte     | Nom             | Prenom          | Telephone       | Em.|\n");
    printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
    for (int i = 0; i < nb_emprunteurs; i++) {
        afficher_emprunteur(&emprunteurs[i]);
    }
    printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
    printf("  Total : %d emprunteur(s)\n", nb_emprunteurs);
}

int rechercher_emprunteur_par_id(const Emprunteur *emprunteurs, int nb_emprunteurs, int id) {
    for (int i = 0; i < nb_emprunteurs; i++) {
        if (emprunteurs[i].id == id) {
            return i;
        }
    }
    return -1;
}

void rechercher_emprunteur_par_nom(const Emprunteur *emprunteurs, int nb_emprunteurs, const char *nom) {
    int count = 0;
    printf("\n");
    afficher_titre("RESULTATS DE RECHERCHE PAR NOM");
    for (int i = 0; i < nb_emprunteurs; i++) {
        if (contient_sous_chaine(emprunteurs[i].nom, nom) ||
            contient_sous_chaine(emprunteurs[i].prenom, nom)) {
            if (count == 0) {
                printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
                printf("  | ID   | No Carte     | Nom             | Prenom          | Telephone       | Em.|\n");
                printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
            }
            afficher_emprunteur(&emprunteurs[i]);
            count++;
        }
    }
    if (count > 0) {
        printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
        printf("  %d resultat(s) trouve(s)\n", count);
    } else {
        afficher_info("Aucun emprunteur trouve avec ce nom.");
    }
}

void rechercher_emprunteur_par_carte(const Emprunteur *emprunteurs, int nb_emprunteurs, const char *numero_carte) {
    printf("\n");
    afficher_titre("RESULTAT DE RECHERCHE PAR CARTE");
    for (int i = 0; i < nb_emprunteurs; i++) {
        if (strcmp(emprunteurs[i].numero_carte, numero_carte) == 0) {
            printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
            printf("  | ID   | No Carte     | Nom             | Prenom          | Telephone       | Em.|\n");
            printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
            afficher_emprunteur(&emprunteurs[i]);
            printf("  +------+--------------+-----------------+-----------------+-----------------+----+\n");
            printf("\n  Adresse  : %s\n", emprunteurs[i].adresse);
            printf("  Email    : %s\n", emprunteurs[i].email);
            return;
        }
    }
    afficher_info("Aucun emprunteur trouve avec ce numero de carte.");
}

int ajouter_emprunteur(Emprunteur **emprunteurs, int *nb_emprunteurs, int *capacite) {
    char buffer[TAILLE_BUFFER];
    Emprunteur nouveau;
    int max_id = 0;
    printf("\n");
    afficher_titre("AJOUT D'UN EMPRUNTEUR");
    for (int i = 0; i < *nb_emprunteurs; i++) {
        if ((*emprunteurs)[i].id > max_id) {
            max_id = (*emprunteurs)[i].id;
        }
    }
    nouveau.id = max_id + 1;
    generer_numero_carte(nouveau.numero_carte, nouveau.id);
    printf("  Nom : ");
    if (!lire_chaine(buffer, sizeof(buffer))) {
        afficher_erreur("Le nom ne peut pas etre vide.");
        return 0;
    }
    strncpy(nouveau.nom, buffer, NOM_MAX - 1);
    nouveau.nom[NOM_MAX - 1] = '\0';
    printf("  Prenom : ");
    if (!lire_chaine(buffer, sizeof(buffer))) {
        afficher_erreur("Le prenom ne peut pas etre vide.");
        return 0;
    }
    strncpy(nouveau.prenom, buffer, PRENOM_MAX - 1);
    nouveau.prenom[PRENOM_MAX - 1] = '\0';
    printf("  Adresse : ");
    if (!lire_chaine(buffer, sizeof(buffer))) {
        afficher_erreur("L'adresse ne peut pas etre vide.");
        return 0;
    }
    strncpy(nouveau.adresse, buffer, ADRESSE_MAX - 1);
    nouveau.adresse[ADRESSE_MAX - 1] = '\0';
    printf("  Telephone : ");
    if (!lire_chaine(buffer, sizeof(buffer)) || !valider_telephone(buffer)) {
        afficher_erreur("Numero de telephone invalide (minimum 8 chiffres).");
        return 0;
    }
    strncpy(nouveau.telephone, buffer, TELEPHONE_MAX - 1);
    nouveau.telephone[TELEPHONE_MAX - 1] = '\0';
    printf("  Email : ");
    if (!lire_chaine(buffer, sizeof(buffer)) || !valider_email(buffer)) {
        afficher_erreur("Adresse email invalide.");
        return 0;
    }
    strncpy(nouveau.email, buffer, EMAIL_MAX - 1);
    nouveau.email[EMAIL_MAX - 1] = '\0';
    nouveau.nb_emprunts_actifs = 0;
    if (*nb_emprunteurs >= *capacite) {
        int nouvelle_capacite = *capacite * 2;
        Emprunteur *temp = (Emprunteur *)realloc(*emprunteurs, nouvelle_capacite * sizeof(Emprunteur));
        if (temp == NULL) {
            afficher_erreur("Erreur d'allocation memoire. Impossible d'ajouter l'emprunteur.");
            return 0;
        }
        *emprunteurs = temp;
        *capacite = nouvelle_capacite;
    }
    (*emprunteurs)[*nb_emprunteurs] = nouveau;
    (*nb_emprunteurs)++;
    afficher_succes("Emprunteur ajoute avec succes !");
    printf("  ID attribue       : %d\n", nouveau.id);
    printf("  Numero de carte   : %s\n", nouveau.numero_carte);
    return 1;
}

int modifier_emprunteur(Emprunteur *emprunteurs, int nb_emprunteurs) {
    char buffer[TAILLE_BUFFER];
    int id, index;
    printf("\n");
    afficher_titre("MODIFICATION D'UN EMPRUNTEUR");
    id = lire_entier("  ID de l'emprunteur a modifier : ");
    index = rechercher_emprunteur_par_id(emprunteurs, nb_emprunteurs, id);
    if (index == -1) {
        afficher_erreur("Emprunteur non trouve.");
        return 0;
    }
    printf("\n  Emprunteur actuel :\n");
    afficher_emprunteur(&emprunteurs[index]);
    printf("\n  Laissez vide pour conserver la valeur actuelle.\n\n");
    printf("  Nouveau nom [%s] : ", emprunteurs[index].nom);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        strncpy(emprunteurs[index].nom, buffer, NOM_MAX - 1);
        emprunteurs[index].nom[NOM_MAX - 1] = '\0';
    }
    printf("  Nouveau prenom [%s] : ", emprunteurs[index].prenom);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        strncpy(emprunteurs[index].prenom, buffer, PRENOM_MAX - 1);
        emprunteurs[index].prenom[PRENOM_MAX - 1] = '\0';
    }
    printf("  Nouvelle adresse [%s] : ", emprunteurs[index].adresse);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        strncpy(emprunteurs[index].adresse, buffer, ADRESSE_MAX - 1);
        emprunteurs[index].adresse[ADRESSE_MAX - 1] = '\0';
    }
    printf("  Nouveau telephone [%s] : ", emprunteurs[index].telephone);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        if (valider_telephone(buffer)) {
            strncpy(emprunteurs[index].telephone, buffer, TELEPHONE_MAX - 1);
            emprunteurs[index].telephone[TELEPHONE_MAX - 1] = '\0';
        } else {
            afficher_erreur("Telephone invalide, modification ignoree.");
        }
    }
    printf("  Nouvel email [%s] : ", emprunteurs[index].email);
    if (lire_chaine(buffer, sizeof(buffer)) && strlen(buffer) > 0) {
        if (valider_email(buffer)) {
            strncpy(emprunteurs[index].email, buffer, EMAIL_MAX - 1);
            emprunteurs[index].email[EMAIL_MAX - 1] = '\0';
        } else {
            afficher_erreur("Email invalide, modification ignoree.");
        }
    }
    afficher_succes("Emprunteur modifie avec succes !");
    return 1;
}

int supprimer_emprunteur(Emprunteur **emprunteurs, int *nb_emprunteurs) {
    int id, index;
    printf("\n");
    afficher_titre("SUPPRESSION D'UN EMPRUNTEUR");
    id = lire_entier("  ID de l'emprunteur a supprimer : ");
    index = rechercher_emprunteur_par_id(*emprunteurs, *nb_emprunteurs, id);
    if (index == -1) {
        afficher_erreur("Emprunteur non trouve.");
        return 0;
    }
    printf("\n  Emprunteur a supprimer :\n");
    afficher_emprunteur(&(*emprunteurs)[index]);
    if ((*emprunteurs)[index].nb_emprunts_actifs > 0) {
        afficher_erreur("Cet emprunteur a des emprunts actifs. Impossible de le supprimer.");
        return 0;
    }
    if (!demander_confirmation("\n  Confirmer la suppression ?")) {
        afficher_info("Suppression annulee.");
        return 0;
    }
    for (int i = index; i < *nb_emprunteurs - 1; i++) {
        (*emprunteurs)[i] = (*emprunteurs)[i + 1];
    }
    (*nb_emprunteurs)--;
    afficher_succes("Emprunteur supprime avec succes !");
    return 1;
}

int sauvegarder_emprunteurs(const Emprunteur *emprunteurs, int nb_emprunteurs) {
    FILE *fichier = fopen(FICHIER_EMPRUNTEURS, "wb");
    if (fichier == NULL) {
        afficher_erreur("Impossible d'ouvrir le fichier de sauvegarde des emprunteurs.");
        afficher_erreur(strerror(errno));
        return 0;
    }
    if (fwrite(&nb_emprunteurs, sizeof(int), 1, fichier) != 1) {
        fclose(fichier);
        afficher_erreur("Erreur lors de l'ecriture du nombre d'emprunteurs.");
        return 0;
    }
    if (nb_emprunteurs > 0) {
        if (fwrite(emprunteurs, sizeof(Emprunteur), (size_t)nb_emprunteurs, fichier) != (size_t)nb_emprunteurs) {
            fclose(fichier);
            afficher_erreur("Erreur lors de l'ecriture des emprunteurs.");
            return 0;
        }
    }
    fclose(fichier);
    return 1;
}

int charger_emprunteurs(Emprunteur **emprunteurs, int *capacite) {
    FILE *fichier = fopen(FICHIER_EMPRUNTEURS, "rb");
    int nb_emprunteurs = 0;
    if (fichier == NULL) {
        *capacite = CAPACITE_INITIALE;
        *emprunteurs = (Emprunteur *)malloc(*capacite * sizeof(Emprunteur));
        if (*emprunteurs == NULL) {
            afficher_erreur("Erreur d'allocation memoire pour les emprunteurs.");
            return 0;
        }
        return 0;
    }
    if (fread(&nb_emprunteurs, sizeof(int), 1, fichier) != 1) {
        fclose(fichier);
        afficher_erreur("Erreur lors de la lecture du fichier d'emprunteurs (fichier corrompu ?).");
        *capacite = CAPACITE_INITIALE;
        *emprunteurs = (Emprunteur *)malloc(*capacite * sizeof(Emprunteur));
        return 0;
    }
    *capacite = nb_emprunteurs + CAPACITE_INITIALE;
    *emprunteurs = (Emprunteur *)malloc(*capacite * sizeof(Emprunteur));
    if (*emprunteurs == NULL) {
        fclose(fichier);
        afficher_erreur("Erreur d'allocation memoire pour les emprunteurs.");
        *capacite = 0;
        return 0;
    }
    if (nb_emprunteurs > 0) {
        if (fread(*emprunteurs, sizeof(Emprunteur), (size_t)nb_emprunteurs, fichier) != (size_t)nb_emprunteurs) {
            fclose(fichier);
            free(*emprunteurs);
            *emprunteurs = NULL;
            *capacite = 0;
            afficher_erreur("Erreur lors de la lecture des emprunteurs (fichier incomplet ?).");
            return 0;
        }
    }
    fclose(fichier);
    return nb_emprunteurs;
}

void liberer_emprunteurs(Emprunteur **emprunteurs) {
    if (*emprunteurs != NULL) {
        free(*emprunteurs);
        *emprunteurs = NULL;
    }
}
