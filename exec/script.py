import networkx as nx
import matplotlib.pyplot as plt
from collections import Counter
import json
import sys


def create_graph(vectors):
    G = nx.Graph()
    edge_labels = {}
    for vector in vectors["vectors"]:
        for node in vector[:2]:
            if node not in G.nodes():
                G.add_node(node)
        for node1, node2 in zip(vector, vector[1:2]):
            if len(vector) == 3:
                if (node2, node1) in edge_labels:
                    edge_labels[(node2, node1)] += " " + vector[2]
                else:
                    G.add_edge(node1, node2, label=vector[2:3])
                    edge_labels[(node1, node2)] = vector[2]
            else:
                G.add_edge(node1, node2)
    return G, edge_labels


def count_strings(vectors):
    counts = Counter(string for vector in vectors["vectors"] for string in vector)
    return counts

def color_nodes_by_count(G, vectors):
    counts = count_strings(vectors)
    node_colors = ['red' if counts[node] >= 5 else 'green' for node in G.nodes()]
    return node_colors

def create_table(vectors):
    linear_signal = {lst[0].replace("_",""): "D: " + lst[1].replace("_","") + ' U:' + lst[2].replace("_","")  for lst in vectors["sectionID"]}
    return linear_signal

def main(json_file, name_png, route1=None, route2=None):
        with open(json_file) as user_file:
            routes = []
            route_edges =[]
            edge_colors = []
            
            name_png = "..\\fig\\" + name_png
            file_contents = user_file.read()
            vectors = json.loads(file_contents)
            G_merged,edge_labels = create_graph(vectors)
            node_colors = color_nodes_by_count(G_merged, vectors)
            linear_signal = create_table(vectors)
            cell_text = [[key, value] for key, value in linear_signal.items()]

            # Create a 2x1 grid of subpl   
            fig, ax1 = plt.subplots(figsize=(9, 9))


            # Draw the graph on the upper subplot
            pos = nx.kamada_kawai_layout(G_merged)
            if(route1 != None):
                routes.append(vectors["routes"][int(route1)])
                name_png = name_png + "_" + route1
                if(route2 != None):
                    routes.append(vectors["routes"][int(route2)])
                    name_png = name_png + "_" + route2
                for route in routes:
                    for element in route:
                        index = route.index(element)
                        if index < len(route) - 1:
                            next_element = route[index+1]
                            route_edges.append((element, next_element))
                for edge in G_merged.edges(data=True):
                    if edge[0:2] in route_edges or edge[1::-1] in route_edges:
                        edge_colors.append("red")
                    else:
                        edge_colors.append("black")
            nx.draw_networkx_edges(G_merged, pos, edge_color=edge_colors)
            if(len(edge_colors) != 0):
                nx.draw(G_merged, pos, with_labels=True, node_size=400, node_color=node_colors, edge_color=edge_colors, font_size=5)
            else:
                nx.draw(G_merged, pos, with_labels=True, node_size=400, node_color=node_colors,font_size=5)
            nx.draw_networkx_edge_labels(G_merged, pos, edge_labels=edge_labels, font_size=5, font_color='blue')
            table = ax1.table(cellText=cell_text, colLabels=["Linears", "Signals"],cellLoc="center",loc="center", bbox=[-0.15, 0.75, 0.3, 0.4])
            table.auto_set_font_size(False)
            table.set_fontsize(4)
            table.scale(1, 4)
            table.auto_set_column_width(col=[0,1])
            cellDict = table.get_celld()
            for i in range(0,len(["Linears", "Signals"])):
                cellDict[(0,i)].set_height(.7)
                for j in range(1,len(cell_text)+1):
                    cellDict[(j,i)].set_height(.7)

            # Remove the borders and ticks from both axes
            ax1.axis('off')
            ax1.set_frame_on(False)
            ax1.tick_params(axis='both', which='both', length=0)

            # Adjust the spacing between the two axes
            fig.subplots_adjust(wspace=0.1)
            plt.savefig(name_png, dpi=800)
            plt.show()


if __name__ == "__main__":
    json_file = None
    name_png = None
    route1 = None
    route2 = None

    for i, arg in enumerate(sys.argv):
        if arg == "--json" and len(sys.argv) > i+1:
            json_file = sys.argv[i+1]
        if arg == "--name-file" and len(sys.argv) > i+1:
            name_png = sys.argv[i+1]
        if arg == "--route1" and len(sys.argv) > i+1:
            route1 = sys.argv[i+1]
        if arg == "--route2" and len(sys.argv) > i+1:
            route2 = sys.argv[i+1]
        if arg == "--help":
            help_called = True
            break

    main(json_file, name_png, route1, route2)
