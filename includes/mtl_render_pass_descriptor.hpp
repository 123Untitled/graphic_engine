#ifndef MTL_RENDER_PASS_DESCRIPTOR_HPP
#define MTL_RENDER_PASS_DESCRIPTOR_HPP


#include <Metal/Metal.hpp>

#include "mtl_view.hpp"


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- R E N D E R  P A S S  D E S C R I P T O R ---------------------------

	class render_pass_descriptor final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::render_pass_descriptor;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline render_pass_descriptor(void) noexcept
			: _descriptor{nullptr} {}

			/* view constructor */
			inline render_pass_descriptor(const mtl::view& view)
			: _descriptor{view.underlying().currentRenderPassDescriptor()} {
				if (_descriptor == nullptr)
					throw std::runtime_error{"failed to create render pass descriptor"};
				_descriptor->retain();
			}

			/* destructor */
			inline ~render_pass_descriptor(void) noexcept {
				if (_descriptor == nullptr) return;
				_descriptor->release();
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::RenderPassDescriptor& {
				return *_descriptor;
			}


		private:

			// -- private members ---------------------------------------------

			/* render pass descriptor */
			MTL::RenderPassDescriptor* _descriptor;


	};

}

#endif // MTL_RENDER_PASS_DESCRIPTOR_HPP
