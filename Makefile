CXX = g++
CFLAGS = -g -Wall -Wextra -Werror -std=c++17 -O0
OBJ = \
			obj/ball.o \
			obj/sim_io.o \
			obj/sim_run.o \
			obj/event.o \
			obj/main.o

LINK = 
EXE = BEARS

.SECONDARY:

obj/%.o: src/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $< -c -o $@

BEARS: $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(OBJ) -o $@ $(LINK)

clean:
	rm -rf obj $(EXE)
