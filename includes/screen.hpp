#ifndef ENGINE_SCREEN_HPP
#define ENGINE_SCREEN_HPP

#include <ApplicationServices/ApplicationServices.h>
#include <iostream>

// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- S C R E E N -----------------------------------------------------------

	class screen final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::screen;

			/* screen size type */
			using size_type = CGFloat;


			// -- public static methods ---------------------------------------

			/* shared */
			static inline auto shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}

			static inline auto width(void) noexcept -> size_type {
				return shared()._screen_w;
			}

			static inline auto height(void) noexcept -> size_type {
				return shared()._screen_h;
			}

			/* ratio */
			static inline auto ratio(void) noexcept -> size_type {
				return shared()._ratio;
			}

			/* initiale view size */
			static inline auto frame(void) noexcept -> CGRect {
				auto& self = shared();
				return CGRect{
					{(self._screen_w - self._view_w) / 2,  // position x
					 (self._screen_h - self._view_h) / 2}, // position y
					 {self._view_w, // width
					  self._view_h} // height
				};
			}

			/* update view size */
			static inline auto update(const size_type width, const size_type height) noexcept -> void {
				auto& self = shared();
				self._view_w = width;
				self._view_h = height;
				self._ratio = self._view_w / self._view_h;
			}

		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline screen(void)
			:	_screen_w{static_cast<size_type>(::CGDisplayPixelsWide(kCGDirectMainDisplay))},
				_screen_h{static_cast<size_type>(::CGDisplayPixelsHigh(kCGDirectMainDisplay))},
				_view_w{_screen_w / 1.5},
				_view_h{_screen_h / 1.5},
				_ratio{_view_w / _view_h} {

				//auto screen = CGDisplayBounds(CGMainDisplayID());
			}


			// -- private members ---------------------------------------------

			/* screen width */
			size_type _screen_w;

			/* screen height */
			size_type _screen_h;

			/* view width */
			size_type _view_w;

			/* view height */
			size_type _view_h;

			/* view ratio */
			size_type _ratio;

	};
}

#endif // ENGINE_SCREEN_HPP
