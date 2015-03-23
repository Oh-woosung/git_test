#include "Dijkstra.h"

#define Infinity 1000

Dijkstra :: Dijkstra(void)					//�����ڿ��� �Ϻ� private �������� �ʱ�ȭ �Ѵ�.
{
	DistArr = new int*[100];
	memset(Visit,false,100);
	for(int i = 0 ; i < 100 ; i++)
	{
		DistArr[i] = new int[100];
		for(int j = 0 ; j < 100 ; j++)
			DistArr[i][j] = Infinity;
		Vertex[i] = NULL;
	}
	index = 0;
}
Dijkstra :: ~Dijkstra(void)
{
}
void Dijkstra :: CreateArr(GuVertex* Node1)						//�Ÿ� arr�� ����� �Լ�
{
	GuEdge* Edge = NULL;
	int key = 0;

	//loop�� ���鼭 ���ϴ� Vertex�� Vertex�迭�� ã�Ƽ� �� �ε��� ���� ��ȯ�ϰ�, ������ �� �迭�� �Ÿ��� �����Ѵ�.

	for(int i = 0 ; i < index ; i++)
	{
		Edge = Node1->GetHead();
		for(int j = 0 ; Edge  ; j++)
		{
			key = SearchVertex(Edge->GetToGuName());			//���ϴ� Vertex�� �ε����� ã�´�.
			DistArr[i][key] = Edge->GetDist();
			
			Edge = Edge->GetNext();								//���� Vertex�� �̵�
		}
		DistArr[i][i] = 0;										//�ڱ��� ��ġ�� �̵��Ÿ��� 0�̹Ƿ� 0���� �ʱ�ȭ �Ѵ�.
		Node1 = Node1->GetNext();	
	}
	DistArr[index-1][index-1] = 0;
}
void Dijkstra :: CreateVertex(GuVertex* Node1)					//Vertex�迭�� �����.
{
	while(Node1)												//Node1�� NULL�� �ƴҶ����� ��� �̵��Ѵ�.
	{
		Vertex[index] = Node1;									//Vertex�迭�� �ϳ��� �� �����Ѵ�.
		Node1 = Node1->GetNext();
		index++;
	}
}
GuVertex* Dijkstra :: GetVertex(int index)
{
	return Vertex[index];
}
int Dijkstra :: SearchVertex(char* pGu)
{
	//Vertex �迭���� Ư�� Vertex�� ã�Ƽ� �� �ε����� ��ȯ.
	for(int i = 0 ; i < index ; i++)
	{
		if(strcmp(Vertex[i]->GetGuName(),pGu) == 0)
			return i;
	}

	return -1;			//ã�� ���ϸ� -1 ��ȯ.
}
void Dijkstra :: path_init(int path[][100])					//path�� �ʱ�ȭ ��Ų��.
{
	for(int i = 0 ; i < index ; i++)
		for(int j = 0 ; j < index ; j++)
			path[i][j] = Infinity;							//���Ѵ�� �� �ʱ�ȭ
}
int Dijkstra :: choose(int distance[],int n,bool found[])
{
	int i, min, minpos;
	min = 999999;		//min�� minimum value�� �ǹ� �ʱⰪ�� �ſ� ū ������ ����
	minpos = -1;		//minpos�� minimu value�� index�� -1�� �ʱ�ȭ �Ѵ�.
	
	//loop�� ���鼭 distance�� �ִ� ���� ���� ���� ���Ƽ� ��ȯ�Ѵ�.
	//�� �湮�� �ȵ� Vertex���� �Ѵ�.

	for(i = 0 ; i < n ; i++)
	{
		if(distance[i] < min && !found[i])
		{
			min = distance[i];					
			minpos = i;
		}
	}
	
	return minpos;
}

void Dijkstra :: shortest_path(int start,int n)
{
	int i,j, u,w;

	for(i = 0 ; i < n ; i++)				//�ʱ�ȭ
	{
		distance[i] = DistArr[start][i];
		Visit[i] = false;
		check[i] = 1;
		path[i][0] = start;
	}

	Visit[start] = true;					//���� ������ �湮�ߴٰ� ǥ���Ѵ�.
	distance[start] = 0;

	for(i = 0 ; i <n-2 ; i++)				
	{
		u = choose(distance,n,Visit);		//�ִܰŸ� ������ ã�´�.
		Visit[u] = true;					//���� �湮�ߴٰ� flag�� ���� �Ѵ�.
		for(w = 0 ; w < n ; w++)
		{
			if(!Visit[w])					//w�� �湮���ΰ� false�϶�
			{
				if(distance[u] + DistArr[u][w] < distance[w])	//u�� �Ÿ��� u���� w�ΰ��� �Ÿ��� ���ؼ� w�� �Ÿ����� �������
				{
					if(i == 0)						//i�� 0�϶� �� ó���ϴ�
					{
						path[w][check[w]] = u;
						check[w]++;
					}
					else
					{
						for(j = 0 ; j < (check[u]+1) ; j++)
						{
							path[w][j] = path[u][j];
							path[w][j+1] = u;
							check[w]++;
						}
					}

					distance[w] = distance[u]+DistArr[u][w];
				}
			}
		}
	}
}
void Dijkstra :: MakeDijkstra(GuVertex* Start,int num)
{

	path_init(path);										//path �ʱ�ȭ �Ѵ�.

	shortest_path(SearchVertex(Start->GetGuName()),num);	//���ͽ�Ʈ�� �����.

}
void Dijkstra :: Print(GuVertex* Start, GuVertex* End)		//�ִܰ�θ� ����Ѵ�.
{
	ofstream fout("repair_log.txt",ios_base::app);

	int Index= SearchVertex(Start->GetGuName());
	fout<<Start->GetGuName()<<" -> ";
	
	for(int j= index-1 ; j > 0 ; j--)
	{
		if(path[Index][j] != Infinity)							//���Ѵ밡 �ƴϸ� �湮�ߴٴ� ������ Ȯ���ؼ� ����Ѵ�.
			fout<<Vertex[path[Index][j]]->GetGuName()<<" -> ";
	}
	fout<<End->GetGuName()<<endl;
	fout.close();
}
GuVertex* Dijkstra :: SearchShortestpath(char *pGu)
{
	//���ϴ� Vertex�� ã�Ƽ� �� index�� ��ȯ�Ѵ�.
	int i = 0;
	for(i = 0 ; i < index ; i++)
	{
		if(strcmp(Vertex[i]->GetGuName(),pGu) == 0)
			break;
	}

	return Vertex[i];
}
GuVertex* Dijkstra :: Compare(GuVertex* Vertex1, GuVertex* Vertex2)
{
	int index1,index2;
	for(int i = 0 ; i < index ; i++)		//Vertex�迭���� Vertex1�� �ε����� ã�Ƽ� index1�� �����Ѵ�.
	{
		if(strcmp(Vertex[i]->GetGuName(),Vertex1->GetGuName()) == 0)
		{
			index1 = i;
			break;
		}
	}
	for(int i = 0 ; i < index ; i++)		//Vertex�迭���� Vertex2�� �ε����� ã�Ƽ� index2�� �����Ѵ�.
	{
		if(strcmp(Vertex[i]->GetGuName(),Vertex2->GetGuName()) == 0)
		{
			index2 = i;
			break;
		}
	}

	if(distance[index1] > distance[index2])		//2���� �Ÿ��� ���ؼ� ū���� Vertex�� return �Ѵ�.
		return Vertex1;
	else
		return Vertex2;
}

void Dijkstra :: Init()
{
	//���ͽ�Ʈ�� �ʱ�ȭ �Ѵ�.
	for(int i = 0 ; i < 100 ; i++)
	{
		Vertex[i] = NULL;
		distance[i] = 0;
		for(int j = 0 ; j < 100 ; j++)
			path[i][j] = 0;
		Visit[i] = true;
		check[i] = 1;
	}
	index = 0;
}