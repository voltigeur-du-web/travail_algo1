import osmnx as ox
import folium
import json
import os

from backend import DATA_FOLDER

# Charger le graphe (exemple : Namur)
G = ox.graph_from_place("Ohey, Belgique", network_type="drive")

# Creer une carte centree sur Ohey avec un fond satellite
m = folium.Map(
    location=[50.43914764175772, 5.1594092545434975],  # Coordonnees de Ohey
    zoom_start=15,
    tiles="https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}",
    attr="ESRI World Imagery",
)

# Ajouter les aretes du graphe
for u, v, data in G.edges(data=True):
    start = (G.nodes[u]["y"], G.nodes[u]["x"])
    end = (G.nodes[v]["y"], G.nodes[v]["x"])
    folium.PolyLine(
        [start, end],
        color="blue",
        weight=2,
        popup=f"{int(data['length'])}m",
    ).add_to(m)

# Ajouter les noeuds du graphe
for node in G.nodes():
    lat, lon = G.nodes[node]["y"], G.nodes[node]["x"]
    folium.CircleMarker(
        location=[lat, lon],
        radius=3,
        color="red",
        fill=True,
        popup=f"{node}",
    ).add_to(m)

# 1. Extraire les villes et villages de Wallonie
place = "Wallonie, Belgique"
tags = {"place": ["city", "town", "village"]}
gdf = ox.features_from_place(place, tags=tags)
#print(gdf)
# Filtrer uniquement les geometries de type Point
points_gdf = gdf[gdf.geometry.type == "Point"]
points_gdf = points_gdf.sort_values(by="name")
# Afficher les resultats
# for index, row in points_gdf.iterrows():
#     print(f"{row['name']}, {row['place']}, {row.geometry.x}, {row.geometry.y}")

# 3. Ajouter les villes et villages a la carte
for index, row in points_gdf.iterrows():
    lat, lon = row.geometry.y, row.geometry.x
    folium.Marker(
        location=[lat, lon],
        popup=f"{row['name']} ({row['place']})",  # Affiche le nom et le type (city/town/village)
        icon=folium.DivIcon(
            html=f"""
            <div style="font-size: 10pt; color: yellow; text-align: center;">
                <b>{row['name']}</b>
            </div>
            """
        ),
    ).add_to(m)



# Load the JSON data from the file
filename = "itinerary_Ohey_202511142320.json"
filepath = os.path.join(DATA_FOLDER,filename)
with open(filepath, 'r') as file:
    data = json.load(file)


# Extract the nodes and total distance
nodes = data['nodes']
total_distance = data['total_distance']
print(f"Total distance: {total_distance}")

# Add lines to connect the nodes in the path
for i in range(len(nodes) - 1):
    current_node = nodes[i]["osm_id"]
    next_node = nodes[i + 1]["osm_id"]
    print(f"current {current_node} and next {next_node}")
    start = (nodes[i]["lat"], nodes[i]["lon"])
    end = (nodes[i+1]["lat"], nodes[i+1]["lon"])
    folium.PolyLine(
        [start, end],
        color="yellow",
        weight=5,
    ).add_to(m)

    # # Ajouter les noeuds du graphe
    # for node in G.nodes():
    # lat, lon = G.nodes[node]["y"], G.nodes[node]["x"]
    # folium.CircleMarker(
    #     location=[lat, lon],
    #     radius=3,
    #     color="red",
    #     fill=True,
    #     popup=f"{node}",
    # ).add_to(m)

# Sauvegarder la carte dans un fichier HTML
m.save("graphe_ohey_satellite.html")
