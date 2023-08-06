touch profile_data.csv
## Run serial code
g++ -std=c++20 serial.cpp -o prog
./prog > profile_data.csv
rm ./prog

export I_MPI_CXX=icpx
mpiicpc -std=c++17 -DDOM_SIZE=512 parallel.cpp -o prog
mpirun -n 2 ./prog >> profile_data.csv
rm ./prog