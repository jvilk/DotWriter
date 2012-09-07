/*
 * Graph class. Allows you to create a directed graph of nodes, and output
 * it to a DOT file.
 *
 * Author: John Vilk (jvilk@cs.umass.edu)
 */

#ifndef GRAPH_H_
#define GRAPH_H_

// Included here so that people who use GNode don't have to.
#include "GEnums.h"

#include <set>
#include <string>
#include <vector>
#include <map>

/**
 * TODO: Have an enum of attributes. Default to 0 for each? Make sure the default is sane (or EMPTY)
 * node->setAttribute(GNodeAttribute.BLAH, Value)
 *
 * Graph Attributes:
 * 	* fontcolor
 * Node Attributes:
 *  * Style
 *  * fontcolor
 *  * Shape
 * Edge Attributes:
 *  * Style
 *  * fontcolor
 *  * arrowhead
 *  * arrowsize
 * Make a UGraph for undirected graphs.
 * Clusters?
 */

/**
 * Represents a node in a graph.
 */
class GNode {
private:
	unsigned int _nodeId;
	std::string _label;
	GNodeStyle _style;

protected:
	// Nodes should only be created / destroyed via Graph.
	// Hence, these are protected.
	GNode(unsigned int nodeId, std::string label = "") :
		_nodeId(nodeId), _label(label) {

	}
	
	virtual ~GNode();

public:
	const std::string toString();

  /** Simple getters / setters **/
	void setStyle(GNodeStyle style) {
		_style = style;
	}

	std::string& getLabel() {
		return _label;
	}

	void setLabel(const std::string& label) {
		_label = label;
	};

	unsigned int getNodeId() {
		return _nodeId;
	}
};

/**
 * Represents an edge in a graph.
 */
class GEdge {
private:
	GNode * _src;
	GNode * _dst;

protected:
	GEdge(GNode * src, GNode * dst) {
		_src = src;
		_dst = dst;
	}

	virtual ~GEdge();

public:
	GNode * getSource() {
		return _src;
	}

	GNode * getDest() {
		return _dst;
	}

	std::string toString() {
		return "";
	}
};

/**
 * Represents a graph. It's a bag of nodes and edges, basically.
 */
class Graph {
private:
	std::string _title;
	std::string _label;
	GColor _fontColor;
	unsigned int _nextNodeId;
	std::set<GNode *> _nodes;
	std::set<GEdge *> _edges;
	std::set<Graph *> _subgraphs;

public:
	Graph(std::string title = "Graph") :
		_title(title), _nextNodeId(0) {
	}

	/** Simple getters and setters **/

	void setFontColor(GColor fontColor) {
		_fontColor = fontColor;
	}

	GColor getFontColor() {
		return _fontColor;
	}

	std::string getTitle() {
		return _title;
	}

	void setTitle(std::string title) {
		_title = title;
	}

	std::string& getLabel() {
		return _label;
	}

	void setLabel(std::string label) {
		_label = label;
	}

	/**
	 * Constructs a GNode object, adds it to the graph, and returns it.
	 */
	GNode * addNode(std::string name = NULL, std::vector<GNode *> * inNodes = NULL);

	/**
	 * Removes the node from the graph.
	 *
	 * Note that this function deletes the node object, and removes all edges
	 * to/from it. Also, note that it is currently more expensive than you may
	 * expect -- O(|E|).
	 */
	void removeNode(GNode *);

	/**
	 * Add an edge to the graph. Returns a reference to the edge that can be
	 * manipulated to change edge properties.
	 */
	GEdge * addEdge(GNode * src, GNode * dst, std::string label = NULL);

	/**
	 * Removes the edge from the graph. Note that this also deletes the GEdge
	 * object.
	 */
	void removeEdge(GEdge *);

	/**
	 * Removes any edges from src to dst from the graph. Note that this version
	 * of the function is currently O(|E|).
	 */
	void removeEdge(GNode * src, GNode * dst);

	/**
	 * Returns a string representation of the graph in DOT file format.
	 */
	std::string toString();

	/**
	 * Returns a string representation of the graph as a subgraph section
	 * of a DOT file.
	 */
	std::string toStringSubgraph();

	/**
	 * Writes the graph to the specified filename in the DOT format.
	 *
	 * Returns true if successful, false otherwise.
	 */
	bool writeToFile(std::string& filename);

};

#endif /* GRAPH_H_ */
