#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test4 ---\n");

	//Free an object that needs to be coalesced with the left neighbor only
  int * p1 = (int *) malloc( 16 );
  *p1 = 10;
  
  int * p2 = (int *) malloc( 16 );
  *p2 = 10;
  
  free( p1 );
  free( p2 );

  int * p = (int *) malloc( 32 );
  *p = 10;
  free( p );
  
  printf(">>>> test4 passed\n\n");

  exit(0);
}
