#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

struct Vertex
{
	char *block;
	int block_number;
	int edge;
	int first_index;
	int second_index;
	 
	void add(int block_number)
	{
		edge = 0;
		this->block_number = block_number;
		this->block = new char[block_number];
	}
	void fill(char block[])
	{
		for (int i = 0; i < block_number; i++)
		{
			this->block[i] = block[i];
		}
	}
};

void merge(Vertex first, Vertex second, Vertex couple[], int index, int vert_numb, int i, int k)
{	
	int *T;
	T = new int[first.block_number + second.block_number + 3];

	couple[index].add(first.block_number + second.block_number + 1);

	for (int j = 0; j <= first.block_number + second.block_number; j++)
	{
		if (j < second.block_number)
		couple[index].block[j] = second.block[j];

		else if (j == second.block_number)
		couple[index].block[j] = '#';

		else if (j > second.block_number)
		couple[index].block[j] = first.block[j - second.block_number - 1];
	}
	couple[index].first_index = k;
	couple[index].second_index = i;

	//Searching for the longest prefix-suffix:
	int j = 0; int l = 1; T[0] = 0;
	
	while (l < couple[index].block_number)
	{
		if (couple[index].block[l] == couple[index].block[j])
		{
			T[l] = j + 1;
			j++;
			l++;
			if (l >= couple[index].block_number)
			break;

			while (couple[index].block[l] != couple[index].block[j])
			{
				j = T[j - 1];
				if (j == 0)
				{
					while (couple[index].block[l] != couple[index].block[j])
					{
						T[l] = 0;
						l++;
						if (l >= couple[index].block_number)
						break;
					}
					if (l >= couple[index].block_number)
					break;
				}
			}
		}
		else
		{
			T[l] = 0;
			l++;
		}
	}
	
	if (T[l-1] == 0)
	couple[index].edge = INT_MAX;

	else
	couple[index].edge = second.block_number - T[l-1];

	delete[] T;
}

struct Dijkstr
{
	int edge;
	bool open;

	Dijkstr()
	{
		edge = INT_MAX;
		open = true;
	}
};

struct Graph
{
	Dijkstr *dijkstra_array;
	int **M;
	int vert_numb;

	void search_path(int vert_numb, int begin_index, int end_index, Vertex v);
	Graph(int vert_numb)
	{
		this->vert_numb = vert_numb;

		dijkstra_array = new Dijkstr[vert_numb]; 
										
		M = new int*[vert_numb];

		for (int i = 0; i < vert_numb; i++)
		{
			M[i] = new int[vert_numb];
		}

		for (int i = 0; i < vert_numb; i++)
		{
			for (int j = 0; j < vert_numb; j++)
			{
				if (i == j)
				M[i][j] = 0;

				else
				M[i][j] = INT_MAX;
			}
		}
	}
	~Graph()
	{
		delete[] dijkstra_array;

		for (int i = 0; i < vert_numb; i++)
		{
			delete[] M[i];
		}
		delete[] M;
	}
};


void add_graph(Graph &g, Vertex couple[])
{
//CREATING GRAPH EDGES:
	for (int i = 0; i < g.vert_numb*g.vert_numb - g.vert_numb; i++)
	{
		g.M[couple[i].first_index][couple[i].second_index] = couple[i].edge;
	}
}

void Graph::search_path(int vert_numb, int begin_index, int end_index, Vertex v)
{
	bool flag = false;
	int min = INT_MAX;
	
	for (int i = 0; i < vert_numb; i++)
	{
		dijkstra_array[i].edge = INT_MAX;
		dijkstra_array[i].open = true;
	}

	while (end_index != begin_index)
	{
		for (int i = 0; i < vert_numb; i++)
		{
			if (dijkstra_array[i].open == true)
			{
				if (M[begin_index][i] != INT_MAX)
				{
					if (dijkstra_array[begin_index].edge == INT_MAX)
					{
						dijkstra_array[i].edge = M[begin_index][i];
					}
					else
					{
						if (dijkstra_array[i].edge > M[begin_index][i] + dijkstra_array[begin_index].edge)
						{
							dijkstra_array[i].edge = M[begin_index][i] + dijkstra_array[begin_index].edge;
						}
					}
				}
			}
		}
		
		dijkstra_array[begin_index].open = false;
		
		//search for the smallest value:
		for (int i = 0; i < vert_numb; i++)
		{
			if (dijkstra_array[i].edge < min && dijkstra_array[i].open == true)
			{
				min = dijkstra_array[i].edge;
				begin_index = i;
			}
		}
		if (min == INT_MAX)
		{
			std::cout << '0';
			flag = true;
			break;
		}
		min = INT_MAX;
	}

	//VIEW THE FINAL SOLUTION:
	if (flag == false)
	std::cout << dijkstra_array[end_index].edge + v.block_number << '\n';
}

//---------------INPUT FUNCTIONS:
int exp(int base, int index)
{
	int x = 1;

	for (int i = 0; i < index; i++)
	{
		x *= base;
	}

	return x;
}

int input_int(char *&pointer)
{
	int x = 0;
	int k = 0;

	while (*pointer != ' ' && *pointer != '\n' && *pointer != '\0')
	{
		k++;
		pointer++;
	}
	for (int i = 0; i < k; i++)
	{
		pointer--;
		x = x + ((*pointer - 48) * exp(10, i));
	}
	pointer = pointer + k;

	return x;
}

char input_char(char *&pointer)
{
	char x;

	if (*pointer == '1' || *pointer == '2' || *pointer == '3' || *pointer == '4' || *pointer == '5' || *pointer == '6' || *pointer == '7' || *pointer == '8' || *pointer == '9' || *pointer == '0')
	{
		x = input_int(pointer);
		pointer++;
		return x;
	}
	else
	{
		x = *pointer;
		pointer++;
		return x;
	}
}

//MAIN:
int main()
{
	int vert_numb; int block_numb = 0; char array_in[10000]; char array_out[10000]; char *pointer;
	int begin_index; int end_index;
	Vertex *v; Vertex *couple;
	FILE *file;

	//FILE CONTAINS THE EXAMPLE OF BLOCKS: 
	file = fopen("blocks.txt", "r");

	//ENTER A NUMBER OF VERTEX:
	std::cin >> vert_numb;
	std::cin.ignore();
	Graph g(vert_numb);
	v = new Vertex[vert_numb];
	couple = new Vertex[vert_numb*vert_numb];

	//ENTER THE BLOCKS:
	int k = 0;
	for (int i = 0; i < vert_numb; i++)
	{
		fgets(array_in, 10000, file);
		pointer = &array_in[0];
		int k = 0;
		while (*pointer != '\0')
		{
			if (*pointer != '\n'&& *pointer != '\0')
			{
				array_out[k] = input_char(pointer);
				block_numb++;
				k++;
			}
			else
			{
				pointer++;
			}
		}

		if (array_out[0] == 'p') // MARK THE START INDEX
			begin_index = i;
		if (array_out[k - 1] == 'z') // MARK THE END INDEX
			end_index = i;

		v[i].add(block_numb);
		v[i].fill(array_out);
		block_numb = 0;
	}

	//VERTEX PAIRING:
	k = 0;
	for (int j = 0; j < vert_numb; j++)
	{
		for (int i = 0; i < vert_numb; i++)
		{
			if (i != j)
			{
				merge(v[j], v[i], couple, k, vert_numb, i, j);
				k++;
			}
		}
	}

	//CREATING A GRAPH:
	add_graph(g, couple);
	
	//THE SHORTEST PATH SEARCHING AND VIEW THE FINAL SOLUTION:
	g.search_path(vert_numb, begin_index, end_index, v[begin_index]); 

	delete[] couple;
	delete[] v;

    return 0;
}

