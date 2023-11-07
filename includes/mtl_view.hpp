#ifndef METAL_VIEW_HPP
#define METAL_VIEW_HPP


#include <MetalKit/MetalKit.hpp>


namespace engine {
	class view_delegate;
}

namespace mtl {


	class view final {

		friend class engine::view_delegate;
		friend class command_buffer;

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline view(void) noexcept = default;

			/* destructor */
			inline ~view(void) noexcept {
				if (_view == nullptr) return;
				_view->release();
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTK::View& {
				return *_view;
			}


		private:

			// -- private members ---------------------------------------------

			/* view */
			MTK::View* _view;
	};

}



#endif // METAL_VIEW_HPP
