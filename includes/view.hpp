#ifndef ENGINE_VIEW_HPP
#define ENGINE_VIEW_HPP

// local headers
#include "renderer.hpp"
#include "mtl_view.hpp"
#include "mtl_command_queue.hpp"

#include <MetalKit/MetalKit.hpp>


// -- E N G I N E -------------------------------------------------------------

namespace engine {


	// -- V I E W -------------------------------------------------------------

	class view_delegate final : public MTK::ViewDelegate {


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			view_delegate(void);

			/* deleted copy constructor */
			view_delegate(const view_delegate&) = delete;

			/* deleted move constructor */
			view_delegate(view_delegate&&) noexcept = delete;

			/* destructor */
			~view_delegate(void) noexcept override;


			// -- overrides ---------------------------------------------------

			/* draw in view */
			void drawInMTKView(MTK::View*) override;

			/* drawable size will change */
			void drawableSizeWillChange(MTK::View*, CGSize) override;



			// -- public assignment operators ---------------------------------

			/* deleted copy assignment */
			auto operator=(const view_delegate&) -> view_delegate& = delete;

			/* deleted move assignment */
			auto operator=(view_delegate&&) noexcept -> view_delegate& = delete;


		private:

			// -- private members ---------------------------------------------

			/* renderer */
			engine::renderer _renderer;

	};


}

#endif // ENGINE_VIEW_HPP
