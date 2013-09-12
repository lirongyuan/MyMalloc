#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test6 ---\n");

	//Free an object that needs to be coalesced with both right and left neighbors
  int * p1 = (int *) malloc( 16 );
  *p1 = 10;
  
  int * p2 = (int *) malloc( 16 );
  *p2 = 10;
  
  int * p3 = (int *) malloc( 16 );
  *p2 = 10;
  
  free( p3 );
  free( p1 );
  
  free( p2 );

  int * p = (int *) malloc( 48 );
  *p = 10;
  free( p );
  
  printf(">>>> test6 passed\n\n");

  exit(0);
}
