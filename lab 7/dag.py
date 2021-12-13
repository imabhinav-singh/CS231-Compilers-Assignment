import re 
import networkx as nx
from matplotlib import pyplot as plt

file = open('output.txt')
lines = file.readlines();

temp_lines = list()
for i in range(len(lines)):
    lines[i] = lines[i].replace('\n', '')
    if lines[i] not in temp_lines:
        temp_lines.append(lines[i])

lines = temp_lines
# print(temp_lines)

graph = {}

for line in lines:
    line = line.replace(' ', '')
    temp = line.split('=')
    lhs = temp[0]
    rhs = temp[1]
    nodes = re.split('\+|\-|/|\*', rhs)
    
    for node in nodes:
        if node not in graph:
            graph[node] = {'val' : node, 'adj' : []}
    
    op = re.sub('[a-e]|t|[0-9]', '', rhs)
    graph[lhs] = {'val' : op, 'adj' : []}

    graph[nodes[0]]['adj'].append(lhs)
    graph[nodes[1]]['adj'].append(lhs)
    graph[lhs]['adj'].append(nodes[0])
    graph[lhs]['adj'].append(nodes[1])

for key in graph:
    print(key+' ('+graph[key]['val']+') : '+str(graph[key]['adj']))

g1 = nx.DiGraph()

for key in graph:
    for edge in graph[key]['adj']:
        u, v = key, edge
        if u.startswith('t'):
            u = key + " (" + graph[key]['val'] + ")"
        if v.startswith('t'):
            v = edge + " (" + graph[edge]['val'] + ")"
        g1.add_edges_from([(u, v)])

plt.tight_layout()
nx.draw_networkx(g1, arrows=False, node_color='blue', node_size=1500)
plt.savefig("g1.png", format="PNG")
plt.show()