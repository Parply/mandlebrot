all: mandlebrot mandlebrot_cuda
mandlebrot: mandlebrot.cpp
	g++ mandlebrot.cpp -o mandlebrot -Ofast -lsfml-graphics -lsfml-window -lsfml-system -fopenmp -march=native
mandlebrot_cuda: mandlebrot_cuda.cu
	nvcc -x cu mandlebrot_cuda.cu -o mandlebrot_cuda -O3 -lsfml-graphics -lsfml-window -lsfml-system -Xcompiler "-I ~/cuda-samples/Common -Ofast -lsfml-graphics -lsfml-window -lsfml-system -march=native" 
