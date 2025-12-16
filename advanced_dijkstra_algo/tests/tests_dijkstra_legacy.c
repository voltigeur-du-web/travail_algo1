#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "../src/dijkstra.h"
#include "../src/nodes.h"
#include "../src/types.h"


// Example from https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
Graph_t create_test_graph() {
    int n_nodes = 6;
    int connexions[6] = {3, 3, 4, 3, 2, 3};

    Graph_t graph = init_graph(n_nodes); // 3 nodes

    // Initialize nodes
    for (unsigned int i = 0; i < n_nodes; i++) {
        graph->nodes[i]->id = i;
        graph->nodes[i]->osm_id = i+1;
        graph->nodes[i]->lat = 0.0;
        graph->nodes[i]->lon = 0.0;
        graph->nodes[i]->n_relations = connexions[i]; // Each node has min 2 neighbors
        graph->nodes[i]->relations = (Relation_t*) malloc(connexions[i] * sizeof(Relation_t));
        graph->nodes[i]->distance_from_source = FLT_MAX;
    }

    // Set up relations (edges)
    Relation_t rel = init_relation();
    rel->node = graph->nodes[1];
    rel->distance = 7.0;
    graph->nodes[0]->relations[0]=rel;
    rel = init_relation();
    rel->node = graph->nodes[2];
    rel->distance = 9.0;
    graph->nodes[0]->relations[1]=rel;
    rel = init_relation();
    rel->node = graph->nodes[5];
    rel->distance = 14.0;
    graph->nodes[0]->relations[2]=rel;

    rel = init_relation();
    rel->node = graph->nodes[0];
    rel->distance = 7.0;
    graph->nodes[1]->relations[0]=rel;
    rel = init_relation();
    rel->node = graph->nodes[2];
    rel->distance = 10.0;
    graph->nodes[1]->relations[1]=rel;
    rel = init_relation();
    rel->node = graph->nodes[3];
    rel->distance = 15.0;
    graph->nodes[1]->relations[2]=rel;

    rel = init_relation();
    rel->node = graph->nodes[0];
    rel->distance = 9.0;
    graph->nodes[2]->relations[0]=rel;
    rel = init_relation();
    rel->node = graph->nodes[1];
    rel->distance = 10.0;
    graph->nodes[2]->relations[1]=rel;
    rel = init_relation();
    rel->node = graph->nodes[3];
    rel->distance = 11.0;
    graph->nodes[2]->relations[2]=rel;
    rel = init_relation();
    rel->node = graph->nodes[5];
    rel->distance = 2.0;
    graph->nodes[2]->relations[3]=rel;

    rel = init_relation();
    rel->node = graph->nodes[1];
    rel->distance = 15.0;
    graph->nodes[3]->relations[0]=rel;
    rel = init_relation();
    rel->node = graph->nodes[2];
    rel->distance = 11.0;
    graph->nodes[3]->relations[1]=rel;
    rel = init_relation();
    rel->node = graph->nodes[4];
    rel->distance = 6.0;
    graph->nodes[3]->relations[2]=rel;
    
    rel = init_relation();
    rel->node = graph->nodes[3];
    rel->distance = 6.0;
    graph->nodes[4]->relations[0]=rel;
    rel = init_relation();
    rel->node = graph->nodes[5];
    rel->distance = 9.0;
    graph->nodes[4]->relations[1]=rel;

    rel = init_relation();
    rel->node = graph->nodes[0];
    rel->distance = 14.0;
    graph->nodes[5]->relations[0]=rel;
    rel = init_relation();
    rel->node = graph->nodes[2];
    rel->distance = 2.0;
    graph->nodes[5]->relations[1]=rel;
    rel = init_relation();
    rel->node = graph->nodes[4];
    rel->distance = 9.0;
    graph->nodes[5]->relations[2]=rel;

    return graph;
}

// Test 1: Test initialization of an itinerary
void test_init_itinerary() {
    int n_nodes = 3;
    Itinerary_t iti = init_itinerary(n_nodes);
    assert(iti != NULL);
    assert(iti->n_nodes == n_nodes);
    assert(iti->nodes != NULL);
    assert(iti->total_distance == 0.0);
    free_itinerary(iti);
    printf(" -test_init_itinerary: PASSED\n");
}

// Test 2: Test initialization of an exploration
void test_init_exploration() {
    Exploration_t explo = init_exploration();
    assert(explo != NULL);
    // Add more assertions based on your Exploration struct
    free_exploration(explo);
    printf(" -test_init_exploration: PASSED\n");
}

// Test 3: Test adding a node to exploration
void test_add_node_to_explore() {
    Graph_t graph = create_test_graph();
    Exploration_t explo = init_exploration();
    unsigned int node_id = 0;

    // explo = add_node_to_explore(explo, graph, node_id);

    free_exploration(explo);
    free_graph(graph);
    printf(" -test_add_node_to_explore: PASSED\n");
}

// Test 5: Test exploring a node
void test_explore_node() {
    Graph_t graph = create_test_graph();
    Exploration_t explo = init_exploration();
    unsigned int node_id = 0;
    graph->nodes[node_id]->distance_from_source = 0.0;

    assert(explo->nodes_to_explore->size == 0);
    explo = add_node_to_explore(explo, graph, node_id);
    assert(explo->nodes_to_explore->size == 1);
    printf(" -Before exploration of node %u at pointer %p:\n n relations: %i ; distance from source: %f; explored: %i ; previous: %p \n", graph->nodes[node_id]->id, graph->nodes[node_id],
        graph->nodes[node_id]->n_relations, 
        graph->nodes[node_id]->distance_from_source, 
        graph->nodes[node_id]->explored, 
        graph->nodes[node_id]->previous_node);

    explo = explore_node(explo, graph);
    assert(explo->nodes_to_explore->size == 3);
    
    int tolook[3] = {1,2,5};

    printf(" -After exploration of node %u at pointer %p:\n n relations: %i ; distance from source: %f; explored: %i ; previous: %p \n", graph->nodes[node_id]->id, graph->nodes[node_id],
        graph->nodes[node_id]->n_relations, 
        graph->nodes[node_id]->distance_from_source, 
        graph->nodes[node_id]->explored, 
        graph->nodes[node_id]->previous_node);

    printf(" - Checking neighbors after exploration:\n");
    for(int i=0;i<3;i++){
        unsigned int nid = tolook[i];
        printf(" - Neighbor node %u : distance from source: %f; explored: %i ; previous: %p \n", graph->nodes[nid]->id, 
            graph->nodes[nid]->distance_from_source, 
            graph->nodes[nid]->explored, 
            graph->nodes[nid]->previous_node);
    }

    explo = explore_node(explo, graph); // Explore node 1
    assert(graph->nodes[1]->explored == true);
    assert(graph->nodes[2]->explored == false);
    assert(graph->nodes[5]->explored == false);
    printf(" - Checking neighbors after exploration:\n");
    for(int i=0;i<3;i++){
        unsigned int nid = tolook[i];
        printf(" - Neighbor node %u : distance from source: %f; explored: %i ; previous: %p \n", graph->nodes[nid]->id, 
            graph->nodes[nid]->distance_from_source, 
            graph->nodes[nid]->explored, 
            graph->nodes[nid]->previous_node);
    }

    free_exploration(explo);
    free_graph(graph);
    printf(" -test_explore_node: PASSED\n");
}

// Test 6: Test Dijkstra's shortest path algorithm
void test_dijkstra_find_shortest_path() {
    Graph_t graph = create_test_graph();
    unsigned int source = 1;
    unsigned int destination = 5;
    //Expected path: 0 -> 2 -> 5 -> 4 with total distance 20.0

    Itinerary_t iti = dijkstra_find_shortest_path(graph, source, destination);
    assert(iti != NULL);
    assert(iti->n_nodes == 4);
    assert(iti->total_distance == 20.0);
    assert(iti->nodes[0]->id == 0);
    assert(iti->nodes[1]->id == 2);
    assert(iti->nodes[2]->id == 5);
    assert(iti->nodes[3]->id == 4);
    
    for(int i=0;i<iti->n_nodes;i++){
        printf(" - Itinerary node %d: id %u\n", i, iti->nodes[i]->osm_id);
    }

    free_itinerary(iti);
    free_graph(graph);
    printf(" -test_dijkstra_find_shortest_path: PASSED\n");
}

// Test 7: Test freeing an itinerary
void test_free_itinerary() {
    int n_nodes = 3;
    Itinerary_t iti = init_itinerary(n_nodes);
    free_itinerary(iti);
    printf(" -test_free_itinerary: PASSED\n");
}

// Test 8: Test freeing an exploration
void test_free_exploration() {
    Exploration_t explo = init_exploration();
    explo = free_exploration(explo);
    assert(explo == NULL);
    printf(" -test_free_exploration: PASSED\n");
}

int main() {
    test_init_itinerary();
    test_init_exploration();
    test_add_node_to_explore();
    test_explore_node();
    test_dijkstra_find_shortest_path();
    test_free_itinerary();
    test_free_exploration();
    printf(" -All tests PASSED!\n");
    return 0;
}
