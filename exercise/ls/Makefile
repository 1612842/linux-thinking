all: ls.exe
 
# $@ matches the target; $< matches the first dependent
ls.exe: ls.o
	gcc -o $@ $<

ls.o: ls.c
	gcc -c $<
     
clean:
	rm ls.o ls.exe
