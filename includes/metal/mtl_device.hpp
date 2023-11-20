#ifndef ENGINE_DEVICE_HPP
#define ENGINE_DEVICE_HPP

#include <Metal/Metal.hpp>

#include <string>

namespace mtl {

	class command_queue;
	class library;

	class device final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = mtl::device;


			// -- public lifecycle --------------------------------------------

			/* deleted copy constructor */
			device(const device&) = delete;

			/* deleted move constructor */
			device(device&&) = delete;

			/* destructor */
			inline ~device(void) noexcept {
				if (_device == nullptr) return;
				_device->release();
			}

			/* deleted copy assignment */
			auto operator=(const device&) -> device& = delete;

			/* deleted move assignment */
			auto operator=(device&&) -> device& = delete;



			// -- public static accessors -------------------------------------

			/* instance */
			static inline auto shared(void) -> self& {
				static mtl::device instance;
				return instance;
			}

			/* underlying */
			static inline auto underlying(void) -> MTL::Device& {
				mtl::device& device = shared();
				return *device._device;
			}


			// -- public methods ----------------------------------------------

			/* new render pipeline state */
			//inline auto new_render_pipeline_state(const MTL::RenderPipelineDescriptor&) const -> mtl::render_pipeline_state;


			// -- public convertion operators ---------------------------------

			/* underlying reference */
			inline operator MTL::Device&(void) noexcept {
				return *_device;
			}

			/* underlying const reference */
			inline operator const MTL::Device&(void) const noexcept {
				return *_device;
			}

			/* underlying pointer */
			inline operator MTL::Device*(void) noexcept {
				return _device;
			}

			/* underlying const pointer */
			inline operator const MTL::Device*(void) const noexcept {
				return _device;
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline device(void)
			: _device(MTL::CreateSystemDefaultDevice()) {
				if (_device == nullptr)
					throw std::runtime_error("failed to create system default device");
			}


			// -- private members ---------------------------------------------

			/* device */
			MTL::Device* _device;
	};

}

#endif
