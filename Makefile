CXX = g++
CFLAGS = -g -pg -Wall -Wextra -Werror -std=c++17 -Ofast
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
			obj/event.o \
			obj/vec.o \
			obj/random.o
HDR = src/event.hpp \
			src/sim.hpp \
			src/bfstream.hpp \
			src/colour.hpp
LINK = -lpthread
EXE = BEARS
DEFS = 

.SECONDARY:

obj/%.o: src/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@

BEARS: obj/main.o $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) obj/main.o $(OBJ) -o $@ $(LINK)

configgen: obj/config_main.o obj/config.o $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) obj/config_main.o obj/config.o $(OBJ) -o $@ $(LINK)

prof_pdf:
	gprof BEARS gmon.out > analysis.txt
	gprof2dot -o d.dot analysis.txt
	dot -Tpdf d.dot > prof.pdf

clean:
	rm -rf obj $(EXE) configgen
