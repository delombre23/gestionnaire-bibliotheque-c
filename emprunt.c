/*
 * emprunt.c
 * Implementation du module de gestion des emprunts
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 *
 * Ce module gere toutes les operations liees aux emprunts :
 * - Enregistrement d'un emprunt (verifie disponibilite et limites)
 * - Retour d'un livre (calcule les amendes en cas de retard)
 * - Affichage des emprunts en cours, en retard, historique
 * - Calcul des dates (emprunt, retour prevue, retard)
 * - Sauvegarde et chargement depuis fichier binaire
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "emprunt.h"
#include "utils.h"

/*
 * Obtient la date du jour au format JJ/MM/AAAA.
 * Utilise la fonction time() et localtime() de la bibliotheque standard.
 */
void obtenir_date_du_jour(char *date) {
    time_t maintenant = time(NULL);
    struct tm *tm_info = localtime(&maintenant);
    snprintf(date, DATE_MAX, "%02d/%02d/%04d",
             tm_info->tm_mday,
             tm_info->tm_mon + 1,
             tm_info->tm_year + 1900);
}

/*
 * Convertit une date JJ/MM/AAAA en nombre de jours depuis une reference.
 * Permet de comparer deux dates et de calculer des differences.
 * C'est une approximation suffisante pour ce projet.
 */
static int date_en_jours(const char *date) {
    int jour, mois, annee;
    sscanf(date, "%d/%d/%d", &jour, &mois, &annee);
    
    /* Simplification : nombre de jours approximatif depuis l'an 0 */
    return annee * 365 + mois * 30 + jour;
}

/*
 * Compare deux dates au format JJ/MM/AAAA.
 * Retourne : <0 si date1 < date2, 0 si egales, >0 si date1 > date2.
 */
int comparer_dates(const char *date1, const char *date2) {
    return date_en_jours(date1) - date_en_jours(date2);
}

/*
 * Calcule la date de retour prevue (date du jour + DUREE_EMPRUNT_JOURS).
 * La duree d'emprunt standard est definie dans emprunt.h (14 jours).
 */
void calculer_date_retour_prevue(const char *date_emprunt, char *date_retour) {
    int jours = date_en_jours(date_emprunt) + DUREE_EMPRUNT_JOURS;
    int annee = jours / 365;
    int reste = jours % 365;
    int mois = reste / 30;
    int jour = reste % 30;
    
    if (mois == 0) { mois = 1; }
    if (jour == 0) { jour = 1; }
    if (mois > 12) { mois = 12; }
    
    snprintf(date_retour, DATE_MAX, "%02d/%02d/%04d", jour, mois, annee);
}

/*
 * Calcule le nombre de jours de retard pour un emprunt.
 * Compare la date du jour avec la date de retour prevue.
 * Retourne le nombre de jours de retard (0 si pas de retard).
 */
int calculer_jours_retard(const char *date_retour_prevue) {
    char date_aujourd_hui[DATE_MAX];
    obtenir_date_du_jour(date_aujourd_hui);
    
    int diff = comparer_dates(date_aujourd_hui, date_retour_prevue);
    if (diff > 0) {
        return diff;
    }
    return 0;
}

/*
 * Calcule le montant des amendes pour un emprunt.
 * Si l'emprunt est deja retourne, retourne les amendes deja calculees.
 * Sinon, calcule en fonction du retard actuel.
 * Le taux est defini dans emprunt.h (500 FC par jour).
 */
int calculer_amendes(const Emprunt *emprunt) {
    if (emprunt->est_retourne) {
        return emprunt->amendes;
    }
    int retard = calculer_jours_retard(emprunt->date_retour_prevue);
    return retard * AMENDE_PAR_JOUR;
}

/*
 * Verifie si un livre est disponible pour l'emprunt.
 * Un livre est disponible si nb_disponibles > 0.
 * Retourne 1 si disponible, 0 sinon.
 */
int verifier_disponibilite(const Livre *livres, int nb_livres, int id_livre) {
    int index = rechercher_livre_par_id(livres, nb_livres, id_livre);
    if (index == -1) return 0;
    return (livres[index].nb_disponibles > 0);
}

/*
 * Compte le nombre d'emprunts actifs (non retournes) d'un emprunteur.
 * Utilisee pour verifier la limite d'emprunts simultanes.
 * Retourne le nombre d'emprunts en cours.
 */
int compter_emprunts_actifs(const Emprunt *emprunts, int nb_emprunts, int id_emprunteur) {
    int count = 0;
    for (int i = 0; i < nb_emprunts; i++) {
        if (emprunts[i].id_emprunteur == id_emprunteur && !emprunts[i].est_retourne) {
            count++;
        }
    }
    return count;
}

/*
 * Enregistre un nouvel emprunt.
 * Verifications effectuees :
 *   - Le livre existe et est disponible
 *   - L'emprunteur existe
 *   - L'emprunteur n'a pas atteint la limite d'emprunts simultanes (5)
 * Mise a jour des compteurs : nb_disponibles du livre, nb_emprunts_actifs de l'emprunteur.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int emprunter_livre(Emprunt **emprunts, int *nb_emprunts, int *capacite,
                    Livre *livres, int nb_livres,
                    Emprunteur *emprunteurs, int nb_emprunteurs) {
    Emprunt nouvel_emprunt;
    int id_livre, id_emprunteur;
    int idx_livre, idx_emprunteur;

    printf("\n");
    afficher_titre("EMPRUNT D'UN LIVRE");

    /* Saisie de l'ID du livre */
    id_livre = lire_entier("  ID du livre a emprunter : ");
    idx_livre = rechercher_livre_par_id(livres, nb_livres, id_livre);
    
    if (idx_livre == -1) {
        afficher_erreur("Livre non trouve.");
        return 0;
    }

    /* Verification de la disponibilite du livre */
    if (livres[idx_livre].nb_disponibles <= 0) {
        afficher_erreur("Ce livre n'est pas disponible (tous les exemplaires sont empruntes).");
        return 0;
    }

    /* Affichage du livre selectionne */
    printf("\n  Livre selectionne :\n");
    afficher_livre(&livres[idx_livre]);

    /* Saisie de l'ID de l'emprunteur */
    id_emprunteur = lire_entier("\n  ID de l'emprunteur : ");
    idx_emprunteur = rechercher_emprunteur_par_id(emprunteurs, nb_emprunteurs, id_emprunteur);
    
    if (idx_emprunteur == -1) {
        afficher_erreur("Emprunteur non trouve.");
        return 0;
    }

    /* Verification du nombre d'emprunts actifs de l'emprunteur */
    int emprunts_actifs = compter_emprunts_actifs(*emprunts, *nb_emprunts, id_emprunteur);
    if (emprunts_actifs >= MAX_EMPRUNTS_PAR_EMPRUNTEUR) {
        afficher_erreur("Cet emprunteur a atteint le maximum d'emprunts simultanes (5).");
        return 0;
    }

    printf("\n  Emprunteur : %s %s\n", emprunteurs[idx_emprunteur].prenom, emprunteurs[idx_emprunteur].nom);

    /* Confirmation de l'utilisateur */
    if (!demander_confirmation("\n  Confirmer l'emprunt ?")) {
        afficher_info("Emprunt annule.");
        return 0;
    }

    /* Creation de l'emprunt */
    nouvel_emprunt.id = (*nb_emprunts > 0) ? (*emprunts)[*nb_emprunts - 1].id + 1 : 1;
    nouvel_emprunt.id_livre = id_livre;
    nouvel_emprunt.id_emprunteur = id_emprunteur;
    obtenir_date_du_jour(nouvel_emprunt.date_emprunt);
    calculer_date_retour_prevue(nouvel_emprunt.date_emprunt, nouvel_emprunt.date_retour_prevue);
    strcpy(nouvel_emprunt.date_retour_effective, "");
    nouvel_emprunt.est_retourne = 0;
    nouvel_emprunt.amendes = 0;

    /* Mise a jour des compteurs */
    livres[idx_livre].nb_disponibles--;
    emprunteurs[idx_emprunteur].nb_emprunts_actifs++;

    /* Reallocation dynamique si le tableau est plein */
    if (*nb_emprunts >= *capacite) {
        int nouvelle_capacite = *capacite * 2;
        Emprunt *temp = (Emprunt *)realloc(*emprunts, nouvelle_capacite * sizeof(Emprunt));
        if (temp == NULL) {
            afficher_erreur("Erreur d'allocation memoire. Impossible d'enregistrer l'emprunt.");
            /* Annulation des modifications */
            livres[idx_livre].nb_disponibles++;
            emprunteurs[idx_emprunteur].nb_emprunts_actifs--;
            return 0;
        }
        *emprunts = temp;
        *capacite = nouvelle_capacite;
    }

    /* Ajout de l'emprunt a la fin du tableau */
    (*emprunts)[*nb_emprunts] = nouvel_emprunt;
    (*nb_emprunts)++;

    afficher_succes("Emprunt enregistre avec succes !");
    printf("  Date d'emprunt         : %s\n", nouvel_emprunt.date_emprunt);
    printf("  Date de retour prevue  : %s\n", nouvel_emprunt.date_retour_prevue);

    return 1;
}

/*
 * Enregistre le retour d'un livre.
 * Calcule automatiquement les amendes en cas de retard.
 * Mise a jour des compteurs : nb_disponibles du livre, nb_emprunts_actifs de l'emprunteur.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int retourner_livre(Emprunt *emprunts, int nb_emprunts,
                    Livre *livres, int nb_livres,
                    Emprunteur *emprunteurs, int nb_emprunteurs) {
    int id_emprunt, idx_emprunt;
    int idx_livre, idx_emprunteur;

    printf("\n");
    afficher_titre("RETOUR D'UN LIVRE");

    id_emprunt = lire_entier("  ID de l'emprunt : ");

    /* Recherche de l'emprunt actif (non retourne) */
    idx_emprunt = -1;
    for (int i = 0; i < nb_emprunts; i++) {
        if (emprunts[i].id == id_emprunt && !emprunts[i].est_retourne) {
            idx_emprunt = i;
            break;
        }
    }

    if (idx_emprunt == -1) {
        afficher_erreur("Emprunt non trouve ou deja retourne.");
        return 0;
    }

    /* Recuperation des index du livre et de l'emprunteur */
    idx_livre = rechercher_livre_par_id(livres, nb_livres, emprunts[idx_emprunt].id_livre);
    idx_emprunteur = rechercher_emprunteur_par_id(emprunteurs, nb_emprunteurs, emprunts[idx_emprunt].id_emprunteur);

    /* Calcul des amendes */
    obtenir_date_du_jour(emprunts[idx_emprunt].date_retour_effective);
    int jours_retard = calculer_jours_retard(emprunts[idx_emprunt].date_retour_prevue);
    emprunts[idx_emprunt].amendes = jours_retard * AMENDE_PAR_JOUR;

    /* Affichage des informations du retour */
    printf("\n  Livre : %s\n", (idx_livre != -1) ? livres[idx_livre].titre : "Inconnu");
    printf("  Emprunteur : %s %s\n", 
           (idx_emprunteur != -1) ? emprunteurs[idx_emprunteur].prenom : "?",
           (idx_emprunteur != -1) ? emprunteurs[idx_emprunteur].nom : "?");
    printf("  Date de retour prevue    : %s\n", emprunts[idx_emprunt].date_retour_prevue);
    printf("  Date de retour effective : %s\n", emprunts[idx_emprunt].date_retour_effective);
    
    if (jours_retard > 0) {
        printf("  Jours de retard : %d\n", jours_retard);
        printf("  Amendes a payer : %d FC\n", emprunts[idx_emprunt].amendes);
    } else {
        printf("  Pas de retard. Aucune amende.\n");
    }

    /* Confirmation de l'utilisateur */
    if (!demander_confirmation("\n  Confirmer le retour ?")) {
        /* Annulation : reinitialise les champs modifies */
        strcpy(emprunts[idx_emprunt].date_retour_effective, "");
        emprunts[idx_emprunt].amendes = 0;
        afficher_info("Retour annule.");
        return 0;
    }

    /* Mise a jour definitive */
    emprunts[idx_emprunt].est_retourne = 1;
    
    if (idx_livre != -1) {
        livres[idx_livre].nb_disponibles++;
    }
    if (idx_emprunteur != -1) {
        emprunteurs[idx_emprunteur].nb_emprunts_actifs--;
    }

    afficher_succes("Livre retourne avec succes !");
    return 1;
}

/*
 * Affiche l'historique complet de tous les emprunts.
 * Affiche un message si aucun emprunt n'est enregistre.
 */
void afficher_historique_emprunts(const Emprunt *emprunts, int nb_emprunts) {
    if (nb_emprunts == 0) {
        afficher_info("Aucun emprunt enregistre.");
        return;
    }

    printf("\n");
    afficher_titre("HISTORIQUE COMPLET DES EMPRUNTS");
    printf("  +------+---------+-------------+------------------+------------------+----------+--------+%c\n", ' ');
    printf("  | ID   | ID Livre| ID Emprunt. | Date Emprunt     | Retour Prevue    | Retourne | Amende |\n");
    printf("  +------+---------+-------------+------------------+------------------+----------+--------+%c\n", ' ');

    for (int i = 0; i < nb_emprunts; i++) {
        printf("  | %-4d | %-7d | %-11d | %-16s | %-16s | %-8s | %-6d |\n",
               emprunts[i].id,
               emprunts[i].id_livre,
               emprunts[i].id_emprunteur,
               emprunts[i].date_emprunt,
               emprunts[i].date_retour_prevue,
               emprunts[i].est_retourne ? "OUI" : "NON",
               emprunts[i].amendes);
    }

    printf("  +------+---------+-------------+------------------+------------------+----------+--------+%c\n", ' ');
    printf("  Total : %d emprunt(s)\n", nb_emprunts);
}

/*
 * Affiche les emprunts en cours (non retournes).
 * Affiche le titre du livre, le nom de l'emprunteur, la date de retour prevue et le retard eventuel.
 */
void afficher_emprunts_en_cours(const Emprunt *emprunts, int nb_emprunts,
                                const Livre *livres, int nb_livres,
                                const Emprunteur *emprunteurs, int nb_emprunteurs) {
    int count = 0;

    printf("\n");
    afficher_titre("EMPRUNTS EN COURS");

    for (int i = 0; i < nb_emprunts; i++) {
        if (!emprunts[i].est_retourne) {
            if (count == 0) {
                printf("  +------+---------------------------+----------------------+------------------+----------+\n");
                printf("  | ID   | Livre                     | Emprunteur           | Retour Prevue    | Retard   |\n");
                printf("  +------+---------------------------+----------------------+------------------+----------+\n");
            }

            /* Recherche des informations du livre et de l'emprunteur */
            const char *titre_livre = "Inconnu";
            const char *nom_emprunteur = "Inconnu";
            
            for (int j = 0; j < nb_livres; j++) {
                if (livres[j].id == emprunts[i].id_livre) {
                    titre_livre = livres[j].titre;
                    break;
                }
            }
            
            for (int j = 0; j < nb_emprunteurs; j++) {
                if (emprunteurs[j].id == emprunts[i].id_emprunteur) {
                    nom_emprunteur = emprunteurs[j].nom;
                    break;
                }
            }

            int retard = calculer_jours_retard(emprunts[i].date_retour_prevue);
            char retard_str[20];
            if (retard > 0) {
                snprintf(retard_str, sizeof(retard_str), "%d jours", retard);
            } else {
                strcpy(retard_str, "Aucun");
            }

            printf("  | %-4d | %-25s | %-20s | %-16s | %-8s |\n",
                   emprunts[i].id, titre_livre, nom_emprunteur,
                   emprunts[i].date_retour_prevue, retard_str);
            count++;
        }
    }

    if (count > 0) {
        printf("  +------+---------------------------+----------------------+------------------+----------+\n");
        printf("  Total : %d emprunt(s) en cours\n", count);
    } else {
        afficher_info("Aucun emprunt en cours.");
    }
}

/*
 * Affiche les emprunts en retard (date prevue passee et non retournes).
 * Affiche le montant des amendes dues pour chaque emprunt en retard.
 */
void afficher_emprunts_en_retard(const Emprunt *emprunts, int nb_emprunts,
                                 const Livre *livres, int nb_livres,
                                 const Emprunteur *emprunteurs, int nb_emprunteurs) {
    int count = 0;

    printf("\n");
    afficher_titre("EMPRUNTS EN RETARD");

    for (int i = 0; i < nb_emprunts; i++) {
        if (!emprunts[i].est_retourne) {
            int retard = calculer_jours_retard(emprunts[i].date_retour_prevue);
            if (retard > 0) {
                if (count == 0) {
                    printf("  +------+---------------------------+----------------------+--------+----------+\n");
                    printf("  | ID   | Livre                     | Emprunteur           | Retard | Amende   |\n");
                    printf("  +------+---------------------------+----------------------+--------+----------+\n");
                }

                const char *titre_livre = "Inconnu";
                const char *nom_emprunteur = "Inconnu";
                
                for (int j = 0; j < nb_livres; j++) {
                    if (livres[j].id == emprunts[i].id_livre) {
                        titre_livre = livres[j].titre;
                        break;
                    }
                }
                
                for (int j = 0; j < nb_emprunteurs; j++) {
                    if (emprunteurs[j].id == emprunts[i].id_emprunteur) {
                        nom_emprunteur = emprunteurs[j].nom;
                        break;
                    }
                }

                printf("  | %-4d | %-25s | %-20s | %6d | %8d |\n",
                       emprunts[i].id, titre_livre, nom_emprunteur,
                       retard, retard * AMENDE_PAR_JOUR);
                count++;
            }
        }
    }

    if (count > 0) {
        printf("  +------+---------------------------+----------------------+--------+----------+\n");
        printf("  Total : %d emprunt(s) en retard\n", count);
    } else {
        afficher_info("Aucun emprunt en retard.");
    }
}

/*
 * Affiche l'historique des emprunts d'un emprunteur specifique.
 * Affiche tous les emprunts (retournes et non retournes) de cet emprunteur.
 */
void afficher_historique_par_emprunteur(const Emprunt *emprunts, int nb_emprunts,
                                        int id_emprunteur,
                                        const Livre *livres, int nb_livres) {
    int count = 0;

    printf("\n");
    afficher_titre("HISTORIQUE DES EMPRUNTS");

    for (int i = 0; i < nb_emprunts; i++) {
        if (emprunts[i].id_emprunteur == id_emprunteur) {
            if (count == 0) {
                printf("  +------+---------------------------+------------------+----------+--------+\n");
                printf("  | ID   | Livre                     | Date Emprunt     | Retourne | Amende |\n");
                printf("  +------+---------------------------+------------------+----------+--------+\n");
            }

            const char *titre_livre = "Inconnu";
            for (int j = 0; j < nb_livres; j++) {
                if (livres[j].id == emprunts[i].id_livre) {
                    titre_livre = livres[j].titre;
                    break;
                }
            }

            printf("  | %-4d | %-25s | %-16s | %-8s | %-6d |\n",
                   emprunts[i].id, titre_livre, emprunts[i].date_emprunt,
                   emprunts[i].est_retourne ? "OUI" : "NON",
                   emprunts[i].amendes);
            count++;
        }
    }

    if (count > 0) {
        printf("  +------+---------------------------+------------------+----------+--------+\n");
        printf("  Total : %d emprunt(s)\n", count);
    } else {
        afficher_info("Aucun emprunt pour cet emprunteur.");
    }
}

/*
 * Sauvegarde les emprunts dans un fichier binaire.
 * Format du fichier : [int nb_emprunts][Emprunt emprunt1]...
 * Gestion des erreurs : fichier inaccessible, erreur d'ecriture.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int sauvegarder_emprunts(const Emprunt *emprunts, int nb_emprunts) {
    FILE *fichier = fopen(FICHIER_EMPRUNTS, "wb");
    if (fichier == NULL) {
        afficher_erreur("Impossible d'ouvrir le fichier de sauvegarde des emprunts.");
        afficher_erreur(strerror(errno));
        return 0;
    }

    if (fwrite(&nb_emprunts, sizeof(int), 1, fichier) != 1) {
        fclose(fichier);
        afficher_erreur("Erreur lors de l'ecriture du nombre d'emprunts.");
        return 0;
    }

    if (nb_emprunts > 0) {
        if (fwrite(emprunts, sizeof(Emprunt), (size_t)nb_emprunts, fichier) != (size_t)nb_emprunts) {
            fclose(fichier);
            afficher_erreur("Erreur lors de l'ecriture des emprunts.");
            return 0;
        }
    }

    fclose(fichier);
    return 1;
}

/*
 * Charge les emprunts depuis un fichier binaire.
 * Alloue dynamiquement le tableau avec la capacite initiale.
 * Gestion des erreurs : fichier inexistant (premier demarrage), corruption, memoire insuffisante.
 * Retourne le nombre d'emprunts charges.
 */
int charger_emprunts(Emprunt **emprunts, int *capacite) {
    FILE *fichier = fopen(FICHIER_EMPRUNTS, "rb");
    int nb_emprunts = 0;

    if (fichier == NULL) {
        /* Le fichier n'existe pas encore : c'est normal au premier demarrage */
        *capacite = CAPACITE_INITiale;
        *emprunts = (Emprunt *)malloc(*capacite * sizeof(Emprunt));
        if (*emprunts == NULL) {
            afficher_erreur("Erreur d'allocation memoire pour les emprunts.");
            return 0;
        }
        return 0;
    }

    if (fread(&nb_emprunts, sizeof(int), 1, fichier) != 1) {
        fclose(fichier);
        afficher_erreur("Erreur lors de la lecture du fichier d'emprunts (fichier corrompu ?).");
        *capacite = CAPACITE_INITiale;
        *emprunts = (Emprunt *)malloc(*capacite * sizeof(Emprunt));
        return 0;
    }

    *capacite = nb_emprunts + CAPACITE_INITiale;
    *emprunts = (Emprunt *)malloc(*capacite * sizeof(Emprunt));
    if (*emprunts == NULL) {
        fclose(fichier);
        afficher_erreur("Erreur d'allocation memoire pour les emprunts.");
        *capacite = 0;
        return 0;
    }

    if (nb_emprunts > 0) {
        if (fread(*emprunts, sizeof(Emprunt), (size_t)nb_emprunts, fichier) != (size_t)nb_emprunts) {
            fclose(fichier);
            free(*emprunts);
            *emprunts = NULL;
            *capacite = 0;
            afficher_erreur("Erreur lors de la lecture des emprunts (fichier incomplet ?).");
            return 0;
        }
    }

    fclose(fichier);
    return nb_emprunts;
}

/*
 * Libere la memoire allouee pour le tableau d'emprunts.
 * Met le pointeur a NULL apres liberation pour eviter les dangling pointers.
 */
void liberer_emprunts(Emprunt **emprunts) {
    if (*emprunts != NULL) {
        free(*emprunts);
        *emprunts = NULL;
    }
}
