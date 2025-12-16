#include "dijkstra.h"
#include "nodes.h"
#include "types.h"
#include "priority_queue_pointeur.h"
#include <limits.h>

// Fonction de comparaison pour la file de priorité
static float leq(float a, float b){
    return a-b;
}

// Exploration d'un noeud: mise à jour des distances des voisins et ajout à la file de priorité
PQ_t explore_node(PQ_t nodes_to_explore, Graph_t graph) {
    //Noeud à explorer: plus courte distance par rapport à la source (retourné par PQ)
    Node_PQ_t pq_node = peek_pq(nodes_to_explore);
    Mapnode_t current_node = (Mapnode_t) pq_node->data;
    // Marqué noeud courant comme exploré
    current_node->explored = true;
    // Explorer les voisins du noeud courant
    for (unsigned int i = 0; i < current_node->n_relations; i++) {
        // Si le noeud voisin n'a pas encore été exploré
        if (current_node->relations[i]->node->explored == false) {
            Mapnode_t neighbour_node = current_node->relations[i]->node;
            float new_distance_from_source_to_neighbour = get_distance_from_source(current_node) + current_node->relations[i]->distance;
            // Le noeud voisin n'a pas été visité auparavant apd d'un autre noeud: enlever l'ancienne distance de PQ
            if (neighbour_node->previous_node == NULL){
                neighbour_node->distance_from_source=new_distance_from_source_to_neighbour;
                neighbour_node->previous_node=current_node;
                neighbour_node->n_nodes_from_source = current_node->n_nodes_from_source+1;
                add_to_pq(nodes_to_explore,new_distance_from_source_to_neighbour,neighbour_node);
            }
            // Le noeud voisin a déjà été visité au paravant et nouvelle distance plus courte
            else if (new_distance_from_source_to_neighbour<get_distance_from_source(neighbour_node)){
                //On enleve l'ancienne valeur
                remove_pointer_from_pq(nodes_to_explore, neighbour_node);
                neighbour_node->distance_from_source=new_distance_from_source_to_neighbour;
                neighbour_node->previous_node=current_node;
                neighbour_node->n_nodes_from_source = current_node->n_nodes_from_source+1;
                add_to_pq(nodes_to_explore,new_distance_from_source_to_neighbour,neighbour_node);
            }
            // Le noeud voisin a déjà été visité et nouvelle distance plus longue
            // Pas d'action
        }
    }
    // Enlever noeud exploré de la file de priorité
    serve_pq(nodes_to_explore);
    return nodes_to_explore;
}

// Retourne l'itinéraire (tableau) le plus court entre la source et la destination ainsi que la distance totale
Graph_t dijkstra_find_shortest_path(Graph_t graph, unsigned int source_osm_id, unsigned int destination_osm_id){
    // Initalisation de PQ d'exploration
    PQ_t nodes_to_explore = create_pq(leq,graph->n_nodes);

    // Trouver la valeur index des noeuds de départ et d'arrivée
    unsigned int source_id = find_node_index(graph, source_osm_id);
    unsigned int destination_id = find_node_index(graph, destination_osm_id);
    
    // Initialisation de l'exploration au point de départ
    graph->nodes[source_id]->distance_from_source = 0.0;
    graph->nodes[source_id]->n_nodes_from_source = 1;
    add_to_pq(nodes_to_explore, graph->nodes[source_id]->distance_from_source, graph->nodes[source_id]);

    // Explore les noeuds jusqu'à plus de nouveaux noeuds à explorer ou que la destination soit explorée
    Node_PQ_t pq_node_list=peek_pq(nodes_to_explore);
    Mapnode_t exploring_node=(Mapnode_t)pq_node_list->data;
    while(!is_pq_empty(nodes_to_explore) && graph->nodes[destination_id]->explored == false){ 
        pq_node_list = peek_pq(nodes_to_explore);
        exploring_node = (Mapnode_t) pq_node_list->data;
        //nodes_to_explore = explore_node(nodes_to_explore, graph);
        if(exploring_node->id==destination_id){
            exploring_node->explored=true;
            exploring_node->n_nodes_from_source++;
        }
        else{
            nodes_to_explore = explore_node(nodes_to_explore, graph);
        }
    }

    // Ajouter la relation next_node à la chaine
    Mapnode_t current_node=graph->nodes[destination_id];
    Mapnode_t previous_node=current_node->previous_node;
    while(previous_node!=NULL){
        previous_node->next_node=current_node;
        current_node=previous_node;
        previous_node=current_node->previous_node;
    }
    
    // Free exploration structure
    destroy_pq(nodes_to_explore);
    return graph;
    //return shortest_way;
}
