#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

Manager::Manager()
{
	graph = nullptr;
	fout.open("log.txt", ios::out);
	load = 0;	//Anything is not loaded
}

Manager::~Manager()
{
	if (load)	//if graph is loaded, delete graph
		delete graph;
	if (fout.is_open())	//if fout is opened, close file
		fout.close();	//close log.txt File
}

void Manager::run(const char* command_txt) {
	ifstream fin;	//Command File Input File Stream
	fin.open(command_txt, ios_base::in);//File open with read mode

	if (!fin) { //If command File cannot be read, Print error
		fout << "command file open error" << endl;
		return;	//Return
	}

	string line;
	while (getline(fin, line))
	{
		stringstream ss(line);
		string command;
		vector<string> factors;

		ss >> command;

		string factor;
		while (ss >> factor)
		{
			factors.push_back(factor);
		}

		if (command == "LOAD")
		{
			if (factors.size() == 1)
			{
				LOAD(factors[0].c_str());
			}
			else
			{
				printErrorCode(100);
			}
		}
		else if (command == "PRINT")
		{
			if (factors.empty())
			{
				PRINT();
			}
			else
			{
				printErrorCode(200);
			}
		}
		else if (command == "BFS")
		{
			if (factors.size() == 2)
			{
				char option = factors[0][0];
				int vertex = stoi(factors[1]);
				mBFS(option, vertex);
			}
			else
			{
				printErrorCode(300);
			}
		}
		else if (command == "DFS")
		{
			if (factors.size() == 2)
			{
				char option = factors[0][0];
				int vertex = stoi(factors[1]);
				mDFS(option, vertex);
			}
			else
			{
				printErrorCode(400);
			}
		}
		else if (command == "DIJKSTRA")
		{
			if (factors.size() == 2)
			{
				char option = factors[0][0];
				int vertex = stoi(factors[1]);
				mDIJKSTRA(option, vertex);
			}
			else
			{
				printErrorCode(600);
			}
		}
		else if (command == "KRUSKAL")
		{
			if (factors.empty())
			{
				mKRUSKAL();
			}
			else
			{
				printErrorCode(500);
			}
		}
		else if (command == "BELLMANFORD")
		{
			if (factors.size() == 3)
			{
				char option = factors[0][0];
				int s_vertex = stoi(factors[1]);
				int e_vertex = stoi(factors[2]);
				mBELLMANFORD(option, s_vertex, e_vertex);
			}
			else
			{
				printErrorCode(700);
			}
		}
		else if (command == "FLOYD")
		{
			if (factors.size() == 1)
			{
				char option = factors[0][0];
				mFLOYD(option);
			}
			else
			{
				printErrorCode(300);
			}
		}
		else if (command == "CENTRALITY")
		{
			if (factors.empty())
			{
				mCentrality();
			}
			else
			{
				printErrorCode(900);
			}
		}
		else if (command == "EXIT")
		{
			printSuccessCode("EXIT");
			fin.close();
			return;
		}
		else
		{
			fout << "wrong command : " << command << endl;
		}
	}

	fin.close();
	return;
}

bool Manager::LOAD(const char* filename)
{
	ifstream loadFile;
	loadFile.open(filename, ios_base::in);

	if (!loadFile) {
		printErrorCode(100);
		return false;
	}

	char graphType;
	int graphSize;

	string line;
	getline(loadFile, line);
	graphType = line[0];
	
	getline(loadFile, line);
	graphSize = stoi(line);

	if (graphType != 'L' && graphType != 'M')
	{
		if (load)
		{
			delete graph;
			graph = nullptr;
			load = 0;
		}
		printErrorCode(100);
		loadFile.close();
		return false;
	}

	if (load)
	{
		delete graph;
		graph = nullptr;
	}

	if (graphType == 'L')
	{
		graph = new ListGraph(true, graphSize);

		int currentVertex = -1;
		while (getline(loadFile, line))
		{
			stringstream ss(line);
			vector<int> values;
			int value;

			while (ss >> value)
			{
				values.push_back(value);
			}

			if (values.empty())
			{
				continue;
			}

			if (values.size() == 1)
			{
				currentVertex = values[0];

				if (currentVertex < 0 || currentVertex >= graphSize)
				{
					printErrorCode(100);
					delete graph;
					load = 0;
					loadFile.close();
					return false;
				}
			}
			else if (values.size() == 2)
			{
				int to = values[0];
				int weight = values[1];

				if (to < 0 || to >= graphSize)
				{
					printErrorCode(100);
					delete graph;
					load = 0;
					loadFile.close();
					return false;
				}

				graph->insertEdge(currentVertex, to, weight);
			}
			else
			{
				printErrorCode(100);
				delete graph;
				load = 0;
				loadFile.close();
				return false;
			}
		}
	}
	else if (graphType == 'M')
	{
		graph = new MatrixGraph(false, graphSize);

		int currentVertex = 0;
		while (getline(loadFile, line))
		{
			stringstream ss(line);
			int weight;
			int j = 0;

			while (ss >> weight)
			{
				if (j >= graphSize)
				{
					printErrorCode(100);
					delete graph;
					load = 0;
					loadFile.close();
					return false;
				}

				if (weight != 0)
				{
					graph->insertEdge(currentVertex, j, weight);
				}
				j++;
			}

			if (j != graphSize)
			{
				printErrorCode(100);
				delete graph;
				load = 0;
				loadFile.close();
				return false;
			}
			currentVertex++;
		}
	}

	load = 1;
	printSuccessCode("LOAD");

	loadFile.close();
	return true;
}

bool Manager::PRINT()
{
	if (!load)
	{
		printErrorCode(200);
		return false;
	}

	fout << "========PRINT========" << endl;
	graph->printGraph(&fout);
	fout << "=====================" << endl << endl;
	return true;
}

bool Manager::mBFS(char option, int vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(300);
		return false;
	}

	string path_result;
	if (BFS(graph, option, vertex, path_result))
	{
		string graph_type = (option == 'O') ? "Directed Graph BFS" : "Undirected Graph BFS";
		fout << "========BFS=========" << endl;
		fout << graph_type << endl;
		fout << "Start: " << vertex << endl;
		fout << path_result << endl;
		fout << "====================" << endl << endl;
		return true;
	}
	else
	{
		printErrorCode(300);
		return false;
	}
}

bool Manager::mDFS(char option, int vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(400);
		return false;
	}

	string path_result;
	if (DFS(graph, option, vertex, path_result))
	{
		string graph_type = (option == 'O') ? "Directed Graph DFS" : "Undirected Graph DFS";
		fout << "========DFS=========" << endl;
		fout << graph_type << endl;
		fout << "Start: " << vertex << endl;
		fout << path_result << endl;
		fout << "====================" << endl << endl;
		return true;
	}
	else
	{
		printErrorCode(400);
		return false;
	}
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	if (!load || graph == nullptr || vertex < 0)
	{
		printErrorCode(600);
		return false;
	}

	vector<int> distance;
	vector<int> previous;

	if (Dijkstra(graph, option, vertex, distance, previous))
	{
		fout << "========DIJKSTRA========" << endl;
		string graph_type = (option == 'O') ? "Directed Graph Dijkstra" : "Undirected Graph Dijkstra";
		fout << graph_type << endl;
		fout << "Start: " << vertex << endl;
		int size = graph->getSize();
		for (int i = 0; i < size; i++)
		{
			fout << "[" << i << "] ";
			if (distance[i] == INFINITE)
			{
				fout << "x" << endl;
			}
			else
			{
				vector<int> path;
				int current = i;

				while (current != -1)
				{
					path.push_back(current);
					if (current == vertex)
					{
						break;
					}
					current = previous[current];
				}
				for (int j = path.size() - 1; j >= 0; j--)
				{
					fout << path[j];
					if (j > 0)
					{
						fout << " -> ";
					}
				}
				fout << " (" << distance[i] << ")" << endl;
			}
		}
		fout << "=========================" << endl << endl;
		return true;
	}
	else
	{
		printErrorCode(600);
		return false;
	}
}

bool Manager::mKRUSKAL()
{
	if (!load || graph == nullptr)
	{
		printErrorCode(500);
		return false;
	}

	vector<pair<int, pair<int, int>>> edges;
	int cost = 0;

	if (Kruskal(graph, edges, cost))
	{
		fout << "========KRUSKAL========" << endl;
		int size = graph->getSize();
		map<int, vector<pair<int, int>>> adjacents;

		for (const auto& edge : edges)
		{
			int weight = edge.first;
			int u = edge.second.first;
			int v = edge.second.second;

			adjacents[u].push_back(make_pair(v, weight));
			adjacents[v].push_back(make_pair(u, weight));
		}

		for (int i = 0; i < size; i++)
		{
			fout << "[" << i << "]";
			auto& neighbors = adjacents[i];
			sort(neighbors.begin(), neighbors.end());

			for (const auto& neighbor : neighbors)
			{
				fout << " " << neighbor.first << "(" << neighbor.second << ")";
			}
			fout << endl;
		}
		fout << "Cost: " << cost << endl;
		fout << "=======================" << endl << endl;

		return true;
	}
	else
	{
		printErrorCode(500);
		return false;
	}
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(700);
		return false;
	}

	vector<int> distance;
	vector<int> previous;

	if (Bellmanford(graph, option, s_vertex, e_vertex, distance, previous))
	{
		fout << "========BELLMANFORD========" << endl;
		string graph_type = (option == 'O') ? "Directed Graph Bellman-Ford" : "Undirected Graph Bellman-Ford";
		fout << graph_type << endl;

		if (distance[e_vertex] == INFINITE)
		{
			fout << "x" << endl;
			fout << "Cost: x" << endl;
		}
		else
		{
			vector<int> path;
			int current = e_vertex;

			while (current != -1)
			{
				path.push_back(current);
				if (current == s_vertex)
				{
					break;
				}
				current = previous[current];
			}

			for (int j = path.size() - 1; j >= 0; j--)
			{
				fout << path[j];
				if (j > 0)
				{
					fout << " -> ";
				}
			}
			fout << endl;
			fout << "Cost: " << distance[e_vertex] << endl;
		}
		fout << "===========================" << endl << endl;
		return true;
	}
	else
	{
		printErrorCode(700);
		return false;
	}
}

bool Manager::mFLOYD(char option)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(800);
		return false;
	}

	vector<vector<int>> distanceMatrix;
	int size = graph->getSize();

	if (FLOYD(graph, option, distanceMatrix))
	{
		fout << "========FLOYD========" << endl;
		string graph_type = (option == 'O') ? "Directed Graph Floyd" : "Undirected Graph Floyd";
		fout << graph_type << endl;

		for (int i = 0; i < size; i++)
		{
			fout << "\t[" << i << "]";
		}
		fout << endl;

		for (int i = 0; i < size; i++)
		{
			fout << "[" << i << "]";
			for (int j = 0; j < size; j++)
			{
				fout << "\t";
				if (distanceMatrix[i][j] == INFINITE)
				{
					fout << "x";
				}
				else
				{
					fout << distanceMatrix[i][j];
				}
			}
			fout << endl;
		}
		fout << "=====================" << endl << endl;
		return true;
	}
	else
	{
		printErrorCode(800);
		return false;
	}
}

bool Manager::mCentrality() 
{
	if (!load || graph == nullptr)
	{
		printErrorCode(900);
		return false;
	}

	vector<pair<int, int>> results;
	int size = graph->getSize();
	
	if (Centrality(graph, results))
	{
		fout << "========CENTRALITY========" << endl;
		const int MAX_INT = 0x3f3f3f3f;
		int minDenominator = MAX_INT;

		for (const auto& result : results)
		{
			int currentDenominator = result.second;
			if (currentDenominator > 0)
			{
				if (currentDenominator < minDenominator)
				{
					minDenominator = currentDenominator;
				}
			}
		}
		for (const auto& result : results)
		{
			int v = result.first;
			int sum = result.second;

			fout << "[" << v << "] ";
			if (sum == -1)
			{
				fout << "x" << endl;
			}
			else
			{
				fout << size - 1 << "/" << sum;
				if (sum > 0 && sum == minDenominator && minDenominator != MAX_INT)
				{
					fout << " <- Most Central";
				}
				fout << endl;
			}
		}
		fout << "=======================" << endl << endl;
		return true;
	}
	else
	{
		printErrorCode(900);
		return false;
	}
}

void Manager::printErrorCode(int n)
{
	fout << "========ERROR=======" << endl;
	fout << n << endl;
	fout << "====================" << endl << endl;
}

void Manager::printSuccessCode(string success)
{
	fout << "========" << success << "=======" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl << endl;
}