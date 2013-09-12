#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test7 ---\n");
  
	//Free an object that does not need to be coalesced
  int * p = (int *) malloc( 20 );
  *p = 10;
  free( p );
  
  printf(">>>> test7 passed\n\n");

  exit(0);
}
