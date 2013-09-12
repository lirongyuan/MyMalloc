#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test3 ---\n");

  //Malloc an object from a  free list that if split will give a remainder too small to be used in another allocation
  int * p = (int *) malloc( 20 );
  *p = 10;
  free( p );

  p = (int *) malloc( 16);
  *p = 10;
  
  
  char * m= (char *) malloc (4);
  *m='c';
  
  free( p );
  
  free(m);
  
  printf(">>>> test3 passed\n\n");

  exit(0);
}
