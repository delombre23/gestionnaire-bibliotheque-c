/*
 * livre.h
 * Module de gestion des livres
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 */

#ifndef LIVRE_H
#define LIVRE_H

#define TITRE_MAX 100
#define AUTEUR_MAX 100
#define ISBN_MAX 20
#define EDITEUR_MAX 100
#define FICHIER_LIVRES "livres.dat"

/*
 * Structure representant un livre dans la bibliotheque.
 * Chaque livre possede un identifiant unique, un titre, un auteur,
 * un ISBN, un editeur, une annee de publication, un nombre d'exemplaires
 * et un compteur d'exemplaires disponibles.
 */
typedef struct {
    int id;                     /* Identifiant unique du livre */
    char titre[TITRE_MAX];      /* Titre du livre */
    char auteur[AUTEUR_MAX];    /* Nom de l'auteur */
    char isbn[ISBN_MAX];        /* Numero ISBN */
    char editeur[EDITEUR_MAX];  /* Nom de l'editeur */
    int annee;                  /* Annee de publication */
    int nb_exemplaires;         /* Nombre total d'exemplaires */
    int nb_disponibles;         /* Nombre d'exemplaires disponibles */
} Livre;

/* Prototypes des fonctions de gestion des livres */

/*
 * Ajoute un nouveau livre dans le tableau dynamique.
 * Alloue de la memoire si necessaire.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int ajouter_livre(Livre **livres, int *nb_livres, int *capacite);

/*
 * Modifie les informations d'un livre existant.
 * Retourne 1 en cas de succes, 0 si le livre n'est pas trouve.
 */
int modifier_livre(Livre *livres, int nb_livres);

/*
 * Supprime un livre du tableau.
 * Decale les elements et libere la memoire si necessaire.
 * Retourne 1 en cas de succes, 0 si le livre n'est pas trouve.
 */
int supprimer_livre(Livre **livres, int *nb_livres);

/*
 * Recherche un livre par son identifiant.
 * Retourne l'index du livre dans le tableau, ou -1 si non trouve.
 */
int rechercher_livre_par_id(const Livre *livres, int nb_livres, int id);

/*
 * Recherche des livres par titre (recherche partielle, insensible a la casse).
 * Affiche les resultats trouves.
 */
void rechercher_livre_par_titre(const Livre *livres, int nb_livres, const char *titre);

/*
 * Recherche des livres par auteur (recherche partielle, insensible a la casse).
 * Affiche les resultats trouves.
 */
void rechercher_livre_par_auteur(const Livre *livres, int nb_livres, const char *auteur);

/*
 * Recherche un livre par son ISBN exact.
 * Affiche le resultat trouve.
 */
void rechercher_livre_par_isbn(const Livre *livres, int nb_livres, const char *isbn);

/*
 * Affiche la liste complete de tous les livres.
 */
void afficher_tous_les_livres(const Livre *livres, int nb_livres);

/*
 * Affiche uniquement les livres disponibles (nb_disponibles > 0).
 */
void afficher_livres_disponibles(const Livre *livres, int nb_livres);

/*
 * Affiche les details d'un seul livre.
 */
void afficher_livre(const Livre *livre);

/*
 * Sauvegarde tous les livres dans un fichier binaire.
 * Retourne 1 en cas de succes, 0 en cas d'echec.
 */
int sauvegarder_livres(const Livre *livres, int nb_livres);

/*
 * Charge les livres depuis un fichier binaire.
 * Alloue dynamiquement le tableau.
 * Retourne le nombre de livres charges, ou 0 si le fichier n'existe pas.
 */
int charger_livres(Livre **livres, int *capacite);

/*
 * Genere un nouvel identifiant unique pour un livre.
 */
int generer_id_livre(const Livre *livres, int nb_livres);

/*
 * Verifie si un ISBN est valide (format basique).
 * Retourne 1 si valide, 0 sinon.
 */
int valider_isbn(const char *isbn);

/*
 * Libere la memoire allouee pour le tableau de livres.
 */
void liberer_livres(Livre **livres);

#endif /* LIVRE_H */
