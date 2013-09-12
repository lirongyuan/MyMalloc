#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test10 ---\n");

	//See that memory is being reused
  int * p = (int *) malloc( 20 );
  *p = 10;
  free( p );

  p = (int *) malloc( 20 );
  *p = 10;
  free( p );
  
  printf(">>>> test10 passed\n\n");

  exit(0);
}
