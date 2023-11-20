#ifndef ENGINE_APP_HPP
#define ENGINE_APP_HPP

// macos headers
#include <AppKit/AppKit.hpp>
#include <ApplicationServices/ApplicationServices.h>

// local headers
#include "window.hpp"
#include "event.hpp"

#include <thread>

// -- E N G I N E -------------------------------------------------------------

namespace engine {


	// -- A P P ---------------------------------------------------------------

	class app : public NS::ApplicationDelegate {

		public:

			/* default constructor */
			app(void);

			/* delete copy constructor */
			app(const app&) = delete;

			/* delete move constructor */
			app(app&&) noexcept = delete;

			/* destructor */
			~app(void) noexcept override;


			// -- public assignment operators ---------------------------------

			/* delete copy assignment operator */
			auto operator=(const app&) -> app& = delete;

			/* delete move assignment operator */
			auto operator=(app&&) noexcept -> app& = delete;



			void applicationWillFinishLaunching(NS::Notification*) override;
			void applicationDidFinishLaunching(NS::Notification*) override;
			bool applicationShouldTerminateAfterLastWindowClosed(NS::Application*) override;
			bool applicationShouldTerminate(NS::Application*) override;
			void applicationWillTerminate(NS::Notification*) override;

			void applicationWillBecomeActive(NS::Notification*) override;
			void applicationWillResignActive(NS::Notification*) override;

			/* shared application */
			static auto shared(void) noexcept -> NS::Application&;


		private:

			// -- private static methods --------------------------------------

			/* create menu bar */
			auto create_menu(void) noexcept -> NS::Menu*;

			/* notif to application */
			static auto to_application(NS::Notification&) noexcept -> NS::Application&;


			// -- private members ---------------------------------------------

			//NS::AutoreleasePool* _pool;

			/* window */
			engine::window _window;

			/* event tap */
			CFMachPortRef _event_tap;

	};

}

#endif // ENGINE_APP_HPP
