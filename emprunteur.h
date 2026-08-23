/*
 * emprunteur.h
 * Module de gestion des emprunteurs
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 */

#ifndef EMPRUNTEUR_H
#define EMPRUNTEUR_H

#define NOM_MAX 50
#define PRENOM_MAX 50
#define ADRESSE_MAX 200
#define TELEPHONE_MAX 20
#define EMAIL_MAX 100
#define NUMERO_CARTE_MAX 20
#define FICHIER_EMPRUNTEURS "emprunteurs.dat"

/*
 * Structure representant un emprunteur (adherent) de la bibliotheque.
 * Chaque emprunteur possede un identifiant unique, un numero de carte,
 * des informations personnelles et un compteur d'emprunts actifs.
 */
typedef struct {
    int id;                         /* Identifiant unique */
    char numero_carte[NUMERO_CARTE_MAX]; /* Numero de carte unique */
    char nom[NOM_MAX];              /* Nom de famille */
    char prenom[PRENOM_MAX];        /* Prenom */
    char adresse[ADRESSE_MAX];      /* Adresse complete */
    char telephone[TELEPHONE_MAX];  /* Numero de telephone */
    char email[EMAIL_MAX];          /* Adresse email */
    int nb_emprunts_actifs;         /* Nombre d'emprunts en cours */
} Emprunteur;

/* Prototypes des fonctions de gestion des emprunteurs */

/*
 * Ajoute un nouvel emprunteur dans le tableau dynamique.
 * Genere automatiquement un numero de carte unique.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int ajouter_emprunteur(Emprunteur **emprunteurs, int *nb_emprunteurs, int *capacite);

/*
 * Modifie les informations d'un emprunteur existant.
 * Retourne 1 en cas de succes, 0 si non trouve.
 */
int modifier_emprunteur(Emprunteur *emprunteurs, int nb_emprunteurs);

/*
 * Supprime un emprunteur du tableau.
 * Verifie qu'il n'a pas d'emprunts actifs avant suppression.
 * Retourne 1 en cas de succes, 0 si non trouve ou emprunts actifs.
 */
int supprimer_emprunteur(Emprunteur **emprunteurs, int *nb_emprunteurs);

/*
 * Recherche un emprunteur par son identifiant.
 * Retourne l'index dans le tableau, ou -1 si non trouve.
 */
int rechercher_emprunteur_par_id(const Emprunteur *emprunteurs, int nb_emprunteurs, int id);

/*
 * Recherche des emprunteurs par nom (recherche partielle).
 * Affiche les resultats trouves.
 */
void rechercher_emprunteur_par_nom(const Emprunteur *emprunteurs, int nb_emprunteurs, const char *nom);

/*
 * Recherche un emprunteur par son numero de carte exact.
 * Affiche le resultat trouve.
 */
void rechercher_emprunteur_par_carte(const Emprunteur *emprunteurs, int nb_emprunteurs, const char *numero_carte);

/*
 * Affiche la liste complete de tous les emprunteurs.
 */
void afficher_tous_les_emprunteurs(const Emprunteur *emprunteurs, int nb_emprunteurs);

/*
 * Affiche les details d'un seul emprunteur.
 */
void afficher_emprunteur(const Emprunteur *emprunteur);

/*
 * Sauvegarde tous les emprunteurs dans un fichier binaire.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int sauvegarder_emprunteurs(const Emprunteur *emprunteurs, int nb_emprunteurs);

/*
 * Charge les emprunteurs depuis un fichier binaire.
 * Alloue dynamiquement le tableau.
 * Retourne le nombre d'emprunteurs charges.
 */
int charger_emprunteurs(Emprunteur **emprunteurs, int *capacite);

/*
 * Genere un numero de carte unique au format "CARTE-XXXX".
 */
void generer_numero_carte(char *numero_carte, int id);

/*
 * Verifie si un numero de telephone est valide.
 * Retourne 1 si valide, 0 sinon.
 */
int valider_telephone(const char *telephone);

/*
 * Verifie si une adresse email est valide (format basique).
 * Retourne 1 si valide, 0 sinon.
 */
int valider_email(const char *email);

/*
 * Libere la memoire allouee pour le tableau d'emprunteurs.
 */
void liberer_emprunteurs(Emprunteur **emprunteurs);

#endif /* EMPRUNTEUR_H */
