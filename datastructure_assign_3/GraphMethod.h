#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"

const int INFINITE = 0x3f3f3f3f;

bool BFS(Graph* graph, char option, int vertex, string& path_result);
bool DFS(Graph* graph, char option, int vertex, string& path_result);
bool Centrality(Graph* graph, vector<pair<int, int>>& results);
bool Kruskal(Graph* graph, vector<pair<int, pair<int, int>>>& edgesResult, int& costResult);
bool Dijkstra(Graph* graph, char option, int vertex, vector<int>& distanceResult, vector<int>& previousResult);    //Dijkstra
bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex, vector<int>& distanceResult, vector<int>& previousResult); //Bellman - Ford
bool FLOYD(Graph* graph, char option, vector<vector<int>>& distanceMatrix);   //FLoyd

#endif