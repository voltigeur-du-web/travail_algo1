#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "../src/nodes.h"
#include "../src/types.h"

// Test 1: Test initialization of a node
void test_init_node() {
    Mapnode_t node = init_node();
    assert(node != NULL);
    assert(node->id == 0); // Default or expected value
    assert(node->lat == 0.0); // Default or expected value
    assert(node->lon == 0.0); // Default or expected value
    assert(node->distance_from_source == FLT_MAX); // Should be infinity initially
    assert(node->n_relations == 0); // No relations initially
    assert(node->relations == NULL); // No relations initially
    free_node(node);
    printf(" -test_init_node: PASSED\n");
}

// Test 2: Test initialization of a graph
void test_init_graph() {
    unsigned int n_nodes = 5;
    Graph_t graph = init_graph(n_nodes);
    assert(graph != NULL);
    assert(graph->n_nodes == n_nodes);
    assert(graph->nodes != NULL);
    for (unsigned int i = 0; i < n_nodes; i++) {
        assert(graph->nodes[i] != NULL);
    }
    free_graph(graph);
    printf(" -test_init_graph: PASSED\n");
}


// Test 4: Test getting the distance from source
void test_get_distance_from_source() {
    Mapnode_t node = init_node();
    float expected_distance = FLT_MAX;
    assert(get_distance_from_source(node) == expected_distance);
    assert(node->previous_node == NULL);

    free_node(node);
    printf(" -test_get_distance_from_source: PASSED\n");
}

// Test 5: Test clearing computed distances
void test_clear_computed_distances_from_source() {
    unsigned int n_nodes = 2;
    Graph_t graph = init_graph(n_nodes);

    // Set some distances
    graph->nodes[0]->distance_from_source = 10.0;
    graph->nodes[1]->distance_from_source = 20.0;

    graph->nodes[0]->explored = true;
    graph->nodes[0]->explored = false;

    graph->nodes[1]->previous_node = graph->nodes[0];
    for(int i=0; i<n_nodes; i++){
        printf(" -Before CLEAR: Node %d distance: %f, explored: %d, previous_node: %p\n", i, graph->nodes[i]->distance_from_source, graph->nodes[i]->explored, (void*)graph->nodes[i]->previous_node);
    }

    graph = clear_computed_distances_from_source(graph);
    for(int i=0; i<n_nodes; i++){
        assert(graph->nodes[i]->distance_from_source == FLT_MAX);
        assert(graph->nodes[i]->explored == false);
        assert(graph->nodes[i]->previous_node == NULL);
        printf(" -After CLEAR:  Node %d distance: %f, explored: %d, previous_node: %p\n", i, graph->nodes[i]->distance_from_source, graph->nodes[i]->explored, (void*)graph->nodes[i]->previous_node);
    }

    free_graph(graph);
    printf(" -test_clear_computed_distances_from_source: PASSED\n");
}

// Test 6: Test freeing a node
void test_free_node() {
    Mapnode_t node = init_node();
    free_node(node);
    // No assert possible, but valgrind can check for memory leaks
    printf(" -test_free_node: PASSED\n");
}

// Test 7: Test freeing a graph
void test_free_graph() {
    unsigned int n_nodes = 2;
    Graph_t graph = init_graph(n_nodes);
    free_graph(graph);
    // No assert possible, but valgrind can check for memory leaks
    printf(" -test_free_graph: PASSED\n");
}

// Test 7: Test Add relation from osm_id
void test_add_relation_from_osm_id() {
    unsigned int n_nodes = 5;
    Graph_t graph = init_graph(n_nodes);

    // Set some distances
    graph->nodes[0]->osm_id=1000;
    graph->nodes[1]->osm_id=1001;
    graph->nodes[2]->osm_id=1002;
    graph->nodes[3]->osm_id=1003;
    graph->nodes[4]->osm_id=1004;

    assert(graph->nodes[0]->id==0);
    assert(graph->nodes[1]->id==1);

    add_relation_from_osm_id(graph, 1000, 1001, 10.0);
    add_relation_from_osm_id(graph, 1000, 1002, 20.0);
    add_relation_from_osm_id(graph, 1000, 1003, 30.0);
    add_relation_from_osm_id(graph, 1001, 1000, 10.0);
    add_relation_from_osm_id(graph, 1001, 1004, 40.0);
    add_relation_from_osm_id(graph, 1001, 1005, 50.0);
    add_relation_from_osm_id(graph, 1004, 1001, 40.0);
    add_relation_from_osm_id(graph, 1005, 1001, 50.0);
    add_relation_from_osm_id(graph, 1004, 1005, 60.0);
    add_relation_from_osm_id(graph, 1005, 1004, 60.0);

    assert(graph->nodes[0]->relations[0]->distance==10.0);
    assert(graph->nodes[0]->relations[1]->distance==20.0);
    assert(graph->nodes[0]->relations[2]->distance==30.0);
    assert(graph->nodes[0]->relations[0]->node->osm_id==1001);
    assert(graph->nodes[0]->relations[0]->node->osm_id==1002);
    assert(graph->nodes[0]->relations[0]->node->osm_id==1003);

    free_graph(graph);
    printf(" -test_add_relation_From_osm_id: PASSED\n");
}

int main() {
    test_init_node();
    test_init_graph();
    test_get_distance_from_source();
    test_clear_computed_distances_from_source();
    test_free_node();
    test_free_graph();
    printf(" -All tests PASSED!\n\n");
    return 0;
}

// cd /home/pamarchand/dev/unamur/algo1/dijkstra_algo_c/tests
// gcc -o test_nodes tests_nodes.c ../src/nodes.c ../src/dijkstra.c ../src/priority_queue_pointeur.c 