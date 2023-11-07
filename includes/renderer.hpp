#ifndef ENGINE_RENDERER_HPP
#define ENGINE_RENDERER_HPP

#include <iostream>

#include "mtl_device.hpp"
#include "mtl_command_queue.hpp"
#include "mtl_command_buffer.hpp"
#include "mtl_render_pass_descriptor.hpp"
#include "mtl_render_command_encoder.hpp"
#include "mtl_render_pipeline_state.hpp"
#include "mtl_view.hpp"


class mesh final {

	public:
		int numberOfVertices{0};
		MTL::Buffer* vertexPositionsBuffer{nullptr};
		MTL::Buffer* vertexColorsBuffer{nullptr};
};

class renderpass final {

	private:

		MTL::RenderPipelineState* _pipeline;
		mesh* _mesh;

};




// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	class renderer final {

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			renderer(void);

			/* destructor */
			inline ~renderer(void) noexcept = default;

			/* draw */
			auto draw(const mtl::view&) -> void;


		private:

			// -- private members ---------------------------------------------

			/* command queue */
			mtl::command_queue _queue;

			/* pipeline state */
			mtl::render_pipeline_state _state;

	};

}

#endif // ENGINE_RENDERER_HPP
