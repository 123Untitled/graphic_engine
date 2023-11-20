#include "view.hpp"

#include "camera.hpp"

/* default constructor */
engine::view_delegate::view_delegate(void)
: MTK::ViewDelegate{}, _renderer{} {}

/* destructor */
engine::view_delegate::~view_delegate(void) noexcept {
}

/* draw in view */
void engine::view_delegate::drawInMTKView(MTK::View* view) {
	mtl::view mview{};
	mview._view = view;
	_renderer.draw(mview);
	mview._view = nullptr;
}

/* drawable size will change */
void engine::view_delegate::drawableSizeWillChange(MTK::View*, CGSize size) {
	engine::screen::update(size.width, size.height);
}
