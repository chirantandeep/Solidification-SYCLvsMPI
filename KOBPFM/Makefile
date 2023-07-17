# Compiler Info
CC        = icpx
OPTIMIZE  = yes
DEBUG     = yes

# Dimentionality
DIM = 2

# File info
program = prog
source = ./kobaniso.cpp
initfile = "./initfile.in"
runfile = "./runfile.in"
obj = $(source:.cpp=.o)

# Standard Flags
CFLAGS := -fsycl -std=c++17 -Wall ##-fsycl-enable-function-pointers

# Input flags
INPFLAGS := -DKOB_SYSTEM_DIMENTIONS=$(DIM)

# Linker Flags
LDFLAGS = 

# Debug Flags
ifeq ($(DEBUG),yes)
  CFLAGS  += -g -DDEBUG
  LDFLAGS += -g
endif

# Optimization Flags
ifeq ($(OPTIMIZE),yes)
  CFLAGS += -O3
endif

# Targets to Build

build: $(source)
	$(CC) $(CFLAGS) $(INPFLAGS) -o $(program) $(source) $(LDFLAGS)

run: $(program)
	./$(program) $(initfile) $(runfile)

clean:
	rm ./out/*.vtk