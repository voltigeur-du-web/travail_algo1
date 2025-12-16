#include "dijkstra.h"
#include "pq.h"
#include <math.h>

float** create_test_weights(void){
    int n_nodes = 7;

    float** weights=(float**)malloc(sizeof(float*)*n_nodes);
    for(int i=0;i<n_nodes;i++){
        weights[i]=(float*)malloc(n_nodes*sizeof(float));
        for(int j=0;j<n_nodes;j++){
            //INFINITY signifie aucune connexion entre les noeuds i et j
            weights[i][j]=INFINITY;
        }
    }

    // Au noeud 0
    weights[0][1]=7.0;
    weights[0][2]=9.0;
    weights[0][5]=14.0;

    // Au noeud 1
    weights[1][1]=7.0;
    weights[1][2]=10.0;
    weights[1][3]=15.0;

    // Au noeud 2
    weights[2][0]=9.0;
    weights[2][1]=10.0;
    weights[2][3]=11.0;
    weights[2][5]=2.0;

    // Au noeud 3
    weights[3][1]=15.0;
    weights[3][2]=11.0;
    weights[3][4]=6.0;

    // Au noeud 4
    weights[4][3]=6.0;
    weights[4][5]=9.0;
    // weights[4][6]=100.0;

    // Au noeud 5
    weights[5][0]=14.0;
    weights[5][2]=2.0;
    weights[5][4]=9.0;
    

    // weights[6][4]=100.0;

    return weights;
}

int main() {
    unsigned int n_nodes = 7;
    float** weights = create_test_weights(); // Matrice des poids
    unsigned int source_id = 0;
    unsigned int destination_id = 4;
    unsigned int* path;
    int path_n_nodes = shortest_path(n_nodes, weights, source_id, destination_id, &path);

    if (path != NULL) {
        printf("Chemin : ");
        for (int i = 0; i < path_n_nodes; i++) {
            printf("%i ", path[i]);
        }
        printf("\n");
    free(path); // Liberer le tableau apres utilisation
    }

    return 0;
}