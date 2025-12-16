#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <float.h>

#include "types.h"
#include "nodes.h"
#include "priority_queue_pointeur.h"

//PRE: explo est initialisé. graph est initialisé.
//POST: Retire le noeud le plus prioritaire de la file à explorer, 
// calcule la distance depuis la source avec ses voisisns encore à explorer,
// ajoute les noeuds voisin à la file de priorité (avec valeur mise à jour)
PQ_t explore_node(PQ_t explo, Graph_t graph);

//PRE: graph est initialisé, source_osm_id et destination_osm_id sont des entiers positifs.
//POST: Retourne graph mise à jour incluant une liste double chainée de mapnode.
    // Cette liste représente le chemin le plus court possible entre le noeud source et le noeud de destination.
Graph_t dijkstra_find_shortest_path(Graph_t graph, unsigned int source_osm_id, unsigned int destination_osm_id);

#endif