gl=-lGL -lglut

all:
	gcc -Wall -pthread 8bitScope.c -o8bitScope -lm $(gl)
	jack-stdout -b8 0 1 | ./8bitScope
