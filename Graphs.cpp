#include <iostream>

#define ArrayCount(Array) (sizeof(Array)/sizeof((Array)[0]))
#define Assert(Expression) if(!(Expression)) { *(int *)0 = 0; } 

//
// NOTE(georgy): Graph, BFS, DFS
//

struct graph_vertex_neighbour
{
	uint32_t Index;
	graph_vertex_neighbour* Next;
};
struct graph_vertex
{
	uint32_t Index;
	graph_vertex_neighbour* Neighbours;

	// NOTE(georgy): These are needed for BFS
	bool Visited;
	uint32_t D;
	uint32_t ParentIndex;

	// NOTE(georgy): This is needed for DFS
	uint32_t F;
};
struct graph
{
	uint32_t VertexCount;
	graph_vertex Vertices[16];

	// NOTE(georgy): This is needed for DFS
	uint32_t Time;
};

static void
AddVertex(graph *Graph)
{
	Assert(Graph->VertexCount < ArrayCount(Graph->Vertices));

	Graph->Vertices[Graph->VertexCount].Index = Graph->VertexCount + 1;
	++Graph->VertexCount;
}

static void 
AddNeighbourToVertex(graph* Graph, uint32_t VertexIndex, uint32_t NeighbourIndex)
{
	graph_vertex_neighbour* Neighbour = (graph_vertex_neighbour*)malloc(sizeof(graph_vertex_neighbour));
	Neighbour->Index = NeighbourIndex;

	Neighbour->Next = Graph->Vertices[VertexIndex - 1].Neighbours;
	Graph->Vertices[VertexIndex - 1].Neighbours = Neighbour;
}


struct queue
{
	uint32_t Head, Tail;
	graph_vertex* Elements[16];
};

static bool
IsEmpty(queue* Queue)
{
	bool Result = (Queue->Head == Queue->Tail);
	return(Result);
}

static void
Enqueue(queue* Queue, graph_vertex* Vertex)
{
	uint32_t NewTail = (Queue->Tail + 1) % ArrayCount(Queue->Elements);
	if (NewTail != Queue->Head)
	{
		Queue->Elements[Queue->Tail] = Vertex;
		Queue->Tail = NewTail;
	}
	else
	{
		Assert(!"Queue overflow!");
	}
}

static graph_vertex *
Dequeue(queue* Queue)
{
	graph_vertex* Result = 0;

	if (!IsEmpty(Queue))
	{
		Result = Queue->Elements[Queue->Head];
		Queue->Head = (Queue->Head + 1) % ArrayCount(Queue->Elements);
	}
	else
	{
		Assert(!"Queue underflow!");
	}

	return(Result);
}


static void
BFS(graph* Graph, uint32_t StartVertexIndex)
{
	graph_vertex* StartVertex = &Graph->Vertices[StartVertexIndex - 1];

	for (uint32_t I = 0; I < Graph->VertexCount; I++)
	{
		graph_vertex* Vertex = Graph->Vertices + I;

		Vertex->Visited = false;
		Vertex->D = UINT32_MAX;
		Vertex->ParentIndex = 0;
	}

	StartVertex->Visited = true;
	StartVertex->D = 0;
	StartVertex->ParentIndex = 0;

	queue Queue = {};
	Enqueue(&Queue, StartVertex);
	while (!IsEmpty(&Queue))
	{
		graph_vertex* Vertex = Dequeue(&Queue);

		graph_vertex_neighbour* Neighbour = Vertex->Neighbours;
		while (Neighbour)
		{
			graph_vertex* NeighbourVertex = &Graph->Vertices[Neighbour->Index - 1];
			if (!NeighbourVertex->Visited)
			{
				NeighbourVertex->Visited = true;
				NeighbourVertex->D = Vertex->D + 1;
				NeighbourVertex->ParentIndex = Vertex->Index;
				Enqueue(&Queue, NeighbourVertex);
			}

			Neighbour = Neighbour->Next;
		}
	}
}

static void
PrintPath(graph* Graph, uint32_t StartVertexIndex, uint32_t EndVertexIndex)
{
	graph_vertex* StartVertex = &Graph->Vertices[StartVertexIndex - 1];
	graph_vertex* EndVertex = &Graph->Vertices[EndVertexIndex - 1];

	if (StartVertex == EndVertex)
	{
		std::cout << StartVertexIndex << std::endl;
	}
	else if (EndVertex->ParentIndex == 0)
	{
		std::cout << "No path from " << StartVertexIndex << " to " << EndVertexIndex << " exists." << std::endl;
	}
	else
	{
		PrintPath(Graph, StartVertexIndex, EndVertex->ParentIndex);
		std::cout << EndVertexIndex << std::endl;
	}
}

static void
DFSVisit(graph* Graph, graph_vertex* Vertex)
{
	++Graph->Time;
	Vertex->D = Graph->Time;
	Vertex->Visited = true;

	graph_vertex_neighbour* Neighbour = Vertex->Neighbours;
	while (Neighbour)
	{
		graph_vertex* NeighbourVertex = &Graph->Vertices[Neighbour->Index - 1];
		if (!NeighbourVertex->Visited)
		{
			NeighbourVertex->ParentIndex = Vertex->Index;
			DFSVisit(Graph, NeighbourVertex);
		}

		Neighbour = Neighbour->Next;
	}

	++Graph->Time;
	Vertex->F = Graph->Time;
}

static void
DFS(graph* Graph)
{
	Graph->Time = 0;
	for (int I = 0; I < Graph->VertexCount; I++)
	{
		graph_vertex* Vertex = Graph->Vertices + I;

		Vertex->Visited = false;
		Vertex->ParentIndex = 0;
	}

	for (int I = 0; I < Graph->VertexCount; I++)
	{
		graph_vertex* Vertex = Graph->Vertices + I;
		if (!Vertex->Visited)
		{
			DFSVisit(Graph, Vertex);
		}
	}
}

int main(void)
{
	//
	// NOTE(georgy): Graph, BFS, DFS
	//
	graph Graph0 = {};
	for (int I = 0; I < 8; I++)
	{
		AddVertex(&Graph0);
	}

	AddNeighbourToVertex(&Graph0, 1, 2);
	AddNeighbourToVertex(&Graph0, 1, 3);
	AddNeighbourToVertex(&Graph0, 2, 4);
	AddNeighbourToVertex(&Graph0, 3, 5);
	AddNeighbourToVertex(&Graph0, 3, 6);
	AddNeighbourToVertex(&Graph0, 5, 6);
	AddNeighbourToVertex(&Graph0, 5, 7);
	AddNeighbourToVertex(&Graph0, 6, 7);
	AddNeighbourToVertex(&Graph0, 6, 8);
	AddNeighbourToVertex(&Graph0, 7, 8);

	BFS(&Graph0, 1);
	PrintPath(&Graph0, 1, 8);
	std::cout << std::endl;


	graph Graph1 = {};
	for (int I = 0; I < 6; I++)
	{
		AddVertex(&Graph1);
	}

	AddNeighbourToVertex(&Graph1, 1, 4);
	AddNeighbourToVertex(&Graph1, 1, 2);
	AddNeighbourToVertex(&Graph1, 2, 5);
	AddNeighbourToVertex(&Graph1, 3, 5);
	AddNeighbourToVertex(&Graph1, 3, 6);
	AddNeighbourToVertex(&Graph1, 4, 2);
	AddNeighbourToVertex(&Graph1, 5, 4);
	AddNeighbourToVertex(&Graph1, 6, 6);

	DFS(&Graph1);

	return(0);
}