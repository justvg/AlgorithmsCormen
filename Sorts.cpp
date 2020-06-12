#include <iostream>

#define ArrayCount(Array) (sizeof(Array)/sizeof((Array)[0]))
#define Assert(Expression) if(!(Expression)) { *(int *)0 = 0; } 

//
// NOTE(georgy): InsertionSort, SelectionSort, MergeSort and BinarySearch
//

static void
InsertionSort(int Length, int *A)
{
	for (int J = 1; J < Length; J++)
	{
		int Key = A[J];
		int I = J - 1;

		while ((I >= 0) && (A[I] > Key))
		{
			A[I + 1] = A[I];
			I--;
		}

		A[I + 1] = Key;
	}
}

static void
SelectionSort(int Length, int *A)
{
	for (int I = 0; I < Length - 1; I++)
	{
		int MinIndex = I;

		for (int J = I + 1; J < Length; J++)
		{
			if (A[J] < A[MinIndex])
			{
				MinIndex = J;
			}
		}

		int Min = A[MinIndex];
		A[MinIndex] = A[I];
		A[I] = Min;
	}
}

static void
MergeSort(int Length, int *First, int *TempMem)
{
	if(Length == 1)
	{
		return;
	}
	else if (Length == 2)
	{
		if(First[0] > First[1])
		{
			int Temp = First[0];
			First[0] = First[1];
			First[1] = Temp;
		}
	}
	else
	{
		int LengthHalf0 = Length / 2;
		int LengthHalf1 = Length - LengthHalf0;

		int *Half0 = First;
		int *Half1 = First + LengthHalf0;
		MergeSort(LengthHalf0, Half0, TempMem);
		MergeSort(LengthHalf1, Half1, TempMem);

		int *ReadHalf0 = Half0;
		int *ReadHalf1 = Half1;
		int *End = First + Length;

		int *Out = TempMem;
		for (int I = 0; I < Length; I++)
		{
			if(ReadHalf0 == Half1)
			{
				*Out++ = *ReadHalf1++;
			}
			else if(ReadHalf1 == End)
			{
				*Out++ = *ReadHalf0++;
			}
			else if(*ReadHalf0 < *ReadHalf1)
			{
				*Out++ = *ReadHalf0++;
			}
			else
			{
				*Out++ = *ReadHalf1++;
			}
		}

		for (int I = 0; I < Length; I++)
		{
			First[I] = TempMem[I];
		}
	}
}

static int
BinarySearch(int* A, int Start, int End, int Value)
{
	if(End >= Start)
	{
		int Mid = Start  + ((End - Start) / 2);

		if(A[Mid] == Value)
		{
			return(Mid);
		}
		else if(A[Mid] > Value)
		{
			return(BinarySearch(A, Start, Mid - 1, Value));
		}
		else
		{
			return(BinarySearch(A, Mid + 1, End, Value));
		}
	}

	// NOTE(georgy): The value isn't in the array
	return(-1);
}


//
// NOTE(georgy): binary_heap, max-priority queue, Heap Sort
//

struct binary_heap
{
	uint32_t NodeCount;
	int Heap[64];
};
#define Parent(Index) (((Index)-1)/2)
#define Left(Index) (2*(Index)+1)
#define Right(Index) (2*(Index)+2)

static void
MaxHeapify(binary_heap *BinaryHeap, uint32_t Index)
{
#if 0
	// NOTE(georgy): Recursive method
	uint32_t Left = Left(Index);
	uint32_t Right = Right(Index);
	uint32_t Largest = Index;

	if((Left < BinaryHeap->NodeCount) && (BinaryHeap->Heap[Left] > BinaryHeap->Heap[Largest]))
	{
		Largest = Left;
	}
	if((Right < BinaryHeap->NodeCount) && (BinaryHeap->Heap[Right] > BinaryHeap->Heap[Largest]))
	{
		Largest = Right;
	}

	if(Largest != Index)
	{
		int Temp = BinaryHeap->Heap[Largest];
		BinaryHeap->Heap[Largest] = BinaryHeap->Heap[Index];
		BinaryHeap->Heap[Index] = Temp;
		MaxHeapify(BinaryHeap, Largest);
	}
#else
	// NOTE(georgy): Iterative method
	uint32_t Largest = Index;
	while(Index < (BinaryHeap->NodeCount / 2))
	{
		uint32_t Left = Left(Index);
		uint32_t Right = Right(Index);
		if((Left < BinaryHeap->NodeCount) && (BinaryHeap->Heap[Left] > BinaryHeap->Heap[Largest]))
		{
			Largest = Left;
		}
		if((Right < BinaryHeap->NodeCount) && (BinaryHeap->Heap[Right] > BinaryHeap->Heap[Largest]))
		{
			Largest = Right;
		}

		if(Largest != Index)
		{
			int Temp = BinaryHeap->Heap[Largest];
			BinaryHeap->Heap[Largest] = BinaryHeap->Heap[Index];
			BinaryHeap->Heap[Index] = Temp;

			Index = Largest;
		}
		else
		{
			break;
		}
	}
#endif
}

static void
BuildMaxHeap(binary_heap* BinaryHeap)
{
	if(BinaryHeap->NodeCount > 1)
	{
		for(int Index = (BinaryHeap->NodeCount / 2) - 1;
			Index >= 0;
			Index--)
		{
			MaxHeapify(BinaryHeap, Index);
		}
	}
}

static void
HeapSort(binary_heap* BinaryHeap)
{
	BuildMaxHeap(BinaryHeap);
	uint32_t NodeCount = BinaryHeap->NodeCount;
	for(uint32_t Index = BinaryHeap->NodeCount - 1;
		Index > 0;
		Index--)
	{
		int Max = BinaryHeap->Heap[0];
		BinaryHeap->Heap[0] = BinaryHeap->Heap[Index];
		BinaryHeap->Heap[Index] = Max;

		BinaryHeap->NodeCount--;

		MaxHeapify(BinaryHeap, 0);
	}
	BinaryHeap->NodeCount = NodeCount;
}

inline int
HeapMaximum(binary_heap* BinaryHeap)
{	
	int Result = BinaryHeap->Heap[0];
	return(Result);
}

static int
HeapExtractMaximum(binary_heap* BinaryHeap)
{
	int Max = INT_MIN;
	if(BinaryHeap->NodeCount > 0)
	{
		Max = BinaryHeap->Heap[0];
		BinaryHeap->Heap[0] = BinaryHeap->Heap[--BinaryHeap->NodeCount];
		MaxHeapify(BinaryHeap, 0);
	}
	else
	{
		Assert(!"Priority-queue is empty!");
	}

	return(Max);
}

static void
HeapIncreaseKey(binary_heap* BinaryHeap, uint32_t Index, int NewKey)
{
	if(NewKey > BinaryHeap->Heap[Index])
	{
		while((Index > 0) && (BinaryHeap->Heap[Parent(Index)] <= NewKey))
		{
			BinaryHeap->Heap[Index] = BinaryHeap->Heap[Parent(Index)];

			Index = Parent(Index);
		}

		BinaryHeap->Heap[Index] = NewKey;
	}
}

static void
MaxHeapInsert(binary_heap* BinaryHeap, int Key)
{
	BinaryHeap->Heap[BinaryHeap->NodeCount++] = INT_MIN;
	HeapIncreaseKey(BinaryHeap, BinaryHeap->NodeCount - 1, Key);
}

static void
HeapDeleteNode(binary_heap* BinaryHeap, int Index)
{
	HeapIncreaseKey(BinaryHeap, Index, INT_MAX);
	HeapExtractMaximum(BinaryHeap);
}

//
// NOTE(georgy): QuickSort, RandomizedQuickSort, HoareQuickSort
//

static int
Partition(int *A, int FirstIndex, int LastIndex)
{
	int PivotValue = A[LastIndex];
	int I = FirstIndex - 1;
	for (int J = FirstIndex; J < LastIndex; J++)
	{
		if(A[J] <= PivotValue)
		{
			++I;

			int Temp = A[I];
			A[I] = A[J];
			A[J] = Temp;
		}
	}

	A[LastIndex] = A[I + 1];
	A[I + 1] = PivotValue;

	return(I + 1);
}

static void
QuickSort(int *A, int FirstIndex, int LastIndex)
{
	if(FirstIndex < LastIndex)
	{
		int PivotIndex = Partition(A, FirstIndex, LastIndex);
		QuickSort(A, FirstIndex, PivotIndex - 1);
		QuickSort(A, PivotIndex + 1, LastIndex);
	}
}

static int
RandomizedPartition(int* A, int FirstIndex, int LastIndex)
{
	int RandomIndex = FirstIndex + (rand() % (LastIndex + 1 - FirstIndex));
	int Pivot = A[RandomIndex];
	A[RandomIndex] = A[LastIndex];
	A[LastIndex] = Pivot;

	return(Partition(A, FirstIndex, LastIndex));
}

static void
RandomizedQuickSort(int* A, int FirstIndex, int LastIndex)
{
	if (FirstIndex < LastIndex)
	{
		int PivotIndex = RandomizedPartition(A, FirstIndex, LastIndex);
		RandomizedQuickSort(A, FirstIndex, PivotIndex - 1);
		RandomizedQuickSort(A, PivotIndex + 1, LastIndex);
	}
}

static int 
HoarePartition(int* A, int FirstIndex, int LastIndex)
{
	int PivotValue = A[FirstIndex];
	int I = FirstIndex - 1;
	int J = LastIndex + 1;
	
	while (true)
	{
		do
		{
			J = J - 1;
		} while (A[J] > PivotValue);

		do
		{
			I = I + 1;
		} while (A[I] < PivotValue);

		if (I < J)
		{
			int Temp = A[I];
			A[I] = A[J];
			A[J] = Temp;
		}
		else
		{
			return(J);
		}
	}
}

static void
HoareQuickSort(int* A, int FirstIndex, int LastIndex)
{
	if (FirstIndex < LastIndex)
	{
		int PivotIndex = HoarePartition(A, FirstIndex, LastIndex);
		HoareQuickSort(A, FirstIndex, PivotIndex);
		HoareQuickSort(A, PivotIndex + 1, LastIndex);
	}
}


//
// NOTE(georgy): CountingSort, RadixSort, BucketSort
//

static void
CountingSort(uint32_t Length, uint32_t* A, uint32_t* B, uint32_t MaxValue)
{
	uint32_t* C = (uint32_t*)malloc(sizeof(uint32_t)*(MaxValue + 1));
	if(C)
	{
		memset(C, 0, sizeof(uint32_t) * (MaxValue + 1));

		for(int I = 0; I < Length; I++)
		{
			++C[A[I]];
		}
		for(int I = 1; I <= MaxValue; I++)
		{
			C[I] += C[I - 1];
		}

		for(int I = Length-1; I >= 0; I--)
		{
			B[C[A[I]] - 1] = A[I];
			--C[A[I]];
		}

		free(C);
	}
}

static void
RadixSort(uint32_t Length, uint32_t* A, uint32_t* Temp)
{
	uint32_t* Source = A;
	uint32_t* Dest = Temp;

	for(uint32_t ByteOffset = 0;
		ByteOffset < 32;
		ByteOffset += 8)
	{
		uint32_t Offsets[256] = {};

		for(uint32_t SourceIndex = 0;
			SourceIndex < Length;
			SourceIndex++)
		{
			uint32_t Byte = (Source[SourceIndex] >> ByteOffset) & 0xFF;
			++Offsets[Byte];
		}

		uint32_t Total = 0;
		for(uint32_t OffsetIndex = 0;
			OffsetIndex < ArrayCount(Offsets);
			OffsetIndex++)
		{
			uint32_t Count = Offsets[OffsetIndex];
			Offsets[OffsetIndex] = Total;
			Total += Count;
		}

		for(uint32_t SourceIndex = 0;
			SourceIndex < Length;
			SourceIndex++)
		{
			uint32_t Byte = (Source[SourceIndex] >> ByteOffset) & 0xFF;
			Dest[Offsets[Byte]] = Source[SourceIndex];
			++Offsets[Byte];
		}

		uint32_t* Temp = Source;
		Source = Dest;
		Dest = Temp;
	}
}

struct bucket
{
	float Value;

	bucket* Prev;
	bucket* Next;
};
static void
BucketSort(uint32_t Length, float* A)
{
	bucket* Buckets[10] = {};

	for(uint32_t I = 0; I < Length; I++)
	{
		uint32_t BucketIndex = (uint32_t)(A[I]*ArrayCount(Buckets));
		bucket* Node = (bucket *)malloc(sizeof(bucket));
		Node->Value = A[I];

		Node->Next = Buckets[BucketIndex];
		Node->Prev = 0;
		if(Buckets[BucketIndex])
		{
			Buckets[BucketIndex]->Prev = Node;
		}
		Buckets[BucketIndex] = Node;
	}

	for(uint32_t BucketIndex = 0;
		BucketIndex < ArrayCount(Buckets);
		BucketIndex++)
	{
		// NOTE(georgy): Sort every bucket using insertion sort
		bucket* Bucket = Buckets[BucketIndex];
		if(Bucket)
		{
			bucket* Node = Bucket->Next; // NOTE(georgy): We start with the second one
			while(Node)
			{
				float Key = Node->Value;
				bucket* CurrentPos = Node;
				bucket* TargetNode = Node->Prev;

				while(TargetNode && (TargetNode->Value > Key))
				{
					CurrentPos->Value = TargetNode->Value;
					CurrentPos = TargetNode;

					TargetNode = TargetNode->Prev;
				}
				
				CurrentPos->Value = Key;
				Node = Node->Next;
			}
		}
	}

	uint32_t AIndex = 0;
	for(uint32_t BucketIndex = 0;
		BucketIndex < ArrayCount(Buckets);
		BucketIndex++)
	{
		bucket* Bucket = Buckets[BucketIndex];
		while(Bucket)
		{
			A[AIndex++] = Bucket->Value;
			Bucket = Bucket->Next;
		}
	}

	for(uint32_t BucketIndex = 0;
		BucketIndex < ArrayCount(Buckets);
		BucketIndex++)
	{
		bucket* Bucket = Buckets[BucketIndex];
		while(Bucket)
		{
			bucket* Free = Bucket;
			Bucket = Bucket->Next;
			free(Free);
		}
	}
}

int main(void)
{
	//
	// NOTE(georgy): InsertionSort, SelectionSort, MergeSort and BinarySearch
	//
	int A[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	InsertionSort(ArrayCount(A), A);
	for (int Value : A)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;


	int B[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	SelectionSort(ArrayCount(B), B);
	for (int Value : B)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;


	int C[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	int* TempMem = (int*)malloc(sizeof(int) * ArrayCount(C));
	MergeSort(ArrayCount(C), C, TempMem);
	for (int Value : C)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;
	free(TempMem);

	int ValueToFind = 5;
	int Index = BinarySearch(C, 0, ArrayCount(C), ValueToFind);
	if (Index == -1)
	{
		std::cout << "Value " << ValueToFind << " is not in the array C" << std::endl;
	}
	else
	{
		std::cout << "Value " << ValueToFind << " is in the array C. Index is " << Index << std::endl;
	}

	//
	// NOTE(georgy): binary_heap, max-priority queue, Heap Sort
	//
	binary_heap BinaryHeap;
	BinaryHeap.NodeCount = 10;
	BinaryHeap.Heap[0] = 16;
	BinaryHeap.Heap[1] = 14;
	BinaryHeap.Heap[2] = 10;
	BinaryHeap.Heap[3] = 8;
	BinaryHeap.Heap[4] = 7;
	BinaryHeap.Heap[5] = 9;
	BinaryHeap.Heap[6] = 3;
	BinaryHeap.Heap[7] = 2;
	BinaryHeap.Heap[8] = 4;
	BinaryHeap.Heap[9] = 1;
	HeapSort(&BinaryHeap);
	for (int I = 0; I < BinaryHeap.NodeCount; I++)
	{
		std::cout << BinaryHeap.Heap[I] << ' ';
	}
	std::cout << std::endl;

	binary_heap BinaryHeap1;
	BinaryHeap1.NodeCount = 10;
	BinaryHeap1.Heap[0] = 16;
	BinaryHeap1.Heap[1] = 14;
	BinaryHeap1.Heap[2] = 10;
	BinaryHeap1.Heap[3] = 8;
	BinaryHeap1.Heap[4] = 7;
	BinaryHeap1.Heap[5] = 9;
	BinaryHeap1.Heap[6] = 3;
	BinaryHeap1.Heap[7] = 2;
	BinaryHeap1.Heap[8] = 4;
	BinaryHeap1.Heap[9] = 1;
	HeapIncreaseKey(&BinaryHeap1, 8, 15);
	for (int I = 0; I < BinaryHeap1.NodeCount; I++)
	{
		std::cout << BinaryHeap1.Heap[I] << ' ';
	}
	std::cout << std::endl;

	HeapDeleteNode(&BinaryHeap1, 2);
	for (int I = 0; I < BinaryHeap1.NodeCount; I++)
	{
		std::cout << BinaryHeap1.Heap[I] << ' ';
	}
	std::cout << std::endl;


	//
	// NOTE(georgy): QuickSort, RandomizedQuickSort, HoareQuickSort
	//
	int D[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	QuickSort(D, 0, ArrayCount(D) - 1);
	for (int Value : D)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;


	srand(1337);
	int E[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	RandomizedQuickSort(E, 0, ArrayCount(E) - 1);
	for (int Value : E)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;


	int F[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	HoareQuickSort(F, 0, ArrayCount(F) - 1);
	for (int Value : F)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;


	//
	// NOTE(georgy): CountingSort, RadixSort, BucketSort
	//
	uint32_t G0[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	uint32_t G1[ArrayCount(G0)];
	CountingSort(ArrayCount(G0), G0, G1, 10);
	for (int Value : G1)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;

	
	uint32_t H0[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	uint32_t H1[ArrayCount(H0)];
	RadixSort(ArrayCount(H0), H0, H1);
	for (int Value : H0)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;

	
	float K[] = { 0.78f, 0.17f, 0.39f, 0.26f, 0.72f, 0.94f, 0.21f, 0.12f, 0.23f, 0.68f };
	BucketSort(ArrayCount(K), K);
	for (float Value : K)
	{
		std::cout << Value << ' ';
	}
	std::cout << std::endl;

	return(0);
}