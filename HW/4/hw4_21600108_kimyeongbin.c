// My program works for all processes

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXTIME 900 // unit : second

typedef struct
{
		int level ;
		int benefit ;
		int weight ;
		float bound ;
} Node ;

typedef struct
{
		Node *queue ;
		int size ;
} Nqueue ;

Nqueue q ; // make a queue structure

clock_t start ;

float Greedy(int *input, int i, int weight[], int value[]) ;
int Dp(int *input, int i, int weight[], int value[]) ;
int BranchAndBound(int *input, int i, int weight[], int value[]) ;
int *GetValue(int num) ;
int *GetWeight(int num) ;
void desc(int num, float ratio[], int weight[], int value[]) ;
float GetBound(Node *n, int size, int cap, int *weight, int *value) ;
int q_push(Node in, int size) ;
void h_swap(Node *x, Node *y) ;
Node q_pop() ;
void f_swap(float *x, float *y) ;
void i_swap(int *x, int *y) ;

int
main(void) 
{
  	int InputNum[9] = {10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000} ;
    int Numsize = sizeof(InputNum) / sizeof(InputNum[0]) ;
    double greedy, dynamic, branch ;
    float g_max = 0.0 ;
    int b_max = 0, d_max = 0 ;
	FILE *fp = fopen("output.txt", "w") ;

    printf("Items\tGreedy\t\t\tD. P.\t\t\tB. & B.\n") ;
    printf("----------------------------------------------------------------\n") ;
   
    fprintf(fp, "Items\tGreedy\t\t\tD. P.\t\t\tB. & B.\n") ;
    fprintf(fp, "----------------------------------------------------------------\n") ;

    for(int i = 0 ; i < Numsize ; i++)
    {
        int *value = GetValue(InputNum[i]) ;
        int *weight = GetWeight(InputNum[i]) ;
        float *ratio = (float*)malloc(sizeof(float) * InputNum[i]) ;
        for(int j = 0 ; j < InputNum[i] ; j++) ratio[j] = (float)value[j] / weight[j] ;
        
        start = clock() ;
        d_max = Dp(InputNum, i, weight, value) ;
        dynamic = (double)(clock() - start) ;
        
        desc(InputNum[i], ratio, weight, value) ;
        free(ratio) ;
        
        start = clock() ;
        g_max = Greedy(InputNum, i, weight, value) ;
        greedy = (double)(clock() - start) ;
        
        start = clock() ;
        b_max = BranchAndBound(InputNum, i, weight, value) ;
        branch = (double)(clock() - start) ;
        
        printf("%d\t%.3f/%.3lf\t\t%d/%.3lf\t\t\%d/%.3lf\n", InputNum[i], g_max, greedy, d_max, dynamic, b_max, branch) ;
        fprintf(fp, "%d\t%.3f/%.3lf\t\t%d/%.3lf\t\t\%d/%.3lf\n", InputNum[i], g_max, greedy, d_max, dynamic, b_max, branch) ;
        
		free(value) ;
        free(weight) ;
        
        sleep(2) ;
    }
 	
	fclose(fp) ;

	return 0;
}

float Greedy(int *input, int i, int weight[], int value[])
{
    int k = 0 ;
    float max = 0.0 ;
    int cap = input[i] * 40 ;
    float *result = (float*)calloc(input[i], sizeof(float)) ; // init elements '0'

    for(k = 0 ; k < input[i] ; k++)
    {
        if(weight[k] > cap) break ;
        else
        {
            result[k] = 1.0 ;
            max += (float)value[k] ;
            cap -= weight[k] ;
        
		}
		if(((clock() - start) / CLOCKS_PER_SEC) > MAXTIME) return 0 ;
    }
    
    if(k < input[i])
    {
        result[k] = (float)cap / weight[k] ;
        max += result[k] * (float)value[k] ;
    }

    free(result) ;
    
    return max ;
}

int Dp(int *input, int i, int weight[], int value[])
{
    int max = 0 ;
    int cap = input[i] * 40 ;
    int **benefit ;
	
    benefit = (int**)malloc(sizeof(int*) * 2) ; // 2 size, not input[i] + 1
    for(int j = 0 ; j <= 1 ; j++) benefit[j] = (int*)malloc(sizeof(int) * (cap + 1)) ;
    
    for(int k = 0 ; k <= cap ; k++) benefit[0][k] = 0 ;
    for(int k = 0 ; k <= 1 ; k++) benefit[k][0] = 0 ;

    for(int k = 1 ; k <= input[i] ; k++)
    {
	    if(((clock() - start) / CLOCKS_PER_SEC) > MAXTIME) return 0 ;
        for(int j = 1 ; j <= cap ; j++)
        {
            if(weight[k - 1] <= j)
            {
                if(value[k - 1] + benefit[0][j - weight[k - 1]] > benefit[0][j]) benefit[1][j] = value[k - 1] + benefit[0][j - weight[k - 1]] ;
                else benefit[1][j] = benefit[0][j] ;
            }
            else benefit[1][j] = benefit[0][j] ;
		}

        for(int j = 0 ; j <= cap ; j++) benefit[0][j] = benefit[1][j] ;
    }
    
    max = benefit[1][cap] ;

    for(int j = 0 ; j <= 1 ; j++) free(benefit[j]) ;
    free(benefit) ;

    return max ;
}

int BranchAndBound(int *input, int i, int weight[], int value[])
{
    int max_benefit = 0 ;
    int cap = input[i] * 40 ;
    Node r, t ; // root, temp Node
    q.queue = (Node*)malloc(sizeof(Node) * (input[i] * 2)) ;
    q.size = 0 ;

    t.level = -1, t.weight = 0, t.benefit = 0 ; // Using temp node
    q_push(t, input[i] * 2) ;

    while(q.size != 0)
    {
	    if(((clock() - start) / CLOCKS_PER_SEC) > MAXTIME) return 0 ;
        
		t = q_pop() ;
	
        if(t.level == input[i] - 1) continue ;
				 
        if(t.bound < max_benefit) continue ;

        // Get right node that contains item.
        r.level = t.level + 1 ;
        r.weight = t.weight + weight[r.level] ;
        r.benefit = t.benefit + value[r.level] ;
        if(r.weight <= cap && r.benefit > max_benefit) max_benefit = r.benefit ;

        r.bound = GetBound(&r, input[i], cap, weight, value) ;
				  
        if(r.bound > max_benefit) q_push(r, input[i] * 2) ;
        
        // Get letf node that does not contains item.
        r.weight = t.weight ;
        r.benefit = t.benefit ;
        r.bound = GetBound(&r, input[i], cap, weight, value) ;
        
        if(r.bound > max_benefit) q_push(r, input[i] * 2) ;
    }

    free(q.queue) ;
    
    return max_benefit ;
}

float GetBound(Node *n, int size, int cap, int *weight, int *value)
{
		if(n->weight >= cap) return 0 ;

		float profit_bound = (float)n->benefit ;
		int tot_weight = n->weight ;
		int idx = n->level + 1 ;

		while(idx < size && (tot_weight + weight[idx] <= cap))
		{
				tot_weight += weight[idx] ;
				profit_bound += (float)value[idx] ;
				idx++ ;
		}
		
		if(idx < size) profit_bound += (cap - tot_weight) * (float)value[idx] / weight[idx] ;

		return profit_bound ;
}

int *GetValue(int num)
{
  	srand(time(NULL)) ;

  	int *value = (int*)malloc(sizeof(int) * num) ;

  	for(int i = 0 ; i < num ; i++) value[i] = (rand() % 300) + 1 ;

  	return value ;
}

int *GetWeight(int num)
{
  	int *weight = (int*)malloc(sizeof(int) * num) ;

  	for(int i = 0 ; i < num ; i++) weight[i] = (rand() % 100) + 1 ;

  	return weight ;
}

void i_swap(int *x, int *y)
{
    int temp = *x ;
    *x = *y ;
    *y  = temp ;
}

void f_swap(float *x, float *y)
{
    float temp = *x ;
    *x = *y ;
    *y  = temp ;
}

void desc(int num, float ratio[], int weight[], int value[])
{
    for(int i = 0 ; i < num ; i++)
    {
        for(int j = i ; j < num ; j++)
        {
            if(ratio[i] < ratio[j])
            {
                f_swap(&ratio[i],&ratio[j]) ;
                i_swap(&weight[i],&weight[j]) ;
                i_swap(&value[i],&value[j]) ;
            }
        }
    }
}

Node q_pop()
{
	Node temp = q.queue[0] ; // maintain root node 
    q.size-- ; // delete node

    q.queue[0] = q.queue[q.size] ;

    int cur = 0 ;
    int left = cur * 2 + 1 ;
    int right = cur * 2 + 2 ;
    int pivot = cur ; // change current

    while (left < q.size) 
	{
        if (q.queue[pivot].bound < q.queue[left].bound) pivot = left ;
        if (right < q.size && q.queue[pivot].bound < q.queue[right].bound) pivot = right ;

        if (pivot == cur) break ;
        else
		{
            h_swap(&q.queue[cur], &q.queue[pivot]) ;
            cur = pivot ;
            left = cur * 2 + 1 ;
            right = cur * 2 + 2 ;
        }
    }

    return temp ;
}

void h_swap(Node *x, Node *y)
{
		Node temp = *x ;
		*x = *y ;
		*y = temp ;
}

int q_push(Node in, int size) 
{
    if (q.size + 1 > size) return 0 ; // full

    q.queue[q.size] = in ; // insert node at leaf
    int cur = q.size ;
    int par = (q.size - 1) / 2 ;
    // swim (standard : bound value)
    while (cur > 0 && q.queue[cur].bound > q.queue[par].bound) 
	{
        h_swap(&q.queue[cur], &q.queue[par]) ;
        cur = par ;
        par = (par - 1) / 2 ;
    }

    q.size++;

    return 1;
}
