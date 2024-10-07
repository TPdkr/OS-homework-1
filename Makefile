#this was done by Timofei Podkorytov
#for assignment 1 for Operating Systems course
#fall 2024
p2: task12.c
	gcc -Wall -o p2 task12.c
p3: env.c
	gcc -Wall -o env env.c
all: env.c task12.c
	gcc -Wall -o env env.c
	gcc -Wall -o p2 task12.c

