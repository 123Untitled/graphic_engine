#include "renderer.hpp"

#include "vertex.hpp"
#include "mesh.hpp"

/* default constructor */
engine::renderer::renderer(void)
: _queue{}, _state{mtl::render_pipeline_descriptor{}} {
	std::cout << "renderer constructor" << std::endl;
}

static auto make_rectange(const float scale) -> engine::vpackage {

	/* ------------ */
	/* |          | */
	/* |          | */
	/* ------------ */

	// top line y
	const float tly = +0.5f * scale;
	// bottom line y
	const float bly = -0.5f * scale;

	// left line x
	const float llx = -0.5f * scale;
	// right line x
	const float rlx = +0.5f * scale;

	//constexpr float tly = +0.5f; // top    line y
	//constexpr float bly = +0.2f; // bottom line y
	//
	//constexpr float llx = -1.0f; // left   line x
	//constexpr float rlx =  0.5f; // right  line x

	engine::vertices vertices;
	vertices.reserve(4);
	// top left corner
	vertices.emplace_back(llx, tly, 0.0f);
	// top right corner
	vertices.emplace_back(rlx, tly, 0.0f);
	// bottom right corner
	vertices.emplace_back(rlx, bly, 0.0f);
	// bottom left corner
	vertices.emplace_back(llx, bly, 0.0f);

	engine::indexes indexes;
	indexes.reserve(6);

	indexes.emplace_back(0);
	indexes.emplace_back(1);
	indexes.emplace_back(2);

	indexes.emplace_back(0);
	indexes.emplace_back(2);
	indexes.emplace_back(3);

	return engine::vpackage{vertices, indexes};
}

/* make cube */
static auto make_cube(const float scale) -> engine::vpackage {

	/* ------------ */
	/* |          | */
	/* |          | */
	/* ------------ */

	// top line y
	const float tly = +0.5f * scale;
	// bottom line y
	const float bly = -0.5f * scale;

	// left line x
	const float llx = -0.5f * scale;
	// right line x
	const float rlx = +0.5f * scale;

	//constexpr float tly = +0.5f; // top    line y
	//constexpr float bly = +0.2f; // bottom line y
	//
	//constexpr float llx = -1.0f; // left   line x
	//constexpr float rlx =  0.5f; // right  line x

	engine::vertices vertices;
	vertices.reserve(4);
	// top left corner
	vertices.emplace_back(llx, tly, 0.0f);
	// top right corner
	vertices.emplace_back(rlx, tly, 0.0f);
	// bottom right corner
	vertices.emplace_back(rlx, bly, 0.0f);
	// bottom left corner
	vertices.emplace_back(llx, bly, 0.0f);

	engine::indexes indexes;
	indexes.reserve(6);

	indexes.emplace_back(0);
	indexes.emplace_back(1);
	indexes.emplace_back(2);

	indexes.emplace_back(0);
	indexes.emplace_back(2);
	indexes.emplace_back(3);

	return engine::vpackage{vertices, indexes};
}

static engine::camera& cam = engine::camera::shared();

	/* will be moved in the class to avoid recreating the mesh every frame */
static engine::mesh rec{make_rectange(1.0f)};

/* draw in view */
auto engine::renderer::draw(const mtl::view& view) -> void {

	auto command    = mtl::command_buffer{_queue};
	auto descriptor = mtl::render_pass_descriptor{view};
	auto encoder    = mtl::render_command_encoder{command, descriptor};

	encoder.set_render_pipeline_state(_state);

	engine::time::delta = 1.0f / view.fps();



	cam.update();
	cam.render(encoder);
	rec.render(encoder);

	encoder.end_encoding();
	command.present_drawable(view);
	command.commit();
}
