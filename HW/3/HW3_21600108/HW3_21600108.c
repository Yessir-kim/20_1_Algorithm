/* This assignment was conducted on repl.it */

#include <stdio.h>
#include <time.h>

int re_combiantion(int n, int r) ;
int dp_combination(int n, int r) ;

int main(void) {
  int N, R ;
  int result ;
  clock_t start, end ;

  do
  {
    printf("Input numbers N, R : ") ;
    scanf("%d %d",&N,&R) ;
  } while(N < 0 || R < 0) ;
  
  start = clock() ;
  result = re_combiantion(N, R) ;
  end = clock() ;

  printf("Recursive result is %d // Time : %f\n",result,(end - start)/(double)1000) ;

  start = clock() ;
  result = dp_combination(N, R) ;
  end = clock() ;

  printf("Dynamic result is %d // Time : %f\n",result,(end - start)/(double)1000) ;

  return 0 ;
}

int re_combiantion(int n, int r)
{
  if(n == 0) return 0 ; // Formula 3
  if(n == r || r == 0) return 1 ; // Formula 2, 4
  
  return re_combiantion(n-1, r-1) + re_combiantion(n-1, r) ; // Formula 1
}

int dp_combination(int n, int r)
{
  int com[n+1][r+1] ;

  for(int i = 0 ; i <= n ; i++) // increasing n variable 
  {
    for(int j = 0 ; j <= r ; j++) // increasing r variable
    {
      if(i == j) com[i][j] = 1 ; // formula 4 when n == r
      else if(j == 0) com[i][j] = 1 ; // formula 2 when r == 0
      else if(i == 0) com[i][j] = 0 ; // formula 3 when n == 0
      else com[i][j] = com[i-1][j-1] + com[i-1][j] ;
    }
  }

  return com[n][r] ;
}