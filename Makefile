all : test

test : test.cpp quadrangle.h scene.h pixel.h ray.h color_rgb.h color_hsv.h color_lab.h color_xyz.h color.h point.h vector.h figure.h triangle.h sphere.h 3d_tree.h constants.h lighter.h optional.h plane.h bounding_box.h reader.h
	g++ -g -Wall -std=c++14 -o test test.cpp `pkg-config --cflags --libs cairo`
