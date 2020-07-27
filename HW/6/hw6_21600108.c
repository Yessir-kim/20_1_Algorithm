#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node
{
	int idx ;
	struct Node *next ;
} Node ;	

void push_node(Node *rt, int idx) ;
void print_node(Node *rt, char *list) ;
Node** transpose_list(Node **rt, int size) ;

int
main()
{
	FILE *fp = fopen("hw6_data.txt","r") ;
	int mSize = 0, row = 1, col = 0 ;
	char in ;
	char **aMatrix ;
	
	if(fp == NULL) 
	{
		printf("File open error!") ;
		exit(1) ;
	}

	while(in != '\n')
	{
		fscanf(fp, "%c", &in) ;
		if(mSize == 0 && in == '\n') in = '0' ; // 행렬의 크기를 확인하는 부분에서 엔터가 여러개 있을 경우 처리
		if(in >= 65 && in <= 122) mSize++ ;
	}

	aMatrix = (char**)malloc(sizeof(char*) * (mSize + 1) ) ;
    for(int j = 0 ; j <= mSize ; j++) aMatrix[j] = (char*)malloc(sizeof(char) * (mSize + 1)) ;
	
	aMatrix[0][0] = ' ' ; // '\'

	// Matrix를 채우는 부분
	while(EOF != fscanf(fp, "%c", &in)) // EOF가 아닐 때 동안 반복
	{
		if((in >= 48 && in <= 57) || (in >= 65 && in <= 122)) // 알파벳 사이의 문자면 입력 받음 
		{	
			aMatrix[row][col] = in ;
			if(col == mSize) // 인덱스 범위를 초과한 경우 행을 바꿔주고 열의 값을 0으로 바꿈
			{
				row++ ;
				col = 0 ;
			}
			else col++ ;
		}
		if(row == mSize + 1) break ; // 행의 값이 초과되면 입력이 끝
	}
	// Alphabet list 
	char *alph = (char*)malloc(sizeof(char) * (mSize + 1)) ;
	// 알파벳을 똑같이 채워넣는 부분
	for(int i = 1 ; i <= mSize ; i++)
	{
		aMatrix[0][i] = aMatrix[i][0] ;
		alph[i] = aMatrix[0][i] ;
	}
	/* 값 채우기 완료
	for(int i = 0 ; i <= mSize ; i++)
	{
		for(int j = 0 ; j <= mSize ; j++)
		{
			printf("%c",aMatrix[i][j]) ;
		}
		printf("\n") ;
	}
	*/
	Node** new ; // transpose list 
	Node** aList = (Node**)malloc(sizeof(Node*) * (mSize + 1)) ; // org list
	for(int i = 0 ; i <= mSize ; i++)
	{
		aList[i] = (Node*)malloc(sizeof(Node)) ;
		aList[i]->next = NULL ;
	}
	// Make a adjacency list
	for(int i = 1 ; i <= mSize ; i++)
	{
		for(int j = mSize ; j >= 1 ; j--)
		{
			if(aMatrix[i][j] == '1') push_node(aList[i], j) ; 
		}
	}
	// Print first part
	printf("<Array of adjacency list of above graph>\n") ;
	for(int i = 1 ; i <= mSize ; i++)
	{
		printf("Node[%c] : ", aMatrix[0][i]) ;
		print_node(aList[i], alph) ;
		printf("\n") ;
	}
	// Make a transpose adjacency list
	new = transpose_list(aList, mSize) ;
	// Print second part
	printf("\n<Array of adjacency list of transpose graph>\n") ;
	for(int i = 1 ; i <= mSize ; i++)
	{
		printf("Node[%c] : ",aMatrix[0][i]) ;
		print_node(new[i], alph) ;
		printf("\n") ;
	}
	// Memory deallocation part
	for(int j = 0 ; j <= mSize ; j++) free(aMatrix[j]) ;
    free(aMatrix) ;
	for(int j = 0 ; j <= mSize ; j++) free(aList[j]) ;
	free(aList) ;
	for(int j = 0 ; j <= mSize ; j++) free(new[j]) ;
	free(new) ;
	free(alph) ;
	// Close file
	fclose(fp) ;
	
	return 0 ;
}

// Function that print nodes in adjacency list
void print_node(Node *rt, char *list)
{
	Node *temp = rt->next ;

	while(temp != NULL)
	{
		printf("->Node[%c]", list[temp->idx]) ;
		temp = temp->next ;
	}
}

// Function that push node in adjacency list
void push_node(Node *rt, int idx)
{
	Node *new = (Node*)malloc(sizeof(Node)) ;

	new->idx = idx ;
	new->next = rt->next ;
	rt->next = new ;
}


// Function that transpose adjacency list
Node** transpose_list(Node **rt, int size)
{
	Node** new = (Node**)malloc(sizeof(Node*) * (size + 1)) ;
	for(int i = 0 ; i <= size ; i++)
	{
		new[i] = (Node*)malloc(sizeof(Node)) ;
		new[i]->next = NULL ;
	}

	for(int i = size ; i >= 1 ; i--)
	{
		Node *temp = rt[i]->next ;
		while(temp != NULL)
		{
			push_node(new[temp->idx], i) ;
			temp = temp->next ;
		}
	}

	return new ;
}
