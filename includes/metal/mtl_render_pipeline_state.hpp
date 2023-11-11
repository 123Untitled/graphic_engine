#ifndef METAL_RENDER_PIPELINE_STATE_HPP
#define METAL_RENDER_PIPELINE_STATE_HPP

#include <Metal/Metal.hpp>

#include "mtl_render_pipeline_descriptor.hpp"
#include "mtl_device.hpp"


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- R E N D E R  P I P E L I N E  S T A T E -----------------------------

	class render_pipeline_state final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::render_pipeline_state;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline render_pipeline_state(void) noexcept
			: _state{nullptr} {}

			/* render pipeline descriptor constructor */
			inline render_pipeline_state(const mtl::render_pipeline_descriptor& descriptor)
			: _state{nullptr} {

				NS::Error* error = nullptr;
						  _state = mtl::device::underlying().newRenderPipelineState(descriptor, &error);

				if (_state == nullptr) {
					if (error)
						std::cout << error->code() << ": " << error->localizedDescription()->utf8String() << std::endl;
					throw std::runtime_error{"failed to create render pipeline state"};
				}
			}


			/* destructor */
			inline ~render_pipeline_state(void) noexcept {
				if (_state == nullptr) return;
				_state->release();
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::RenderPipelineState& {
				return *_state;
			}


			// -- public conversion operators ---------------------------------

			/* underlying */
			inline operator MTL::RenderPipelineState&(void) const noexcept {
				return *_state;
			}

			/* underlying */
			inline operator const MTL::RenderPipelineState*(void) const noexcept {
				return _state;
			}


		private:

			// -- private members ---------------------------------------------

			/* underlying */
			MTL::RenderPipelineState* _state;

	};

}

#endif // ifndef METAL_RENDER_PIPELINE_STATE_HPP
