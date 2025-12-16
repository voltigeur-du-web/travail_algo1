#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>
#include "pq.h"

//PRE: n est le nombre de noeuds du graphe, 
// - w est une matrice de poids de nombres a virgules flottantes non negatifs, 
// - s l'identifiant du noeud source, 
// - d est l'identifiant du noeud de destination, 
// - path est un pointeur vers un tableau d'entiers naturels
//POST: retourne le nombre de noeuds dans le chemin le plus court entre la source et la destination
    // Soit 1 si la source est egale a la destination
    // Soit 0 si aucun chemin n'existe entre la source et la destination
    // Soit un entier m ou  1 < m <= n_nodes est le nombre de noeuds dans le chemin le plus court
// - path est alloue dynamiquement et contient la sequence d'identifiants des noeuds formant le chemin le plus court
unsigned int shortest_path(unsigned int n, float** w, unsigned int s, unsigned int d, unsigned int** path);


#endif