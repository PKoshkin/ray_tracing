all : test

test : test.cpp scene.h pixel.h color_rgb.h color_hsv.h color_lab.h color_xyz.h color.h point.h vector.h figure.h triangle.h sphere.h 3d_tree.h constants.h lighter.h optional.h plane.h bounding_box.h reader.h
	g++ -g -std=c++14 -o test test.cpp `pkg-config --cflags --libs cairo`
