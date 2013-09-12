
#GCC = /opt/SUNWspro/bin/CC -g

GCC = g++ -g

all: test1 test2 test3 MyMalloc.so runtests

runtests: runtest1-first runtest2-first runtest3-first runtest4-first runtest1-best runtest2-best runtest3-best runtest4-best


clearcounter:
	rm -f npassed
	rm -f nfailed

MyMalloc.so: MyMalloc.cc
	$(GCC) -c -g MyMalloc.cc
	ld -G -o MyMalloc.so MyMalloc.o

mtest1: mtest1.cc MyMalloc.so
	$(GCC) -o mtest1 mtest1.cc MyMalloc.so

mtest2: mtest2.cc MyMalloc.so
	$(GCC) -o mtest2 mtest2.cc MyMalloc.so

mtest3: mtest3.cc MyMalloc.so
	$(GCC) -o mtest3 mtest3.cc MyMalloc.so

mtest4: mtest4.cc MyMalloc.so
	$(GCC) -o mtest4 mtest4.cc MyMalloc.so

mtest5: mtest5.cc MyMalloc.so
	$(GCC) -o mtest5 mtest5.cc MyMalloc.so

mtest6: mtest6.cc MyMalloc.so
	$(GCC) -o mtest6 mtest6.cc MyMalloc.so

mtest7: mtest7.cc MyMalloc.so
	$(GCC) -o mtest7 mtest7.cc MyMalloc.so

mtest8: mtest8.cc MyMalloc.so
	$(GCC) -o mtest8 mtest8.cc MyMalloc.so

mtest9: mtest9.cc MyMalloc.so
	$(GCC) -o mtest9 mtest9.cc MyMalloc.so

mtest10: mtest10.cc MyMalloc.so
	$(GCC) -o mtest10 mtest10.cc MyMalloc.so

mtest11: mtest11.cc MyMalloc.so
	$(GCC) -o mtest11 mtest11.cc MyMalloc.so

mtest12: mtest12.cc MyMalloc.so
	$(GCC) -o mtest12 mtest12.cc MyMalloc.so

mtest13: mtest13.cc MyMalloc.so
	$(GCC) -o mtest13 mtest13.cc MyMalloc.so


test1: test1.cc MyMalloc.so
	$(GCC) -o test1 test1.cc MyMalloc.so

test2: test2.cc MyMalloc.so
	$(GCC) -o test2 test2.cc MyMalloc.so

test3: test3.cc MyMalloc.so
	$(GCC) -o test3 test3.cc MyMalloc.so -lthread

runtest1-first:
	MALLOCPOLICY=FIRSTFIT && export MALLOCPOLICY && \
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd` && export LD_LIBRARY_PATH && \
	echo "--- Running test1 MALLOCPOLICY=FIRSTFIT ---" && \
	test1

runtest2-first:
	MALLOCPOLICY=FIRSTFIT && export MALLOCPOLICY && \
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd` && export LD_LIBRARY_PATH && \
	echo "--- Running test2 MALLOCPOLICY=FIRSTFIT ---" && \
	test2

runtest3-first:
	MALLOCPOLICY=FIRSTFIT && export MALLOCPOLICY && \
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd` && export LD_LIBRARY_PATH && \
	echo "--- Running test3 MALLOCPOLICY=FIRSTFIT ---" && \
	test3

runtest4-first: MyMalloc.so
	MALLOCPOLICY=FIRSTFIT && export MALLOCPOLICY && \
	echo && \
	echo "---- Running test4 (run cc with your malloc) MALLOCPOLICY=FIRSTFIT ----" && \
	LD_PRELOAD=`pwd`/MyMalloc.so && \
	export LD_PRELOAD && \
	/opt/SUNWspro/bin/cc hello.c && \
	echo ">>> Test4 passed\n" || \
	echo "*** Test4 failed\n"

runtest1-best:
	MALLOCPOLICY=BESTFIT && export MALLOCPOLICY && \
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd` && export LD_LIBRARY_PATH && \
	echo "--- Running test1 MALLOCPOLICY=BESTFIT ---" && \
	test1

runtest2-best:
	MALLOCPOLICY=BESTFIT && export MALLOCPOLICY && \
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd` && export LD_LIBRARY_PATH && \
	echo "--- Running test2 MALLOCPOLICY=BESTFIT ---" && \
	test2

runtest3-best:
	MALLOCPOLICY=BESTFIT && export MALLOCPOLICY && \
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd` && export LD_LIBRARY_PATH && \
	echo "--- Running test3 MALLOCPOLICY=BESTFIT ---" && \
	test3

runtest4-best: MyMalloc.so
	MALLOCPOLICY=BESTFIT && export MALLOCPOLICY && \
	echo && \
	echo "---- Running test4 (run cc with your malloc) MALLOCPOLICY=BESTFIT ----" && \
	LD_PRELOAD=`pwd`/MyMalloc.so && \
	export LD_PRELOAD && \
	/opt/SUNWspro/bin/cc hello.c && \
	echo ">>> Test4 passed\n" || \
	echo "*** Test4 failed\n"

clean:
	rm -f *.o test1 test2 test3 test4 test5 MyMalloc.so core a.out mtest1 mtest2 mtest3 mtest4 mtest5 mtest6 mtest7 mtest8 mtest9 mtest10 mtest11 mtest12 *~ out
