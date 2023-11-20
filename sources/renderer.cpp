#include "renderer.hpp"
#include "shapes.hpp"

#include "vertex.hpp"
#include "mesh.hpp"

#include "Metal/Metal.hpp"

/* default constructor */
engine::renderer::renderer(void)
: _queue{}, _scenes{} {
	_scenes.emplace_back();
}


/* draw in view */
auto engine::renderer::draw(const mtl::view& view) -> void {

	static unsigned int iteration = 0;
	++iteration;


	engine::time::update();


	auto command    = mtl::command_buffer{_queue};
	auto descriptor = mtl::render_pass_descriptor{view};
	auto encoder    = mtl::render_command_encoder{command, descriptor};

	encoder.set_render_pipeline_state(mtl::render_pipeline_state_library::state<"default_3D">());
	encoder.set_depth_stencil_state(mtl::depth_stencil_state_library::state<"default">());


	// create texture
	MTL::Texture* texture = nullptr;
	MTL::TextureDescriptor* tdesc = MTL::TextureDescriptor::texture2DDescriptor(
			MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB,
			engine::screen::width(),
			engine::screen::height(),
			false);

	tdesc->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
	tdesc->setStorageMode(MTL::StorageModePrivate);

	texture = mtl::device::underlying().newTexture(tdesc);



	encoder.set_fragment_bytes(&iteration, sizeof(iteration), 2);
	for (auto& scene : _scenes)
		scene.render(encoder);




	//encoder.set_render_pipeline_state<"default_2D">();
	//
	//const auto& mesh = engine::shapes::get<"square">();
	//mesh.render(encoder, engine::options{});

	encoder.end_encoding();
	command.present_drawable(view);
	command.commit();

}
