g++ -c HaarFeature.cpp
g++ -c haarDriver.cpp
g++ -o haarDriver.exe haarDriver.o HaarFeature.o

rm haarDriver.o HaarFeature.o
./haarDriver.exe
