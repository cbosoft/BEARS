CXX = g++
CFLAGS = -g -Wall -Wextra -Werror -std=c++17 -O0
OBJ = \
			obj/ball.o \
			obj/ball_collision.o \
			obj/ball_properties.o \
			obj/sim_io.o \
			obj/sim_run.o \
			obj/sim_bounds.o \
			obj/sim_properties.o \
			obj/sim_traj.o \
			obj/sim.o \
			obj/version.o \
			obj/vec.o \
			obj/random.o

LINK = -lpthread
EXE = BEARS
DEFS = 

.SECONDARY:

obj/%.o: src/%.cpp
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@

BEARS: obj/main.o $(OBJ)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) obj/main.o $(OBJ) -o $@ $(LINK)

configgen: obj/config_main.o obj/config.o $(OBJ)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) obj/config_main.o obj/config.o $(OBJ) -o $@ $(LINK)

clean:
	rm -rf obj $(EXE) configgen
