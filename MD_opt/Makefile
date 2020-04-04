SRC=MD.c control.c util.c 
OBJ=$(SRC:.c=.o)
CC=icc 
CFLAGS= -g -Ofast -ipo -no-prec-div -no-prec-sqrt -qopt-report5\
-Wall -Winline -pg


all: MD

MD: $(OBJ)
	$(CC) $(CFLAGS)  -o $@  $(OBJ) -lm
	rm -f *.o


diff:
	./Test/diff-output Test/output.dat100 output/output.dat100
	./Test/diff-output Test/output.dat200 output/output.dat200
	./Test/diff-output Test/output.dat300 output/output.dat300
	./Test/diff-output Test/output.dat400 output/output.dat400
	./Test/diff-output Test/output.dat500 output/output.dat500

clean:
	rm -f MD $(OBJ)
	rm -f *.optrpt
	rm -f gmon.out

$(OBJ) : coord.h Makefile


