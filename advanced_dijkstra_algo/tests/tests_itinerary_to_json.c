#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "../src/nodes.h"
#include "../src/types.h"
#include "../src/jsonize.h"

Graph_t create_sample_itinerary(void) {
    Graph_t itinerary = init_graph(3);
    int last=2;
    itinerary->n_nodes = 3;
    itinerary->nodes[last]->distance_from_source = 15.75;

    for (int i = 0; i <= last; i++) {
        itinerary->nodes[i]->id = i;
        itinerary->nodes[i]->osm_id = 1000 + i;
        itinerary->nodes[i]->lat = 50.0 + i * 0.1;
        itinerary->nodes[i]->lon = 5.0 + i * 0.1;
        itinerary->nodes[i]->distance_from_source = 5.0 + i * 2.5;
    }

    return itinerary;
}

int main() {
    // Create a sample itinerary
    Graph_t itinerary = create_sample_itinerary();
    if (itinerary == NULL) {
        fprintf(stderr, "Error creating sample itinerary\n");
        return 1;
    }

    // Write the itinerary to a JSON file
    const char *filename = "/home/pamarchand/dev/unamur/algo1/data/itinerary_test.json";
    if (convert_itinerary_to_json(filename, itinerary, 0, 2)==1) {
        fprintf(stderr, "Error writing itinerary to file\n");
        free_graph(itinerary);
        return 1;
    }

    printf("Wrote itinerary to %s: PASSED\n", filename);

    // Free the itinerary
    free_graph(itinerary);

    return 0;
}