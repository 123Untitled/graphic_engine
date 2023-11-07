#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <iostream>

#include "app.hpp"
#include "renderer.hpp"
#include "mtl_buffer.hpp"
#include "mesh.hpp"
#include "memory_pool.hpp"

#include "vertex.hpp"
#include <termios.h>
#include "model_loader.hpp"




int main(int ac, char** av) {

	engine::model_loader::parse("untitled.obj");


	return 0;




	try {
		engine::app app;
	} catch (std::exception& except) {
		std::cerr << except.what() << std::endl;
	}
	std::cout << ":) MAIN END (:" << std::endl;
	return 0;
}
