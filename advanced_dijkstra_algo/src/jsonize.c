#include "dijkstra.h"
#include "nodes.h"
#include "types.h"
#include "priority_queue_pointeur.h"
#include "jsonize.h"
#include <limits.h>

int convert_itinerary_to_json(const char *filename, Graph_t itinerary, unsigned int starting_index, unsigned int end_index){
    if (filename == NULL || itinerary == NULL) {
        return 1;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write JSON directly to the file
    fprintf(file, "{\n");
    fprintf(file, "  \"n_nodes\": %u,\n", itinerary->nodes[end_index]->n_nodes_from_source);
    fprintf(file, "  \"total_distance\": %.2f,\n", itinerary->nodes[end_index]->distance_from_source);
    fprintf(file, "  \"nodes\": [\n");
    
    
    Mapnode_t node=itinerary->nodes[starting_index];
    Mapnode_t next_node=node->next_node;
    while(node!=NULL){
        fprintf(file, "    {\n");
        fprintf(file, "      \"id\": %u,\n", node->id);
        fprintf(file, "      \"osm_id\": %u,\n", node->osm_id);
        fprintf(file, "      \"lat\": %.6f,\n", node->lat);
        fprintf(file, "      \"lon\": %.6f,\n", node->lon);
        fprintf(file, "      \"distance_from_source\": %.2f\n", node->distance_from_source);
        fprintf(file, "    }");      
        node=next_node;
        if(node!=NULL){
            fprintf(file, ",");
            next_node=node->next_node;
        }
    }
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");
    fclose(file);
    return 0;
}

int export_dijkstra_shortest_path_to_file(const char *filename, Graph_t graph, unsigned int source_id, unsigned int destination_id){
    printf("Data received");
    dijkstra_find_shortest_path(graph, source_id, destination_id);
    int result = 1;
    unsigned int starting_index = find_node_index(graph,source_id);
    unsigned int end_index = find_node_index(graph,destination_id);
    result = convert_itinerary_to_json(filename, graph,starting_index,end_index);
    free_graph(graph);
    return result;
}
