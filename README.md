MyMalloc Project:
=================

http://www.cs.purdue.edu/homes/grr/cs354/

```
MyMalloc.cc  to implement a best fit allocator using boundary tags and segregated free lists. 
The data structure is a table of free lists. There will be 65 double-linked lists in total numbered 0 to 64.
Lists 0 to 63 will have blocks of size 8 * i, where i is the number of the list.
List 64 will contain blocks larger or equal than 64 * 8 = 512 and will be ordered by size in ascending order. 
 
When an object is freed, the allocator will check if the neighbor objects before and after are free and if possible
coalesce the freed object with its neighbors by doing the following:
* Check the footer of the left neighbor object (the object just before the object being freed) to see if it is free.
If that is the case, remove the left neighbor from its free list using the previous and next pointers and coalesce
this object with the object being freed.
* Check the header of the right neighbor object (the object just after the object being freed) to see if it is free.
If that is the case, remove the right neighbor from its free list using the previous and next pointers and coalesce it
with the object being  freed.
* Place the freed object in the corresponding free list and update the header and footer.

If the object being freed happens to be at the beginning of the heap, it is likely that the coalescing algorithm will
try to coalesce the memory beyond the the beginning of the heap crashing your allocator. The same will happen if the
object being freed is at the end of the heap and the coalescing algorithm tries to coalesce the  memory beyond the
end of the heap. Additionally, since your malloc library is not the only one that calls sbrk() bit other libraries do,
it will be possible that there are going to be areas in the heap that cannot be coalesced because they were not
allocated by the malloc library. To prevent this problem, every time a new free chunk of memory is requested from
the OS, the allocator will add a "dummy footer" or fence post at the beginning of the chunk with the flag set to 1,
meaning that the memory before this section of memory is not free and cannot be coalesced. Also,
at the end of the chunk, add a "dummy header" or fence post with the flag set to 1 meaning that the memory
beyond the chunk cannot be coalesced. That is in addition to the header and footer added to the chunk
when it is placed in the free list.
```

A. Notes for mtest(1-12).cc:
============================
The list of tests programs I wrote and what parts of my implementation they test.
I wrote mtest(1-12).cc and all of them can be easily tested by executing run-my-test.

Test descriptions:
------------------
1. Malloc an object from a free list that is a precise match
2. Malloc an object from a free list that needs to be split.
3. Malloc an object from a  free list that if split will give a remainder too small to be used in another allocation
4. Free an object that needs to be coalesced with the left neighbor only
5. Free an object that needs to be coalesced with the right neighbor only
6. Free an object that needs to be coalesced with both right and left neighbors
7. Free an object that does not need to be coalesced
8. Free an object at the beginning of  the heap to test fence posts.
9. Free an object at the end of the heap to test fence posts
10. See that memory is being reused
11. Free an already freed object should not crash
12. Free an object that was not allocated should not crash.

B. Notes for test(1-5).cc:
==========================
If you want to test test1, 2, 3 and 5, please edit Makefile so that:
GCC = g++ -g

If you want to test test4, please edit Makefile so that:
GCC = /opt/SUNWspro/bin/CC -g


