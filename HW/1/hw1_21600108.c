/* This assignment was conducted on repl.it */

#include <stdio.h>
#include <string.h>

// Declare heap structure. 
typedef struct pq
{
  char heap_name[31][20] ; // 2D array 
  int heap_num[31] ;
  int size ;
} Queue ;

// Decalre Function.
void swap_value(int *a, int *b) ;
void swap_name(char list[][20], int name1, int name2) ;
void heapify(Queue *pq, int current, int parent) ;
void Insert_que(Queue *pq, char* name, int value) ;
void Delete_que(Queue *pq) ;

int main(void) {
  char menu ; // Menu를 받을 때 사용하는 Variable
  int flag = 1 ; // While loop의 true, false를 판단하는 Variable
  char name[20] ; // Name을 받을 때 사용하는 Variable
  int value, idx ; // Value, index를 받을 때 사용하는 Variable
  Queue pq ;
  pq.size = 0 ; // init size variable.

  while(flag)
  {
    // Menu 
    printf("******************** MENU **********************\n") ;
    printf("I : Insert new element into queue.\n") ;
    printf("D : Delete element with largest key from queue.\n") ;
    printf("R : Retrieve element with largest key from queue.\n") ;
    printf("K : Increase key of element in queue.\n") ;
    printf("P : print out all elements in queue.\n") ;
    printf("Q : Quit\n\n") ;
    printf("Choose menu : ") ;
    scanf("%c",&menu) ;
    getchar() ; // Remove buffer.

    switch(menu)
    {
      case 'I': // If 'I'
        if(pq.size == 30) // Exception handling.
        {
          printf("Size is full!\n\n") ;
          break ;
        }
        printf("Enter name of element : ") ;
        scanf("%s", name) ;
        printf("Enter key value of element : ") ;
        scanf("%d", &value) ;
        Insert_que(&pq, name, value) ; // Call insertion func.
        printf("New element [%s, %d] is inserted\n\n",name, value) ;
        getchar() ; // Remove buffer.
        break ;
      case 'D': // If 'D'
        if(pq.size == 0) printf("Not exist element!\n\n") ; // If size == 0
        else
        {
          printf("[%s, %d] is deleted.\n\n", pq.heap_name[1], pq.heap_num[1]) ;
          Delete_que(&pq) ; // Call deletion func.
        }
        break ;
      case 'R': // If 'R'
        if(pq.size == 0) printf("Not exist element!\n\n") ; // If size == 0
        else printf("[%s, %d]\n\n", pq.heap_name[1], pq.heap_num[1]) ;
        break ;
      case 'K': // If 'K'
        if(pq.size == 0) printf("Not exist element!\n\n") ; // If size == 0
        else
        {
          printf("Enter index of element : ") ;
          scanf("%d", &idx) ;
          printf("Enter new key value : ") ;
          scanf("%d", &value) ;
          if(idx > pq.size) printf("Index out of range!\n\n") ; // Exception handling.
          else
          {
            printf("\n") ;
            pq.heap_num[idx] = value ;
            heapify(&pq, idx, idx / 2) ; // Call heapify func.
          }    
          getchar() ; // Remove buffer.
        }
        break ;
      case 'P': // If 'P'
        if(pq.size == 0) printf("Not exist element!\n\n") ; // If size == 0
        else 
        {
          for(int i = 1 ; i <= pq.size ; i++) printf("[%s, %d] ", pq.heap_name[i], pq.heap_num[i]) ;
          printf("\n\n") ;
        }
        break ;
      case 'Q': // If 'Q'
        printf("Thank you, Bye!\n") ;
        flag = 0 ;
        break ;
    }  
  }

  return 0 ;
}

void swap_value(int *a, int *b) // Swap a and b.
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void swap_name(char list[][20], int name1, int name2) // Swap name1 index and name2 index.
{
  char temp[20] ;
  strcpy(temp, list[name1]) ;
  strcpy(list[name1], list[name2]) ;
  strcpy(list[name2], temp) ;
}

// Continue to repeat if the value of the current is greater than 1 and is greater than the value of the parent.
void heapify(Queue *pq, int current, int parent)
{
  while(current > 1 && (pq->heap_num[current] > pq->heap_num[parent]))
    {
      swap_value(&pq->heap_num[current], &pq->heap_num[parent]) ;
      swap_name(pq->heap_name, current, parent) ;
      current = parent ;
      parent = parent / 2 ;
    }
}

void Insert_que(Queue *pq, char* name, int value)
{
  if(pq->size == 0) // If size == 0
  {
    strcpy(pq->heap_name[1], name) ; // Using Strcpy func to copy string.
    pq->heap_num[1] = value ;
    pq->size += 1 ;
  }
  else
  {
    pq->size += 1 ;
    pq->heap_num[pq->size] = value ; // Set value at index end.
    strcpy(pq->heap_name[pq->size], name) ; // Set name at index end.

    heapify(pq, pq->size, pq->size / 2) ; // Call heapify func
  }
}

void Delete_que(Queue *pq)
{
  // Swap root node and end node.
  swap_value(&pq->heap_num[1], &pq->heap_num[pq->size]) ;
  swap_name(pq->heap_name, 1, pq->size) ;

  // Decrease heap size and call heapify func.
  pq->size -= 1 ;
  heapify(pq, pq->size, pq->size / 2) ;
}