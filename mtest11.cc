#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test11 ---\n");

	//Free an already freed object should not crash
  int * p1 = (int *) malloc( 20 );
  *p1 = 10;
  free( p1 );
  free( p1 );
  free( p1 );
  
  int * p2 = (int *) malloc( 20 );
  *p2 = 10;
  free( p1 );
  free( p2 );
  free( p2 );
  
  int * p3 = (int *) malloc( 20 );
  *p3 = 10;
  free( p2 );
  free( p3 );
  free( p3 );
  printf(">>>> test11 passed\n\n");

  exit(0);
}
