#ifndef ENGINE_EVENT_HEADER
#define ENGINE_EVENT_HEADER

#include <ApplicationServices/ApplicationServices.h>

#include <iostream>

#include "time.hpp"

#include <xns>

// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {



	class mouse final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::mouse;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline mouse(void)
			: _sensitivity{10.00f},
			  _mouse_x{0.0f},
			  _mouse_y{0.0f} {}

			/* non-assignable class */
			non_assignable(mouse);

			/* destructor */
			inline ~mouse(void) noexcept = default;



			/* accelerate y */
			inline auto accelerate_y(const double y) noexcept -> void {
				_mouse_y -= (y * _sensitivity * engine::time::delta());
				// clamp y axis
				_mouse_y = _mouse_y > 90.0 ? 90.0 : _mouse_y < -90.0 ? -90.0 : _mouse_y;
			}

			/* accelerate x */
			inline auto accelerate_x(const double x) noexcept -> void {
				_mouse_x -= (x * _sensitivity * engine::time::delta());
				// avoid overflow
				_mouse_x > 360.0 ? _mouse_x -= 360.0 : _mouse_x < -360.0 ? _mouse_x += 360.0 : _mouse_x;
			}

			/* x axis */
			inline auto x_axis(void) const noexcept -> double {
				return _mouse_x * RADIANS;
			}

			/* y axis */
			inline auto y_axis(void) const noexcept -> double {
				return _mouse_y * RADIANS;
			}


		private:

			// -- private constants -------------------------------------------

			/* radians */
			static constexpr double RADIANS = M_PI / 180.0;

			/* max pitch */
			static constexpr double MAX_PITCH = M_PI_2;

			// -- private members ---------------------------------------------

			/* sensitivity */
			float _sensitivity;

			/* mouse x */
			double _mouse_x;

			/* mouse y */
			double _mouse_y;


	};


	class event final {


		public:

			enum key : CGKeyCode {
				LOWER_S = 1,
				LOWER_D = 2,
				LOWER_F = 3,
				LOWER_E = 14,
				LOWER_J = 38,
				LOWER_K = 40,
				LOWER_L = 37,
				LOWER_I = 34,
				LOWER_T = 17,
				LOWER_G = 5,
				SPACE = 49,

			};


			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::event;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline event(void)
			: _keys{} {}

			/* non-assignable class */
			non_assignable(event);

			/* destructor */
			inline ~event(void) noexcept = default;



			// -- public static methods ---------------------------------------

			/* callback */
			static inline auto callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) -> CGEventRef {

				auto& instance = shared();

				// get macos trackpad event
				if (type == kCGEventMouseMoved) {
					// get mouse acceleration
					auto x = ::CGEventGetDoubleValueField(event, kCGMouseEventDeltaX);
					instance._mouse.accelerate_x(x);
					auto y = ::CGEventGetDoubleValueField(event, kCGMouseEventDeltaY);
					instance._mouse.accelerate_y(y);
					//return event;
				}

				if (type == kCGEventKeyDown) {
					auto key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
					shared()._keys[static_cast<engine::event::key>(key)] = true;
					//switch (key) {
					//	case key::LOWER_J:
					//		instance._mouse.accelerate_x(-1.0);
					//		break;
					//	case key::LOWER_L:
					//		instance._mouse.accelerate_x(1.0);
					//		break;
					//	case key::LOWER_I:
					//		instance._mouse.accelerate_y(1.0);
					//		break;
					//	case key::LOWER_K:
					//		instance._mouse.accelerate_y(-1.0);
					//		break;
					//	default:
					//		break;
					//}
					return nullptr;
				}
				if (type == kCGEventKeyUp) {
					auto key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
					shared()._keys[static_cast<engine::event::key>(key)] = false;
					return nullptr;
				}
				//return nullptr;
				return event;
			}

			/* shared */
			static inline auto shared(void) noexcept -> self& {
				static self _shared;
				return _shared;
			}

			/* is pressed */
			static inline auto is_pressed(const engine::event::key key) noexcept -> bool {
				return shared()._keys[key];
			}

			/* get mouse */
			//static inline auto mouse(void) noexcept -> const engine::mouse& {
			//	return shared()._mouse;
			//}

			/* get mouse */
			static inline auto mouse(void) noexcept -> engine::mouse& {
				return shared()._mouse;
			}



		private:

			// -- private members ---------------------------------------------

			/* keys */
			std::unordered_map<engine::event::key, bool> _keys;

			/* mouse */
			engine::mouse _mouse;





	};



}

#endif // ENGINE_EVENT_HEADER
