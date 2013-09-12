#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  printf("\n---- Running test8 ---\n");
	
	//Free an object at the beginning of  the heap to test fence posts.
  int *p1,*p2,*p3,*p4,*p5;
  
  p1=(int *)malloc(8);
  p2=(int *)malloc(8);
  p3=(int *)malloc(8);
  p4=(int *)malloc(8);
  p5=(int *)malloc(8);

  free(p2);
  free(p3);
  free(p4);
  free(p5);
  
  free(p1);
  
  printf(">>>> test8 passed\n\n");

  exit(0);
}
