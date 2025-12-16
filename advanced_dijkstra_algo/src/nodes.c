#include "nodes.h"
#include "types.h"
#include <limits.h>

//Initialise un graphe avec n_nodes noeuds (tableau de structure avec id comme identifiant de noeud et position dans tableau)
Graph_t init_graph(unsigned int n_nodes){
    Graph_t graph = (Graph_t) malloc(sizeof(struct Graph));
    if (graph == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le graphe\n");
        exit(EXIT_FAILURE);
    }
    graph->n_nodes = n_nodes;
    graph->nodes = (Mapnode_t*) malloc(n_nodes * sizeof(Mapnode_t));
    if (graph->nodes == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les noeuds du graphe\n");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < n_nodes; i++) {
        graph->nodes[i] = init_node();
        graph->nodes[i]->id = i;
    }
    return graph;
}

// Initialisation des structures de données
//PRE: /
//POST: Initialise un noeud avec son id, ses coordonnées (lat, lon), sa distance calculée depuis la source (initialisée à l'infini), son nombre de relations et son tableau de relations
Mapnode_t init_node(void){
    Mapnode_t node = (Mapnode_t) malloc(sizeof(struct Mapnode));
    if (node == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le noeud\n");
        exit(EXIT_FAILURE);
    }
    node->id = 0;
    node->osm_id = 0;
    node->lat = 0.0;
    node->lon = 0.0;
    node->n_relations = 0;
    node->distance_from_source = FLT_MAX; // Initialisée à l'infini
    node->previous_node = NULL;
    node->next_node = NULL;
    node->explored = false;
    node->n_nodes_from_source = 0;
    return node;
}

Graph_t define_node(Graph_t graph, unsigned int id, unsigned int osm_id, float lat, float lon){
    Mapnode_t node = init_node();
    node->id = id;
    node->osm_id = osm_id;
    node->lat = lat;
    node->lon = lon;
    graph->nodes[id] = node;
    return graph;
}

Relation_t init_relation(void){
    Relation_t rel = (Relation_t) malloc(sizeof(struct Relation));
    rel->distance = 0.0;
}

//PRE: /
//POST: Ajoute une relation (voisin et distance) à un noeud
Mapnode_t add_relation(Mapnode_t node, Mapnode_t neighbor, float distance) {
    if(node == NULL || neighbor == NULL) {
        fprintf(stderr, "Erreur: Noeud ou voisin NULL lors de l'ajout de relation\n");
        exit(EXIT_FAILURE);
    }
    else if(node == neighbor) {
        fprintf(stderr, "Erreur: Tentative d'ajout d'une relation à soi-même pour le noeud %u\n", node->id);
        exit(EXIT_FAILURE);
    }
    Relation_t rel = init_relation();
    rel->node = neighbor;
    rel->distance = distance;
    if(node->relations==NULL || node->n_relations==0){
        node->relations = (Relation_t*) malloc(sizeof(Relation_t));
        node->relations[0]=rel;
        node->n_relations++;
        return node;
    }
    else if(node->n_relations>0){
        for(unsigned int i = 0; i < node->n_relations; i++) {
            if (node->relations[i]->node != NULL && node->relations[i]->node == neighbor) {
                // Relation already exists, update distance if needed
                node->relations[i]->distance = distance;
                return node;
            }
        }
    }
    node->relations = (Relation_t*) realloc(node->relations, (node->n_relations + 1) * sizeof(Relation_t));
    if (node->relations == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les relations du noeud\n");
        exit(EXIT_FAILURE);
    }
    node->relations[node->n_relations]=rel;
    node->n_relations += 1;
    return node;
}

//PRE: /
//POST: Initialise une structure de relations avec un nombre de relations
float get_distance_from_source(Mapnode_t node) {
    if (node != NULL) {
        return node->distance_from_source;
    }
    return FLT_MAX; 
}


unsigned int find_node_index(Graph_t graph, unsigned int osm_id){
    for (unsigned int i = 0; i < graph->n_nodes; i++) {
        if (graph->nodes[i]->osm_id == osm_id) {
            return graph->nodes[i]->id;
        }
    }
    return UINT_MAX; // Not found
}

Mapnode_t add_relation_from_osm_id(Graph_t graph, unsigned int osm_id_source, unsigned int osm_id_neighbor, float distance) {
    unsigned int source_index = find_node_index(graph, osm_id_source);
    unsigned int neighbor_index = find_node_index(graph, osm_id_neighbor);
    if (source_index == UINT_MAX || neighbor_index == UINT_MAX) {
        fprintf(stderr, "Erreur: Noeud source ou voisin non trouvé pour les OSM IDs %u et %u\n", osm_id_source, osm_id_neighbor);
        return NULL;
    }
    Mapnode_t source_node = graph->nodes[source_index];
    Mapnode_t neighbor_node = graph->nodes[neighbor_index];
    add_relation(source_node, neighbor_node, distance);
    return source_node;
}

//PRE: /
//POST: /
//Graph_t clear_computed_distances_from_source(Graph_t graph);
Graph_t clear_computed_distances_from_source(Graph_t graph){
    if (graph != NULL) {
        for (unsigned int i = 0; i < graph->n_nodes; i++) {
            graph->nodes[i]->distance_from_source = FLT_MAX;
            graph->nodes[i]->previous_node = NULL;
            graph->nodes[i]->explored = false;
            graph->nodes[i]->n_nodes_from_source = -1;
        }
    }
    return graph;
}

// Libération des structures de données
void free_node(Mapnode_t node) {
    if (node != NULL) {
        if (node->relations != NULL) {
            free(node->relations);
            node->relations = NULL;
        }
        free(node);
    }
}

void free_graph(Graph_t graph) {
    if (graph != NULL) {
        if (graph->nodes != NULL) {
            for (unsigned int i = 0; i < graph->n_nodes; i++) {
                if (graph->nodes[i] != NULL) {
                    free_node(graph->nodes[i]);
                }
            }
            free(graph->nodes);
            graph->nodes = NULL;
        }
        free(graph);
    }
}
