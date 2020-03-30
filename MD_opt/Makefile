SRC=MD.c control.c util.c 
OBJ=$(SRC:.c=.o)
CC=icc 
CFLAGS= -g -Ofast -ipo -qopt-report5


all: MD

MD: $(OBJ)
	$(CC) $(CFLAGS)  -o $@  $(OBJ) -lm


output.dat: MD input.dat
	./MD

diff:
	./Test/diff-output Test/output.dat100 output.dat100
	./Test/diff-output Test/output.dat200 output.dat200
	./Test/diff-output Test/output.dat300 output.dat300
	./Test/diff-output Test/output.dat400 output.dat400
	./Test/diff-output Test/output.dat500 output.dat500

clean:
	rm -f MD $(OBJ)
	rm -f *.optrpt

$(OBJ) : coord.h Makefile


