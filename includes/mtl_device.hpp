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


			/* deleted copy constructor */
			device(const device&) = delete;

			/* deleted move constructor */
			device(device&&) = delete;

			/* destructor */
			~device(void) noexcept;

			/* deleted copy assignment */
			auto operator=(const device&) -> device& = delete;

			/* deleted move assignment */
			auto operator=(device&&) -> device& = delete;



			// -- public static accessors -------------------------------------

			/* instance */
			static auto shared(void) -> self&;

			/* underlying */
			static inline auto underlying(void) -> MTL::Device& {
				mtl::device& device = shared();
				return *device._device;
			}


			// -- public convertion operators ---------------------------------

			/* underlying */
			inline operator MTL::Device&(void) noexcept {
				return *_device;
			}

			/* new buffer */
			static auto new_buffer(const std::size_t, const std::size_t) -> MTL::Buffer*;

			/* new buffer */
			static auto new_buffer(const std::size_t) -> MTL::Buffer*;


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			device(void);


			// -- private members ---------------------------------------------

			/* device */
			MTL::Device* _device;
	};

}

#endif
