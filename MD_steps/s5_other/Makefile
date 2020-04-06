SRC=MD.c control.c util.c 
OBJ=$(SRC:.c=.o)
CC=icc 
CFLAGS= -g -Ofast -ipo\
 -no-prec-div -no-prec-sqrt\
 -Wall -Winline -pg
REPORT_FLAGS= -qopt-report-file=report/ipo.optout -qopt-report=5

all: MD

MD: $(OBJ)
	$(CC) $(CFLAGS)  -o $@  $(OBJ) -lm $(REPORT_FLAGS)
	rm -f *.o


diff:
	./Test/diff-output Test/output.dat100 output/output.dat100
	./Test/diff-output Test/output.dat200 output/output.dat200
	./Test/diff-output Test/output.dat300 output/output.dat300
	./Test/diff-output Test/output.dat400 output/output.dat400
	./Test/diff-output Test/output.dat500 output/output.dat500

clean:
	rm -f MD $(OBJ)
	rm -f gmon.out

cclean:
	make clean
	rm -f *.e*
	rm -f *.o*
	rm -f output/*
	rm -f report/*

$(OBJ) : coord.h Makefile


