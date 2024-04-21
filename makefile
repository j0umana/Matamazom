CC = gcc
OBJS = amount_set.o products.o orders.o matamazom_main.o matamazom_tests.o matamazom_print.o matamazom.o 
AMOUNT_SET_OBJECTS = amount_set.o amount_set_tests.o amount_set_main.o
EXEC = matamazom
AMOUNT_SET_EXECUTABLE = amount_set
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@ -L. -lm -lmtm

$(AMOUNT_SET_EXECUTABLE) : $(AMOUNT_SET_OBJECTS)
	$(CC) $(DEBUG_FLAG) $(AMOUNT_SET_OBJECTS) -o $@

amount_set.o: amount_set.c amount_set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

amount_set_main.o: amount_set_main.c amount_set_tests.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

amount_set_tests.o: amount_set_tests.c amount_set_tests.h amount_set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

matamazom_main.o: matamazom_main.c matamazom_tests.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
 
matamazom_tests.o: matamazom_tests.c matamazom_tests.h matamazom.h amount_set.h set.h matamazom_print.h orders.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

matamazom.o: matamazom.c matamazom.h amount_set.h set.h matamazom_print.h orders.h products.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

matamazom_print.o: matamazom_print.c matamazom_print.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

orders.o: orders.c orders.h amount_set.h set.h products.h matamazom.h matamazom_print.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

products.o: products.c products.h amount_set.h set.h matamazom.h matamazom_print.h orders.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(AMOUNT_SET_EXECUTABLE) $(AMOUNT_SET_OBJECTS) $(EXEC)