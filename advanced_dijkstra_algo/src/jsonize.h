
#ifndef JSONIZE_H
#define JSONIZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

#include "types.h"

//PRE: graph est initilialisé; 
//  starting_index est un entier positif représentant l'id dans graph du noeud de départ
//  end_index est un entier positif représentant l'id dans graph du noeud de destination 
//  filename est une chaine de charactère représentant un chemin de valide de fichier
//POST: retourne 1 si echec ou 0 si succès.
    // Ecris un fichier json sous filename

int convert_itinerary_to_json(const char *filename, Graph_t itinerary, unsigned int starting_index, unsigned int end_index);

//PRE: graph est initilialisé; 
//  starting_index est un entier positif représentant l'osm_id dans graph du noeud de départ 
//  end_index est un entier positif représentant l'osm_id dans graph du noeud de destination
//  filename est une chaine de charactère représentant un chemin de valide de fichier
//POST: retourne 1 si echec ou 0 si succès.
    // Ecris un fichier json sous filename
    // Ce fichier représente la liste chainée dans graphe qui est le résultat de l'algoritme:
    // le chemin le plus court entre la source et la destination.
int export_dijkstra_shortest_path_to_file(const char *filename, Graph_t graph, unsigned int source_id, unsigned int destination_id);

#endif