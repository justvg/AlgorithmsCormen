#include <iostream>

#define ArrayCount(Array) (sizeof(Array)/sizeof((Array)[0]))
#define Assert(Expression) if(!(Expression)) { *(int *)0 = 0; } 


//
// NOTE(georgy): Stack, Queue, Linked List
//
struct stack
{
	uint32_t Top;
	int Elements[64];
};

static bool
IsEmpty(stack* Stack)
{
	bool Result = (Stack->Top == 0);
	return(Result);
}

static void
Push(stack* Stack, int Value)
{
	if(Stack->Top < ArrayCount(Stack->Elements))
	{
		Stack->Elements[Stack->Top++] = Value;
	}
	else
	{
		Assert(!"Stack overflow!");
	}
}

static int
Pop(stack* Stack)
{
	int Result = 0;

	if(!IsEmpty(Stack))
	{
		Result = Stack->Elements[--Stack->Top];
	}
	else
	{
		Assert(!"Stack underflow!");
	}

	return(Result);
}

static void
Print(stack* Stack)
{
	for (int I = Stack->Top - 1; I >= 0; I--)
	{
		std::cout << Stack->Elements[I] << ' ';
	}
	std::cout << std::endl;
}


struct queue
{
	uint32_t Head, Tail;
	int Elements[64];
};

static bool
IsEmpty(queue* Queue)
{
	bool Result = (Queue->Head == Queue->Tail);
	return(Result);
}

static void
Enqueue(queue* Queue, int Value)
{
	uint32_t NewTail = (Queue->Tail + 1) % ArrayCount(Queue->Elements);
	if (NewTail != Queue->Head)
	{
		Queue->Elements[Queue->Tail] = Value;
		Queue->Tail = NewTail;
	}
	else
	{
		Assert(!"Queue overflow!");
	}
}

static int
Dequeue(queue* Queue)
{
	int Result = 0;

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
Print(queue* Queue)
{
	if (Queue->Tail >= Queue->Head)
	{
		for (int I = Queue->Head; I < Queue->Tail; I++)
		{
			std::cout << Queue->Elements[I] << ' ';
		}
	}
	else
	{
		for (int I = Queue->Head; I < ArrayCount(Queue->Elements); I++)
		{
			std::cout << Queue->Elements[I] << ' ';
		}
		for (int I = 0; I < Queue->Tail; I++)
		{
			std::cout << Queue->Elements[I] << ' ';
		}
	}
	std::cout << std::endl;
}


struct node
{
	int Value;

	node* Prev;
	node* Next;
};

static node*
ListSearch(node* Sentinel, int Value)
{
	node* Node = Sentinel->Next;
	while ((Node != Sentinel) && (Node->Value != Value))
	{
		Node = Node->Next;
	}

	if(Node == Sentinel)
	{
		Node = 0;
	}
	
	return(Node);
}

static void
ListInsert(node* Sentinel, node* NewNode)
{
	NewNode->Next = Sentinel->Next;
	NewNode->Prev = Sentinel;

	Sentinel->Next->Prev = NewNode;
	Sentinel->Next = NewNode;
}

static void
ListDelete(node* Sentinel, node* DeleteNode)
{
	DeleteNode->Next->Prev = DeleteNode->Prev;
	DeleteNode->Prev->Next = DeleteNode->Next;
}

static void
Print(node* Sentinel)
{
	for(node* Node = Sentinel->Next;
		Node != Sentinel;
		Node = Node->Next)
	{
		std::cout << Node->Value << ' ';
	}
	std::cout << std::endl;
}


//
// NOTE(georgy): Binary Tree, Tree
//
struct binary_tree
{
	int Key;

	binary_tree* Parent;
	binary_tree* Left;
	binary_tree* Right;
};

static void
Print(binary_tree* Tree)
{
#if 0
#if 1
	// NOTE(georgy): Recursive
	if(Tree)
	{
		std::cout << Tree->Key << ' ';
		Print(Tree->Left);
		Print(Tree->Right);
	}
#else
	// NOTE(georgy): Non-recursive with stack
	uint32_t Count = 0;
	binary_tree* Stack[64];
	
	Stack[Count++] = Tree;
	while(Count)
	{
		Tree = Stack[--Count];
		std::cout << Tree->Key << ' ';
		
		if(Tree->Right)
		{
			Stack[Count++] = Tree->Right;
		}

		if(Tree->Left)
		{
			Stack[Count++] = Tree->Left;
		}
	}
#endif
#else
	// NOTE(georgy): Non-recursive with constant extra space
	binary_tree* Current = Tree;
	binary_tree* Old = 0;
	while (Current)
	{
		if (Old && Old->Parent == Current)
		{
			if (Current->Right && (Old != Current->Right))
			{
				Old = Current;
				Current = Current->Right;
			}
			else
			{
				Old = Current;
				Current = Current->Parent;
			}
		}
		else
		{
			std::cout << Current->Key << ' ';
			if (Current->Left)
			{
				Old = Current;
				Current = Current->Left;
			} 
			else if (Current->Right)
			{
				Old = Current;
				Current = Current->Right;
			}
			else
			{
				Old = Current;
				Current = Current->Parent;
			}
		}
	}
#endif
}


struct unbounded_tree
{
	int Key;

	unbounded_tree* Parent;
	unbounded_tree* Left;
	unbounded_tree* RightSibling;
};

static void
Print(unbounded_tree* Tree)
{
	if(Tree)
	{
		std::cout << Tree->Key << ' ';
		Print(Tree->Left);
		Print(Tree->RightSibling);
	}
}


//
// NOTE(georgy): Chained Hash Table, Open Addressing Hash Table
//
#define DIVISION_METHOD 0
#define MULTIPLICATION_METHOD 1
struct chained_hash_table
{
	node* Table[64];
};

static void
ChainedHashInsert(chained_hash_table* HashTable, int Value)
{
#if DIVISION_METHOD
	int HashIndex = Value % ArrayCount(HashTable->Table);
#elif MULTIPLICATION_METHOD
	float A = 0.6180339887f;
	int HashIndex = (int)(ArrayCount(HashTable->Table)*((Value * A) - (int)(Value * A)));
#endif
	
	node* NewNode = (node*)malloc(sizeof(node));
	NewNode->Value = Value;
	NewNode->Prev = 0;
	NewNode->Next = HashTable->Table[HashIndex];

	if (HashTable->Table[HashIndex])
	{
		HashTable->Table[HashIndex]->Prev = NewNode;
	}
	HashTable->Table[HashIndex] = NewNode;
}

static node *
ChainedHashSearch(chained_hash_table* HashTable, int Value)
{
#if DIVISION_METHOD
	int HashIndex = Value % ArrayCount(HashTable->Table);
#elif MULTIPLICATION_METHOD
	float A = 0.6180339887f;
	int HashIndex = (int)(ArrayCount(HashTable->Table) * ((Value * A) - (int)(Value * A)));
#endif

	node* Result = 0;
	node* Node = HashTable->Table[HashIndex];
	while (Node)
	{
		if (Node->Value == Value)
		{
			Result = Node;
			break;
		}

		Node = Node->Next;
	}

	return(Result);
}

static void
ChainedHashDelete(chained_hash_table* HashTable, node* Node)
{
#if DIVISION_METHOD
	int HashIndex = Node->Value % ArrayCount(HashTable->Table);
#elif MULTIPLICATION_METHOD
	float A = 0.6180339887f;
	int HashIndex = (int)(ArrayCount(HashTable->Table) * ((Node->Value * A) - (int)(Node->Value * A)));
#endif
	node* HashBucket = HashTable->Table[HashIndex];

	if (Node->Prev)
	{
		Node->Prev->Next = Node->Next;
	}
	else
	{
		HashBucket = Node->Next;
	}

	if (Node->Next)
	{
		Node->Next->Prev = Node->Prev;
	}
}


#define FREE_SLOT INT_MIN
struct open_addressing_hash_table
{
	int Table[128];
};

static uint32_t
OpenAddressHashInsert(open_addressing_hash_table* HashTable, int Value)
{
	uint32_t HashIndex = Value % ArrayCount(HashTable->Table);

	for(uint32_t I = 0;
		I < ArrayCount(HashTable->Table);
		I++)
	{
		uint32_t Index = (HashIndex + I) % ArrayCount(HashTable->Table);
		if (HashTable->Table[Index] == FREE_SLOT)
		{
			HashTable->Table[Index] = Value;
			return(Index);
		}
	}

	Assert(!"Hash table overflow!");
}

static int
OpenAddressHashSearch(open_addressing_hash_table* HashTable, int Value)
{
	int Result = -1;
	uint32_t HashIndex = Value % ArrayCount(HashTable->Table);

	for(uint32_t I = 0;
		I < ArrayCount(HashTable->Table);
		I++)
	{
		uint32_t Index = (HashIndex + I) % ArrayCount(HashTable->Table);
		if (HashTable->Table[Index] == Value)
		{
			Result = Index;
			break;
		}
	}

	return(Result);
}

static void
OpenAddressHashDelete(open_addressing_hash_table* HashTable, int Index)
{
	HashTable->Table[Index] = FREE_SLOT;
}


//
// NOTE(georgy): Binary Search Tree
//
struct binary_search_tree_node
{
	int Key;

	binary_search_tree_node* Parent;
	binary_search_tree_node* Left;
	binary_search_tree_node* Right;
};
struct binary_search_tree
{
	binary_search_tree_node* Root;
};

static void
BinarySearchTreeInsert(binary_search_tree* Tree, int Key)
{
	binary_search_tree_node* NewNode = (binary_search_tree_node*)malloc(sizeof(binary_search_tree_node));
	NewNode->Key = Key;
	NewNode->Left = NewNode->Right = 0;

	binary_search_tree_node* Old = 0;
	binary_search_tree_node* Current = Tree->Root;
	while (Current)
	{
		Old = Current;
		
		if (Key < Current->Key)
		{
			Current = Current->Left;
		}
		else
		{
			Current = Current->Right;
		}
	}

	NewNode->Parent = Old;
	if (Old == 0)
	{
		Tree->Root = NewNode;
	}
	else if (Key < Old->Key)
	{
		Old->Left = NewNode;
	}
	else
	{
		Old->Right = NewNode;
	}
}

static binary_search_tree_node *
BinarySearchTreeSearch(binary_search_tree_node* Node, int Key)
{
	while (Node && (Node->Key != Key))
	{
		if (Key < Node->Key)
		{
			Node = Node->Left;
		}
		else
		{
			Node = Node->Right;
		}
	}

	return(Node);
}

static binary_search_tree_node*
BinarySearchTreeMinimum(binary_search_tree_node* Node)
{
	while (Node->Left)
	{
		Node = Node->Left;
	}

	return(Node);
}

static binary_search_tree_node*
BinarySearchTreeMaximum(binary_search_tree_node* Node)
{
	while (Node->Right)
	{
		Node = Node->Right;
	}

	return(Node);
}

static binary_search_tree_node*
BinarySearchTreeSuccessor(binary_search_tree_node* Node)
{
	if (Node->Right)
	{
		return(BinarySearchTreeMinimum(Node->Right));
	}

	binary_search_tree_node* Parent = Node->Parent;
	while (Parent && (Node == Parent->Right))
	{
		Node = Parent;
		Parent = Parent->Parent;
	}

	return(Parent);
}

static binary_search_tree_node*
BinarySearchTreePredecessor(binary_search_tree_node* Node)
{
	if (Node->Left)
	{
		return(BinarySearchTreeMaximum(Node->Left));
	}

	binary_search_tree_node* Parent = Node->Parent;
	while (Parent && (Node == Parent->Left))
	{
		Node = Parent;
		Parent = Parent->Parent;
	}

	return(Parent);
}

static void
BinarySearchTreeTransplant(binary_search_tree* Tree, binary_search_tree_node* Old, binary_search_tree_node* New)
{
	if (!Old->Parent)
	{
		Tree->Root = New;
	}
	else if (Old == Old->Parent->Left)
	{
		Old->Parent->Left = New;
	}
	else
	{
		Old->Parent->Right = New;
	}

	if (New)
	{
		New->Parent = Old->Parent;
	}
}

static void
BinarySearchTreeDelete(binary_search_tree* Tree, binary_search_tree_node* NodeToDelete)
{
	if (!NodeToDelete->Left)
	{
		BinarySearchTreeTransplant(Tree, NodeToDelete, NodeToDelete->Right);
	}
	else if (!NodeToDelete->Right)
	{
		BinarySearchTreeTransplant(Tree, NodeToDelete, NodeToDelete->Left);
	}
	else
	{
		binary_search_tree_node* ReplaceWith = BinarySearchTreeMinimum(NodeToDelete->Right);
		if (ReplaceWith->Parent != NodeToDelete)
		{
			BinarySearchTreeTransplant(Tree, ReplaceWith, ReplaceWith->Right);
			ReplaceWith->Right = NodeToDelete->Right;
			ReplaceWith->Right->Parent = ReplaceWith;
		}
		BinarySearchTreeTransplant(Tree, NodeToDelete, ReplaceWith);
		ReplaceWith->Left = NodeToDelete->Left;
		NodeToDelete->Left->Parent = ReplaceWith;
	}

	free(NodeToDelete);
}

static void
BinarySearchTreeInorderPrint(binary_search_tree_node* Node)
{
	if (Node)
	{
		BinarySearchTreeInorderPrint(Node->Left);
		std::cout << Node->Key << " ";
		BinarySearchTreeInorderPrint(Node->Right);
	}
}


//
// NOTE(georgy): Red-Black Tree
enum rb_color
{
	RB_COLOR_RED,
	RB_COLOR_BLACK
};
struct rb_node
{
	int Key;

	rb_color Color;
	rb_node* Parent;
	rb_node* Left;
	rb_node* Right;
};
struct rb_tree
{
	rb_node* Root;
	rb_node Nil;
};

static void
RBLeftRotate(rb_tree* Tree, rb_node* Node)
{
	rb_node* Rot = Node->Right;
	Node->Right = Rot->Left;
	if (Rot->Left != &Tree->Nil)
	{
		Rot->Left->Parent = Node;
	}

	Rot->Parent = Node->Parent;
	if (Node->Parent == &Tree->Nil)
	{
		Tree->Root = Rot;
	}
	else if (Node == Node->Parent->Left)
	{
		Node->Parent->Left = Rot;
	}
	else
	{
		Node->Parent->Right = Rot;
	}

	Rot->Left = Node;
	Node->Parent = Rot;
}

static void
RBRightRotate(rb_tree* Tree, rb_node* Node)
{
	rb_node* Rot = Node->Left;
	Node->Left = Rot->Right;
	if (Rot->Right != &Tree->Nil)
	{
		Rot->Right->Parent = Node;
	}

	Rot->Parent = Node->Parent;
	if (Node->Parent == &Tree->Nil)
	{
		Tree->Root = Rot;
	}
	else if (Node == Node->Parent->Left)
	{
		Node->Parent->Left = Rot;
	}
	else
	{
		Node->Parent->Right = Rot;
	}

	Rot->Right = Node;
	Node->Parent = Rot;
}

static void
RBInsertFixup(rb_tree* Tree, rb_node* Node)
{
	while (Node->Parent->Color == RB_COLOR_RED)
	{
		if (Node->Parent == Node->Parent->Parent->Left)
		{
			rb_node* Uncle = Node->Parent->Parent->Right;
			if (Uncle->Color == RB_COLOR_RED)
			{
				Node->Parent->Color = RB_COLOR_BLACK;
				Uncle->Color = RB_COLOR_BLACK;
				Node->Parent->Parent->Color = RB_COLOR_RED;
				Node = Node->Parent->Parent;
			}
			else
			{
				if (Node == Node->Parent->Right)
				{
					Node = Node->Parent;
					RBLeftRotate(Tree, Node);
				}
				Node->Parent->Color = RB_COLOR_BLACK;
				Node->Parent->Parent->Color = RB_COLOR_RED;
				RBRightRotate(Tree, Node->Parent->Parent);
			}
		}
		else
		{
			rb_node* Uncle = Node->Parent->Parent->Left;
			if (Uncle->Color == RB_COLOR_RED)
			{
				Node->Parent->Color = RB_COLOR_BLACK;
				Uncle->Color = RB_COLOR_BLACK;
				Node->Parent->Parent->Color = RB_COLOR_RED;
				Node = Node->Parent->Parent;
			}
			else
			{
				if (Node == Node->Parent->Left)
				{
					Node = Node->Parent;
					RBRightRotate(Tree, Node);
				}
				Node->Parent->Color = RB_COLOR_BLACK;
				Node->Parent->Parent->Color = RB_COLOR_RED;
				RBLeftRotate(Tree, Node->Parent->Parent);
			}
		}
	}
	Tree->Root->Color = RB_COLOR_BLACK;
}

static void
RBInsert(rb_tree* Tree, int Key)
{
	rb_node* NewNode = (rb_node*)malloc(sizeof(rb_node));
	NewNode->Key = Key;
	NewNode->Color = RB_COLOR_RED;
	NewNode->Left = NewNode->Right = &Tree->Nil;

	rb_node* Old = &Tree->Nil;
	rb_node* Current = Tree->Root;
	while (Current != &Tree->Nil)
	{
		Old = Current;
		if (Key < Current->Key)
		{
			Current = Current->Left;
		}
		else
		{
			Current = Current->Right;
		}
	}
	NewNode->Parent = Old;
	if (Old == &Tree->Nil)
	{
		Tree->Root = NewNode;
	}
	else if (Key < Old->Key)
	{
		Old->Left = NewNode;
	}
	else
	{
		Old->Right = NewNode;
	}

	RBInsertFixup(Tree, NewNode);
}

int main(void)
{
	//
	// NOTE(georgy): Stack, Queue, Linked List
	//
	stack Stack = {};
	for (int I = 0; I < ArrayCount(Stack.Elements); I++)
	{
		Push(&Stack, I);
	}
	Print(&Stack);

	while (!IsEmpty(&Stack))
	{
		std::cout << Pop(&Stack) << ' ';
	}
	std::cout << std::endl;

	if(IsEmpty(&Stack))
	{
		std::cout << "Stack is empty, bro!" << std::endl;
	}
	std::cout << std::endl;


	queue Queue = {};
	if(IsEmpty(&Queue))
	{
		std::cout << "Queue is empty!" << std::endl;
	}

	for (int I = 0; I < ArrayCount(Queue.Elements) - 1; I++)
	{
		Enqueue(&Queue, I);
	}
	Print(&Queue);

	for (int I = 0; I < 10; I++)
	{
		std::cout << Dequeue(&Queue) << ' ';
	}
	std::cout << std::endl;

	for (int I = 0; I < 10; I++)
	{
		Enqueue(&Queue, I);
	}
	Print(&Queue);
	std::cout << std::endl;


	node Sentinel;
	Sentinel.Next = &Sentinel;
	Sentinel.Prev = &Sentinel;
	for (int I = 0; I < 10; I++)
	{
		node* Node = (node *)malloc(sizeof(node));
		Node->Value = I;
		ListInsert(&Sentinel, Node);
	}
	Print(&Sentinel);

	node* NodeToDelete = ListSearch(&Sentinel, 5);
	if (NodeToDelete)
	{
		ListDelete(&Sentinel, NodeToDelete);
		std::cout << "Node with value " << NodeToDelete->Value << " was deleted!" << std::endl;
		free(NodeToDelete);
	}
	Print(&Sentinel);

	NodeToDelete = ListSearch(&Sentinel, 15);
	if (!NodeToDelete)
	{
		std::cout << "Node with value " << 15 << " wasn't found!" << std::endl;
	}
	std::cout << std::endl;


	//
	// NOTE(georgy): Binary Tree, Tree
	//
	binary_tree* Tree;
	Tree = (binary_tree*)malloc(sizeof(binary_tree));
	Tree->Key = 10;
	Tree->Parent = 0;
	Tree->Left = (binary_tree*)malloc(sizeof(binary_tree));
	Tree->Right = (binary_tree*)malloc(sizeof(binary_tree));

	Tree->Left->Key = 5;
	Tree->Left->Parent = Tree;
	Tree->Left->Left = 0;
	Tree->Left->Right = (binary_tree*)malloc(sizeof(binary_tree));

	Tree->Right->Key = 7;
	Tree->Right->Parent = Tree;
	Tree->Right->Left = (binary_tree*)malloc(sizeof(binary_tree));
	Tree->Right->Right = 0;

	Tree->Left->Right->Key = 15;
	Tree->Left->Right->Parent = Tree->Left;
	Tree->Left->Right->Left = 0;
	Tree->Left->Right->Right = 0;

	Tree->Right->Left->Key = 3;
	Tree->Right->Left->Parent = Tree->Right;
	Tree->Right->Left->Left = 0;
	Tree->Right->Left->Right = 0;

	Print(Tree);
	std::cout << std::endl;


	unbounded_tree* UTree;
	UTree = (unbounded_tree*)malloc(sizeof(unbounded_tree));
	UTree->Key = 10;
	UTree->Parent = 0;
	UTree->Left = (unbounded_tree*)malloc(sizeof(unbounded_tree));
	UTree->RightSibling = 0;

	UTree->Left->Key = 15;
	UTree->Left->Parent = UTree;
	UTree->Left->Left = 0;
	UTree->Left->RightSibling = (unbounded_tree*)malloc(sizeof(unbounded_tree));

	UTree->Left->RightSibling->Key = 9;
	UTree->Left->RightSibling->Parent = UTree;
	UTree->Left->RightSibling->Left = (unbounded_tree*)malloc(sizeof(unbounded_tree));
	UTree->Left->RightSibling->RightSibling = (unbounded_tree*)malloc(sizeof(unbounded_tree));
	
	UTree->Left->RightSibling->RightSibling->Key = 19;
	UTree->Left->RightSibling->RightSibling->Parent = UTree;
	UTree->Left->RightSibling->RightSibling->Left = 0;
	UTree->Left->RightSibling->RightSibling->RightSibling = 0;

	UTree->Left->RightSibling->Left->Key = 13;
	UTree->Left->RightSibling->Left->Parent = UTree->Left->RightSibling;
	UTree->Left->RightSibling->Left->Left = 0;
	UTree->Left->RightSibling->Left->RightSibling = 0;

	Print(UTree);
	std::cout << std::endl;
	std::cout << std::endl;


	//
	// NOTE(georgy): Chained Hash Table, Open Addressing Hash Table
	//
	chained_hash_table ChainedHashTable = {};
	for (int I = 0; I < 128; I++)
	{
		ChainedHashInsert(&ChainedHashTable, I);
	}
	node* Nine = ChainedHashSearch(&ChainedHashTable, 9);
	if (Nine)
	{
		std::cout << "Node with value 9 is in the hash table!" << std::endl;
	}
	ChainedHashDelete(&ChainedHashTable, Nine);
	Nine = ChainedHashSearch(&ChainedHashTable, 9);
	if (!Nine)
	{
		std::cout << "After deletion, node with value 9 is not in the hash table!" << std::endl;
	}
	std::cout << std::endl;


	open_addressing_hash_table OpenAddressHashTable;
	for (int I = 0; I < ArrayCount(OpenAddressHashTable.Table); I++)
	{
		OpenAddressHashTable.Table[I] = FREE_SLOT;
	}

	int Value = 0;
	for (int I = 0; I < 128; I++)
	{
		OpenAddressHashInsert(&OpenAddressHashTable, Value);
		Value += 2;
	}
	int SeventyTwoIndex = OpenAddressHashSearch(&OpenAddressHashTable, 72);
	if (SeventyTwoIndex != -1)
	{
		std::cout << "Value 72 is in the hash table!" << std::endl;
	}
	OpenAddressHashDelete(&OpenAddressHashTable, SeventyTwoIndex);
	SeventyTwoIndex = OpenAddressHashSearch(&OpenAddressHashTable, 72);
	if (SeventyTwoIndex == -1)
	{
		std::cout << "After deletion, value 72 is not in the hash table!" << std::endl;
	}
	std::cout << std::endl;


	//
	// NOTE(georgy): Binary Search Tree
	//
	binary_search_tree BSTree = {};
	BinarySearchTreeInsert(&BSTree, 12);
	BinarySearchTreeInsert(&BSTree, 5);
	BinarySearchTreeInsert(&BSTree, 2);
	BinarySearchTreeInsert(&BSTree, 9);
	BinarySearchTreeInsert(&BSTree, 18);
	BinarySearchTreeInsert(&BSTree, 15);
	BinarySearchTreeInsert(&BSTree, 19);
	BinarySearchTreeInsert(&BSTree, 13);
	BinarySearchTreeInsert(&BSTree, 17);

	binary_search_tree_node* ThirteenNode = BinarySearchTreeSearch(BSTree.Root, 13);
	if (ThirteenNode)
	{
		std::cout << "Key 13 is in the binary search tree!" << std::endl;
	}

	binary_search_tree_node* Max = BinarySearchTreeMaximum(BSTree.Root);
	if (Max)
	{
		std::cout << "Key " << Max->Key << " is the maximum key in the binary search tree!" << std::endl;
	}

	binary_search_tree_node* Min = BinarySearchTreeMinimum(BSTree.Root);
	if (Min)
	{
		std::cout << "Key " << Min->Key << " is the minimum key in the binary search tree!" << std::endl;
	}

	binary_search_tree_node* Successor = BinarySearchTreeSuccessor(Min);
	if (Successor)
	{
		std::cout << "Node with key " << Successor->Key << " is the successor of min node of the binary search tree!" << std::endl;
	}

	binary_search_tree_node* Predecessor = BinarySearchTreePredecessor(Max);
	if (Predecessor)
	{
		std::cout << "Node with key " << Predecessor->Key << " is the predecessor of max node of the binary search tree!" << std::endl;
	}

	BinarySearchTreeInorderPrint(BSTree.Root);
	std::cout << std::endl;

	BinarySearchTreeDelete(&BSTree, Min);
	BinarySearchTreeDelete(&BSTree, ThirteenNode);
	std::cout << "Binary search tree after deletion of 2 and 13: ";
	BinarySearchTreeInorderPrint(BSTree.Root);
	std::cout << std::endl;

	BinarySearchTreeDelete(&BSTree, BSTree.Root);
	std::cout << "Binary search tree after deletion of 12 (the root): ";
	BinarySearchTreeInorderPrint(BSTree.Root);
	std::cout << std::endl;


	//
	// NOTE(georgy): Red-Black Tree
	//
	rb_tree RBTree = {};
	RBTree.Root = &RBTree.Nil;
	RBTree.Nil.Color = RB_COLOR_BLACK;
	RBInsert(&RBTree, 11);
	RBInsert(&RBTree, 2);
	RBInsert(&RBTree, 14);
	RBInsert(&RBTree, 1);
	RBInsert(&RBTree, 7);
	RBInsert(&RBTree, 15);
	RBInsert(&RBTree, 5);
	RBInsert(&RBTree, 8);

	RBInsert(&RBTree, 4);

	return(0);
}