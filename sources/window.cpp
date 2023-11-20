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

	auto frame = engine::screen::frame();

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

	CGRect rect = engine::screen::frame();
	rect.size.width = rect.size.width / 10;
	rect.size.height = rect.size.height / 10;

	// -- metal view ----------------------------------------------------------

	_mtkview = MTK::View::alloc()->init(rect, &device);

	if (_mtkview == nullptr)
		throw std::runtime_error("failed to create metal view");

	_mtkview->retain();

	_mtkview->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	_mtkview->setClearColor(MTL::ClearColor::Make(0.1, 0.1, 0.1, 0.0));
	_mtkview->setDelegate(&_view_delegate);

	//_mtkview->setDrawableSize(rect.size);


	_window->setContentView(_mtkview);
	_window->setTitle(NS::String::string("engine", NS::StringEncoding::UTF8StringEncoding));
	//_window->center();
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


