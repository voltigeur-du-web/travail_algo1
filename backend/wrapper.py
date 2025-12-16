import ctypes
import os
from typing import List, Dict, Optional
import networkx as nx
from datetime import datetime
import struct

from backend.get_osm_data import get_ways_network_from_town
from backend import DATA_FOLDER

# Load the C library
# gcc -o 
lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../dijkstra_algo_c/build/libdijkstra.so"))
dijkstra_lib = ctypes.CDLL(lib_path)


class Mapnode(ctypes.Structure):
    pass  # Placeholder to allow self-referential fields
# Define C structs as Python classes
class Relation(ctypes.Structure):
    _fields_ = [
        ("node", ctypes.POINTER(Mapnode)),
        ("distance", ctypes.c_float),
    ]

Mapnode._fields_ = [
    ("id", ctypes.c_uint),
    ("osm_id", ctypes.c_uint),
    ("lat", ctypes.c_float),
    ("lon", ctypes.c_float),
    ("n_relations", ctypes.c_uint),
    ("relations", ctypes.POINTER(Relation)),  # Pointeur vers un tableau de Relation
    ("distance_from_source", ctypes.c_float),
    ("previous_node", ctypes.POINTER(Mapnode)),
    ("explored", ctypes.c_bool),
]

class Graph(ctypes.Structure):
    _fields_ = [
        ("n_nodes", ctypes.c_uint),
        ("nodes", ctypes.POINTER(Mapnode)),
    ]

class Itinerary(ctypes.Structure):
    _fields_ = [
        ("n_nodes", ctypes.c_uint),
        ("nodes", ctypes.POINTER(Mapnode)),
        ("total_distance", ctypes.c_float),
    ]

# # Define the C function signature
# # Define the function signatures
dijkstra_lib.init_graph.argtypes = [ctypes.c_uint]
dijkstra_lib.init_graph.restype = ctypes.POINTER(Graph)

dijkstra_lib.define_node.argtypes = [ctypes.POINTER(Graph), ctypes.c_uint, ctypes.c_uint, ctypes.c_float, ctypes.c_float]
dijkstra_lib.define_node.restype =  ctypes.POINTER(Graph)

dijkstra_lib.add_relation_from_osm_id.argtypes = [
    ctypes.POINTER(Graph),  # Graph_t graph
    ctypes.c_uint,    # unsigned int osm_id_source
    ctypes.c_uint,    # unsigned int osm_id_neighbor
    ctypes.c_float    # float distance
]
dijkstra_lib.add_relation_from_osm_id.restype = ctypes.POINTER(Mapnode)

dijkstra_lib.free_graph.argtypes = [ctypes.c_void_p]
dijkstra_lib.free_graph.restype = None

dijkstra_lib.export_dijkstra_shortest_path_to_file.argtypes = [
    ctypes.c_char_p,  # const char *filename
    ctypes.POINTER(Graph),   # Graph_t graph (assuming Graph_t is a pointer to a struct)
    ctypes.c_uint,     # unsigned int source_id
    ctypes.c_uint      # unsigned int destination_id
]
dijkstra_lib.export_dijkstra_shortest_path_to_file.restype = ctypes.c_int

def build_c_graph_from_networkx(G):
    # Initialiser le graphe avec le nombre de noeuds
    c_graph_ptr = dijkstra_lib.init_graph(len(G.nodes))
    c_graph = ctypes.cast(c_graph_ptr, ctypes.POINTER(Graph)).contents

    # Definir chaque noeud dans le graphe
    for id, node_id in enumerate(G.nodes):
        node_data = G.nodes[node_id]
        osm_id = node_id
        lat = node_data["y"]
        lon = node_data["x"]
        n_relations = len(list(G.neighbors(node_id)))

        c_graph_ptr = dijkstra_lib.define_node(
            c_graph_ptr,
            ctypes.c_uint(id),
            ctypes.c_uint(osm_id),
            ctypes.c_float(lat),
            ctypes.c_float(lon),
        )

    # Ajouter les relations entre les noeuds
    for _,node_id in enumerate(G.nodes):      
        for neighbor in G.neighbors(node_id):
            edge_data = G.get_edge_data(node_id, neighbor)
            if edge_data:
                distance = edge_data[0].get("length", 1.0)
                distance = round(distance)
                osm_id_source = node_id
                osm_id_neighbor = neighbor
                if osm_id_source != osm_id_neighbor:
                    dijkstra_lib.add_relation_from_osm_id(
                        c_graph_ptr,
                        ctypes.c_uint(osm_id_source),
                        ctypes.c_uint(osm_id_neighbor),
                        ctypes.c_float(distance)
                    )
    return c_graph_ptr

# Main function to find the shortest path
def find_shortest_path(c_graph_ptr, start_node_id: int, end_node_id: int, town: str = "") -> str:
    """Call the C Dijkstra algorithm and return the shortest path and total distance."""
    # Create a filename with a timestamp
    timestamp = datetime.now().strftime("%Y%m%d%H%M")
    town_str = f"_{town}" if town else ""
    itinerary_filename = f"itinerary{town_str}_{timestamp}.json"
    itinerary_filepath = os.path.join(DATA_FOLDER, itinerary_filename)

    # Convert the filename to bytes for c_char_p
    filename_bytes = itinerary_filepath.encode('utf-8')

    # Call the C function
    result = dijkstra_lib.export_dijkstra_shortest_path_to_file(
        filename_bytes,  # Pass as bytes
        c_graph_ptr,     # Pass the pointer to the graph
        ctypes.c_uint(start_node_id),
        ctypes.c_uint(end_node_id)
    )

    if result != 0:
        print(f"Error exporting shortest path to file: {result}")
    else:
        print(f"Shortest path itinerary saved to: {itinerary_filepath}")
    return itinerary_filepath

    

if __name__ == "__main__":
    ville="Ohey"
    G = get_ways_network_from_town(ville)
    c_graph_ptr = build_c_graph_from_networkx(G)
    # Choose start and end nodes
    start_node = list(G.nodes)[0]
    # start_node = 1152980807
    end_node = list(G.nodes)[100]
    # end_node = 8350148602
    print(f"start node is {start_node} ; end note is {end_node}")
    # Call the find_shortest_path function
    result_path = find_shortest_path(c_graph_ptr, start_node, end_node, ville)