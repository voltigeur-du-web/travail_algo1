#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "../src/priority_queue_pointeur.h"
#include "../src/dijkstra.h"
#include "../src/nodes.h"
#include "../src/types.h"

// Example from https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
Graph_t create_test_graph(void){
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

static float leq_float(float a, float b){
    return a-b;
}

// Test 5: Test exploring a node
void test_explore_node() {
    Graph_t graph = create_test_graph();
    PQ_t nodes_to_explore = create_pq(leq_float,graph->n_nodes);
    unsigned int node_id = 0;
    graph->nodes[node_id]->distance_from_source = 0.0;

    assert(nodes_to_explore->size == 0);
    nodes_to_explore = add_to_pq(nodes_to_explore, graph->nodes[node_id]->distance_from_source, graph->nodes[node_id]);
    assert(nodes_to_explore->size == 1);
    printf(" -Before exploration of node %u at pointer %p:\n n relations: %i ; distance from source: %f; explored: %i ; previous: %p \n", graph->nodes[node_id]->id, graph->nodes[node_id],
        graph->nodes[node_id]->n_relations, 
        graph->nodes[node_id]->distance_from_source, 
        graph->nodes[node_id]->explored, 
        graph->nodes[node_id]->previous_node);

    nodes_to_explore = explore_node(nodes_to_explore, graph);
    assert(nodes_to_explore->size == 3);
    
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

    nodes_to_explore = explore_node(nodes_to_explore, graph); // Explore node 1
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

    free_graph(graph);
    printf(" -test_explore_node: PASSED\n");
}

// Test 6: Test Dijkstra's shortest path algorithm
void test_dijkstra_find_shortest_path() {
    Graph_t graph = create_test_graph();
    unsigned int source = 0;
    unsigned int destination = 4;
    //Expected path: 0 -> 2 -> 5 -> 4 with total distance 20.0

    Graph_t iti = dijkstra_find_shortest_path(graph, source+1, destination+1);

    int count=0;
    int expected[4]={0,2,5,4};
    Mapnode_t current_node=iti->nodes[source];
    Mapnode_t next_node=current_node->next_node;
    for(int i=0;i<6;i++){
        printf("At node %i for next %p\n",iti->nodes[i]->id,iti->nodes[i]->next_node);
    }

    assert(current_node!=NULL);
    while(current_node!=NULL){
        assert(current_node->id == expected[count]);
        printf("Expected node: %i Current node: %i\n",expected[count],current_node->id);
        count++;
        current_node=next_node;
        if(current_node!=NULL){
             next_node=current_node->next_node;
        }
    }

    free_graph(graph);
    printf(" -test_dijkstra_find_shortest_path: PASSED\n");
}


int main() {
    test_explore_node();
    test_dijkstra_find_shortest_path();
    printf(" -All tests PASSED!\n");
    return 0;
}
