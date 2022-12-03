# A Makefile to build the 'trove' project

PROJECT =  trove
HEADERS =  $(PROJECT).h
OBJ     =  trove.o globals.o hashtable.o update.o findfiles.o findwords.o words.o remove.o trovefile.o compression.o


C11     =  cc -std=c11
CFLAGS  =  -Wall -Werror 


$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm


%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean: rm -f $(PROJECT) $(OBJ)