#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <string>
#include <queue>

using namespace std;

bool BFS(Graph* graph, char option, int vertex, string& path_result)
{
	int size = graph->getSize();
	if (vertex < 0 || vertex >= size)
	{
		return false;
	}

	int* visited = new int[size];
	fill(visited, visited + size, 0);

	queue<int> q;
	path_result = "";

	q.push(vertex);
	visited[vertex] = 1;
	path_result += to_string(vertex);

	while (!q.empty())
	{
		int current = q.front();
		q.pop();

		map<int, int> adjacentEdges;

		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(current, &adjacentEdges);
		}
		else if (option == 'X')
		{
			graph->getAdjacentEdges(current, &adjacentEdges);
		}

		for (auto const& pair : adjacentEdges)
		{
			if (visited[pair.first] == 0)
			{
				visited[pair.first] = 1;
				q.push(pair.first);

				path_result += " -> ";
				path_result += to_string(pair.first);
			}
		}
	}

	delete[] visited;
	return true;
}

bool DFS(Graph* graph, char option, int vertex, string& path_result)
{
	int size = graph->getSize();
	if (vertex < 0 || vertex >= size)
	{
		return false;
	}

	int* visited = new int[size];
	fill(visited, visited + size, 0);

	stack<int> s;
	path_result = "";

	s.push(vertex);

	while (!s.empty())
	{
		int current = s.top();
		s.pop();

		if (visited[current] == 1)
		{
			continue;
		}

		visited[current] = 1;
		if (path_result.empty())
		{
			path_result += to_string(current);
		}
		else
		{
			path_result += " -> " + to_string(current);
		}

		map<int, int> adjacentEdges;

		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(current, &adjacentEdges);
		}
		else if (option == 'X')
		{
			graph->getAdjacentEdges(current, &adjacentEdges);
		}

		vector<int> neighbors;
		for (auto const& pair : adjacentEdges)
		{
			neighbors.push_back(pair.first);
		}
		for (int i = neighbors.size() - 1; i >= 0; i--)
		{
			if (visited[neighbors[i]] == 0)
			{
				s.push(neighbors[i]);
			}
		}
	}

	delete[] visited;
	return true;
}

bool Kruskal(Graph* graph, vector<pair<int, pair<int, int>>>& edgesResult, int& costResult)
{
	int size = graph->getSize();

	edgesResult.clear();
	costResult = 0;

	if (size <= 1)
	{
		return false;
	}

	vector<pair<int, pair<int, int>>> edges;

	for (int u = 0; u < size; u++)
	{
		map<int, int> adjacentEdges;
		graph->getAdjacentEdges(u, &adjacentEdges);

		for (auto const& pair : adjacentEdges)
		{
			int v = pair.first;
			int weight = pair.second;

			if (u < v)
			{
				edges.push_back(make_pair(weight, make_pair(u, v)));
			}
		}
	}

	sort(edges.begin(), edges.end());

	vector<int> parent(size); // Number of parent's node of each node
	vector<int> rank(size, 0); // Height of tree

	// Initialize parents of all node to self
	for (int i = 0; i < size; i++)
	{
		parent[i] = i;
	}

	// Constitute of MST
	for (const auto& edge : edges)
	{
		int weight = edge.first;
		int u = edge.second.first;
		int v = edge.second.second;

		// Find root of u
		int rootU = u;

		while (parent[rootU] != rootU)
		{
			rootU = parent[rootU];
		}

		// Administrate path
		int tempU = u;
		while (tempU != rootU)
		{
			int next = parent[tempU];
			parent[tempU] = rootU;
			tempU = next;
		}

		// Find root of v
		int rootV = v;
		while (parent[rootV] != rootV)
		{
			rootV = parent[rootV];
		}

		// Administrate path
		int tempV = v;
		while (tempV != rootV)
		{
			int next = parent[tempV];
			parent[tempV] = rootV;
			tempV = next;
		}

		// Union if roots are different
		if (rootU != rootV)
		{
			if (rank[rootU] < rank[rootV]) // Root of V's rank is higher than root of U's rank
			{
				parent[rootU] = rootV;
			}
			else if (rank[rootU] > rank[rootV]) // Root of U's rank is higher than root of V's rank
			{
				parent[rootV] = rootU;
			}
			else // Same
			{
				parent[rootV] = rootU;
				rank[rootU]++;
			}
			edgesResult.push_back(make_pair(weight, make_pair(u, v)));

			costResult += weight;
		}
	}

	// MST must have n-1 edges
	if (edgesResult.size() != size - 1)
	{
		return false;
	}

	return true;
}

bool Dijkstra(Graph* graph, char option, int vertex, vector<int>& distanceResult, vector<int>& previousResult)
{
	int size = graph->getSize();

	if (vertex < 0 || vertex >= size)
	{
		return false;
	}

	distanceResult.resize(size);
	previousResult.resize(size);

	for (int i = 0; i < size; i++)
	{
		distanceResult[i] = INFINITE;
		previousResult[i] = -1;
	}

	priority_queue<pair<int, int>> pq;

	distanceResult[vertex] = 0;
	previousResult[vertex] = -1;
	pq.push(make_pair(0, vertex));

	while (!pq.empty())
	{
		int distanceN = pq.top().first;
		int u = pq.top().second;
		pq.pop();

		int distanceCurrent = -distanceN;

		if (distanceCurrent > distanceResult[u])
		{
			continue;
		}

		map<int, int> adjacentEdges;

		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(u, &adjacentEdges);
		}
		else if (option == 'X')
		{
			graph->getAdjacentEdges(u, &adjacentEdges);
		}
		else
		{
			return false;
		}

		for (auto const& pair : adjacentEdges)
		{
			int v = pair.first;
			int weight = pair.second;

			if (weight < 0)
			{
				return false;
			}
			
			if (distanceResult[u] != INFINITE && distanceResult[u] + weight < distanceResult[v])
			{
				distanceResult[v] = distanceResult[u] + weight;
				previousResult[v] = u;

				pq.push(make_pair(-distanceResult[v], v));
			}
		}
	}
	return true;
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex, vector<int>& distanceResult, vector<int>& previousResult)
{
	int size = graph->getSize();

	if (s_vertex < 0 || s_vertex >= size)
	{
		return false;
	}

	distanceResult.resize(size);
	previousResult.resize(size);

	for (int i = 0; i < size; i++)
	{
		distanceResult[i] = INFINITE;
		previousResult[i] = -1;
	}

	distanceResult[s_vertex] = 0;

	vector<pair<int, pair<int, int>>> edges;
	for (int u = 0; u < size; u++)
	{
		map<int, int> adjacentEdges;

		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(u, &adjacentEdges);
		}
		else if (option == 'X')
		{
			graph->getAdjacentEdges(u, &adjacentEdges);
		}
		else
		{
			return false;
		}

		for (auto const& pair : adjacentEdges)
		{
			int v = pair.first;
			int weight = pair.second;
			edges.push_back(make_pair(u, make_pair(v, weight)));
		}
	}

	for (int i = 0; i < size - 1; i++)
	{
		bool change = false;
		for (const auto& edge : edges)
		{
			int u = edge.first;
			int v = edge.second.first;
			int weight = edge.second.second;

			if (distanceResult[u] != INFINITE)
			{
				if (distanceResult[u] + weight < distanceResult[v])
				{
					distanceResult[v] = distanceResult[u] + weight;
					previousResult[v] = u;
					change = true;
				}
			}
		}
		if (!change)
		{
			break;
		}
	}
	for (const auto& edge : edges)
	{
		int u = edge.first;
		int v = edge.second.first;
		int weight = edge.second.second;

		if (distanceResult[u] != INFINITE)
		{
			if (distanceResult[u] + weight < distanceResult[v])
			{
				return false;
			}
		}
	}

	return true;
}

bool FLOYD(Graph* graph, char option, vector<vector<int>>& distanceMatrix)
{
	int size = graph->getSize();

	distanceMatrix.resize(size);

	for (int i = 0; i < size; i++)
	{
		distanceMatrix[i].resize(size);

		for (int j = 0; j < size; j++)
		{
			distanceMatrix[i][j] = INFINITE;
		}
		distanceMatrix[i][i] = 0;

		map<int, int> adjacentEdges;

		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(i, &adjacentEdges);
		}
		else if (option == 'X')
		{
			graph->getAdjacentEdges(i, &adjacentEdges);
		}
		else
		{
			return false;
		}

		for (auto const& pair : adjacentEdges)
		{
			int j = pair.first;
			int weight = pair.second;
			distanceMatrix[i][j] = weight;
		}
	}

	for (int k = 0; k < size; k++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (distanceMatrix[i][k] != INFINITE && distanceMatrix[k][j] != INFINITE)
				{
					if (distanceMatrix[i][k] + distanceMatrix[k][j] < distanceMatrix[i][j])
					{
						distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
					}
				}
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (distanceMatrix[i][i] < 0)
		{
			return false;
		}
	}
	return true;
}

bool Centrality(Graph* graph, vector<pair<int, int>>& results)
{
	if (!graph)
	{
		return false;
	}
	
	int size = graph->getSize();

	vector<vector<int>> distanceMatrix;

	if (!FLOYD(graph, 'X', distanceMatrix))
	{
		return false;
	}
	
	results.clear();

	for (int v = 0; v < size; v++)
	{
		int sumDistances = 0;
		bool reach = true;

		for (int u = 0; u < size; u++)
		{
			if (v == u)
			{
				continue;
			}

			int distance = distanceMatrix[v][u];

			if (distance == INFINITE)
			{
				reach = false;
				break;
			}

			sumDistances += distance;
		}

		if (!reach)
		{
			results.push_back(make_pair(v, -1));
		}
		else
		{
			results.push_back(make_pair(v, sumDistances));
		}
	}
	return true;
}