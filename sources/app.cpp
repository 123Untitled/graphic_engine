#include "app.hpp"

/* default constructor */
engine::app::app(void)
: NS::ApplicationDelegate{}, _window{}, _event_tap{} {
	auto& app = shared();
	app.setDelegate(this);
	app.run();
}

/* destructor */
engine::app::~app(void) noexcept {
	CGEventTapEnable(_event_tap, false);
	CFRelease(_event_tap);
}

/* shared application */
auto engine::app::shared(void) noexcept -> NS::Application& {
	return *NS::Application::sharedApplication();
}


void engine::app::applicationWillFinishLaunching(NS::Notification* notif) {

	_event_tap = CGEventTapCreateForPid(
			::getpid(),
			kCGHeadInsertEventTap,
			kCGEventTapOptionDefault,
			kCGEventMaskForAllEvents,
			&engine::event_manager::callback,
			nullptr
	);

	if (!_event_tap) {
		throw std::runtime_error{"failed to create event tap"};
	}

	// bind event tap to run loop
	::CFRunLoopSourceRef run_loop_source = ::CFMachPortCreateRunLoopSource(kCFAllocatorDefault, _event_tap, 0);
	::CFRunLoopAddSource(CFRunLoopGetCurrent(), run_loop_source, kCFRunLoopCommonModes);
	::CGEventTapEnable(_event_tap, true);



	std::cout << "application will finish launching" << std::endl;
	to_application(*notif).setMainMenu(create_menu());
	to_application(*notif).setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void engine::app::applicationDidFinishLaunching(NS::Notification* notif) {
	std::cout << "application did finish launching" << std::endl;
	_window.init();
	to_application(*notif).activateIgnoringOtherApps(true);
}


bool engine::app::applicationShouldTerminateAfterLastWindowClosed(NS::Application* app) {
	std::cout << "application should terminate after last window closed" << std::endl;
	app->stop(app);
	return false;
}

bool engine::app::applicationShouldTerminate(NS::Application* app) {
	std::cout << "application should terminate" << std::endl;
	app->terminate(app);
	return false;
}

void engine::app::applicationWillTerminate(NS::Notification* notif) {
	std::cout << "application will terminate" << std::endl;
}

/* application will become active */
void engine::app::applicationWillBecomeActive(NS::Notification* notif) {
	std::cout << "focus gained" << std::endl;
}

/* application will resign active */
void engine::app::applicationWillResignActive(NS::Notification* notif) {
	std::cout << "focus lost" << std::endl;
}


auto engine::app::to_application(NS::Notification& notif) noexcept -> NS::Application& {
	return *reinterpret_cast<NS::Application*>(notif.object());
}

auto engine::app::create_menu(void) noexcept -> NS::Menu* {
	using NS::StringEncoding::UTF8StringEncoding;

	// create a main menu instance
	auto* main_menu = NS::Menu::alloc()->init();

	// create an app menu instance 
	auto* app_menu = NS::Menu::alloc()->init(NS::String::string("Appname", UTF8StringEncoding));

	// create a menuItem instance for
	auto* menu_item = NS::MenuItem::alloc()->init();

	// ptr to application title name
	auto* app_name = NS::RunningApplication::currentApplication()->localizedName();

	// create a string and append a string to it
	NS::String* quit_item_name = NS::String::string("Quit ", UTF8StringEncoding)->stringByAppendingString(app_name);

	// callback object pointer that closes application
	SEL quit_callback = NS::MenuItem::registerActionCallback("appQuit", [](void*,SEL,const NS::Object* sender){
		engine::app::shared().stop(sender);
	});

	// add a menu item that invokes a callback when pressing q while command key is pressed
	NS::MenuItem* app_quit_item = app_menu->addItem(quit_item_name, quit_callback, NS::String::string("q", UTF8StringEncoding));

	app_quit_item->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

	// add as submenu option for menu
	menu_item->setSubmenu(app_menu);
	// add menu items to main menu
	main_menu->addItem(menu_item);

	// release memory
	menu_item->release();
	 app_menu->release();

	// return ptr holding main menu
	return main_menu->autorelease();
}

