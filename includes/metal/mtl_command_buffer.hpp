#ifndef METAL_COMMAND_BUFFER_HPP
#define METAL_COMMAND_BUFFER_HPP

#include <Metal/Metal.hpp>

#include "mtl_render_pass_descriptor.hpp"
#include "mtl_view.hpp"
#include "mtl_command_queue.hpp"

#include <iostream>


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- C O M M A N D  B U F F E R ------------------------------------------

	class command_buffer final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::command_buffer;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline command_buffer(void) noexcept
			: _command{nullptr} {}

			/* queue constructor */
			inline command_buffer(const mtl::command_queue& queue)
			: _command{queue.underlying().commandBuffer()} {
				if (_command == nullptr)
					throw std::runtime_error{"failed to create command buffer"};
				_command->retain();
			}

			/* destructor */
			inline ~command_buffer(void) noexcept {
				if (_command == nullptr) return;
				_command->release();
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::CommandBuffer& {
				return *_command;
			}


			/* commit */
			inline void commit(void) const noexcept {
				_command->commit();
			}

			/* present drawable */
			inline void present_drawable(const mtl::view& view) const noexcept {
				_command->presentDrawable(view.underlying().currentDrawable());
			}



		private:

			// -- private members ---------------------------------------------

			/* command buffer */
			MTL::CommandBuffer* _command;

	};


}

#endif // METAL_COMMAND_BUFFER_HPP
