#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_Mat = new int* [m_Size];
	for (int i = 0; i < m_Size; i++)
	{
		m_Mat[i] = new int[m_Size];
		for (int j = 0; j < m_Size; j++)
		{
			m_Mat[i][j] = 0;
		}
	}
}

MatrixGraph::~MatrixGraph()
{
	for (int i = 0; i < m_Size; i++)
	{
		delete[] m_Mat[i];
	}
	delete[] m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	m->clear();

	for (int i = 0; i < m_Size; i++)
	{
		int weight = m_Mat[vertex][i];
		if (weight != 0)
		{
			m->insert(pair<int, int>(i, weight));
		}
	}

	for (int i = 0; i < m_Size; i++)
	{
		int weight = m_Mat[i][vertex];
		if (weight != 0)
		{
			m->insert(pair<int, int>(i, weight));
		}
	}
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	m->clear();

	for (int i = 0; i < m_Size; i++)
	{
		int weight = m_Mat[vertex][i];
		if (weight != 0)
		{
			m->insert(pair<int, int>(i, weight));
		}
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)
{
	if (from >= 0 && from < m_Size && to < m_Size)
	{
		m_Mat[from][to] = weight;
	}
}

bool MatrixGraph::printGraph(ofstream* fout)
{
	if (fout == nullptr || !fout->is_open())
	{
		return false;
	}

	for (int i = 0; i < m_Size; i++)
	{
		*fout << "\t[" << i << "]";
	}
	*fout << endl;

	for (int i = 0; i < m_Size; i++)
	{
		*fout << "[" << i << "]";
		for (int j = 0; j < m_Size; j++)
		{
			*fout << "\t" << m_Mat[i][j];
		}
		*fout << endl;
	}

	return true;
}