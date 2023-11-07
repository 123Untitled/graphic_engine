#include "window.hpp"



/* default constructor */
engine::window::window(void)
: _window{nullptr}, _mtkview{nullptr}, _view_delegate{} {
}

/* destructor */
engine::window::~window(void) noexcept {
	std::cout << "window destructor" << std::endl;
	destroy();
}

/* init */
auto engine::window::init(void) -> void {


	// -- window --------------------------------------------------------------

	auto frame = CGRect{{400.0, 200.0}, {800.0, 600.0}};

	_window = NS::Window::alloc()->init(
				frame,
				// The window displays a title bar.
				NS::WindowStyleMaskTitled |
				// The window displays a close button.
				NS::WindowStyleMaskClosable |
				// The window displays a minimize button.
				NS::WindowStyleMaskMiniaturizable |
				// The window can be resized by the user.
				NS::WindowStyleMaskResizable,
				// The window has a backing store.
				NS::BackingStoreBuffered,

				false);

	if (_window == nullptr)
		throw std::runtime_error("failed to create window");

	_window->retain();

	// get metal device
	auto& device = mtl::device::underlying();


	// -- metal view ----------------------------------------------------------

	_mtkview = MTK::View::alloc()->init(frame, &device);

	if (_mtkview == nullptr)
		throw std::runtime_error("failed to create metal view");

	_mtkview->retain();

	_mtkview->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	_mtkview->setClearColor(MTL::ClearColor::Make(0.8, 0.3, 0.5, 0.0));
	_mtkview->setDelegate(&_view_delegate);
	_window->setContentView(_mtkview);
	_window->setTitle(NS::String::string("engine", NS::StringEncoding::UTF8StringEncoding));
	_window->center();
	_window->makeKeyAndOrderFront(nullptr);

}

/* destroy */
auto engine::window::destroy(void) noexcept -> void {
	//_pool->release();
	if (_window != nullptr) {
		_window->release();
		_window = nullptr;
	}
	if (_mtkview != nullptr) {
		_mtkview->release();
		_mtkview = nullptr;
	}
}


