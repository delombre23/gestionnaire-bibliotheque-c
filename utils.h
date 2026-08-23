/*
 * utils.h
 * Utilitaires communs
 * Projet : Gestionnaire de Bibliotheque - Langage C
 * Prof. Patrick Mukala - UPN L2 Informatique 2025-2026
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define TAILLE_BUFFER 256
#define CAPACITE_INITiale 10

/*
 * Efface l'ecran de la console.
 * Fonctionne sur Linux/Unix et Windows.
 */
void effacer_ecran(void);

/*
 * Affiche une ligne de separation pour l'interface.
 */
void ligne_separation(void);

/*
 * Affiche un titre centre avec encadrement.
 */
void afficher_titre(const char *titre);

/*
 * Affiche un message d'erreur formate.
 */
void afficher_erreur(const char *message);

/*
 * Affiche un message de succes formate.
 */
void afficher_succes(const char *message);

/*
 * Affiche un message d'information formate.
 */
void afficher_info(const char *message);

/*
 * Attend que l'utilisateur appuie sur Entree.
 */
void pause_console(void);

/*
 * Lit une ligne de texte depuis l'entree standard.
 * Supprime le caractere de fin de ligne.
 * Retourne 1 en cas de succes, 0 en cas d'erreur.
 */
int lire_chaine(char *buffer, int taille);

/*
 * Lit un entier depuis l'entree standard avec validation.
 * Affiche un message d'invite et verifie la saisie.
 * Retourne l'entier saisi.
 */
int lire_entier(const char *invite);

/*
 * Lit un entier positif depuis l'entree standard avec validation.
 * Retourne l'entier saisi (forcement positif).
 */
int lire_entier_positif(const char *invite);

/*
 * Demande une confirmation a l'utilisateur (O/N).
 * Retourne 1 pour Oui, 0 pour Non.
 */
int demander_confirmation(const char *message);

/*
 * Convertit une chaine en minuscules (modifie la chaine en place).
 */
void minuscules(char *chaine);

/*
 * Verifie si une chaine contient une sous-chaine (insensible a la casse).
 * Retourne 1 si trouve, 0 sinon.
 */
int contient_sous_chaine(const char *texte, const char *recherche);

/*
 * Vide le buffer d'entree standard.
 * Utilise apres un scanf pour eviter les problemes de lecture.
 */
void vider_buffer(void);

/*
 * Affiche le menu principal de l'application.
 */
void afficher_menu_principal(void);

/*
 * Affiche le sous-menu de gestion des livres.
 */
void afficher_menu_livres(void);

/*
 * Affiche le sous-menu de gestion des emprunteurs.
 */
void afficher_menu_emprunteurs(void);

/*
 * Affiche le sous-menu de gestion des emprunts.
 */
void afficher_menu_emprunts(void);

/*
 * Affiche le sous-menu de rapports et statistiques.
 */
void afficher_menu_rapports(void);

/*
 * Affiche l'en-tete de l'application.
 */
void afficher_en_tete(void);

#endif /* UTILS_H */
