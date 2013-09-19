MyMalloc:
=========

A best fit allocator
--------------------

MyMalloc.cc implements a best fit allocator using boundary tags and segregated free lists. The data structure is a table of free lists. 

http://www.cs.purdue.edu/homes/grr/cs354/lab2-dl-malloc/

Download
--------
```
$ git clone https://github.com/lirongyuan/MyMalloc.git
```

Setup
-----
```
$ make
```

Usage
-----
Compile your program with ``MyMalloc.so``.
```
$ gcc -o your-program your-program.cc MyMalloc.so
```

Tests
-----
**1. mtest(1-12)**
```
$ ./run-my-test
```

``mtest(1-12).cc`` are written to test the following situations.

1. Malloc an object from a free list that is a precise match.

2. Malloc an object from a free list that needs to be split.

3. Malloc an object from a  free list that if split will give a remainder too small to be used in another allocation.

4. Free an object that needs to be coalesced with the left neighbor only.

5. Free an object that needs to be coalesced with the right neighbor only.

6. Free an object that needs to be coalesced with both right and left neighbors.

7. Free an object that does not need to be coalesced.

8. Free an object at the beginning of  the heap to test fence posts.

9. Free an object at the end of the heap to test fence posts.

10. See that memory is being reused.

11. Free an already freed object should not crash.

12. Free an object that was not allocated should not crash.

**2. test(1-5)**
```
$ make runtests
```
* If you want to run tests 1, 2, 3 and 5, please edit Makefile so that: ``GCC = g++ -g``.
* If you want to run test 4, please edit Makefile so that: ``GCC = /opt/SUNWspro/bin/CC -g``.



