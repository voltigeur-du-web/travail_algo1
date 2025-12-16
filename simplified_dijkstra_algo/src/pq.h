#ifndef PQ_H
#define PQ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

struct Node_PQ;
typedef struct Node_PQ* Node_PQ_t;

struct PQ;
typedef struct PQ* PQ_t;

//PRE: leq est une fonction d'ordre entre deux float
//POST: Une file de priorite vide est initialisee de taille n_nodes
PQ_t create_pq(float (*leq)(float, float), unsigned int n_nodes);

//PRE: pq est une file de priorite initialisee
//POST: Si la file de priorite est vide, retourne true, sinon false
bool is_pq_empty(PQ_t pq);

//PRE: pq est une file de priorite initialisee, la capacite de pq n'est pas atteinte
// Soit un element a ajouter avec une priorite "float" et un id comme nombre entier
//POST: Ajoute l'element defini par id et priority a file de priorite en respectant la propriete du tas binaire
PQ_t add_to_pq(PQ_t pq, float priority, int id);

//PRE: pq est une file de priorite initialisee et non vide
//POST: enleve l'element le plus prioritaire de la file de priorite en respectant la propriete du tas binaire
PQ_t serve_pq(PQ_t pq);

//PRE: pq est une file de priorite initialisee
//POST: desalloue pq de la memoire
// Fonction pour detruire la file de priorite avec structure
void destroy_pq(PQ_t pq);

//PRE: pq est une file de priorite initialisee avec au moins un element
//POST: retourne un pointeur vers l'element le plus prioritaire sans le retirer
// Fonction pour obtenir l'element le plus prioritaire sans le retirer
Node_PQ_t peek_pq(PQ_t pq);

//PRE: Une file de priorite initialisee et non vide
//POST: Affiche les elements de la file de priorite dans la console
// Fonction pour afficher les elements de la file de priorite
void display_pq(PQ_t pq);

//PRE: Une file de priorite initialisee et non vide
//POST: Enleve un element defini par son id de la file de priorite en respectant la propriete du tas binaire
// Fonction pour enlever un element donner en se basant sur son pointeur
bool remove_id_from_pq(PQ_t pq, int id);


#endif