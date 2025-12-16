#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../src/priority_queue_pointeur.h" // Assurez-vous que ce fichier contient les déclarations de vos fonctions
#include "../src/types.h" // Assurez-vous que ce fichier contient les déclarations de vos fonctions

#define MAX_SIZE 5000

static float leq2(float a, float b){
    return a-b;
}

// Test pour create_pq
void test_create_pq() {
    PQ_t pq = create_pq(leq2,MAX_SIZE);
    assert(pq != NULL);
    assert(is_pq_empty(pq));
    assert(pq->capacity = MAX_SIZE);
    assert(pq->size == 0);
    destroy_pq(pq);
    printf(" -test_create_pq: PASSED\n");
}

// Test pour add_to_pq
void test_add_to_pq_leq() {
    PQ_t pq = create_pq(leq2,MAX_SIZE);
    int data1 = 10;
    int data2 = 20;
    
    pq = add_to_pq(pq, 2.0, &data1);
    assert(peek_pq(pq)->data == &data1);
    assert(peek_pq(pq)->priority == 2.0);
    printf(" -Pointeur data attendu %p\n",(void*)&data1);
    printf(" -Pointeur data dans pq %p\n",(void*)peek_pq(pq)->data);

    assert(pq->capacity = MAX_SIZE-1);
    assert(pq->size == 1);

    pq = add_to_pq(pq, 1.0, &data2);
    assert(peek_pq(pq)->data == &data2);
    assert(peek_pq(pq)->priority == 1.0);
    printf(" -Pointeur data attendu %p\n",(void*)&data2);
    printf(" -Pointeur data dans pq %p\n",(void*)peek_pq(pq)->data);

    assert(pq->capacity = MAX_SIZE-2);
    assert(pq->size == 2);
    assert(!is_pq_empty(pq));

    destroy_pq(pq);
    printf(" -test_add_to_pq_leq PASSED\n");
}

// Test pour serve_pq
void test_serve_pq() {
    PQ_t pq = create_pq(leq2,MAX_SIZE);
    int data1 = 10;
    int data2 = 20;

    pq = add_to_pq(pq, 1.0, &data1);
    pq = add_to_pq(pq, 2.0, &data2);

    assert(peek_pq(pq)->priority == 1.0);
    assert(*(int*)peek_pq(pq)->data == 10);

    pq = serve_pq(pq);
    assert(peek_pq(pq)->priority == 2.0);
    assert(*(int*)peek_pq(pq)->data == 20);

    pq = serve_pq(pq);
    assert(is_pq_empty(pq));

    destroy_pq(pq);
    printf(" -test_serve_pq PASSED\n");
}

// Test pour valeur des pointeurs && is_pq_empty
void test_is_pq_empty() {
    PQ_t pq = create_pq(leq2,MAX_SIZE);
    assert(is_pq_empty(pq));

    int data = 10;
    printf(" -Pointeur data avant add to pq %p\n",(void*)&data);
    pq = add_to_pq(pq, 1, &data);

    printf(" -Pointeur data dans pq %p\n",(void*)pq->list->data);
    assert(!is_pq_empty(pq));

    destroy_pq(pq);
    printf(" -Pointeur data après destroy pq %p\n",(void*)&data);
    printf(" -test_is_pq_empty PASSED\n");
}

// Test pour is_pq_full (supposons une taille maximale de 10 pour l'exemple)
// PQ s'aggrandit dynamiquement donc plus relevant

// Test pour peek_pq
void test_peek_pq() {
    PQ_t pq = create_pq(leq2,MAX_SIZE);
    int data1 = 10;
    int data2 = 20;

    pq = add_to_pq(pq, 100.1, &data1);
    pq = add_to_pq(pq, 100.2, &data2);

    Node_PQ_t top = peek_pq(pq);
    assert(top->priority > 100.0 && top->priority < 100.2);
    assert(*(int*)top->data == 10);

    destroy_pq(pq);
    printf(" -test_peek_pq PASSED\n");
}

// Test pour remove_pointer_from_pq
void test_remove_pointer_from_pq() {
    PQ_t pq = create_pq(leq2,MAX_SIZE);
    int data1 = 10;
    int data2 = 20;
    int data3 = 30;
    int data4 = 40;
    int data5 = 50;

    pq = add_to_pq(pq, 5.0, &data5);
    pq = add_to_pq(pq, 4.0, &data4);
    pq = add_to_pq(pq, 1.0, &data1);
    display_pq(pq);
    serve_pq(pq);
    serve_pq(pq);
    display_pq(pq);
    pq = add_to_pq(pq, 1.0, &data1);
    assert(peek_pq(pq)->priority== 1.0);

    pq = add_to_pq(pq, 3.0, &data3);
    display_pq(pq);
    pq = add_to_pq(pq, 2.0, &data2);
    display_pq(pq);
    bool result = remove_pointer_from_pq(pq,&data1);
    display_pq(pq);
    serve_pq(pq);
    display_pq(pq);
    pq = add_to_pq(pq, 1.0, &data1);
    display_pq(pq);
    serve_pq(pq);
    display_pq(pq);
    serve_pq(pq);
    display_pq(pq);
    
    assert(result);
    assert(peek_pq(pq)->priority == 5.0);
    assert(*(int*)peek_pq(pq)->data == 50);

    destroy_pq(pq);
    printf(" -test_remove_pointer_from_pq PASSED\n");
}

int main() {
    test_create_pq();
    test_add_to_pq_leq();
    test_serve_pq();
    test_is_pq_empty();
    test_peek_pq();
    test_remove_pointer_from_pq();
    printf(" -All tests PASSED\n\n");
    return 0;
}

// clear && gcc -o test_pq.out tests_priority_queue_pointeur.c  ../src/nodes.c ../src/dijkstra.c ../src/pq_tasbinaire_pointeur.c 