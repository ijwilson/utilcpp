# Note that make clean must bew run before any other compilations

files=gsl_distributions.o gsl_tests.o newio.o

GSLLDFLAGS= $(shell gsl-config --libs)

test:	$(files)
	$(CXX) $(files) -o test $(GSLLDFLAGS)

testoptions: options.o testoptions.o newio.o
	$(CXX) options.o testoptions.o newio.o -o testoptions

clean:
	rm -f *.o test


options.o: options.cpp options.H
testoptions.o: testoptions.cpp options.H