#ifndef NODES_H
#define NODES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

struct Mapnode; 
typedef struct Mapnode* Mapnode_t; 

struct Graph; // Composé de plusieurs Mapnode_t et leurs Relations_t associées
typedef struct Graph* Graph_t; 

struct Relation;
typedef struct Relation* Relation_t;

//PRE: /
//POST: La structure Relation est initialisée
Relation_t init_relation(void);

//PRE: node et neighbour sont initialisés en tant que Mapnode_t; distance est un float
//POST: Ajoute une structure de relation au node
    // Si aucune relation n'existait alors un tableau de Relation est intialisé puis la relation est ajoutée
    // Sinon, le tableau de Relation est realloué en mémoire en augmentant sa taille de une relation (et ajoute la relation)
Mapnode_t add_relation(Mapnode_t node, Mapnode_t neighbor, float distance);

//PRE: La structure Graph est initialisée. Au moins, un noeud est défini. osm_id est un entier positif.
//POST: Retourne le premier id correspondant au osm_id. Si osm_id n'est pas rencontré dans le Graph alors retourne UINT_MAX
unsigned int find_node_index(Graph_t graph, unsigned int osm_id);

//PRE: node et neighbour sont initialisés en tant que Mapnode_t; distance est un float; osm_id est un entier positif.
//POST: Ajoute une structure de relation au node où la valeur de osm_id est trouvée
    // Si aucune relation n'existait alors un tableau de Relation est intialisé puis la relation est ajoutée
    // Sinon, le tableau de Relation est realloué en mémoire en augmentant sa taille de une relation (et ajoute la relation)
Mapnode_t add_relation_from_osm_id(Graph_t graph, unsigned int osm_id_source, unsigned int osm_id_neighbor, float distance);

//PRE: /
//POST: Initialise un graphe avec n_nodes noeuds (tableau de structure avec id comme identifiant de noeud et position dans tableau)
Graph_t init_graph(unsigned int n_nodes);

//PRE: /
//POST: Initialise un noeud avec son id, ses coordonnées (lat, lon), sa distance calculée depuis la source (initialisée à l'infini), son nombre de relations et son tableau de relations
Mapnode_t init_node(void);

//PRE: La Structure Graph est itialisée
//POST: Initialise une structure Mapnode et complète les inputs (id, osm_id, latitude, longitude). Le tableau de relation est initialisée.
Graph_t define_node(Graph_t graph, unsigned int id, unsigned int osm_id, float lat, float lon);

//PRE: Node est initialisé. 
//POST: Retourne la valeur node->distance_from_source. Si le node == NULL, alors retourne FLT_MAX.
float get_distance_from_source(Mapnode_t node);

//PRE: /
//POST: /
Graph_t clear_computed_distances_from_source(Graph_t graph);

//PRE: node est initialisé
//POST: Désaloue node de la mémoire ainsi que les structure de Relation correspondante
void free_node(Mapnode_t node);

//PRE: graph est initialisé.
//POST: Désaloue graph, tous ses noeuds et toutes ses relations
void free_graph(Graph_t graph);

#endif
