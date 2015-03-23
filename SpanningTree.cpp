#include "SpanningTree.h"

SpanningTree :: SpanningTree(void)
{
	InitPrivate();
}
SpanningTree :: ~SpanningTree(void)
{
}
void SpanningTree :: InitPrivate()
{
	//초기화
	memset(Edge,NULL,100);
	memset(Vertex,NULL,100);
	dHead = NULL;
	EdgeIndex = 0;
	VertexIndex = 0;
}
bool SpanningTree :: Insert(MinHeap* Heap)
{
	DongEdge *Dong1, *Dong2,*Temp;
	DongVertex* dVertex1, *dVertex2;
	DongVertex* Store;
	int Heat;
	InitSpanningTree(Heap->GetHeapSize());
	InitVertex();
	int Size = Heap->GetHeapSize();
	int VertexIndex1,VertexIndex2;
	int count = 0;
	int det = 0;

	while(!Heap->Empty())																	//힙에 데이터가 남아 있다면 계속 loop을 돈다.
	{
		Heat = Heap->Pop();																	//힙에서 나온 데이터를 Heat에 저장

		Temp = FindHeat(Heat,Size);															//Temp에 Edge를 찾아서 저장한다.
		if(Temp == NULL)																	//Temp가 NULL일경우 continue
			continue;
		Temp->SetVist(true);																//Temp가 NULL이 아니면 visit을 true로 바꾼다.

		FindDongEdge(Temp->GettoDongName(),Temp->GetfromDongName(),Size)->SetVist(true);	//똑같은 Edge가 있으므로 찾아서 그것 또한 true로 바꾼다.
		dVertex1 = FindDongVertex(Temp->GetfromDongName());
		dVertex2 = FindDongVertex(Temp->GettoDongName());
		
		VertexIndex1 = Findindex(dVertex1);
		VertexIndex2 = Findindex(dVertex2);

		//Edge의 갯수는 Vertex의 개수에서 1을 뺀것이므로 Edge의 개수는 Vertex의 개수보다 1이 작아야 하며, Vertex의 둘중 하나만 index랑 같고 Vertex의 index는 서로 달라야 한다.
		if((dVertex1->GetIndex() == VertexIndex1 || dVertex2->GetIndex() == VertexIndex2) && dVertex2->GetIndex() != dVertex1->GetIndex() && count < VertexIndex-1)
		{
			Dong1 = FindDongEdge(Temp->GetfromDongName(),Temp->GettoDongName(),Size);
			Dong2 = FindDongEdge(Temp->GettoDongName(),Temp->GetfromDongName(),Size);

			//Spanning tree에 dVertex1이 없으면 삽입 한다
			if(!SearchVertex(dVertex1))
				InsertVertex(dVertex1);
			//Spanning tree에 dVertex2가 없으면 삽입 한다
			if(!SearchVertex(dVertex2))
				InsertVertex(dVertex2);
			//dVertex1 dVertex2 위치에서 Edge를 삽입한다.

			dVertex1->SpanningDongEdgeInsert(Dong1);
			dVertex2->SpanningDongEdgeInsert(Dong2);
			
			//유니온을 해야 하는데,비교해서 작은 것으로 합병해야 한다.
			(dVertex1->GetIndex() > dVertex2->GetIndex()) ?  Store = dVertex2 : Store = dVertex1;
			(dVertex1->GetIndex() > dVertex2->GetIndex()) ?  det = dVertex1->GetIndex() : det = dVertex2->GetIndex();
			
			//loop을 돌면서 같은 index를 가지고 있는 Vertex는 다 바꿔 준다.
			for(int i = 0 ; i < VertexIndex ; i++)
			{
				if(Vertex[i]->GetIndex() == det)
					Union(Store,Vertex[i]);
			}

			count++;	//Edge 개수 증가
		}
	}

	if(count == VertexIndex-1)	//Spanningtree가 완성 되었을때
		return true;
	else						//완성 안되 었을때
		return false;
}
void SpanningTree :: InsertVertex(DongVertex* Node1)
{
	//Vertex 삽입
	DongVertex* Vertex = dHead;

	if(!dHead)							//spanning tree 헤드가 비었을 경우 삽입
		dHead = Node1;
	else								//아닐 경우 끝에 있는 노드 까지 이동 한 다음 삽입 한다.
	{
		while(Vertex->GetsNext())
			Vertex = Vertex->GetsNext();

		Vertex->SetSNext(Node1);
	}
}
void SpanningTree :: InitSpanningTree(int Size)
{
	//Edge 인덱스 초기화
	for(int i = 0 ; i < Size ; i++)
		this->Edge[i]->SetIndex(i);
}
void SpanningTree :: InitVertex()
{
	//Vertex 인덱스 초기화
	for(int i = 0 ; Vertex[i] != NULL ; i++)
		this->Vertex[i]->SetIndex(i);
}
void SpanningTree :: InitDongEdgearr(DongEdge* Node1)
{
	//Edge배열 초기화
	Edge[EdgeIndex++] = Node1;
}
void SpanningTree :: initDongVertexarr(DongVertex* Node1)
{
	//Vertex배열 초기화
	Vertex[VertexIndex++] = Node1;
}
DongEdge* SpanningTree :: FindDongEdge(char* FromDong,char* ToDong ,int Size)
{
	//Edge 배열을 순회하면서 같은 Edge가 발견되면 Edge를 반환 한다.
	for(int i = 0 ; i < Size ; i++)
	{
		if(strcmp(Edge[i]->GetfromDongName(),FromDong) == 0 && strcmp(Edge[i]->GettoDongName(),ToDong) == 0)
			return Edge[i];
	}
	return NULL;
}
DongVertex* SpanningTree :: FindDongVertex(char* Dong)
{
	//Vertex배열을 순회하면서 같은 Vertex가 발견되면 Vertex를 반환 한다.
	for(int i = 0 ; Vertex[i] != NULL ; i++)
	{
		if(strcmp(Vertex[i]->GetDongName(),Dong) == 0)
			return Vertex[i];
	}
	return NULL;
}
int SpanningTree :: Findindex(DongVertex* Dong)
{
	//Vertex배열을 순회하면서 같은 Vertex를 찾으면 그 index를 반환 한다.
	for(int i = 0 ; Vertex[i] != NULL ; i++)
	{
		if(Vertex[i] == Dong)
			return i;
	}
}
DongEdge* SpanningTree :: FindHeat(int Heat, int Size)
{
	//열 손실율을 key로하여 Edge배열을 순회하면서 원하는 데이터를 찾는다.
	//단, Edge는 방문이 안된 Edge여야 한다.
	for(int i = 0 ; i < Size ; i ++)
	{
		if(Edge[i]->Getheat() == Heat && Edge[i]->GetVisit() != true)
			return Edge[i];
	}
	return NULL;
}
void SpanningTree :: Union(DongVertex* dVertex1, DongVertex* dVertex2)
{
	int index1,index2;
	//Vertex배열에 저장되어있는 인덱스를 비교해서 같은 집합으로 합병한다.
	
	//Vertex배열을 차례대로 순회하며 같은 Vertex가 나오면 loop을 멈춘다.
	for(index1 = 0 ; index1 < VertexIndex ; index1++)
	{
		if(strcmp(Vertex[index1]->GetDongName(),dVertex1->GetDongName()) == 0)
			break;
	}
	//Vertex배열을 차례대로 순회하며 같은 Vertex가 나오면 loop을 멈춘다.
	for(index2 = 0 ; index2 < VertexIndex ; index2++)
	{
		if(strcmp(Vertex[index2]->GetDongName(),dVertex2->GetDongName()) == 0)
			break;
	}
	//인덱스 크기를 비교해서 작은 인덱스로 합병한다.
	if(Vertex[index1]->GetIndex() > Vertex[index2]->GetIndex())
		Vertex[index1]->SetIndex(Vertex[index2]->GetIndex());
	else
		Vertex[index2]->SetIndex(Vertex[index1]->GetIndex());
}
void SpanningTree :: PrintSpanningTree(char* str1)
{
	DongVertex* pCur = dHead;
	//스패닝 트리를 출력한다.
	if(!dHead)
		return ; 
	else
	{
		while(pCur)
		{
			pCur->PrintSpanningTree(dHead,str1);
			pCur = pCur->GetsNext();
		}
	}
	InitPrintFlag();
}
DongVertex* SpanningTree :: SearchVertex(DongVertex* Node1)
{
	DongVertex* pReturn = dHead;
	//스패닝 트리에서 원하는 Vertex를 찾아서 return 받는다.
		while(pReturn)
		{
			if(strcmp(Node1->GetDongName(),pReturn->GetDongName()) == 0)	//일치하는 데이터가 있을 경우 Vertex return
				return pReturn;
			pReturn = pReturn->GetsNext();
		}

	return NULL;
}
void SpanningTree :: DestroySpanningTree()
{
	//스패닝 트리 파괴
	for(int i = 0 ; i < VertexIndex ; i++)
	{
		if(Vertex[i]->GetsHead())
			Vertex[i]->DestroySpanning();
		if(Vertex[i]->GetsNext())
			Vertex[i]->SetSNext(NULL);
	}
}
void SpanningTree :: InitPrintFlag()
{
	//printflag 초기화
	for(int i = 0 ; i <EdgeIndex ; i++)
		Edge[i]->SetPrintFlag(false);
}