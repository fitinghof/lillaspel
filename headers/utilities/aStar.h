#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "utilities/logger.h"
#include "utilities/minPriorityQueue.h"


typedef std::pair<int, int> AStarPoint;

/// <summary>
/// A path between two vertices
/// </summary>
/// <param name="from">Starting vertex of the edge</param>
/// <param name="to">Ending vertex of the edge</param>
/// <param name="cost">Cost to traverse this edge</param>
struct Edge {
	struct AStarVertex* from;
	struct AStarVertex* to;
	unsigned int cost;
	Edge(struct AStarVertex* from, struct AStarVertex* to, unsigned int cost) : from(from), to(to), cost(cost) {}
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
struct AStarVertex {
	AStarPoint position;
	std::vector<struct Edge> edges;
	int gCost, hCost, fCost;
	AStarVertex* parent;
	AStarVertex(AStarPoint position) : position(position), gCost(INT_MAX), hCost(0), fCost(0), parent(nullptr) {}
	inline bool operator<(const AStarVertex& other) const { return this->fCost < other.fCost; }
	inline bool operator==(const AStarVertex& other) const { return this->position == other.position; }
};

class AStar {
public:
	AStar() = default;
	AStar(AStar& other) = delete;
	AStar& operator=(const AStar&) = delete;

	~AStar() = default;

	bool AddVertex(AStarPoint position);
	void RemoveVertex(AStarPoint position);

	bool AddEdge(AStarVertex* from, AStarVertex* to, const unsigned int cost);

	void SetGoal(AStarPoint goal);

	std::vector<AStarPoint> FindPath(AStarPoint start);

private:
	std::vector<std::unique_ptr<AStarVertex>> vertices;
	AStarVertex* goal;

	MinPriorityQueue<AStarVertex*> openList;
	std::vector<AStarVertex*> closedList;

	std::vector<AStarPoint> path;

	const int CalculateHeuristic(AStarVertex* vertex);
	void ClearPathfindingData();

	AStarVertex* GetVertex(AStarPoint position);
	const int GetEdgeCost(AStarVertex* from, AStarVertex* to);
	std::vector<AStarVertex*> GetNeighbors(AStarVertex* vertex);
	std::vector<AStarPoint>& ReconstructPath();
};


/// <summary>
/// Creates a new vertex at the specified (x, y) position.
/// </summary>
/// <param name="position">Integer pair representing the coordinates of the vertex.</param>
/// <returns>Returns true: Successfully added vertex. Returns false: vertex already exsist</returns>
inline bool AStar::AddVertex(AStarPoint position) {

	if (this->GetVertex(position)) {
		Logger::Log("vertex already exists at position: (", position.first, ", ", position.second, ").");
		return false;
	}

	this->vertices.push_back(std::make_unique<AStarVertex>(position));
	return true;
}

/// <summary>
/// Removes a vertex at the specified position from the graph, along with all edges connected to it.
/// </summary>
/// <param name="position">The position of the vertex to remove from the graph.</param>
inline void AStar::RemoveVertex(AStarPoint position) { 
	AStarVertex* vertexToRemove = this->GetVertex(position);
	if (!vertexToRemove) {
		Logger::Log("No vertex found at position: (", position.first, ", ", position.second, ").");
		return;
	}

	// Remove edges connected to this vertex
	for (auto& vertex : this->vertices) {
		for (auto& edge : vertex->edges) {
			if (*edge.to == vertexToRemove->position) {
				vertex->edges.erase(std::remove_if(vertex->edges.begin(), vertex->edges.end(),
								   [vertexToRemove](const Edge& edge) { return *edge.to == vertexToRemove->position; }),
									vertex->edges.end());
			}
		}
	}

	// Remove the vertex itself
	this->vertices.erase(std::remove_if(this->vertices.begin(), this->vertices.end(),
										[position](const std::unique_ptr<AStarVertex>& vertex) { return vertex->position == position; }),
										this->vertices.end());
}

/// <summary>
/// Adds a non-directional edge between two vertices with the specified cost.
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="cost"></param>
/// <returns>Successfully added edge</returns>
inline bool AStar::AddEdge(AStarVertex* from, AStarVertex* to, const unsigned int cost) {
	
	if (!from || !to) {
		Logger::Log("Invalid vertexs for edge creation.");
		return false;
	}
	
	// Check if the edge already exists
	for (auto& edge : from->edges) {
		if (*edge.to == to->position) {
			Logger::Log("Edge already exists between (", from->position.first, ", ", from->position.second, ") and (",
						to->position.first, ", ", to->position.second, ").");
			return false; // Edge already exists
		}
	}
	from->edges.emplace_back(from, to, cost);
	to->edges.emplace_back(to, from, cost);
	return true;
}

inline void AStar::SetGoal(AStarPoint goal) { this->goal = this->GetVertex(goal); }

/// <summary>
/// Finds the shortest path between two vertices using the A* pathfinding algorithm.
/// </summary>
/// <param name="start">A shared pointer to the starting vertex for the pathfinding search.</param>
/// <param name="goal">A shared pointer to the target vertex to reach.</param>
/// <returns>A vector of coordinate pairs representing the path from start to goal.</returns>
inline std::vector<std::pair<int, int>> AStar::FindPath(AStarPoint start) {
	this->ClearPathfindingData(); // Reset the state of the algorithm before starting a new search

	AStarVertex* startVertex = this->GetVertex(start);
	if (!startVertex || !this->goal) {
	    Logger::Log("Invalid start or goal position.");
	    return {};
	}
	startVertex->gCost = 0;
	this->openList.Enqueue(startVertex, 0); // Force Start with the initial vertex in the open list

	while (!this->openList.IsEmpty()) {
		AStarVertex* current = this->openList.Dequeue();

		if (*current == *this->goal) { // If goal is reached, reconstruct path
			return this->ReconstructPath();
		}

		this->closedList.push_back(current); // Mark current vertex as visited by adding it to the closed list
		for (auto& neighbor : this->GetNeighbors(current)) {
			if (std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end()) {
				continue; // Skip if neighbor is in closed list
			}
			
			int tentativeGCost = current->gCost + this->GetEdgeCost(current, neighbor);

			// If neighbor is already in open list with better cost, skip it
			// Only update if this path is better
			if (tentativeGCost < neighbor->gCost) {
				neighbor->gCost = tentativeGCost;
				neighbor->hCost = this->CalculateHeuristic(neighbor);
				neighbor->fCost = neighbor->gCost + neighbor->hCost;
				neighbor->parent = current;
				this->openList.Enqueue(neighbor, neighbor->fCost);
			}
		}
	}

	return {}; // Return empty path if no path is found
}

/// <summary>
/// Calculates the heuristic cost (hCost) between the current vertex and the goal vertex using the Euclidean distance.
/// </summary>
/// <param name="vertex">Current vertex we want to calculate heuristic for.</param>
/// <param name="goal">Goal vertex used for calculating the heuristic.</param>
/// <returns>The heuristic value in form of an integer.</returns>
inline const int AStar::CalculateHeuristic(AStarVertex* vertex) {
	int dx = vertex->position.first - goal->position.first;
	int dy = vertex->position.second - goal->position.second;
	return static_cast<int>(std::sqrt(dx*dx + dy*dy));
}

/// <summary>
/// Reset the state of the A* algorithm for a new interation.
/// </summary>
inline void AStar::ClearPathfindingData() {
	// Reset state
	this->openList.Clear();
	this->closedList.clear();

	// Reset all vertices
	for (auto& vertex : this->vertices) {
		vertex->gCost = INT_MAX;
		vertex->hCost = 0;
		vertex->fCost = 0;
		vertex->parent = nullptr;
	}
}

/// <summary>
/// Retrieves a vertex from the collection by its position.
/// </summary>
/// <param name="position">The position of the vertex to find.</param>
/// <returns>A shared pointer to the vertex, or nullptr if no matching vertex is found.</returns>
inline AStarVertex* AStar::GetVertex(AStarPoint position) {
	for (auto& vertex : this->vertices) {
		if (vertex->position == position) {
			return vertex.get();
		}
	}
	return nullptr; // vertex not found
}

/// <summary>
/// Gets the cost of the edge between two specified vertices.
/// </summary>
/// <param name="from">A pointer to the source vertex of the edge.</param>
/// <param name="to">A pointer to the destination vertex of the edge.</param>
/// <returns>The cost of the edge.</returns>
inline const int AStar::GetEdgeCost(AStarVertex* from, AStarVertex* to) { 
	for (auto& edge : from->edges) {
		if (*edge.to == to->position) {
			return edge.cost;
		}
	}
	throw std::runtime_error("Edge does not exist between the specified vertices.");
}

/// <summary>
/// Retrieves all neighboring vertices of a given vertex.
/// </summary>
/// <param name="vertex">The vertex whose neighbors should be retrieved.</param>
/// <returns>A vector containing shared pointers to all neighboring vertices.</returns>
inline std::vector<AStarVertex*> AStar::GetNeighbors(AStarVertex* vertex) { 
	std::vector<AStarVertex*> neighbors;
	for (auto& edge : vertex->edges) {
		neighbors.push_back(edge.to);
	}
	return neighbors;
}

/// <summary>
/// Reconstructs the path from the start to the goal by following parent pointers backward from the goal vertex.
/// </summary>
/// <returns>A vector of coordinate pairs representing the path from start to goal.</returns>
inline std::vector<AStarPoint>& AStar::ReconstructPath() {
	this->path.clear();
	AStarVertex* currentvertex = this->goal;
	while (currentvertex) {
		this->path.emplace_back(currentvertex->position);
		currentvertex = currentvertex->parent;
	}

	std::reverse(this->path.begin(), this->path.end());
	return this->path;
}
