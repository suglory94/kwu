#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_List = new map<int, int>[m_Size];
	kw_graph = new vector<int>[m_Size];
}

ListGraph::~ListGraph()
{
	delete[] m_List;
	delete[] kw_graph;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)	 //Definition of getAdjacentEdges(No Direction == Undirected)
{
	*m = m_List[vertex];

	for (int i = 0; i < m_Size; i++)
	{
		auto iterator = m_List[i].find(vertex);
		if (iterator != m_List[i].end())
		{
			int neighbor = i;
			int weight = iterator->second;

			m->insert(pair<int, int>(neighbor, weight));
		}
	}
}

void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)	//Definition of getAdjacentEdges(Directed graph)
{
	*m = m_List[vertex];
}

void ListGraph::insertEdge(int from, int to, int weight) //Definition of insertEdge
{
	m_List[from].insert(pair<int, int>(to, weight));
}

bool ListGraph::printGraph(ofstream* fout)	//Definition of print Graph
{
	if (fout == nullptr || !fout->is_open())
	{
		return false;
	}

	for (int i = 0; i < m_Size; i++)
	{
		*fout << "[" << i << "] ->";

		bool first = true;
		for (auto const& listPair : m_List[i])
		{
			if (!first)
			{
				*fout << " ->";
			}
			*fout << " (" << listPair.first << "," << listPair.second << ")";
			first = false;
		}
		*fout << "\n";
	}
	return true;
}