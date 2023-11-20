#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <iostream>

#include "app.hpp"
#include "renderer.hpp"
#include "mtl_buffer.hpp"
#include "mesh.hpp"

#include "vertex.hpp"
#include <termios.h>
#include "wavefront.hpp"


#include "screen.hpp"

#include "quaternions.hpp"

#include "simd.hpp"


int main(int ac, char** av) {


	//xns::vec<float, 4> vec{1.0f, 2.0f, 3.0f, 4.0f};
	//
	//vec.normalize();
	//
	//std::cout << vec << std::endl;
	//
	//return 0;


	//engine::wavefront<float>::parse("assets/cube.obj");
	//return 0;



	try {
		engine::app app;
	} catch (std::exception& except) {
		std::cerr << except.what() << std::endl;
	}
	std::cout << ":) MAIN END (:" << std::endl;
	return 0;
}
