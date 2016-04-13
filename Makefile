all : test

test : test.cpp screen.h pixel.h color.h point.h vector.h figure.h triangle.h sphere.h 3d_tree.h
	g++-5 std=c++11 -o test test.cpp `pkg-config --cflags --libs cairo`
