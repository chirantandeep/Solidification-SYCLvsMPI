export I_MPI_CXX=icpx
mpiicpc -std=c++17 -DDOM_SIZE=1024 parallel.cpp -o prog
mpirun -n 4 ./prog
rm ./prog