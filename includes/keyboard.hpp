#ifndef ENGINE_KEYBOARD_HPP
#define ENGINE_KEYBOARD_HPP

#include <ApplicationServices/ApplicationServices.h>
#include <iostream>

#include "macros.hpp"

// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {

	enum key : CGKeyCode {
		// 0x00
		LOWER_S = 1,
		LOWER_D = 2,
		LOWER_F = 3,
		LOWER_E = 14,
		LOWER_J = 38,
		LOWER_K = 40,
		LOWER_L = 37,
		LOWER_I = 34,
		MAX_KEY = 8
	};

	// -- L I S T E N E R -----------------------------------------------------

	class listener {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::listener;


			virtual auto key_down(key) -> void = 0;

	};


	class event_manager final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::event_manager;

			/* key callback type */
			using key_callback = void(*)(void);


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline event_manager(void)
			: _listeners{} {}

			/* non-assignable class */
			non_assignable(event_manager);

			/* destructor */
			inline ~event_manager(void) noexcept = default;



			// -- public static methods ---------------------------------------

			/* callback */
			static inline auto callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) -> CGEventRef {
				if (type == kCGEventKeyDown) {

					auto key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

					switch (key) {
						case LOWER_S:
							shared()._listeners[LOWER_S] = true;
							break;
						case LOWER_D:
							shared()._listeners[LOWER_D] = true;
							break;
						case LOWER_F:
							shared()._listeners[LOWER_F] = true;
							break;
						case LOWER_E:
							shared()._listeners[LOWER_E] = true;
							break;
						case LOWER_J:
							shared()._listeners[LOWER_J] = true;
							break;
						case LOWER_K:
							shared()._listeners[LOWER_K] = true;
							break;
						case LOWER_L:
							shared()._listeners[LOWER_L] = true;
							break;
						case LOWER_I:
							shared()._listeners[LOWER_I] = true;
							break;
						default:
							break;
					}
					return nullptr;
				}
				if (type == kCGEventKeyUp) {

					auto key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

					switch (key) {
						case LOWER_S:
							shared()._listeners[LOWER_S] = false;
							break;
						case LOWER_D:
							shared()._listeners[LOWER_D] = false;
							break;
						case LOWER_F:
							shared()._listeners[LOWER_F] = false;
							break;
						case LOWER_E:
							shared()._listeners[LOWER_E] = false;
							break;
						case LOWER_J:
							shared()._listeners[LOWER_J] = false;
							break;
						case LOWER_K:
							shared()._listeners[LOWER_K] = false;
							break;
						case LOWER_L:
							shared()._listeners[LOWER_L] = false;
							break;
						case LOWER_I:
							shared()._listeners[LOWER_I] = false;
							break;
						default:
							break;
					}
					return nullptr;


				}
				return event;
			}

			/* shared */
			static inline auto shared(void) noexcept -> self& {
				static self _shared;
				return _shared;
			}

			/* subscribe */
			static inline void subscribe(engine::key key, key_callback callback) noexcept {
			}

			/* is pressed */
			static inline auto is_pressed(engine::key key) noexcept -> bool {
				return shared()._listeners[key];
			}




		private:

			// -- private members ---------------------------------------------

			/* listeners */
			//std::unordered_map<CGKeyCode, std::vector<key_callback>> _listeners;

			/* listeners */
			bool _listeners[256];




	};



}

#endif // ENGINE_KEYBOARD_HPP
