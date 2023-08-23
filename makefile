all : main run

main : main.c
	gcc -Wall -Wextra -std=c17 -lGL -lGLU -lglut -g -lm -o main main.c

run : main
	./main
