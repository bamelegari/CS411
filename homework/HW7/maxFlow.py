#!/usr/bin/python

# maxFlow.py
# Bryce Melegari, UAF CS411 Fall 2018
# Assignment 7
# Last edited: 12/14/2018
# Implements the Ford-Fulkerson Method for solving the maximum
# flow problem. The specific implementation used is the Edmonds-Karp
# algorithm (using a BFS find the shortest augmenting path).
# Note that networkx does in fact ship with an edmondskarp function,
# but I am avoiding using it or any parts of it, and am only using the package
# for the basic graph representation and utilities.

# For testing, I have hard-coded a list of graphs that is fed to the function, along with
# information to facilitate the drawing of the input graph with capacities labeled, the 
# maximized graph with flows labeled, and each augmenting path. A different directory for each
# test graph will be created in this script's directory, and each of those will contain the images.
# The numaric maximum flow result is printed to the console, along with the name of which directory of
# images it corresponds to. Because the two drawing-related parameters to shortestAugmentingPath() are optional
# it can still be called on graphs, and will simply not print images for them.
# NOTE: the images do not handle multiple edges between the same nodes well. You will see an arrow on each side of
# the edge, but only one of the labels.

import networkx as nx
import matplotlib.pyplot as plt
import sys
if sys.version_info[0] < 3:
	from queue import Queue
else:
	from Queue import Queue
import os



# returns a list of tuples, the first element of which is the test graph, and the second element
# of which is a list of coordinates for drawing the nodes of the graph. The graphs chosen here are all
# examples from various texts and sites, meant to ensure the correctness of the algorithm.
def createTestGraphs():

	G0 = tuple([nx.DiGraph(), [(-2, 0), (-1, 1), (-1, -1), (1, 1), (1, -1), (2, 0)]])
	G0[0].add_nodes_from(range(6))
	G0[0].add_edges_from([(0, 1, {'capacity':16}), (0, 2, {'capacity':13}),
					 (2, 1, {'capacity':4}), (1, 2, {'capacity':10}),
					 (1, 3, {'capacity':12}),(2, 4, {'capacity':14}),
					 (3, 2, {'capacity':9}), (4, 3, {'capacity':7}),
					 (3, 5, {'capacity':20}),(4, 5, {'capacity':4})])

	G1 = tuple([nx.DiGraph(), [(-2, 1), (-2, -1), (-1, 0), (0, 1), (0, -1), (1, 1), (1, -1)]])
	G1[0].add_nodes_from(range(7))
	G1[0].add_edges_from([(0, 3, {'capacity':3}), (0, 1, {'capacity':3}),
					(2, 0, {'capacity':3}), (1, 2, {'capacity':4}),
					(2, 3, {'capacity':1}), (2, 4, {'capacity':2}),
					(4, 1, {'capacity':1}), (3, 4, {'capacity':2}),
					(3, 5, {'capacity':6}), (5, 6, {'capacity':9}),
					(4, 6, {'capacity':1})])

	G2 = tuple([nx.DiGraph(), [(-1, 0), (0, 0), (1, 0), (0, -1), (1, 1), (2, 0)]])
	G2[0].add_nodes_from(range(6))
	G2[0].add_edges_from([(0, 1, {'capacity':2}), (0, 3, {'capacity':3}),
					(1, 2, {'capacity':5}), (3, 2, {'capacity':1}),
					(1, 4, {'capacity':3}), (4, 5, {'capacity':4}),
					(2, 5, {'capacity':2})])

	G3 = tuple([nx.DiGraph(), [(-2, 0), (-1, 1), (-1, -1), (0, 0), (1, 1), (1, -1)]])
	G3[0].add_nodes_from(range(6))
	G3[0].add_edges_from([(0, 1, {'capacity':20}), (0, 2, {'capacity':60}),
					(1, 2, {'capacity':50}), (1, 3, {'capacity':30}),
					(2, 3, {'capacity':35}), (1, 4, {'capacity':10}),
					(2, 5, {'capacity':10}), (3, 4, {'capacity':30}),
					(3, 5, {'capacity':25}), (4, 5, {'capacity':50})])

	G4 = tuple([nx.DiGraph(), [(-2, 0), (-1, 1), (-1, 0), (-1, -1), (1, 1), (1, 0), (1, -1), (2, 0)]])
	G4[0].add_nodes_from(range(8))
	G4[0].add_edges_from([(0, 1, {'capacity':10}), (0, 2, {'capacity':5}),
					(0, 3, {'capacity':15}), (1, 2, {'capacity':4}),
					(1, 5, {'capacity':15}), (1, 4, {'capacity':9}),
					(2, 5, {'capacity':8}), (2, 3, {'capacity':4}),
					(3, 6, {'capacity':16}), (4, 5, {'capacity':15}),
					(4, 7, {'capacity':10}), (5, 7, {'capacity':10}),
					(5, 6, {'capacity':15}), (6, 2, {'capacity':6}),
					(6, 7, {'capacity':10})])
	return [G0, G1, G2, G3, G4]


def drawGraphWithWeights(G, labelKey, pos=[]):
	weightLabels = nx.get_edge_attributes(G, labelKey)
	if not pos:
		pos = nx.random_layout(G)
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
# a forward edge, and '-' a backward edge. The pos and graphDir parameters are only to facilitate
# drawing the augmenting paths, and are optional
def shortestAugmentingPath(G, source, sink, pos=[], graphDir=""):
	Q = Queue()
	nx.set_edge_attributes(G, 0, 'flow')
	nx.set_node_attributes(G, {source: float("inf")}, 'additionalFlow') # set source to infinity
	Q.put(source)	# enqueue source node
	count = 0 # for path drawing
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

			pathEdges = []
			j = sink
			while j != source: # breaks if we reach source
				if G.nodes()[j]['fromNode'][1] == '+':
					nx.set_edge_attributes(G, {(i, j): G.edges()[(i, j)]['flow'] + G.nodes()[sink]['additionalFlow']}, 'flow')
					pathEdges.append(tuple([i, j])) # for path drawing
				else:
					nx.set_edge_attributes(G, {(j, i): G.edges()[(j, i)]['flow'] - G.nodes()[sink]['additionalFlow']}, 'flow')
					pathEdges.append(tuple([j, i])) # for path drawing

				j = i
				if i == 0:
					break
				i = G.nodes()[i]['fromNode'][0]

			# draw this augmenting path--------------------------------------------------
			if pos:
				nx.draw_networkx_nodes(G, pos)
				nx.draw_networkx_labels(G, pos)
				nx.draw_networkx_edges(G, pos, pathEdges)
				edgeLabels = nx.get_edge_attributes(G, 'flow')
				nx.draw_networkx_edge_labels(G, pos, edge_labels=edgeLabels)
				savePath = os.path.join(graphDir, 'augPath' + str(count) + '.png')
				plt.savefig(savePath)
				plt.clf()
				count += 1
			# ------------------------------------------------------------------------------------

			# erase all node labels except source's
			for (nodeNum, dic) in G.nodes(data=True):
				if 'additionalFlow' in dic and nodeNum != source:
					del dic['additionalFlow']
					del dic['fromNode']
			del Q
			Q = Queue()
			Q.put(source)	# enqueue source node

			flowOut = getFlowOut(G, sink)
			flowIn = getFlowIn(G, source)
	if flowIn != flowOut:
		print("ERROR: SOURCE FLOW != SINK FLOW. ALGORITHM FAILED")
		print("flow in: " + str(flowIn))
		print("flow out: " + str(flowOut))
	else:
		if graphDir:
			print(graphDir + ": ")
		print("maximum flow: " + str(flowIn) + '\n')

	return (G)


testGraphs = createTestGraphs()

for i in range(len(testGraphs)):
	# make a new folder for each test graph
	graphDir = "graph" + str(i)
	if not os.path.exists(graphDir):
		os.makedirs(graphDir)
	# draw each initial test graph and place it in correct dir
	drawGraphWithWeights(testGraphs[i][0], 'capacity', testGraphs[i][1])
	plt.savefig(os.path.join(graphDir, 'inputGraph.png'))
	plt.clf()
	maxGraph = shortestAugmentingPath(testGraphs[i][0], 0, nx.number_of_nodes(testGraphs[i][0]) - 1, testGraphs[i][1], graphDir)
	drawGraphWithWeights(maxGraph, 'flow', testGraphs[i][1])
	plt.savefig(os.path.join(graphDir, 'maxFlowGraph.png'))
	plt.clf()