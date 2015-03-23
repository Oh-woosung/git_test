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
	//�ʱ�ȭ
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

	while(!Heap->Empty())																	//���� �����Ͱ� ���� �ִٸ� ��� loop�� ����.
	{
		Heat = Heap->Pop();																	//������ ���� �����͸� Heat�� ����

		Temp = FindHeat(Heat,Size);															//Temp�� Edge�� ã�Ƽ� �����Ѵ�.
		if(Temp == NULL)																	//Temp�� NULL�ϰ�� continue
			continue;
		Temp->SetVist(true);																//Temp�� NULL�� �ƴϸ� visit�� true�� �ٲ۴�.

		FindDongEdge(Temp->GettoDongName(),Temp->GetfromDongName(),Size)->SetVist(true);	//�Ȱ��� Edge�� �����Ƿ� ã�Ƽ� �װ� ���� true�� �ٲ۴�.
		dVertex1 = FindDongVertex(Temp->GetfromDongName());
		dVertex2 = FindDongVertex(Temp->GettoDongName());
		
		VertexIndex1 = Findindex(dVertex1);
		VertexIndex2 = Findindex(dVertex2);

		//Edge�� ������ Vertex�� �������� 1�� �����̹Ƿ� Edge�� ������ Vertex�� �������� 1�� �۾ƾ� �ϸ�, Vertex�� ���� �ϳ��� index�� ���� Vertex�� index�� ���� �޶�� �Ѵ�.
		if((dVertex1->GetIndex() == VertexIndex1 || dVertex2->GetIndex() == VertexIndex2) && dVertex2->GetIndex() != dVertex1->GetIndex() && count < VertexIndex-1)
		{
			Dong1 = FindDongEdge(Temp->GetfromDongName(),Temp->GettoDongName(),Size);
			Dong2 = FindDongEdge(Temp->GettoDongName(),Temp->GetfromDongName(),Size);

			//Spanning tree�� dVertex1�� ������ ���� �Ѵ�
			if(!SearchVertex(dVertex1))
				InsertVertex(dVertex1);
			//Spanning tree�� dVertex2�� ������ ���� �Ѵ�
			if(!SearchVertex(dVertex2))
				InsertVertex(dVertex2);
			//dVertex1 dVertex2 ��ġ���� Edge�� �����Ѵ�.

			dVertex1->SpanningDongEdgeInsert(Dong1);
			dVertex2->SpanningDongEdgeInsert(Dong2);
			
			//���Ͽ��� �ؾ� �ϴµ�,���ؼ� ���� ������ �պ��ؾ� �Ѵ�.
			(dVertex1->GetIndex() > dVertex2->GetIndex()) ?  Store = dVertex2 : Store = dVertex1;
			(dVertex1->GetIndex() > dVertex2->GetIndex()) ?  det = dVertex1->GetIndex() : det = dVertex2->GetIndex();
			
			//loop�� ���鼭 ���� index�� ������ �ִ� Vertex�� �� �ٲ� �ش�.
			for(int i = 0 ; i < VertexIndex ; i++)
			{
				if(Vertex[i]->GetIndex() == det)
					Union(Store,Vertex[i]);
			}

			count++;	//Edge ���� ����
		}
	}

	if(count == VertexIndex-1)	//Spanningtree�� �ϼ� �Ǿ�����
		return true;
	else						//�ϼ� �ȵ� ������
		return false;
}
void SpanningTree :: InsertVertex(DongVertex* Node1)
{
	//Vertex ����
	DongVertex* Vertex = dHead;

	if(!dHead)							//spanning tree ��尡 ����� ��� ����
		dHead = Node1;
	else								//�ƴ� ��� ���� �ִ� ��� ���� �̵� �� ���� ���� �Ѵ�.
	{
		while(Vertex->GetsNext())
			Vertex = Vertex->GetsNext();

		Vertex->SetSNext(Node1);
	}
}
void SpanningTree :: InitSpanningTree(int Size)
{
	//Edge �ε��� �ʱ�ȭ
	for(int i = 0 ; i < Size ; i++)
		this->Edge[i]->SetIndex(i);
}
void SpanningTree :: InitVertex()
{
	//Vertex �ε��� �ʱ�ȭ
	for(int i = 0 ; Vertex[i] != NULL ; i++)
		this->Vertex[i]->SetIndex(i);
}
void SpanningTree :: InitDongEdgearr(DongEdge* Node1)
{
	//Edge�迭 �ʱ�ȭ
	Edge[EdgeIndex++] = Node1;
}
void SpanningTree :: initDongVertexarr(DongVertex* Node1)
{
	//Vertex�迭 �ʱ�ȭ
	Vertex[VertexIndex++] = Node1;
}
DongEdge* SpanningTree :: FindDongEdge(char* FromDong,char* ToDong ,int Size)
{
	//Edge �迭�� ��ȸ�ϸ鼭 ���� Edge�� �߰ߵǸ� Edge�� ��ȯ �Ѵ�.
	for(int i = 0 ; i < Size ; i++)
	{
		if(strcmp(Edge[i]->GetfromDongName(),FromDong) == 0 && strcmp(Edge[i]->GettoDongName(),ToDong) == 0)
			return Edge[i];
	}
	return NULL;
}
DongVertex* SpanningTree :: FindDongVertex(char* Dong)
{
	//Vertex�迭�� ��ȸ�ϸ鼭 ���� Vertex�� �߰ߵǸ� Vertex�� ��ȯ �Ѵ�.
	for(int i = 0 ; Vertex[i] != NULL ; i++)
	{
		if(strcmp(Vertex[i]->GetDongName(),Dong) == 0)
			return Vertex[i];
	}
	return NULL;
}
int SpanningTree :: Findindex(DongVertex* Dong)
{
	//Vertex�迭�� ��ȸ�ϸ鼭 ���� Vertex�� ã���� �� index�� ��ȯ �Ѵ�.
	for(int i = 0 ; Vertex[i] != NULL ; i++)
	{
		if(Vertex[i] == Dong)
			return i;
	}
}
DongEdge* SpanningTree :: FindHeat(int Heat, int Size)
{
	//�� �ս����� key���Ͽ� Edge�迭�� ��ȸ�ϸ鼭 ���ϴ� �����͸� ã�´�.
	//��, Edge�� �湮�� �ȵ� Edge���� �Ѵ�.
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
	//Vertex�迭�� ����Ǿ��ִ� �ε����� ���ؼ� ���� �������� �պ��Ѵ�.
	
	//Vertex�迭�� ���ʴ�� ��ȸ�ϸ� ���� Vertex�� ������ loop�� �����.
	for(index1 = 0 ; index1 < VertexIndex ; index1++)
	{
		if(strcmp(Vertex[index1]->GetDongName(),dVertex1->GetDongName()) == 0)
			break;
	}
	//Vertex�迭�� ���ʴ�� ��ȸ�ϸ� ���� Vertex�� ������ loop�� �����.
	for(index2 = 0 ; index2 < VertexIndex ; index2++)
	{
		if(strcmp(Vertex[index2]->GetDongName(),dVertex2->GetDongName()) == 0)
			break;
	}
	//�ε��� ũ�⸦ ���ؼ� ���� �ε����� �պ��Ѵ�.
	if(Vertex[index1]->GetIndex() > Vertex[index2]->GetIndex())
		Vertex[index1]->SetIndex(Vertex[index2]->GetIndex());
	else
		Vertex[index2]->SetIndex(Vertex[index1]->GetIndex());
}
void SpanningTree :: PrintSpanningTree(char* str1)
{
	DongVertex* pCur = dHead;
	//���д� Ʈ���� ����Ѵ�.
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
	//���д� Ʈ������ ���ϴ� Vertex�� ã�Ƽ� return �޴´�.
		while(pReturn)
		{
			if(strcmp(Node1->GetDongName(),pReturn->GetDongName()) == 0)	//��ġ�ϴ� �����Ͱ� ���� ��� Vertex return
				return pReturn;
			pReturn = pReturn->GetsNext();
		}

	return NULL;
}
void SpanningTree :: DestroySpanningTree()
{
	//���д� Ʈ�� �ı�
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
	//printflag �ʱ�ȭ
	for(int i = 0 ; i <EdgeIndex ; i++)
		Edge[i]->SetPrintFlag(false);
}