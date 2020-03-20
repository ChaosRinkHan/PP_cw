diff-output: diff-output.c
	gcc  -g -o $@ diff-output.c -lm

diff: 
	./diff-output ../C/output.dat100 ../../MD_opt/output.dat100
	./diff-output ../C/output.dat200 ../../MD_opt/output.dat200
	./diff-output ../C/output.dat300 ../../MD_opt/output.dat300
	./diff-output ../C/output.dat400 ../../MD_opt/output.dat400
	./diff-output ../C/output.dat500 ../../MD_opt/output.dat500


clean:
	rm diff-output
