CXX = g++
CFLAGS = -g -pg -Wall -Wextra -Werror -std=c++17 -Ofast
OBJ = \
			../obj/ball.o \
			../obj/ball_collision.o \
			../obj/ball_properties.o \
			../obj/sim_io.o \
			../obj/sim_run.o \
			../obj/sim_bounds.o \
			../obj/sim_properties.o \
			../obj/sim_traj.o \
			../obj/sim.o \
			../obj/version.o \
			../obj/event.o \
			../obj/vec.o \
			../obj/random.o
HDR = ../src/event.hpp \
			../src/sim.hpp \
			../src/bfstream.hpp \
			../src/colour.hpp
LINK = -lpthread
DEFS = 

.SECONDARY:

../obj/%.o: ../src/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@

test: test.cpp $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) test.cpp $(OBJ) -o $@ $(LINK)
