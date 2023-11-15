FLAGS = -std=c++20 -Wall -Wextra -pedantic

program_1: lib_1 exe_1

lib_1: ../lib/program_1.h ../lib/program_1.cpp
	g++ ${FLAGS} -shared ../lib/program_1.cpp -fPIC -o libGCD.so
	export LD_LIBRARY_PATH=.

exe_1: lib_1 ../src/static_main.cpp ../lib/program_1.h
	g++ ${FLAGS} ../src/static_main.cpp -fPIC -L. -lGCD -o program_1


program_2: lib_2 exe_2

lib_2: ../lib/program_2.h ../lib/program_2.cpp
	g++ ${FLAGS} -shared ../lib/program_2.cpp -fPIC -o libNumSystems.so
	export LD_LIBRARY_PATH=.

exe_2: lib_2 ../src/dynamic_main.cpp ../lib/program_2.h
	g++ ${FLAGS} ../src/dynamic_main.cpp -fPIC -ldl -o program_2


clean:
	-rm program_1 libGCD.so program_2 libNumSystems.so
