/*
 * main.c
 * Point d'entree de l'application
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 *
 * Ce fichier contient la fonction principale et les menus de navigation.
 * Il orchestre les differents modules : livres, emprunteurs, emprunts.
 * Architecture : separation claire entre la logique metier (modules .c)
 * et l'interface utilisateur (ce fichier + utils.c).
 */

#include <stdio.h>
#include <stdlib.h>
#include "livre.h"
#include "emprunteur.h"
#include "emprunt.h"
#include "utils.h"

/*
 * Affiche les statistiques generales de la bibliotheque.
 * Compte les emprunts en cours, en retard, et le total des amendes.
 */
void afficher_statistiques(const Livre *livres, int nb_livres,
                           const Emprunteur *emprunteurs, int nb_emprunteurs,
                           const Emprunt *emprunts, int nb_emprunts) {
    int emprunts_en_cours = 0;
    int emprunts_en_retard = 0;
    int total_amendes = 0;

    for (int i = 0; i < nb_emprunts; i++) {
        if (!emprunts[i].est_retourne) {
            emprunts_en_cours++;
            int retard = calculer_jours_retard(emprunts[i].date_retour_prevue);
            if (retard > 0) {
                emprunts_en_retard++;
            }
        }
        total_amendes += emprunts[i].amendes;
    }

    printf("\n");
    afficher_titre("STATISTIQUES GENERALES");
    printf("  | %-35s | %-20d |\n", "Nombre total de livres", nb_livres);
    printf("  | %-35s | %-20d |\n", "Nombre total d'emprunteurs", nb_emprunteurs);
    printf("  | %-35s | %-20d |\n", "Nombre total d'emprunts", nb_emprunts);
    printf("  | %-35s | %-20d |\n", "Emprunts en cours", emprunts_en_cours);
    printf("  | %-35s | %-20d |\n", "Emprunts en retard", emprunts_en_retard);
    printf("  | %-35s | %-20d |\n", "Total des amendes collectees (FC)", total_amendes);
    ligne_separation();
}

/*
 * Sauvegarde toutes les donnees dans les fichiers binaires.
 * Affiche le resultat de chaque sauvegarde.
 */
void sauvegarder_tout(const Livre *livres, int nb_livres,
                      const Emprunteur *emprunteurs, int nb_emprunteurs,
                      const Emprunt *emprunts, int nb_emprunts) {
    printf("\n");
    afficher_titre("SAUVEGARDE DES DONNEES");

    int ok = 1;

    if (sauvegarder_livres(livres, nb_livres)) {
        printf("  [OK] Livres sauvegardes (%d)\n", nb_livres);
    } else {
        printf("  [ERREUR] Echec de la sauvegarde des livres\n");
        ok = 0;
    }

    if (sauvegarder_emprunteurs(emprunteurs, nb_emprunteurs)) {
        printf("  [OK] Emprunteurs sauvegardes (%d)\n", nb_emprunteurs);
    } else {
        printf("  [ERREUR] Echec de la sauvegarde des emprunteurs\n");
        ok = 0;
    }

    if (sauvegarder_emprunts(emprunts, nb_emprunts)) {
        printf("  [OK] Emprunts sauvegardes (%d)\n", nb_emprunts);
    } else {
        printf("  [ERREUR] Echec de la sauvegarde des emprunts\n");
        ok = 0;
    }

    if (ok) {
        afficher_succes("Toutes les donnees ont ete sauvegardees avec succes !");
    } else {
        afficher_erreur("Certaines donnees n'ont pas pu etre sauvegardees.");
    }
}

/*
 * Gestion du sous-menu des livres.
 * Boucle jusqu'a ce que l'utilisateur choisisse de retourner au menu principal.
 */
void menu_livres(Livre **livres, int *nb_livres, int *capacite_livres) {
    int choix;
    char buffer[TAILLE_BUFFER];

    do {
        effacer_ecran();
        afficher_menu_livres();
        choix = lire_entier("\n  Votre choix : ");

        switch (choix) {
            case 1:
                ajouter_livre(livres, nb_livres, capacite_livres);
                break;
            case 2:
                modifier_livre(*livres, *nb_livres);
                break;
            case 3:
                supprimer_livre(livres, nb_livres);
                break;
            case 4:
                printf("\n  Titre a rechercher : ");
                lire_chaine(buffer, sizeof(buffer));
                rechercher_livre_par_titre(*livres, *nb_livres, buffer);
                break;
            case 5:
                printf("\n  Auteur a rechercher : ");
                lire_chaine(buffer, sizeof(buffer));
                rechercher_livre_par_auteur(*livres, *nb_livres, buffer);
                break;
            case 6:
                printf("\n  ISBN a rechercher : ");
                lire_chaine(buffer, sizeof(buffer));
                rechercher_livre_par_isbn(*livres, *nb_livres, buffer);
                break;
            case 7:
                afficher_tous_les_livres(*livres, *nb_livres);
                break;
            case 8:
                afficher_livres_disponibles(*livres, *nb_livres);
                break;
            case 0:
                break;
            default:
                afficher_erreur("Choix invalide.");
        }

        if (choix != 0) {
            pause_console();
        }
    } while (choix != 0);
}

/*
 * Gestion du sous-menu des emprunteurs.
 */
void menu_emprunteurs(Emprunteur **emprunteurs, int *nb_emprunteurs, int *capacite_emprunteurs) {
    int choix;
    char buffer[TAILLE_BUFFER];

    do {
        effacer_ecran();
        afficher_menu_emprunteurs();
        choix = lire_entier("\n  Votre choix : ");

        switch (choix) {
            case 1:
                ajouter_emprunteur(emprunteurs, nb_emprunteurs, capacite_emprunteurs);
                break;
            case 2:
                modifier_emprunteur(*emprunteurs, *nb_emprunteurs);
                break;
            case 3:
                supprimer_emprunteur(emprunteurs, nb_emprunteurs);
                break;
            case 4:
                printf("\n  Nom a rechercher : ");
                lire_chaine(buffer, sizeof(buffer));
                rechercher_emprunteur_par_nom(*emprunteurs, *nb_emprunteurs, buffer);
                break;
            case 5:
                printf("\n  Numero de carte a rechercher : ");
                lire_chaine(buffer, sizeof(buffer));
                rechercher_emprunteur_par_carte(*emprunteurs, *nb_emprunteurs, buffer);
                break;
            case 6:
                afficher_tous_les_emprunteurs(*emprunteurs, *nb_emprunteurs);
                break;
            case 0:
                break;
            default:
                afficher_erreur("Choix invalide.");
        }

        if (choix != 0) {
            pause_console();
        }
    } while (choix != 0);
}

/*
 * Gestion du sous-menu des emprunts.
 */
void menu_emprunts(Emprunt **emprunts, int *nb_emprunts, int *capacite_emprunts,
                   Livre *livres, int nb_livres,
                   Emprunteur *emprunteurs, int nb_emprunteurs) {
    int choix;
    int id_emprunteur;

    do {
        effacer_ecran();
        afficher_menu_emprunts();
        choix = lire_entier("\n  Votre choix : ");

        switch (choix) {
            case 1:
                emprunter_livre(emprunts, nb_emprunts, capacite_emprunts,
                                livres, nb_livres, emprunteurs, nb_emprunteurs);
                break;
            case 2:
                retourner_livre(*emprunts, *nb_emprunts, livres, nb_livres, emprunteurs, nb_emprunteurs);
                break;
            case 3:
                afficher_emprunts_en_cours(*emprunts, *nb_emprunts, livres, nb_livres, emprunteurs, nb_emprunteurs);
                break;
            case 4:
                afficher_emprunts_en_retard(*emprunts, *nb_emprunts, livres, nb_livres, emprunteurs, nb_emprunteurs);
                break;
            case 5:
                id_emprunteur = lire_entier("\n  ID de l'emprunteur : ");
                afficher_historique_par_emprunteur(*emprunts, *nb_emprunts, id_emprunteur, livres, nb_livres);
                break;
            case 6:
                afficher_historique_emprunts(*emprunts, *nb_emprunts);
                break;
            case 0:
                break;
            default:
                afficher_erreur("Choix invalide.");
        }

        if (choix != 0) {
            pause_console();
        }
    } while (choix != 0);
}

/*
 * Gestion du sous-menu des rapports et statistiques.
 */
void menu_rapports(const Livre *livres, int nb_livres,
                   const Emprunteur *emprunteurs, int nb_emprunteurs,
                   const Emprunt *emprunts, int nb_emprunts) {
    int choix;

    do {
        effacer_ecran();
        afficher_menu_rapports();
        choix = lire_entier("\n  Votre choix : ");

        switch (choix) {
            case 1:
                afficher_statistiques(livres, nb_livres, emprunteurs, nb_emprunteurs, emprunts, nb_emprunts);
                break;
            case 2:
                afficher_info("Fonctionnalite a implementer selon les besoins du groupe.");
                break;
            case 3:
                afficher_info("Fonctionnalite a implementer selon les besoins du groupe.");
                break;
            case 4: {
                int total = 0;
                for (int i = 0; i < nb_emprunts; i++) {
                    total += emprunts[i].amendes;
                }
                printf("\n  Total des amendes collectees : %d FC\n", total);
                break;
            }
            case 0:
                break;
            default:
                afficher_erreur("Choix invalide.");
        }

        if (choix != 0) {
            pause_console();
        }
    } while (choix != 0);
}

/*
 * Fonction principale.
 * Initialise les tableaux dynamiques, charge les donnees depuis les fichiers,
 * affiche le menu principal et gere la boucle principale de l'application.
 * Sauvegarde automatiquement les donnees avant de quitter.
 * Libere toute la memoire allouee dynamiquement.
 */
int main(void) {
    /* Tableaux dynamiques pour les livres */
    Livre *livres = NULL;
    int nb_livres = 0;
    int capacite_livres = 0;

    /* Tableaux dynamiques pour les emprunteurs */
    Emprunteur *emprunteurs = NULL;
    int nb_emprunteurs = 0;
    int capacite_emprunteurs = 0;

    /* Tableaux dynamiques pour les emprunts */
    Emprunt *emprunts = NULL;
    int nb_emprunts = 0;
    int capacite_emprunts = 0;

    int choix;

    /* Chargement des donnees depuis les fichiers binaires */
    /* Si les fichiers n'existent pas (premier demarrage), les tableaux sont initialises vides */
    nb_livres = charger_livres(&livres, &capacite_livres);
    nb_emprunteurs = charger_emprunteurs(&emprunteurs, &capacite_emprunteurs);
    nb_emprunts = charger_emprunts(&emprunts, &capacite_emprunts);

    /* Boucle principale de l'application */
    do {
        effacer_ecran();
        afficher_en_tete();
        afficher_menu_principal();
        choix = lire_entier("\n  Votre choix : ");

        switch (choix) {
            case 1:
                menu_livres(&livres, &nb_livres, &capacite_livres);
                break;
            case 2:
                menu_emprunteurs(&emprunteurs, &nb_emprunteurs, &capacite_emprunteurs);
                break;
            case 3:
                menu_emprunts(&emprunts, &nb_emprunts, &capacite_emprunts,
                              livres, nb_livres, emprunteurs, nb_emprunteurs);
                break;
            case 4:
                menu_rapports(livres, nb_livres, emprunteurs, nb_emprunteurs, emprunts, nb_emprunts);
                break;
            case 5:
                sauvegarder_tout(livres, nb_livres, emprunteurs, nb_emprunteurs, emprunts, nb_emprunts);
                pause_console();
                break;
            case 0:
                printf("\n");
                afficher_titre("AU REVOIR !");
                printf("  Merci d'avoir utilise le Gestionnaire de Bibliotheque.\n");
                printf("  UPN - L2 Informatique - Prof. Patrick Mukala\n\n");
                break;
            default:
                afficher_erreur("Choix invalide. Veuillez reessayer.");
                pause_console();
        }
    } while (choix != 0);

    /* Sauvegarde automatique de toutes les donnees avant de quitter */
    sauvegarder_tout(livres, nb_livres, emprunteurs, nb_emprunteurs, emprunts, nb_emprunts);

    /* Liberation de toute la memoire allouee dynamiquement */
    liberer_livres(&livres);
    liberer_emprunteurs(&emprunteurs);
    liberer_emprunts(&emprunts);

    return 0;
}
