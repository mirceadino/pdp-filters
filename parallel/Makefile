IM_CXXFLAGS=`Magick++-config --cxxflags`
IM_LDFLAGS=`Magick++-config --ldflags`
CXXFLAGS=-std=c++14 -pthread $(IM_CXXFLAGS) $(IM_LDFLAGS)

main: main.cpp utils.h matrix.h kernels.h
	g++ $(CXXFLAGS) main.cpp -o main

clean: main
	rm main tmp*
