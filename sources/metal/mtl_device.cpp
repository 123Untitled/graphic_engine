#include "mtl_device.hpp"

#include "mtl_command_queue.hpp"
#include "mtl_library.hpp"


/* default constructor */
mtl::device::device(void)
: _device(MTL::CreateSystemDefaultDevice()) {
	if (_device == nullptr)
		throw std::runtime_error("failed to create system default device");
}

/* destructor */
mtl::device::~device(void) noexcept {
	if (_device == nullptr) return;
	_device->release();
}

/* instance */
auto mtl::device::shared(void) -> self& {
	static mtl::device instance;
	return instance;
}


/* new buffer */
auto mtl::device::new_buffer(const std::size_t length, const std::size_t size) -> MTL::Buffer* {
	return shared()._device->newBuffer(length * size, MTL::ResourceStorageModeManaged);
}

/* new buffer */
auto mtl::device::new_buffer(const std::size_t size) -> MTL::Buffer* {
	return shared()._device->newBuffer(size, MTL::ResourceStorageModeManaged);
}
