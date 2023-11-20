#ifndef ENGINE_RENDERER_HPP
#define ENGINE_RENDERER_HPP

#include <iostream>

#include "camera.hpp"
#include "time.hpp"
#include "mtl_device.hpp"
#include "mtl_command_queue.hpp"
#include "mtl_command_buffer.hpp"
#include "mtl_render_pass_descriptor.hpp"
#include "mtl_render_command_encoder.hpp"
#include "mtl_render_pipeline_state.hpp"
#include "mtl_view.hpp"
#include "mtl_depth_stencil_state.hpp"

#include "scene.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- R E N D E R E R -----------------------------------------------------

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

			/* scenes */
			std::vector<engine::scene> _scenes;

	};

}

#endif // ENGINE_RENDERER_HPP
