EXEC_FILE = main
SRC_FILES = main.c FirstAsm.c SecondAsm.c FileMethods.c MemorySnapShot.c SymbolTable.c HelperMethods.c
HEADER_FILES = FirstAsm.h SecondAsm.h FileMethods.h MemorySnapShot.h SymbolTable.h DataStructers.h HelperMethods.h Constant.h

O_FILES = $(SRC_FILES:.c=.o)

all: $(EXEC_FILE)
$(EXEC_FILE): $(O_FILES)
	gcc -Wall -ansi -pedantic $(O_FILES) -o $(EXEC_FILE)
%.o: %.c $(HEADER_FILES)
	gcc -Wall -ansi -pedantic -c -o $@ $<
clean:
	rm -f *.o $(EXEC_FILE)