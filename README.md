# Gestionnaire de Bibliotheque

**Projet de Programmation en Langage C**  
**UPN - L2 Informatique - Prof. Patrick Mukala**  
**Annee academique 2025-2026**

---

## Description

Ce projet est un systeme de gestion de bibliotheque complet developpe en langage C.
Il permet de gerer les livres, les emprunteurs (adherents) et les emprunts
de maniere interactive via une interface console.

---

## Fonctionnalites

### Gestion des Livres
- Ajouter un livre (titre, auteur, ISBN, editeur, annee, nombre d'exemplaires)
- Modifier un livre existant
- Supprimer un livre (si aucun exemplaire n'est emprunte)
- Rechercher par titre, auteur ou ISBN
- Afficher tous les livres ou uniquement les disponibles

### Gestion des Emprunteurs
- Ajouter un emprunteur (nom, prenom, adresse, telephone, email)
- Modifier un emprunteur existant
- Supprimer un emprunteur (si aucun emprunt actif)
- Rechercher par nom ou numero de carte
- Generation automatique du numero de carte (CARTE-XXXX)

### Gestion des Emprunts
- Emprunter un livre (verifie disponibilite et limite d'emprunts)
- Retourner un livre (calcule automatiquement les amendes)
- Afficher les emprunts en cours
- Afficher les emprunts en retard
- Historique des emprunts par emprunteur

### Rapports et Statistiques
- Statistiques generales (totaux, emprunts en retard)
- Total des amendes collectees

---

## Compilation

```bash
make
