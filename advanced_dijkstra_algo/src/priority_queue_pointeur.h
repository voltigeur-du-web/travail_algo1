/*
Auteur: Pierre-André Marchand (2025-02-15)
Description: file d'entiers. On peut y ajouter des éléments et récupérer à chaque fois l'élément qui est dans la
file depuis le plus longtemps.
*/
#ifndef PQ_H
#define PQ_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct Node_Q;
typedef struct Node_PQ* Node_PQ_t;

struct PQ; //déclaration du nom de ma structure
typedef struct PQ* PQ_t; //déclaration d'un alias qui désigne le type "pointeur vers struct Queue"

//PRE: /
//POST: initialise une PQ vide avec une relation d'ordre 
PQ_t create_pq(float (*leq)(float,float), unsigned int n_nodes);

//PRE: pq est definie et taille de pq < taille max
//POST: ajout de l'élément dans la queue
PQ_t add_to_pq(PQ_t pq, float priority, void* data);

//PRE: pq est definie et de taille > 0
//POST: Retourne pq sans l'élément le plus prioritaire
PQ_t serve_pq(PQ_t pq);

//PRE: pq est défini
//POST: Retourne True si la file est vide, False sinon
bool is_pq_empty(PQ_t pq);

//PRE: pq est défini
//POST: Retourne True si la file est pleine, False sinon
bool is_pq_full(PQ_t pq);

//PRE: pq est défini et de taille > 0
//POST: Retourne l'élément le plus prioritaire
Node_PQ_t peek_pq(PQ_t pq);

//PRE: pq est défini et de taille > 0
//POST: Détruit la file de priorité
void destroy_pq(PQ_t pq);

//PRE: pq est défini et de taille > 0
//POST: Montre les éléments de la file de priorité
void display_pq(PQ_t pq);

//PRE: pq est défini et de taille > 0 && data est un pointeur
//POST: Retourne un booleen si pointeur data trouvé dans PQ (trouvé: true & non trouvé: false)
    // Enlève l'élément de la PQ ayant le même pointeur que data
bool remove_pointer_from_pq(PQ_t pq, void* data);

#endif
