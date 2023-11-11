#ifndef METAL_COMMAND_QUEUE_HPP
#define METAL_COMMAND_QUEUE_HPP


#include <Metal/Metal.hpp>
#include <iostream>

#include "mtl_device.hpp"



// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- C O M M A N D  Q U E U E --------------------------------------------

	class command_queue final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::command_queue;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline command_queue(void)
			: _queue{mtl::device::underlying().newCommandQueue()} {
				if (_queue == nullptr) {
					throw std::runtime_error{"Failed to create command queue"};
				}
			}

			/* deleted copy constructor */
			command_queue(const self&) = delete;

			/* move constructor */
			inline command_queue(self&& other)
			: _queue{other._queue} {
				// invalidate other
				other._queue = nullptr;
			}

			inline ~command_queue(void) {
				if (_queue == nullptr) return;
				_queue->release();
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment */
			auto operator=(const self&) -> self& = delete;

			/* move assignment */
			inline auto operator=(self&& other) noexcept -> self& {
				if (this == &other) return *this;
				// invalidate self
				if (_queue != nullptr) _queue->release();
				// move other
				_queue = other._queue;
				// invalidate other
				other._queue = nullptr;
				// return self
				return *this;
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::CommandQueue& {
				return *_queue;
			}


		private:

			// -- private members ---------------------------------------------

			/* command queue */
			MTL::CommandQueue* _queue;
	};

};






#endif // METAL_COMMAND_QUEUE_HPP
