#include "Dijkstra.h"

#define Infinity 1000

Dijkstra :: Dijkstra(void)					//생성자에서 일부 private 변수들을 초기화 한다.
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
void Dijkstra :: CreateArr(GuVertex* Node1)						//거리 arr를 만드는 함수
{
	GuEdge* Edge = NULL;
	int key = 0;

	//loop을 돌면서 원하는 Vertex를 Vertex배열에 찾아서 그 인덱스 값을 반환하고, 다음에 그 배열에 거리를 저장한다.

	for(int i = 0 ; i < index ; i++)
	{
		Edge = Node1->GetHead();
		for(int j = 0 ; Edge  ; j++)
		{
			key = SearchVertex(Edge->GetToGuName());			//원하는 Vertex의 인덱스를 찾는다.
			DistArr[i][key] = Edge->GetDist();
			
			Edge = Edge->GetNext();								//다음 Vertex로 이동
		}
		DistArr[i][i] = 0;										//자기의 위치는 이동거리가 0이므로 0으로 초기화 한다.
		Node1 = Node1->GetNext();	
	}
	DistArr[index-1][index-1] = 0;
}
void Dijkstra :: CreateVertex(GuVertex* Node1)					//Vertex배열을 만든다.
{
	while(Node1)												//Node1이 NULL이 아닐때까지 계속 이동한다.
	{
		Vertex[index] = Node1;									//Vertex배열에 하나씩 다 저장한다.
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
	//Vertex 배열에서 특정 Vertex를 찾아서 그 인덱스를 반환.
	for(int i = 0 ; i < index ; i++)
	{
		if(strcmp(Vertex[i]->GetGuName(),pGu) == 0)
			return i;
	}

	return -1;			//찾지 못하면 -1 반환.
}
void Dijkstra :: path_init(int path[][100])					//path를 초기화 시킨다.
{
	for(int i = 0 ; i < index ; i++)
		for(int j = 0 ; j < index ; j++)
			path[i][j] = Infinity;							//무한대로 다 초기화
}
int Dijkstra :: choose(int distance[],int n,bool found[])
{
	int i, min, minpos;
	min = 999999;		//min은 minimum value를 의미 초기값은 매우 큰 값으로 설정
	minpos = -1;		//minpos는 minimu value의 index값 -1로 초기화 한다.
	
	//loop을 돌면서 distance에 있는 가장 작은 값을 착아서 반환한다.
	//단 방문이 안된 Vertex여야 한다.

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

	for(i = 0 ; i < n ; i++)				//초기화
	{
		distance[i] = DistArr[start][i];
		Visit[i] = false;
		check[i] = 1;
		path[i][0] = start;
	}

	Visit[start] = true;					//시작 정점은 방문했다고 표시한다.
	distance[start] = 0;

	for(i = 0 ; i <n-2 ; i++)				
	{
		u = choose(distance,n,Visit);		//최단거리 정점을 찾는다.
		Visit[u] = true;					//다음 방문했다고 flag를 변경 한다.
		for(w = 0 ; w < n ; w++)
		{
			if(!Visit[w])					//w의 방문여부가 false일때
			{
				if(distance[u] + DistArr[u][w] < distance[w])	//u의 거리와 u에서 w로가는 거리를 더해서 w의 거리보다 작을경우
				{
					if(i == 0)						//i가 0일때 즉 처음일대
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

	path_init(path);										//path 초기화 한다.

	shortest_path(SearchVertex(Start->GetGuName()),num);	//다익스트라를 만든다.

}
void Dijkstra :: Print(GuVertex* Start, GuVertex* End)		//최단경로를 출력한다.
{
	ofstream fout("repair_log.txt",ios_base::app);

	int Index= SearchVertex(Start->GetGuName());
	fout<<Start->GetGuName()<<" -> ";
	
	for(int j= index-1 ; j > 0 ; j--)
	{
		if(path[Index][j] != Infinity)							//무한대가 아니면 방문했다는 뜻으로 확인해서 출력한다.
			fout<<Vertex[path[Index][j]]->GetGuName()<<" -> ";
	}
	fout<<End->GetGuName()<<endl;
	fout.close();
}
GuVertex* Dijkstra :: SearchShortestpath(char *pGu)
{
	//원하는 Vertex를 찾아서 그 index를 반환한다.
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
	for(int i = 0 ; i < index ; i++)		//Vertex배열에서 Vertex1의 인덱스를 찾아서 index1에 저장한다.
	{
		if(strcmp(Vertex[i]->GetGuName(),Vertex1->GetGuName()) == 0)
		{
			index1 = i;
			break;
		}
	}
	for(int i = 0 ; i < index ; i++)		//Vertex배열에서 Vertex2의 인덱스를 찾아서 index2에 저장한다.
	{
		if(strcmp(Vertex[i]->GetGuName(),Vertex2->GetGuName()) == 0)
		{
			index2 = i;
			break;
		}
	}

	if(distance[index1] > distance[index2])		//2개의 거리를 비교해서 큰쪽의 Vertex를 return 한다.
		return Vertex1;
	else
		return Vertex2;
}

void Dijkstra :: Init()
{
	//다익스트라를 초기화 한다.
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