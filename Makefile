all : test

test : test.cpp scene.h pixel.h color_rgb.h color_hsv.h color_lab.h color_xyz.h color.h point.h vector.h figure.h triangle.h sphere.h 3d_tree.h constants.h lighter.h optional.h sheet.h
	g++ -g -std=c++11 -o test test.cpp `pkg-config --cflags --libs cairo`
