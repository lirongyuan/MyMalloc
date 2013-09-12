//
// MyMalloc Project
// Lirong Yuan
//
//
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <limits.h>

#define DEBUG 0
#define HEAPMEMBUG 0

//thread stuff
pthread_mutex_t mutex;
int init=0;

enum {
  ObjFree = 0,
  ObjAllocated = 1
};

// Header of an object. Used both when the object is allocated and freed
class ObjectHeader {
 public:
  int _flags;		      // flags == ObjFree or flags = ObjAllocated
  size_t _objectSize;         // Size of the object. Used both when allocated and freed.
};


class FreeObjectHeader {
 public:
  int _flags;		      // flags == ObjFree or flags = ObjAllocated
  size_t _objectSize;         // Size of the object. Used both when allocated
			      // and freed.
  ObjectHeader *next;
  ObjectHeader *prev;

};

class Allocator {
  // State of the allocator

  // Size of the heap
  size_t _heapSize;

  // True if heap has been initialized
  int _initialized;

  // Verbose mode
  int _verbose;

  // # malloc calls
  int _mallocCalls;

  // # free calls
  int _freeCalls;

  // # realloc calls
  int _reallocCalls;

  // # realloc calls
  int _callocCalls;

  // array of pointers
  ObjectHeader *head_arr[65];

  int size_arr[65];
  void * lastLoc;

  int a;
  char *alloc_arr[100000];

  //malloc policy
  int mallocPolicy;

public:

  // This is the only instance of the allocator.
  static Allocator TheAllocator;

  //Initializes the heap
  void initialize();

  // Allocates an object
  void * allocateObject( size_t size );

  // Frees an object
  void freeObject( void * ptr );

  // Returns the size of an object
  size_t objectSize( void * ptr );

  // At exit handler
  void atExitHandler();

  //Prints the heap size and other information about the allocator
  void print();

  // Gets memory from the OS
  void * getMemoryFromOS( size_t size );

  void increaseMallocCalls() { _mallocCalls++; }

  void increaseReallocCalls() { _reallocCalls++; }

  void increaseCallocCalls() { _callocCalls++; }

  void increaseFreeCalls() { _freeCalls++; }

  //My methods
  void * addToListFree(void *mem, int index, int size);
  void * addToListAlloc(void *mem, int index, int size);
  void * findFreeBlock(int size, int index);
  void * findBestFreeBlock(int size, int index);
  int freeFromList(void *ptr, int index);

  void CHECKHEAP();

};

Allocator Allocator::TheAllocator;

extern "C" void
atExitHandlerInC()
{
  Allocator::TheAllocator.atExitHandler();
}

void
Allocator::initialize()
{
  // Environment var VERBOSE prints stats at end and turns on debugging
  // Default is on
  _verbose = 1;

  const char * envverbose = getenv( "MALLOCVERBOSE" );
  if ( envverbose && !strcmp( envverbose, "NO") ) {
    _verbose = 0;
  }

  // In verbose mode register also printing statistics at exit
  atexit( atExitHandlerInC );

  _initialized = 1;

  //Get mallocPolicy, firstfit=0, bestfit=1.
  const char * envmalloc = getenv( "MALLOCPOLICY" );
  if ( envmalloc && !strcmp( envmalloc, "BESTFIT") ) {
    mallocPolicy = 1;
  }

  //init the array
  for(int i=0;i<=64;i++){
  	head_arr[i]=NULL;
  	size_arr[i]=0;
  }

  lastLoc=(void *)-1;

  a=0;
  for(int i=0;i<100000;i++){
  	alloc_arr[i]=0;
  }
}

void *
Allocator::allocateObject( size_t size )
{
	size_t round_size, real_size;
	int list_index,i,index2,size2,fence;
	ObjectHeader *ent,*foot,*ent1,*ent2,*o,*f;
	ObjectHeader **ent_next;
	void *mem,*newLoc,*addr;

  	//Make sure that allocator is initialized
  	if ( !_initialized ) {
    	initialize();
  	}

	  //Calculate sizes
  	round_size = (size + 7) & ~7;
  	real_size = round_size + 3*sizeof(ObjectHeader);
  	list_index=(real_size>=512)?64:(real_size>>3);

  	//Check existence of free blocks inside head_arr[(others)]
  	if(mallocPolicy){
  		for(i=list_index;i<=64;i++){
  				ent1=(ObjectHeader *)findBestFreeBlock(real_size,i);
  				if(ent1!=NULL){
  					if(ent1->_objectSize<real_size+32){//Do not split
  						ent1->_flags=ObjAllocated;
  						foot=(ObjectHeader *)((char *)ent1+ent1->_objectSize-sizeof(ObjectHeader));
  						foot->_flags=ObjAllocated;

  						addr=(void *)((char *)ent1+2*sizeof(ObjectHeader));
  						if(a<100000)
  							alloc_arr[a++]=(char *)addr;
  						return addr;
  					}
  					//Split
  					assert(freeFromList(ent1,i)==1);
  					ent2=(ObjectHeader *)((char *)ent1+real_size);
  					size2=(ent1->_objectSize-real_size);
  					index2=(size2>=512)?64:(size2>>3);

  					addToListFree(ent2,index2,size2);

  					addr=addToListAlloc(ent1,list_index,real_size);
  					if(a<100000)
  						alloc_arr[a++]=(char *)addr;
  					return addr;
  				}
  		}
  	}else{
  		for(i=64;i>=list_index;i--){
  				ent1=(ObjectHeader *)findFreeBlock(real_size,i);
  				if(ent1!=NULL){
  					if(ent1->_objectSize<real_size+32){//Do not split
  						ent1->_flags=ObjAllocated;
  						foot=(ObjectHeader *)((char *)ent1+ent1->_objectSize-sizeof(ObjectHeader));
  						foot->_flags=ObjAllocated;

  						addr=(void *)((char *)ent1+2*sizeof(ObjectHeader));
  						if(a<100000)
  							alloc_arr[a++]=(char *)addr;
  						return addr;
  					}
  					//Split
  					assert(freeFromList(ent1,i)==1);
  					ent2=(ObjectHeader *)((char *)ent1+real_size);
  					size2=(ent1->_objectSize-real_size);
  					index2=(size2>=512)?64:(size2>>3);

  					addToListFree(ent2,index2,size2);

  					addr=addToListAlloc(ent1,list_index,real_size);
  					if(a<100000)
  						alloc_arr[a++]=(char *)addr;
  					return addr;
  				}
  		}
  	}

  	//Otherwise get memory from OS
  	fence=0;
  	mem = getMemoryFromOS( real_size + 2*sizeof(ObjectHeader));

  	if(HEAPMEMBUG)
  		printf("lastLoc:%p  mem:%p\n",lastLoc,mem);


  	//add dommy footer @ end
  	o=(ObjectHeader *)((char *)mem+sizeof(ObjectHeader)+real_size);
  	o->_objectSize=0;
  	o->_flags=ObjAllocated;

  	if(mem!=lastLoc){
  		fence=1;
  		//add dommy footer @ head
  		f=(ObjectHeader *)mem;
  		f->_objectSize=0;
  		f->_flags=ObjAllocated;

  		lastLoc=(void *)((char *)mem+real_size + 2*sizeof(ObjectHeader));

  		addr=addToListAlloc((void *)((char *)mem+sizeof(ObjectHeader)), list_index,real_size);
  		if(a<100000)
  			alloc_arr[a++]=(char *)addr;
  		return addr;
  	}

	  lastLoc=(void *)((char *)mem+real_size + 2*sizeof(ObjectHeader));

  	real_size = real_size + 2*sizeof(ObjectHeader);
  	list_index=(real_size>=512)?64:(real_size>>3);

  	addr=addToListAlloc((void *)((char *)mem-sizeof(ObjectHeader)), list_index,real_size);
  	if(a<100000)
  		alloc_arr[a++]=(char *)addr;
  	return addr;
}

void
Allocator::freeObject( void * ptr )
{
	//Check if ptr has been allocated
	int cont=0;
	for(int i=0;i<a;i++){
		if(alloc_arr[i]==ptr){
			cont=1;
			break;
		}
	}
	if(cont==0)return;

	void *memstart;
	ObjectHeader *o,*f,*h;
	size_t real_size,total_size;
	int changed=0,index1,index2,index;

  	// Return the object to the free list sorted by address and you will coalesce it.
  	// if possible.
	h=(ObjectHeader *)( (char *) ptr - 2*sizeof(ObjectHeader) );
    real_size=h->_objectSize;
    total_size=h->_objectSize;

    //check the header of the right neighbor
    o=(ObjectHeader *)( (char *)h + real_size  );
    if(o->_objectSize!=0 && o->_flags==ObjFree){
    	changed=1;
    	memstart=(void *)h;
    	index=(o->_objectSize>=512)?64:(o->_objectSize>>3);
    	assert(freeFromList(o, index)==1);
    	total_size+=o->_objectSize;
    }

    //check the footer of the left neighbor
    f=(ObjectHeader *)( (char *)h -sizeof(ObjectHeader) );

    if(f->_objectSize!=0 && f->_flags==ObjFree){
    	changed=1;
    	memstart=(void *)((char *)h - f->_objectSize);
    	index=(f->_objectSize>=512)?64:(f->_objectSize>>3);
    	assert(freeFromList(memstart, index)==1);
    	total_size+=f->_objectSize;
    }


    //Free the original one and place it in the corresponding list
    if(changed){
    	index1=(real_size>=512)?64:(real_size>>3);
    	index2=(total_size>=512)?64:(total_size>>3);
    	assert(freeFromList((void *)((char *) ptr - 2*sizeof(ObjectHeader)),index1)==1);
    	addToListFree(memstart,index2,total_size);
    }else{
    	o=(ObjectHeader *)h;
    	real_size=o->_objectSize;
    	o->_flags = ObjFree;

    	f=(ObjectHeader *)( (char *)h + real_size -sizeof(ObjectHeader) );
    	f->_flags = ObjFree;
    }

    if(HEAPMEMBUG)
    	printf("free... real_size:%d coa_size:%d...\n",real_size,total_size);

}

size_t
Allocator::objectSize( void * ptr )
{
  // Return the size of the object pointed by ptr. We assume that ptr is a valid obejct.
  ObjectHeader * o =
    (ObjectHeader *) ( (char *) ptr - sizeof(ObjectHeader) );
  // Substract the size of the header
  return o->_objectSize - 3*sizeof(ObjectHeader);
}

void
Allocator::print()
{


  printf("\n-------------------\n");

  printf("Mode:\t%s\n", (mallocPolicy)?"BestFit":"FirstFit" );
  printf("HeapSize:\t%d bytes\n", _heapSize );
  printf("# mallocs:\t%d\n", _mallocCalls );
  printf("# reallocs:\t%d\n", _reallocCalls );
  printf("# callocs:\t%d\n", _callocCalls );
  printf("# frees:\t%d\n", _freeCalls );

  printf("\n-------------------\n");


}

void *
Allocator::getMemoryFromOS( size_t size )
{
  // Use sbrk() to get memory from OS
  _heapSize += size;
  if(HEAPMEMBUG)
  	printf("HeapSize+=%d...\n",size);
  return sbrk( size );
}

void
Allocator::atExitHandler()
{
  // Print statistics when exit
  if ( _verbose ) {
    print();
  }
}

//
// C interface
//

extern "C" void *
malloc(size_t size)
{
  if(!init){
  	pthread_mutex_init(&mutex,NULL);
  	init=1;
  }

  pthread_mutex_lock(&mutex);
  Allocator::TheAllocator.increaseMallocCalls();
  pthread_mutex_unlock(&mutex);

  pthread_mutex_lock(&mutex);
  void *addr=Allocator::TheAllocator.allocateObject( size );
  pthread_mutex_unlock(&mutex);

  return addr;
}

extern "C" void
free(void *ptr)
{
  if(!init){
  	pthread_mutex_init(&mutex,NULL);
  	init=1;
  }

  pthread_mutex_lock(&mutex);
  Allocator::TheAllocator.increaseFreeCalls();
  pthread_mutex_unlock(&mutex);

  if ( ptr == 0 ) {
    // No object to free
    return;
  }

  pthread_mutex_lock(&mutex);
  Allocator::TheAllocator.freeObject( ptr );
  pthread_mutex_unlock(&mutex);
}

extern "C" void *
realloc(void *ptr, size_t size)
{
  pthread_mutex_lock(&mutex);
  Allocator::TheAllocator.increaseReallocCalls();
  // Allocate new object
  void * newptr = Allocator::TheAllocator.allocateObject( size );
  // Copy old object only if ptr != 0
  if ( ptr != 0 ) {
    // copy only the minimum number of bytes
    size_t sizeToCopy =  Allocator::TheAllocator.objectSize( ptr );
    if ( sizeToCopy > size ) {
      sizeToCopy = size;
    }
    memcpy( newptr, ptr, sizeToCopy );
    //Free old object
    Allocator::TheAllocator.freeObject( ptr );
  }
  pthread_mutex_unlock(&mutex);
  return newptr;
}

extern "C" void *
calloc(size_t nelem, size_t elsize)
{
  pthread_mutex_lock(&mutex);
  Allocator::TheAllocator.increaseCallocCalls();
  // calloc allocates and initializes
  size_t size = nelem * elsize;
  void * ptr = Allocator::TheAllocator.allocateObject( size );

  if ( ptr ) {
    // No error
    // Initialize chunk with 0s
    memset( ptr, 0, size );
  }
  pthread_mutex_unlock(&mutex);
  return ptr;
}

extern "C" void
checkHeap()
{
	// Verifies the heap consistency by iterating over all objects
	// in the free lists and checking that the next, previous pointers
	// size, and boundary tags make sense.
	// The checks are done by calling assert( expr ), where "expr"
	// is a condition that should be always true for an object.
	//
	// assert will print the file and line number and abort
	// if the expression "expr" is false.
	//
	pthread_mutex_lock(&mutex);
	Allocator::TheAllocator.CHECKHEAP();
	pthread_mutex_unlock(&mutex);
}



int
Allocator::freeFromList(void *ptr, int index){
	//CHECKHEAP();
	if(ptr==NULL)
		return 0;

	void *nextptr,*prevptr;
	ObjectHeader *ent,*o,*f;
	ObjectHeader **ent_next,**curr_next,**curr_prev,**next_next,**next_prev,**prev_prev,**prev_next;

	o=(ObjectHeader *)ptr;
    o->_flags = ObjFree;
    f=(ObjectHeader *)( (char *)ptr + o->_objectSize -sizeof(ObjectHeader) );
    f->_flags = ObjFree;

    for(ent=head_arr[index];ent!=NULL;){
    	if(ent==ptr){
    		curr_next=(ObjectHeader **)((char *)ptr + sizeof(ObjectHeader));
    		curr_prev=curr_next+1;

    		prevptr=(void *)(*curr_prev);
    		nextptr=(void *)(*curr_next);

    		*curr_next=NULL;*curr_prev=NULL;

    		if(prevptr!=NULL){
    			prev_next=(ObjectHeader **)((char *)prevptr + sizeof(ObjectHeader));
    			*prev_next=(ObjectHeader *)nextptr;
    		}

    		if(nextptr!=NULL){
    			next_next=(ObjectHeader **)((char *)nextptr + sizeof(ObjectHeader));
    			next_prev=next_next+1;
    			*next_prev=(ObjectHeader *)prevptr;
    		}

    		if(head_arr[index]==ptr){
    			head_arr[index]=(ObjectHeader *)nextptr;
    		}

    		size_arr[index]--;
    		return 1;
    	}
    	ent_next=(ObjectHeader **)((char *)ent+sizeof(ObjectHeader));
    	ent=*ent_next;
    }
	return 0;
}


void *
Allocator::addToListFree(void *mem, int index, int size){ //mem=the start of the whole memory
	ObjectHeader *ent,*o,*f;
	ObjectHeader **ent_next,**next,**prev;

  	//header
  	o = (ObjectHeader *) mem;
  	o->_objectSize = size;
  	o->_flags = ObjFree;

  	//footer
  	f = (ObjectHeader *)((char *)mem+size-sizeof(ObjectHeader));
  	f->_objectSize = size;
  	f->_flags = ObjFree;

  	size_arr[index]++;

  	//add the new memory to the list as a head
  	if(head_arr[index]==NULL){
  		head_arr[index] = (ObjectHeader *) mem;
  		//set up next and prev
  		next=(ObjectHeader **)((char *)mem +sizeof(ObjectHeader));
  		prev=1+next;
		*next=NULL;
		*prev=NULL;
		return (void *) ((char *)mem + 2*sizeof(ObjectHeader));
	}

  	//add the next memory to the list not as a head
  	ent=head_arr[index];
	ent_next=(ObjectHeader **)((char *)ent+sizeof(ObjectHeader));
  	while((*ent_next)!=0){
	  	ent=(*ent_next);
	  	ent_next=(ObjectHeader **)((char *)ent+sizeof(ObjectHeader));
  	}
  	*ent_next=(ObjectHeader *)mem;
  	//set up next and prev
  	next=(ObjectHeader **)((char *)mem + sizeof(ObjectHeader));
	prev=1+next;
 	*next=NULL;
  	*prev=ent;
  	return (void *)((char *)mem + 2*sizeof(ObjectHeader));// Return the pointer after the object header.
}

void *
Allocator::addToListAlloc(void *mem, int index, int size){ //mem=the start of the whole memory
	ObjectHeader *ent,*o,*f;
	ObjectHeader **ent_next,**prev,**next;

  	//header
  	o = (ObjectHeader *) mem;
  	o->_objectSize = size;
  	o->_flags = ObjAllocated;
  	//footer
  	f = (ObjectHeader *)((char *)mem+size-sizeof(ObjectHeader));
  	f->_objectSize = size;
  	f->_flags = ObjAllocated;

  	size_arr[index]++;

  	//add the new memory to the list as a head
  	if(head_arr[index]==NULL){
  		head_arr[index] = (ObjectHeader *) mem;
  		next=(ObjectHeader **)((char *)mem +sizeof(ObjectHeader));
  		prev=1+next;
		*next=NULL;
		*prev=NULL;
		return (void *) ((char *)mem + 2*sizeof(ObjectHeader));
	}

  	//add the next memory to the list not as a head
  	ent=head_arr[index];
	ent_next=(ObjectHeader **)((char *)ent+sizeof(ObjectHeader));
  	while((*ent_next)!=NULL){
	  	ent=(*ent_next);
	  	ent_next=(ObjectHeader **)((char *)ent+sizeof(ObjectHeader));
  	}
  	*ent_next=(ObjectHeader *)mem;

  	next=(ObjectHeader **)((char *)mem + sizeof(ObjectHeader));
	prev=1+next;
 	*next=NULL;
  	*prev=ent;
  	return (void *)((char *)mem + 2*sizeof(ObjectHeader));// Return the pointer after the object header.
}

void *
Allocator::findFreeBlock(int size, int index){
	ObjectHeader *ent;
	ObjectHeader **ent_next;
	if(head_arr[index]!=NULL && size_arr[index]!=0){
		ent=head_arr[index];
		while(ent!=NULL){
			if(ent->_flags==ObjFree){//check if this obj is free or not
  				if(index!=64 || size<=ent->_objectSize){
  					return (void *)ent;
  				}
  			}
  			ent_next=(ObjectHeader **)((char *)ent+sizeof(ObjectHeader));
  			ent=*ent_next;
		}
  	}
	return NULL;
}

void *
Allocator::findBestFreeBlock(int size, int index){
	ObjectHeader *ent,*best;
	ObjectHeader **ent_next;
	int smallest=INT_MAX;

	best=NULL;
	if(head_arr[index]!=NULL && size_arr[index]!=0){
		ent=head_arr[index];
		while(ent!=NULL){
			if(ent->_flags==ObjFree){//check if this obj is free or not
  				if(index!=64 || size<=ent->_objectSize){
  					if((ent->_objectSize-size)<smallest){
  						smallest=ent->_objectSize-size;
  						best=ent;
  						if(smallest==0)
  							return (void *)ent;
  					}
  				}
  			}
  			ent_next=(ObjectHeader **)((char *)ent+sizeof(ObjectHeader));
  			ent=*ent_next;
		}
  	}
	return best;
}

void
Allocator::CHECKHEAP(){
	int num;
	ObjectHeader *o,*f,**o_n;

	for(int i=0;i<=64;i++){
		o=head_arr[i];
		num=0;
		while(o!=NULL){
			f=(ObjectHeader *)((char *)o+o->_objectSize-sizeof(ObjectHeader));
			if(i<64){
				assert(o->_objectSize==i*8);
				assert(f->_objectSize==i*8);
			}
			assert(o->_objectSize>=i*8);
			assert(f->_objectSize>=i*8);
			o_n=(ObjectHeader **)((char *)o+sizeof(ObjectHeader));
			o=*(o_n);
			num++;
		}

		assert(size_arr[i]==num);
	}

	//printf("Passed HEAP TEST...\n");
}

