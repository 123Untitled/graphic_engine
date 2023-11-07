#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION


#include "app.hpp"
#include "renderer.hpp"
#include "mtl_buffer.hpp"
#include "mesh.hpp"
#include "memory_pool.hpp"

#include <iostream>

#include "vertex.hpp"

int main(int ac, char** av) {

	//engine::vertex v{0.123f, 0.0f, 3.0f, 12.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	//simd::float3 f3;
	//
	//f3.x = 0.123f;
	////v.print();
	//return 0;

	try {
		engine::app app;
	} catch (std::exception& except) {
		std::cerr << except.what() << std::endl;
	}

	std::cout << "main end" << std::endl;
	return 0;
}
