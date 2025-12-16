import osmnx as ox
import networkx as nx
import geopandas as gpd

def get_ways_network_from_town(town: str, region:str = "Wallonie", country: str = "Belgique", network_type: str = "drive")-> nx.MultiDiGraph:
    """Load OSM data for a specific town using OSMnx"""
    print(f"Loading OSM data of type {network_type} using OSMnx for {town}, {region}, {country}...")
    G = ox.graph_from_place(f"{town}, {region}, {country}", network_type="drive")
    print(f"Loaded graph with {len(G.nodes)} nodes and {len(G.edges)} edges")
    return G

def get_town_names_from_region(region: str, country: str, place_types: list = ["city", "town", "village"]) -> gpd.GeoDataFrame:
    """Extract points of interest from a specified region"""
    place = f"{region}, {country}"
    tags = {"place": place_types}
    print(f"Extracting points of interest in {place} with types {place_types}...")
    gdf = ox.features_from_place(place, tags=tags)
    points_gdf = gdf[gdf.geometry.type == "Point"]
    points_gdf = points_gdf.sort_values(by="name")
    print(f"Extracted {len(points_gdf)} places")
    return points_gdf

def get_coordinates_from_town(town: str, towns_from_region: gpd.GeoDataFrame) -> tuple:
    """Get coordinates of a specific town from the GeoDataFrame"""
    town_row = towns_from_region[towns_from_region['name'] == town]
    if not town_row.empty:
        lat = town_row.geometry.y.values[0]
        lon = town_row.geometry.x.values[0]
        print(f"Coordinates of {town}: ({lat}, {lon})")
        return (lat, lon)
    else:
        print(f"Town {town} not found in the provided GeoDataFrame.")
        return None

if __name__ == "__main__":
    G = get_ways_network_from_town("Ohey")
    index=768899361
    print(f"Node {index}: (lat: {G.nodes[index]['y']}, lon: {G.nodes[index]['x']}), street_count: {G.nodes[index]['street_count']})")
    print(list(G.neighbors(index)))
    for u, v, data in G.edges(data=True):
        print(f"Start node of edge {u}")
        print(f"End node of edge {v}")
        print(f"Distance of edge {int(data['length'])}m")
        break
    # loc = get_town_names_from_region("Wallonie", "Belgique")
    # coords = get_coordinates_from_town("Ohey", loc)
    # print(coords)
