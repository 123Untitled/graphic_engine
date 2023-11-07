#include "renderer.hpp"

#include "vertex.hpp"
#include "mesh.hpp"

/* default constructor */
engine::renderer::renderer(void)
: _queue{}, _state{mtl::render_pipeline_descriptor{}} {
	std::cout << "renderer constructor" << std::endl;
}

#include <simd/simd.h>

/* draw in view */
auto engine::renderer::draw(const mtl::view& view) -> void {

	auto command    = mtl::command_buffer{_queue};
	auto descriptor = mtl::render_pass_descriptor{view};
	auto encoder    = mtl::render_command_encoder{command, descriptor};

	encoder.set_render_pipeline_state(_state);

	std::vector<engine::vertex> vertices;
	vertices.emplace_back(0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices.emplace_back(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices.emplace_back(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f);


	engine::mesh mesh{vertices};

	mesh.render(encoder);

	encoder.end_encoding();
	command.present_drawable(view);
	command.commit();


}
