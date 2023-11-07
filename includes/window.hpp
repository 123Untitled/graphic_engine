#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP


#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "mtl_device.hpp"
#include "view.hpp"


namespace engine {


	class window final {


		public:

			// -- public types ------------------------------------------------

			/* window size type */
			using size_type = CGFloat;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			window(void);

			/* deleted copy constructor */
			window(const window&) = delete;

			/* deleted move constructor */
			window(window&&) noexcept = delete;

			/* destructor */
			~window(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment */
			auto operator=(const window&) -> window& = delete;

			/* deleted move assignment */
			auto operator=(window&&) noexcept -> window& = delete;


			// -- public modifiers --------------------------------------------

			/* initialize */
			auto init(void) -> void;

			/* destroy */
			auto destroy(void) noexcept -> void;

			inline auto underlying(void) noexcept -> NS::Window* {
				return _window;
			}

		private:

			NS::AutoreleasePool* _pool;

			/* window */
			NS::Window* _window;

			/* view */
			MTK::View*  _mtkview;

			/* view delegate */
			engine::view_delegate _view_delegate;

	};

}

#endif // ENGINE_WINDOW_HPP
