#####export I_MPI_CXX=icpx
g++ -std=c++20 serial.cpp -o prog
./prog
rm ./prog