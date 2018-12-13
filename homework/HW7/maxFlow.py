#!/usr/bin/python

# maxFlow.py
# Bryce Melegari, UAF CS411 Fall 2018
# Assignment 7
# Last edited: 12/13/2018
# Implements the Ford-Fulkerson Algorithm for solving the maximum
# flow problem.

import networkx as nx
import matplotlib.pyplot as plt


def createTestGraph():

	G = nx.DiGraph()
	G.add_nodes_from(range(6))
	G.add_edges_from([(0, 1, {'weight':16}), (0, 2, {'weight':13}),
					 (2, 1, {'weight':4}), (1, 2, {'weight':10}),
					 (1, 3, {'weight':12}),(2, 4, {'weight':14}),
					 (3, 2, {'weight':9}), (4, 3, {'weight':7}),
					 (3, 5, {'weight':20}),(4, 5, {'weight':4})])
	return G

def drawGraphWithCapacities(G):
	pos = nx.shell_layout(G, nlist=[range(1, G.number_of_nodes() - 1), [0,G.number_of_nodes() - 1]])
	weightLabels = nx.get_edge_attributes(G, 'weight')
	nx.draw(G, pos, with_labels=True)
	nx.draw_networkx_edge_labels(G, pos, edge_labels=weightLabels)

def drawGraphWithFlows(G):
	pos = nx.shell_layout(G, nlist=[range(1, G.number_of_nodes() - 1), [0,G.number_of_nodes() - 1]])
	weightLabels = nx.get_edge_attributes(G, 'flow')
	nx.draw(G, pos, with_labels=True)
	nx.draw_networkx_edge_labels(G, pos, edge_labels=weightLabels)



# G is of type nx.DiGraph, with defined 'weight' attributes for all edges.
# source and sink are integers representing which nodes in the graph are 
# the source and sink. Returns the integer max flow.
# SIDE EFFECT: sets the 'flow' attributes of all graph edges to optimal values.
def maxFlow(G, source, sink):
	# initialize flow for all edges to 0
	G.set_edge_attributes('flow', 0)

