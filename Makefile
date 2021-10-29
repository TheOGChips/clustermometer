#CXX = OMPI_CXX=clang++ mpic++
CXX = clang++
SRC = clustermometer
TGT = run.exe

all:		$(SRC)

$(SRC):		$(SRC).o
		$(CXX) -o run.exe $(SRC).o -lncurses -pthreads
		
$(SRC).o:	$(SRC).cpp
		$(CXX) -c $(SRC).cpp
		
clean:		$(TGT)
		rm *.o $(TGT)
