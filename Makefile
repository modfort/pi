CC=g++
CFLAGS=-std=c++14 -O3 -Wall -lpthread -lgmpxx -lgmp
EXEC=MEHDI
#DotH=Graph.hpp

COPIE= *.cpp *.hpp Makefile *.txt
COPIEBACK=../$(COPIE)
FILE = TMP
$(EXEC) : test.o  
	$(CC) -o $@ test.o  $(CFLAGS)
test.o:test.cpp 
	$(CC) -o test.o -c test.cpp  $(CFLAGS)


cp :
	cp $(COtestE) $(FILE)
recu:
	cp 	-f $(COPIEBACK) .
mk:
	mkdir $(FILE)


clean:
	rm $(EXEC) *.o 
cleantotal:
	sudo rm -r $(FILE) $(EXEC) *.o  

resume:
	echo "code qui a partir d'un fichier creer un graph et fait circuler des voiture selon le nombre de thread"
