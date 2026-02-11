#pragma once

#include <memory>
#include <vector>
#include <stdexcept>

#include "utilities/logger.h"
#include "utilities/minPriorityQueue.h"


typedef std::pair<int, int> Point;


/// <summary>
/// A path between two vertexs
/// </summary>
/// <param name="from">Starting vertex of the edge</param>
/// <param name="to">Ending vertex of the edge</param>
/// <param name="cost">Cost to traverse this edge</param>
struct Edge {
	std::shared_ptr<Vertex> from;
	std::shared_ptr<Vertex> to;
	unsigned int cost;
	Edge(Vertex* from, Vertex* to, unsigned int cost) : from(from), to(to), cost(cost) {}
};

/// <summary>
/// A vertex represents a point in the graph with its position, edges to neighboring vertexs, and costs for pathfinding.
/// </summary>
/// <param name="position">( x, y ) coordinate of vertex</param>
/// <param name="edges">List of edges connecting this vertex to its neighbors</param>
/// <param name="gCost">Cost from the start vertex to this vertex</param>
/// <param name="hCost">Heuristic cost from this vertex to the goal vertex</param>
/// <param name="fCost">Total cost (gCost + hCost) used for prioritization in the open list</param>
/// <param name="parent">Pointer to the parent vertex in the path, used for path reconstruction</param>
struct Vertex {
	Point position;
	std::vector<Edge> edges;
	int gCost, hCost, fCost;
	std::shared_ptr<Vertex> parent;
	Vertex(Point position) : position(position), gCost(0), hCost(0), fCost(0), parent(nullptr) {}
};

class AStar {
public:
	AStar() = default;
	AStar(AStar& other) = delete;
	AStar& operator=(const AStar&) = delete;

	~AStar() = default;

	bool AddVertex(const int xPos, const int yPos);
	bool AddEdge(Vertex* from, Vertex* to, const unsigned int cost);

	std::vector<Point> FindPath(Vertex start, Vertex goal);

private:
	MinPriorityQueue<std::shared_ptr<Vertex>> priorityQueue;

	std::vector<std::shared_ptr<Vertex>> vertices;
	std::vector<std::shared_ptr<Vertex>> openList;
	std::vector<std::shared_ptr<Vertex>> closedList;
	std::shared_ptr<Vertex> GetVertex(Point position);
	std::vector<Point> ReconstructPath(std::shared_ptr<Vertex> currentvertex);
};

/// <summary>
/// Creates a new vertex at the specified (x, y) position.
/// </summary>
/// <param name="xPos">World X-coordinate of vertex</param>
/// <param name="yPos">World Y-coordinate of vertex</param>
/// <returns>Returns true: Successfully added vertex. Returns false: vertex already exsist</returns>
inline bool AStar::AddVertex(const int xPos, const int yPos) {

	Point newPos = std::make_pair(xPos, yPos);
	if(this->GetVertex(newPos)) {
		Logger::Log("vertex already exists at position: (", xPos, ", ", yPos, ").");
		return false;
	}

	this->vertices.push_back(std::make_shared<Vertex>(newPos));
	return true;
}

/// <summary>
/// Adds a non-directional edge between two vertices with the specified cost.
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="cost"></param>
/// <returns>Successfully added edge</returns>
inline bool AStar::AddEdge(Vertex* from, Vertex* to, const unsigned int cost) {
	
	if (!from || !to) {
		Logger::Log("Invalid vertexs for edge creation.");
		return false;
	}
	
	// Check if the edge already exists
	for (auto& edge : from->edges) {
		if (edge.to.get() == to) {
			Logger::Log("Edge already exists between (", from->position.first, ", ", from->position.second, ") and (",
						to->position.first, ", ", to->position.second, ").");
			return false; // Edge already exists
		}
	}
	from->edges.emplace_back(from, to, cost);
	to->edges.emplace_back(to, from, cost);
	return true;
}

/// <summary>
/// Retrieves a vertex from the collection by its position.
/// </summary>
/// <param name="position">The position of the vertex to find.</param>
/// <returns>A shared pointer to the vertex, or nullptr if no matching vertex is found.</returns>
inline std::shared_ptr<Vertex> AStar::GetVertex(Point position) {
	for (auto& vertex : this->vertices) {
		if (vertex->position == position) {
			return vertex;
		}
	}
	return nullptr; // vertex not found
}
