#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test12 ---\n");
  
	//Free an object that was not allocated should not crash.
  int * p = (int *) malloc( 20 );
  *p = 10;
  free( p );

  free( (void *)-14 );
  free( (void *)12 );
  free( (void *)10000 );
  
  printf(">>>> test12 passed\n\n");

  exit(0);
}
