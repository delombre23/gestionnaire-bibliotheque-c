/*
 * emprunt.h
 * Module de gestion des emprunts
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 */

#ifndef EMPRUNT_H
#define EMPRUNT_H

#include "livre.h"
#include "emprunteur.h"

#define DATE_MAX 12
#define FICHIER_EMPRUNTS "emprunts.dat"
#define DUREE_EMPRUNT_JOURS 14      /* Duree d'emprunt standard : 14 jours */
#define AMENDE_PAR_JOUR 500         /* Amende par jour de retard : 500 FC */
#define MAX_EMPRUNTS_PAR_EMPRUNTEUR 5 /* Maximum d'emprunts simultanes */

/*
 * Structure representant un emprunt.
 * Lie un livre, un emprunteur, avec des dates d'emprunt et de retour prevue.
 */
typedef struct {
    int id;                     /* Identifiant unique de l'emprunt */
    int id_livre;               /* Identifiant du livre emprunte */
    int id_emprunteur;          /* Identifiant de l'emprunteur */
    char date_emprunt[DATE_MAX];    /* Date d'emprunt (JJ/MM/AAAA) */
    char date_retour_prevue[DATE_MAX]; /* Date de retour prevue */
    char date_retour_effective[DATE_MAX]; /* Date de retour effective (vide si non retourne) */
    int est_retourne;           /* 1 si retourne, 0 sinon */
    int amendes;                /* Montant des amendes en cas de retard */
} Emprunt;

/* Prototypes des fonctions de gestion des emprunts */

/*
 * Enregistre un nouvel emprunt.
 * Verifie la disponibilite du livre et le nombre d'emprunts actifs de l'emprunteur.
 * Met a jour les compteurs de disponibilite.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int emprunter_livre(Emprunt **emprunts, int *nb_emprunts, int *capacite,
                    Livre *livres, int nb_livres,
                    Emprunteur *emprunteurs, int nb_emprunteurs);

/*
 * Enregistre le retour d'un livre.
 * Calcule les amendes en cas de retard.
 * Met a jour les compteurs de disponibilite.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int retourner_livre(Emprunt *emprunts, int nb_emprunts,
                    Livre *livres, int nb_livres,
                    Emprunteur *emprunteurs, int nb_emprunteurs);

/*
 * Affiche l'historique complet des emprunts.
 */
void afficher_historique_emprunts(const Emprunt *emprunts, int nb_emprunts);

/*
 * Affiche l'historique des emprunts d'un emprunteur specifique.
 */
void afficher_historique_par_emprunteur(const Emprunt *emprunts, int nb_emprunts,
                                        int id_emprunteur,
                                        const Livre *livres, int nb_livres);

/*
 * Affiche les emprunts en cours (non retournes).
 */
void afficher_emprunts_en_cours(const Emprunt *emprunts, int nb_emprunts,
                                const Livre *livres, int nb_livres,
                                const Emprunteur *emprunteurs, int nb_emprunteurs);

/*
 * Affiche les emprunts en retard (date prevue passee et non retournes).
 */
void afficher_emprunts_en_retard(const Emprunt *emprunts, int nb_emprunts,
                                 const Livre *livres, int nb_livres,
                                 const Emprunteur *emprunteurs, int nb_emprunteurs);

/*
 * Calcule le nombre de jours de retard pour un emprunt.
 * Retourne le nombre de jours de retard (0 si pas de retard).
 */
int calculer_jours_retard(const char *date_retour_prevue);

/*
 * Calcule le montant des amendes pour un emprunt.
 * Retourne le montant total des amendes.
 */
int calculer_amendes(const Emprunt *emprunt);

/*
 * Verifie si un livre est disponible pour l'emprunt.
 * Retourne 1 si disponible, 0 sinon.
 */
int verifier_disponibilite(const Livre *livres, int nb_livres, int id_livre);

/*
 * Compte le nombre d'emprunts actifs d'un emprunteur.
 * Retourne le nombre d'emprunts en cours.
 */
int compter_emprunts_actifs(const Emprunt *emprunts, int nb_emprunts, int id_emprunteur);

/*
 * Obtient la date du jour au format JJ/MM/AAAA.
 */
void obtenir_date_du_jour(char *date);

/*
 * Calcule la date de retour prevue (date du jour + DUREE_EMPRUNT_JOURS).
 */
void calculer_date_retour_prevue(const char *date_emprunt, char *date_retour);

/*
 * Compare deux dates au format JJ/MM/AAAA.
 * Retourne : <0 si date1 < date2, 0 si egales, >0 si date1 > date2.
 */
int comparer_dates(const char *date1, const char *date2);

/*
 * Sauvegarde tous les emprunts dans un fichier binaire.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int sauvegarder_emprunts(const Emprunt *emprunts, int nb_emprunts);

/*
 * Charge les emprunts depuis un fichier binaire.
 * Alloue dynamiquement le tableau.
 * Retourne le nombre d'emprunts charges.
 */
int charger_emprunts(Emprunt **emprunts, int *capacite);

/*
 * Libere la memoire allouee pour le tableau d'emprunts.
 */
void liberer_emprunts(Emprunt **emprunts);

#endif /* EMPRUNT_H */
