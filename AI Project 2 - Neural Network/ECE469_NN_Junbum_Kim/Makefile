train:
	g++ neuraltrain.cpp -std=c++11 -o train.exe

test:
	g++ neuraltest.cpp -std=c++11 -o test.exe

debug_train:
	g++ -std=c++11 -g -o trainDebug.exe neuraltrain.cpp

debug_test:
	g++ -std=c++11 -g -o testDebug.exe neuraltest.cpp

clean:
	rm -f *.exe

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp Makefile backups
