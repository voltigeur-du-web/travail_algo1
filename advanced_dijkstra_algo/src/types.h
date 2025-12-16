#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include "priority_queue_pointeur.h"
#include "nodes.h"


// Graphe représentant l'ensemble des noeuds et leurs relations
struct Graph{
    unsigned int n_nodes; // Nombre de noeuds dans le graphe
    Mapnode_t* nodes; // Tableau de pointeurs vers les noeuds
};

// Noeud du graphe représentant un point sur la carte
struct Mapnode{
    unsigned int id; // Index du noeud
    unsigned int osm_id; // Identifiant OSM du noeud
    float lat; // Latitude du noeud
    float lon; // Longitude du noeud
    unsigned int n_relations; // Nombre de relations (voisins)
    Relation_t* relations; // Distance calculée depuis la source (initialisée à l'infini)
    float distance_from_source;
    struct Mapnode* previous_node; // Noeud précédent dans le chemin le plus court
    struct Mapnode* next_node; // Noeud suivant dans le chemin le plus court
    bool explored;
    int n_nodes_from_source;
}; 

// Arête partant d'un noeud vers d'autres
struct Relation{
    Mapnode_t node; // Identifiant du noeud voisin
    float distance; // Distance associée au noeud voisin
};

// Noeud de la file de priorité
struct Node_PQ{
    float priority;
    void* data;
};

// Liste de priorité utilisant un tas binaire
struct PQ {
    Node_PQ_t list;
    int size;
    int capacity;
    float (*leq)(float, float);
};

#endif