#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

# define INF 999999999

typedef struct Node
{
		int idx ;
		int weight ;
		struct Node *next ;
} Node ;

typedef struct pq
{
  char heap_name[31][10] ; // 2D array 
  int heap_num[31] ;
  int idx[31] ;
  int size ;
} Queue ;

void swap_value(int *a, int *b) ;
void swap_name(char list[][10], int name1, int name2) ;
void heapify(Queue *pq, int current, int parent) ;
void Insert_que(Queue *pq, char* name, int value, int idx) ;
void Delete_que(Queue *pq) ;

int isIn(Queue *pq, int idx) ;
void Init_SS(int size, int idx, int *distance) ;
void push_node(Node *rt, int idx, int weight) ;
void print_node(Node *rt, char **list) ;
int Dijkstra(char **v, Node **adj, int size, int start, int end) ; // return a number that distance for start to en
int** Floyd(char **v, Node **adj, int size) ;

int main()
{
		FILE *ip = fopen("hw7.data.txt", "r") ;
		char name[10] ; // for input
		char first[10] ;
		char **order ;
		char ***list ;
		int mSize = 0, flag = 1, cnt = 0 ;
		clock_t start, end ;
		float sec ;
		int **floyd ;

		if(ip == NULL)
		{
				printf("File open error!") ;
				exit(1) ;
		}

		while(flag)
		{
				fscanf(ip, "%s", name) ;
				
				if(mSize == 0 && name[strlen(name) - 1] == '\n') flag = 1 ; 
				else if(strcmp(first, name) == 0) flag = 0 ;
				else
				{
						// printf("%s\n", name) ;
						mSize++ ;
						if(mSize == 1) strcpy(first, name) ;
				}
		}

		// printf("mSize is %d\n", mSize) ;
		
		list = (char***)malloc(sizeof(char**) * (mSize + 1)) ;
		for(int j = 0 ; j <= mSize ; j++)
		{
				list[j] = (char**)malloc(sizeof(char*) * (mSize + 1)) ;
				for(int i = 0 ; i <= mSize ; i++) list[j][i] = (char*)malloc(sizeof(char) * 10) ;
		}

		order = (char**)malloc(sizeof(char*) * (mSize + 1) ) ;
		for(int j = 0 ; j <= mSize ; j++) order[j] = (char*)malloc(sizeof(char) * 10) ;
		
		// first line 
		strcpy(list[1][cnt], first) ;
		strcpy(order[1], first) ;
		cnt++ ;
		
		for(int i = 1 ; i <= mSize ; i++)
		{	
				while(strcmp(name, "") != 0)
				{
					fscanf(ip, "%s", name) ;
					if(cnt == 0) strcpy(order[i], name) ;
					strcpy(list[i][cnt], name) ;
					// printf("%s\n",list[i][cnt]) ; 
					cnt++ ;

					if(cnt == mSize + 1) break ;
				}
				cnt = 0 ;
		}
		/* Testing line
		for(int i = 1 ; i <= mSize ; i++)
		{
				for(int j = 0 ; j <= mSize ; j++)
						printf("%s ", list[i][j]) ;
				printf("\n") ;
		}
		for(int i = 1 ; i <= mSize ; i++) printf("%s ", order[i]) ;
		*/

		Node** aList = (Node**)malloc(sizeof(Node*) * (mSize + 1)) ;
		for(int i = 0 ; i <= mSize ; i++)
		{
				aList[i] = (Node*)malloc(sizeof(Node)) ;
				aList[i]->next = NULL ;
		}

		for(int i = 1 ; i <= mSize ; i++)
		{
				for(int j = mSize ; j >= 1 ; j--)
				{
						if(strcmp(list[i][j], "INF") != 0)
								if(atoi(list[i][j]) != 0) push_node(aList[i], j, atoi(list[i][j])) ;
				}		
		}
		
		int **result = (int**)malloc(sizeof(int*) * (mSize + 1)) ;
		for(int i = 0 ; i <= mSize ; i++) result[i] = (int*)malloc(sizeof(int)) ;

		/* Adjacency list output
		for(int i = 1 ; i <= mSize ; i++)
		{	
				printf("Node[%s] : ", order[i]) ;
				print_node(aList[i], order) ;
				printf("\n") ;
		}
		*/

/*****************************************************************************************************************************/
		
		start = clock() ;

		for(int i = 1 ; i <= mSize ; i++)
		{
			for(int j =  1 ; j <= mSize ; j++)
			{
				result[i][j] = Dijkstra(order, aList, mSize, i, j) ;
				//printf("%d ", result[i][j]) ;
			}
			//printf("\n") ;
		}
		
		end = clock() ;

		sec = (float)(end - start) / CLOCKS_PER_SEC ;
		printf("It took %.2f seconds to compute shortest path between cities with Dijkstra's algorithm as follows\n", sec) ;

		printf("\t\t") ;

		for(int i = 1 ; i <= mSize ; i++) printf("%s\t\t", order[i]) ;

		printf("\n") ;

		for(int i = 1 ; i <= mSize ; i++)
		{
			printf("%s\t\t", order[i]) ;
			for(int j =  1 ; j <= mSize ; j++)
			{	
				result[i][j] = Dijkstra(order, aList, mSize, i, j) ;
				printf("%d\t\t", result[i][j]) ;
			 	result[i][j] = 0 ;
			}
			printf("\n") ;
		}

/*****************************************************************************************************************************/
		
		start = clock() ;

		floyd = Floyd(order, aList, mSize) ;

		end = clock() ;
		
		sec = (float)(end - start) / CLOCKS_PER_SEC ;
		printf("It took %.2f seconds to compute shortest path between cities with Floyd algorithm as follows\n", sec) ;

		printf("\t\t") ;

		for(int i = 1 ; i <= mSize ; i++) printf("%s\t\t", order[i]) ;

		printf("\n") ;

		for(int i = 1 ; i <= mSize ; i++)
		{	printf("%s\t\t", order[i]) ;
			for(int j =  1 ; j <= mSize ; j++)
			{	
				printf("%d\t\t", floyd[i][j]) ;
			}
			printf("\n") ;
		}
		
		// Memory deallocate
		for(int j = 0 ; j <= mSize ; j++)
		{
			for(int i = 0 ; i <= mSize ; i++) free(list[j][i]) ;
			free(list[j]) ;
		}
		free(list) ;

		for(int j = 0 ; j <= mSize ; j++) 
		{
				free(aList[j]) ;
				free(floyd[j]) ;
				free(order[j]) ;
				free(result[j]) ;
		}
		
		free(aList) ;
		free(floyd) ;
		free(result) ;
		free(order) ;
		// end

		return 0 ;
}

// Function that print nodes in adjacency list
void print_node(Node *rt, char **list)
{
	Node *temp = rt->next ;

	while(temp != NULL)
	{
		printf("->Node[%s]", list[temp->idx]) ;
		temp = temp->next ;
	}
}

// Function that push node in adjacency list
void push_node(Node *rt, int idx, int weight) 
{
	Node *new = (Node*)malloc(sizeof(Node)) ;

	new->idx = idx ;
	new->weight = weight ;
	new->next = rt->next ;
	rt->next = new ;
}

void swap_value(int *a, int *b) // Swap a and b.
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void swap_name(char list[][10], int name1, int name2) // Swap name1 index and name2 index.
{
  char temp[10] ;
  strcpy(temp, list[name1]) ;
  strcpy(list[name1], list[name2]) ;
  strcpy(list[name2], temp) ;
}

// Continue to repeat if the value of the current is greater than 1 and is greater than the value of the parent.
void heapify(Queue *pq, int current, int parent)
{
  while(current > 1 && (pq->heap_num[current] < pq->heap_num[parent]))
    {
      swap_value(&pq->heap_num[current], &pq->heap_num[parent]) ;
	  swap_value(&pq->idx[current], &pq->idx[parent]) ;
      swap_name(pq->heap_name, current, parent) ;
      current = parent ;
      parent = parent / 2 ;
    }
}

void Insert_que(Queue *pq, char* name, int value, int idx)
{
  if(pq->size == 30) printf("Queue is full!!\n") ;

  if(pq->size == 0) // If size == 0
  {
    strcpy(pq->heap_name[1], name) ; // Using Strcpy func to copy string.
    pq->heap_num[1] = value ;
	pq->idx[1] = idx ;
   	pq->size += 1 ;
  }
  else
  {
	pq->size += 1 ;
    pq->heap_num[pq->size] = value ; // Set value at index end.
	pq->idx[pq->size] = idx ;
    strcpy(pq->heap_name[pq->size], name) ; // Set name at index end.

    heapify(pq, pq->size, pq->size / 2) ; // Call heapify func
  }
}

void Delete_que(Queue *pq)
{
  if(pq->size == 0) printf("Queue is empty!!\n") ;

  if(pq->size > 1)
  {
  	// Swap root node and end node.
  	swap_value(&pq->heap_num[1], &pq->heap_num[pq->size]) ;
  	swap_value(&pq->idx[1], &pq->idx[pq->size]) ;
  	swap_name(pq->heap_name, 1, pq->size) ;
  	
  	// Decrease heap size and call heapify func.
  	pq->size -= 1 ;
  	heapify(pq, pq->size, pq->size / 2) ;
  }
  else pq->size -= 1 ;
}

void Init_SS(int size, int idx, int *distance)
{
	for(int i = 1 ; i <= size ; i++) distance[i] = INF ;
	distance[idx] = 0 ;
}

int Dijkstra(char **v, Node **adj, int size, int start, int end)
{
	int *d = (int*)malloc(sizeof(int) * (size + 1)) ;
	int *che = (int*)malloc(sizeof(int) * (size + 1)) ;
	int *isque = (int*)malloc(sizeof(int) * (size + 1)) ;
	int num = 0 ;
	int be ;

	for(int i  = 1 ; i <= size ; i++)
	{
			che[i] = 0 ;
			isque[i] = 0 ;
	}

	Init_SS(size, start, d) ;

	Queue pq ;
	pq.size = 0 ;

	Insert_que(&pq, v[start], d[start], start) ;
	
	che[pq.idx[1]] = 0 ;
	while(pq.size != 0)
	{	
		Node *u = adj[pq.idx[1]] ;
		Node *temp = u->next ;
		int cur = pq.idx[1] ;
		isque[cur] = 1 ;

		if(pq.idx[1] == end) break ;
		
		Delete_que(&pq) ;

		while(temp != NULL)
		{
			if(che[cur] != temp->idx)
			{
				if(isque[temp->idx] != 1)
				{
					if(d[temp->idx] > d[cur] + temp->weight) d[temp->idx] = d[cur] + temp->weight ;
					if((be = isIn(&pq, temp->idx)) != -1)
					{
							pq.heap_num[be] = d[temp->idx] ;
							heapify(&pq, pq.size, pq.size / 2) ;
					}
					else Insert_que(&pq, v[temp->idx], d[temp->idx], temp->idx) ;
					che[temp->idx] = cur ;
				}
			}
			temp = temp->next ;
		}
	}
	
	free(che) ;
	free(isque) ;
	free(d) ;

	return d[end] ;
}

int isIn(Queue *pq, int idx)
{
	for(int i = 1 ; i <= pq->size ; i++) 
			if (pq->idx[i] == idx) return i ;
	
	return -1 ;
}

int** Floyd(char **v, Node **adj, int size)
{
	int setting[size+1][size+1] ;

	int **list ;
	list = (int**)malloc(sizeof(int*) * (size + 1)) ;
	for(int i = 0 ; i <= size ; i++) list[i] = (int*)malloc(sizeof(int) * (size + 1)) ;

	for(int i = 0 ; i <= size;  i++)
			for(int j = 0 ; j <= size ; j++) setting[i][j] = 0 ;

	for(int i = 1 ; i <= size ; i++)
	{
		Node *temp = adj[i] ;
		temp = temp->next ;

		while(temp != NULL) 
		{
			setting[i][(int)temp->idx] = (int)temp->weight ;
			temp = temp->next ;
		}
	}

	
	for(int i = 1 ; i <= size ; i++)
			for(int j = 1 ; j <= size ; j++)
					if(setting[i][j] == 0) if(i != j) setting[i][j] = INF ;
	
	for(int i = 1 ; i <= size ; i++)
			for(int j = 1 ; j <= size ; j++)
					list[i][j] = setting[i][j] ;

	for(int k = 1 ; k <= size ; k++)
			for(int i = 1 ; i <= size ; i++)
					for(int j = 1 ; j <= size ; j++)
							if(list[i][k] + list[k][j] < list[i][j]) list[i][j] = list[i][k] + list[k][j] ;

	/*
	for(int i = 1 ; i <= size ; i++)
	{
			for(int j = 1 ; j <= size ; j++)
			{
					//list[i][j] = setting[i][j] ;
					printf("%d ", list[i][j]) ;
			}
			printf("\n") ;
	}	
	*/
	return list ;
}
