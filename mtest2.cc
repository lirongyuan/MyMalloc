#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test2 ---\n");
  
  //Malloc an object from a free list that needs to be split.
  int * p = (int *) malloc( 20 );
  *p = 10;
  free( p );

  p = (int *) malloc( 8 );
  *p = 10;
  free( p );
  
  printf(">>>> test2 passed\n\n");

  exit(0);
}
