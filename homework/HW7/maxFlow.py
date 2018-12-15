#!/usr/bin/python

# maxFlow.py
# Bryce Melegari, UAF CS411 Fall 2018
# Assignment 7
# Last edited: 12/13/2018
# Implements the Ford-Fulkerson Method for solving the maximum
# flow problem. The specific implementation used is the Edmonds-Karp
# algorithm (using a BFS to start with the shortest augmenting path).
# Note that networkx does in fact ship with an edmondskarp function,
# but I am avoiding using it or any parts of it, and am only using the package
# for the basic graph representation.

import networkx as nx
import matplotlib.pyplot as plt
from Queue import Queue

def createTestGraph():

	G = nx.DiGraph()
	# G.add_nodes_from(range(6))
	# G.add_edges_from([(0, 1, {'capacity':16}), (0, 2, {'capacity':13}),
	# 				 (2, 1, {'capacity':4}), (1, 2, {'capacity':10}),
	# 				 (1, 3, {'capacity':12}),(2, 4, {'capacity':14}),
	# 				 (3, 2, {'capacity':9}), (4, 3, {'capacity':7}),
	# 				 (3, 5, {'capacity':20}),(4, 5, {'capacity':4})])
	# return G

	G.add_nodes_from(range(7))
	G.add_edges_from([(0, 3, {'capacity':3}), (0, 1, {'capacity':3}),
					(2, 0, {'capacity':3}), (1, 2, {'capacity':4}),
					(2, 3, {'capacity':1}), (2, 4, {'capacity':2}),
					(4, 1, {'capacity':1}), (3, 4, {'capacity':2}),
					(3, 5, {'capacity':6}), (5, 6, {'capacity':9}),
					(4, 6, {'capacity':1})])

	return G


def drawGraphWithWeights(G, labelKey):
	# pos = nx.shell_layout(G, nlist=[[2], [0, 1, 3, 4, 5, 6]])
	pos = [(-2, 1), (-2, -1), (-1, 0), (0, 1), (0, -1), (1, 1), (1, -1)]
	weightLabels = nx.get_edge_attributes(G, labelKey)
	nx.draw(G, pos, with_labels=True)
	nx.draw_networkx_edge_labels(G, pos, edge_labels=weightLabels)

# get total flow entering the graph G at the source
def getFlowIn(G, source):
	total = 0
	for neighbor in nx.neighbors(G, source):
		total += G.edges()[(source, neighbor)]['flow']
	return total

# get total flow leaving the graph G at the sink
def getFlowOut(G, sink):
	total = 0
	for pred in G.predecessors(sink):
		total += G.edges()[(pred, sink)]['flow']
	return total

# finds the shortest augmenting path of a residual graph R
# Each node (except source/sink) will be labeled with 'additionalFlow'
# and 'fromNode' -- the node from which the labeled node was reached. 
# 'fromNode' will be a 2-tuple of the form (node, '+'/'-'), where '+' denotes
# a forward edge, and '-' a backward edge.
def shortestAugmentingPath(G, source, sink):
	Q = Queue()
	nx.set_edge_attributes(G, 0, 'flow')
	nx.set_node_attributes(G, {source: float("inf")}, 'additionalFlow') # set source to infinity
	Q.put(source)	# enqueue source node
	count = 0
	while not Q.empty():
		i = Q.get() # dequeue, set i to current node
		for j in nx.neighbors(G, i): # forward edges
			if 'additionalFlow' not in G.nodes()[j]: # j isn't labeled yet
				r = G.edges()[(i, j)]['capacity'] - G.edges()[(i, j)]['flow']
				if r > 0:
					G.nodes()[j]['additionalFlow'] = min(G.nodes()[i]['additionalFlow'], r)
					G.nodes()[j]['fromNode'] = (i, '+')
					Q.put(j) # enqueue labeled neighbor

		backE = [item for item in G.edges() if item[1] == i] # create list of backward edges to i
		for backEdge in backE: # backward edges
			j = backEdge[0] # j is the number of the back edge start node
			if 'additionalFlow' not in G.nodes()[j]: # j isn't labeled yet
				if G.edges()[backEdge]['flow'] > 0:
					G.nodes()[j]['additionalFlow'] = min(G.nodes()[i]['additionalFlow'], G.edges()[backEdge]['flow'])
					G.nodes()[j]['fromNode'] = (i, '-')
					Q.put(j)

		if 'additionalFlow' in G.nodes()[sink]: # if sink is labeled, augment

			# draw this augmenting path --------------------------------------------------
			pos = [(-2, 1), (-2, -1), (-1, 0), (0, 1), (0, -1), (1, 1), (1, -1)]
			nx.draw_networkx_nodes(G, pos)
			nx.draw_networkx_labels(G, pos)
			pathEdges = []
			
			# ---------------------------------------------------------------------------

			j = sink
			while j != source: # breaks if we reach source
				print('i: '+ str(i) + ', j: ' + str(j))
				if G.nodes()[j]['fromNode'][1] == '+':
					print("plus!")
					nx.set_edge_attributes(G, {(i, j): G.edges()[(i, j)]['flow'] + G.nodes()[sink]['additionalFlow']}, 'flow')
					pathEdges.append(tuple([i, j]))
				else:
					print('minus!')
					nx.set_edge_attributes(G, {(j, i): G.edges()[(j, i)]['flow'] - G.nodes()[sink]['additionalFlow']}, 'flow')
					pathEdges.append(tuple([j, i]))

				j = i
				if i == 0:
					break
				i = G.nodes()[i]['fromNode'][0]
				#print('i: '+ str(i) + ', j: ' + str(j))

			nx.draw_networkx_edges(G, pos, pathEdges)
			edgeLabels = nx.get_edge_attributes(G, 'flow')
			nx.draw_networkx_edge_labels(G, pos, edge_labels=edgeLabels)
			plt.savefig('path' + str(count) + '.png')
			plt.clf()

			# erase all node labels except source's
			for (nodeNum, dic) in G.nodes(data=True):
				if 'additionalFlow' in dic and nodeNum != source:
					del dic['additionalFlow']
					del dic['fromNode']
			del Q
			Q = Queue()
			Q.put(source)	# enqueue source node
		count += 1
	if getFlowIn(G, source) != getFlowOut(G, sink):
		print("ERROR: SOURCE FLOW != SINK FLOW. DIDN'T WORK")

	return (G, getFlowIn(G, source), getFlowOut(G, sink))


# G is of type nx.DiGraph, with defined 'capacity' and 'flow' attributes for all edges.
# source and sink are integers representing which nodes in the graph are 
# the source and sink. Returns the integer max flow.

G = createTestGraph()
drawGraphWithWeights(G, 'capacity')
plt.savefig('G.png')
plt.clf()
maxGraph = shortestAugmentingPath(G, 0, 6)
drawGraphWithWeights(maxGraph[0], 'flow')
plt.savefig('max.png')
print("Flow in: " + str(maxGraph[1]))
print("Flow out: " + str(maxGraph[2]))
#print(list(nx.nodes(G))[2])