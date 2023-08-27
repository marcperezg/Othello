OPCIONS = -c -Wno-deprecated -O0 -fopenmp 

reversi.exe: MatrizClass.o TableroClass.o JugadorIA.o JugadorClass.o reversi.o
	g++ -fopenmp -o reversi.exe MatrizClass.o JugadorIA.o JugadorClass.o TableroClass.o reversi.o 
	rm *.o

reversi.o: reversi.cpp TableroClass.h JugadorClass.h JugadorIA.hpp
	g++ -c reversi.cpp $(OPCIONS)


MatrizClass.o: MatrizClass.cpp MatrizClass.h
	g++ -c MatrizClass.cpp $(OPCIONS)

TableroClass.o: TableroClass.cpp TableroClass.h
	g++ -c TableroClass.cpp $(OPCIONS)

JugadorClass.o: JugadorClass.cpp JugadorClass.h
	g++ -c JugadorClass.cpp $(OPCIONS)

JugadorIA.o: JugadorIA.cpp JugadorIA.hpp
	g++ -c JugadorIA.cpp $(OPCIONS)


clean:
	rm *.o
	rm *.exe
	rm *.gch