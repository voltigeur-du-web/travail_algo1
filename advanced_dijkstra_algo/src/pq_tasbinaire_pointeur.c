#include "priority_queue_pointeur.h"
#include "types.h"

#define GRAPH_MAX_SIZE 100000

static float leq(float a, float b){
    return a-b;
}

// Fonction pour créer une file de priorité vide
PQ_t create_pq(float (*leq)(float, float), unsigned int n_nodes) {
    PQ_t pq = (PQ_t)malloc(sizeof(struct PQ));
    pq->list = malloc(n_nodes*sizeof(struct Node_PQ));
    pq->size = 0;
    pq->capacity = n_nodes;
    pq->leq = leq;
    return pq;
}

// Fonction pour vérifier si la file est vide
bool is_pq_empty(PQ_t pq) {
    return pq->size == 0;
}

// Fonction pour vérifier si la file est pleine
bool is_pq_full(PQ_t pq) {
    return pq->size == pq->capacity;
}

// Fonction pour échanger deux éléments
void swap(struct Node_PQ *a, struct Node_PQ *b) {
    struct Node_PQ temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction pour maintenir la propriété du tas (bottom to top)
int order_lineage_up(PQ_t pq, int child) {
    int parent = (child-1)/2;

    if(child == 0 || child >= pq->size || parent < 0){
        fprintf(stderr,"index out of bound\n");
        exit(1);
    }

    // If parent value > child value, then swap
    if (pq->leq(pq->list[parent].priority, pq->list[child].priority)>0) {
        swap(&pq->list[parent],&pq->list[child]);
    }
    return parent;
}

// Fonction pour maintenir la propriété du tas (to to bottom)
int order_lineage_down(PQ_t pq, int parent) {
    int left = 2*parent+1;
    int right = 2*parent+2;

    //Seulement un élément dans l'arbre ou il n'y a plus d'enfant dans la branche
    if (pq->size<=1 || !(pq->size-1>=right || pq->size-1>=left)){
        //printf("Size <=1 ou plus d'enfant dans la branche\n");
        return 0;
    } 
    //Seulement un enfant à gauche et enfant plus petit que parent
    else if(pq->size-1>=left && pq->size-1<right && pq->leq(pq->list[parent].priority, pq->list[left].priority)>0){
        //printf("Enfant à gauche seulement et parent plus grand\n");
        swap(&pq->list[parent],&pq->list[left]);
        return 0;
    }
    //Seulement un enfant à gauche et parent plus petit que enfant
    else if(pq->size-1>=left && pq->size-1<right){
        //printf("Enfant à gauche seulement et parent plus petit\n");
        return 0;
    }
    //Deux enfants, parent plus petit qu'un des deux, gauche est plus petit que droit
    else if((pq->leq(pq->list[parent].priority, pq->list[left].priority)>0 || pq->leq(pq->list[parent].priority, pq->list[right].priority)>0) && 
    pq->leq(pq->list[left].priority, pq->list[right].priority)<=0){
        //printf("Enfants à droite et gauche, parent plus grand qu'un enfant, gauche plus petit que droite\n");
        swap(&pq->list[parent],&pq->list[left]);
        order_lineage_down(pq,left);
    }
    //Deux enfants, parent plus petit qu'un des deux, droit est plus petit que gauche
    else if((pq->leq(pq->list[parent].priority, pq->list[left].priority)>0 || pq->leq(pq->list[parent].priority, pq->list[right].priority)>0) && 
    pq->leq(pq->list[right].priority, pq->list[left].priority)<0){
        //printf("Enfants à droite et gauche, parent plus grand qu'un enfant, droite plus petit que gauche\n");
        swap(&pq->list[parent],&pq->list[right]);
        order_lineage_down(pq,right);
    }
    else{
        //printf("Else\n");
        return 0;
    } 
}

// Fonction pour ajouter un élément à la file de priorité
PQ_t add_to_pq(PQ_t pq, float priority, void* data) {
    if (is_pq_full(pq)) {
        Node_PQ_t tmp = realloc(pq->list,(pq->capacity+GRAPH_MAX_SIZE)*sizeof(struct Node_PQ));
        if (tmp == NULL) {
            fprintf(stderr, "Erreur de réallocation de mémoire\n");
            free(tmp); // Libérer la mémoire initiale en cas d'échec
            return pq;
        }
        else{
            pq->capacity=pq->capacity+GRAPH_MAX_SIZE;
            pq->list=tmp;
        }
    }
    pq->size++;

    //Add to last element
    pq->list[pq->size-1].data=data;
    pq->list[pq->size-1].priority=priority;

    //Reorder parent lineage up if necessary (index is parent of added until index = 0 included)
    int i=pq->size-1;
    while(i > 0){
        i=order_lineage_up(pq,i);
    }
    return pq;
}

// Fonction pour retirer l'élément le plus prioritaire
PQ_t serve_pq(PQ_t pq) {
    if (is_pq_empty(pq)) {
        printf("La file de priorité est vide.\n");
        return pq;
    }

    //Put latest element as top
    swap(&pq->list[0], &pq->list[pq->size-1]);
    pq->size--;

    //Reorder parent lineage down if necessary (index is parent of added until index = 0 included)
    int zero=0;
    order_lineage_down(pq,zero);
    return pq;
}

// Fonction pour détruire la file de priorité avec structure
void destroy_pq(PQ_t pq){
    free(pq->list);
    free(pq);
    pq=NULL;
}

// Fonction pour obtenir l'élément le plus prioritaire sans le retirer
Node_PQ_t peek_pq(PQ_t pq) {
    if (is_pq_empty(pq)) {
        printf("La file de priorité est vide.\n");
    }
    else{
        return pq->list;
    }
}

// Fonction pour afficher les éléments de la file de priorité
void display_pq(PQ_t pq){
    for(int i=0;i<pq->size;i++){
        printf("%f ",pq->list[i].priority);
    }
    printf("\n");
}

// Fonction pour enlever un élément donner en se basant sur son pointeur
bool remove_pointer_from_pq(PQ_t pq, void* data){
    if(data==NULL){
        printf("Data has a NULL pointer\n");
        return false;
    }
    int index=-1;
    // Trouver l'index de l'élément à supprimer
    for(int i=0;i<pq->size;i++){
        if(peek_pq(pq)->data != NULL && peek_pq(pq)->data==data){
            index=i;
            break;
        }
    }
    // Pas trouver: return false
    if(index==-1){
        return false;
    }
    // Trouver en peek: servir pq
    else if(index==0){
        serve_pq(pq);
        return true;
    }
    // Trouver en last element: juste diminuer la taille
    else if(index==pq->size-1){
        pq->size--;
        return true;
    }
    // Trouver element au milieu: swap with top, serve pq, réordonnancement
    else{
        swap(&pq->list[index], &pq->list[0]);
        serve_pq(pq);
        while(index > 0){
                index=order_lineage_up(pq,index);
        }
        pq->size--;
        return true;
    }
    
}