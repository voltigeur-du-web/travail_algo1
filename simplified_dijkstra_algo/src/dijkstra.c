#include <math.h>
#include "dijkstra.h"
#include "pq.h"

// Fonction de comparaison pour la file de priorite
static float leq(float a, float b){
    return a-b;
}

// Noeud de la file de priorite
struct Node_PQ{
    float priority;
    int id;
};

// Liste de priorite utilisant un tas binaire
struct PQ {
    Node_PQ_t list;
    int size;
    int capacity;
    float (*leq)(float, float);
};

// Remove from memory temporary objects used in exploration
void clear_explo_objects(bool* explored, float* dist_from_source, PQ_t pq, int* previous_nodes){
    free(explored);
    free(dist_from_source);
    free(previous_nodes);
    destroy_pq(pq);
}

unsigned int shortest_path(unsigned int n, float** w, unsigned int s, unsigned int d, unsigned int** path){
    // Initialisation des structures et tableaux
    PQ_t pq = create_pq(leq, n);
    float* dist_from_source = (float*)malloc(n * sizeof(float));
    int* previous_nodes = (int*)malloc(n * sizeof(int));
    bool* explored = (bool*)malloc(n * sizeof(bool));

    for (int i = 0; i < n; i++) {
        dist_from_source[i] = FLT_MAX;
        previous_nodes[i] = -1;
        explored[i] = false;
    }

    // Point de depart de l'exploration
    dist_from_source[s] = 0.0;
    add_to_pq(pq, dist_from_source[s], s);

    // Exploration des noeuds
    Node_PQ_t current_node = peek_pq(pq);
    int k = current_node->id;
    float w_k=INFINITY;
    float new=FLT_MAX;
    while (explored[d] == false && !is_pq_empty(pq)) {
        current_node = peek_pq(pq);
        k = current_node->id;
        // Marque noeud courant comme explore
        explored[k] = true;
        if (k != d) {
            // Explorer les voisins du noeud courant
            for (unsigned int i = 0; i < n; i++) {
                w_k=w[k][i];
                // Si le noeud voisin n'a pas encore ete explore
                // Un noeud j est voisin de i si 0<=w[i][j]<+inf
                if (!isinf(w_k) && explored[i] == false) {
                    new = dist_from_source[k] + w_k;
                    // Le noeud voisin n'a pas ete visite auparavant apd d'un autre noeud: enlever l'ancienne distance de PQ
                    if (new<dist_from_source[i]){
                        if( previous_nodes[i]!=-1){
                            remove_id_from_pq(pq, i);
                        }
                        dist_from_source[i]=new;
                        previous_nodes[i]=k;
                        add_to_pq(pq,new,i);
                    }
                    // nouvelle distance plus longue
                        // Pas d'action
                }
            }
            // Enlever noeud explore de la file de priorite
            serve_pq(pq);
        }
    }

        // Cas limite
    if(d==s){
        unsigned int* my_path = (unsigned int*)malloc(sizeof(unsigned int));
        my_path[0]=s;
        *path=my_path;
        printf("Distance totale du chemin: %.2f\n", 0.0);
        printf("Nombre de noeuds dans le chemin: %u\n", 1);
        clear_explo_objects(explored, dist_from_source, pq, previous_nodes);
        return 1;
    }

    // Pas de chemin trouve entre source et destination
    if(explored[d] == false && is_pq_empty(pq)){
        printf("Pas de chemin trouve entre %i et %i\n",s,d);
        printf("Nombre de noeuds dans le chemin: %u\n", 0);
        clear_explo_objects(explored, dist_from_source, pq, previous_nodes);
        return 0;
    }

    // Compter le nombre de noeuds dans le chemin
    int i_node = d;
    unsigned int path_n_nodes = 1; // Inclure la destination dans le compte
    while (previous_nodes[i_node] >= 0) {
        path_n_nodes++;
        i_node = previous_nodes[i_node];
    }

    // Allouer et reconstruire le chemin
    unsigned int* my_path = (unsigned int*)malloc((path_n_nodes) * sizeof(unsigned int)); 
    i_node = d;
    int count = -1;
    while (previous_nodes[i_node] >= 0) {
        my_path[path_n_nodes + count] = i_node;
        count--;
        i_node = previous_nodes[i_node];
    }
    
    printf("Distance totale du chemin: %.2f\n", dist_from_source[d]);
    printf("Nombre de noeuds dans le chemin: %u\n", path_n_nodes);
    *path = my_path; // Assigner le tableau alloue a la variable de sortie

    // Liberer les structures d'exploration
    clear_explo_objects(explored, dist_from_source, pq, previous_nodes);
    return path_n_nodes;
}