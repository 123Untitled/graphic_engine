#ifndef METAL_BUFFER_HPP
#define METAL_BUFFER_HPP


#include <Metal/Metal.hpp>

#include "mtl_device.hpp"
#include "macros.hpp"
#include <array>


struct vec3 {
	double x;
	double y;
	double z;
};

struct vertex {
	vec3 position;
	vec3 color;
	vec3 normal;
};

#include <iostream>


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	class buffer final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::buffer;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline buffer(void) noexcept
			: _buffer{nullptr} {}

			/* size constructor */
			inline buffer(const std::size_t size)
			: _buffer{mtl::device::underlying().newBuffer(size, MTL::ResourceStorageModeManaged)} {
				if (_buffer == nullptr)
					throw std::runtime_error{"failed to create buffer"};
				_buffer->retain();
			}

			/* non-copyable class */
			non_copyable(buffer);

			/* move constructor */
			inline buffer(self&& buffer) noexcept
			: _buffer{buffer._buffer} {
				buffer._buffer = nullptr;
			}

			/* destructor */
			inline ~buffer(void) noexcept {
				if (_buffer == nullptr) return;
				_buffer->release();
			}


			// -- public assignment operators ---------------------------------

			/* move assignment operator */
			inline auto operator=(self&& buffer) noexcept -> self& {
				if (this == &buffer) return *this;
				if (_buffer != nullptr) _buffer->release();
				_buffer = buffer._buffer;
				buffer._buffer = nullptr;
				return *this;
			}


			// -- public modifiers --------------------------------------------

			/* set contents */
			inline auto set_contents(const void* contents, const std::size_t size) noexcept -> void {
				std::memcpy(_buffer->contents(), contents, size > _buffer->length() ? _buffer->length() : size);
			}

			/* set contents */
			inline auto set_contents(const void* contents) noexcept -> void {
				std::memcpy(_buffer->contents(), contents, _buffer->length());
			}


			// -- public conversion operators ---------------------------------

			/* underlying */
			inline operator const MTL::Buffer*(void) const noexcept {
				return _buffer;
			}


			// -- public boolean operators ------------------------------------

			/* boolean operator */
			explicit inline operator bool(void) const noexcept {
				return _buffer != nullptr;
			}

			/* not operator */
			inline auto operator!(void) const noexcept -> bool {
				return _buffer == nullptr;
			}



		private:

			// -- private members ---------------------------------------------

			/* buffer */
			MTL::Buffer* _buffer;

	};

}

#endif // ENGINE_BUFFER_HPP
