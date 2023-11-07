#include "view.hpp"


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
