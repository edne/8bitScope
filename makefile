gl=-lGL -lglut

test: build
	jack-stdout -b8 0 1 | ./8bitScope

rgb: build
	jack-stdout -b8 0 1 | ./8bitScope --rgb

build:
	gcc -Wall -pthread 8bitScope.c -o8bitScope -lm $(gl)
