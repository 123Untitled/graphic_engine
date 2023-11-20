#ifndef METAL_VERTEX_DESCRIPTOR_HPP
#define METAL_VERTEX_DESCRIPTOR_HPP


#include <stdexcept>
#include <Metal/MTLVertexDescriptor.hpp>
#include <simd/simd.h>

#include "vertex.hpp"


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {




	// -- V E R T E X  D E S C R I P T O R ------------------------------------

	class vertex_descriptor final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::vertex_descriptor;


			// -- public lifecycle --------------------------------------------

			/* non-copyable class */
			non_copyable(vertex_descriptor);

			/* move constructor */
			inline vertex_descriptor(self&& other) noexcept
			: _descriptor{other._descriptor} {
				other._descriptor = nullptr;
			}

			/* destructor */
			inline ~vertex_descriptor(void) noexcept {
				if (_descriptor == nullptr) return;
				_descriptor->release();
			}


			// -- public assignment operators ---------------------------------

			/* move assignment operator */
			inline auto operator=(self&& other) noexcept -> self& {
				if (this == &other) return *this;
				if (_descriptor != nullptr) _descriptor->release();
				_descriptor = other._descriptor;
				other._descriptor = nullptr;
				return *this;
			}


			// -- public conversion operators ---------------------------------

			/* underlying reference */
			inline operator MTL::VertexDescriptor&(void) const noexcept {
				return *_descriptor;
			}

			/* underlying const reference */
			inline operator const MTL::VertexDescriptor&(void) const noexcept {
				return *_descriptor;
			}

			/* underlying pointer */
			inline operator MTL::VertexDescriptor*(void) const noexcept {
				return _descriptor;
			}

			/* underlying const pointer */
			inline operator const MTL::VertexDescriptor*(void) noexcept {
				return _descriptor;
			}


			// -- public accessors --------------------------------------------

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::VertexDescriptor& {
				return *_descriptor;
			}


			// -- public static methods ---------------------------------------

			/* default 2D */
			static inline auto default_2D(void) -> mtl::vertex_descriptor {
				mtl::vertex_descriptor descriptor;
				// position
				descriptor.attribute(0, MTL::VertexFormat::VertexFormatFloat2, 0, 0);
				// stride
				descriptor.stride(0, sizeof(engine::vertex_2D));

				return descriptor;
			}

			/* default 3D */
			static inline auto default_3D(void) -> mtl::vertex_descriptor {
				mtl::vertex_descriptor descriptor;
				// position
				descriptor.attribute(0, MTL::VertexFormat::VertexFormatFloat3, 0, 0);
				// normal
				descriptor.attribute(1, MTL::VertexFormat::VertexFormatFloat3, 0, sizeof(simd::float3));
				// texture coordinate
				descriptor.attribute(2, MTL::VertexFormat::VertexFormatFloat2, 0, sizeof(simd::float3) * 2);
				// stride
				descriptor.stride(0, sizeof(engine::vertex));

				return descriptor;
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline vertex_descriptor(void)
			: _descriptor{MTL::VertexDescriptor::alloc()->init()} {
				// check for allocation failure
				if (_descriptor == nullptr)
					throw std::runtime_error{"failed to allocate vertex descriptor"};
			}


			// -- private modifiers -------------------------------------------

			/* attribute */
			inline auto attribute(const unsigned int      index,
								  const MTL::VertexFormat format,
								  const unsigned int      buffer_index,
								  const unsigned int      offset) noexcept -> void {
				_descriptor->attributes()->object(index)->setFormat(format);
				_descriptor->attributes()->object(index)->setBufferIndex(buffer_index);
				_descriptor->attributes()->object(index)->setOffset(offset);
			}

			/* stride */
			inline auto stride(const unsigned int index, const unsigned int stride) noexcept -> void {
				_descriptor->layouts()->object(index)->setStride(stride);
			}


			// -- private members ---------------------------------------------

			/* vertex descriptor */
			MTL::VertexDescriptor* _descriptor;
	};


	// -- V E R T E X  D E S C R I P T O R  L I B R A R Y ---------------------

	class vertex_descriptor_library final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::vertex_descriptor_library;


			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(vertex_descriptor_library);

			/* destructor */
			inline ~vertex_descriptor_library(void) noexcept = default;




			// -- public static accessors -------------------------------------

			/* get vertex descriptor */
			template <xns::basic_string_literal name>
			inline static auto descriptor(void) noexcept -> const mtl::vertex_descriptor& {
				return shared()._descriptors.get<name>();
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline vertex_descriptor_library(void)
			: _descriptors{
				mtl::vertex_descriptor::default_2D(),
				mtl::vertex_descriptor::default_3D()} {
			}


			// -- private static accessors ------------------------------------

			/* instance */
			inline static auto shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}


			// -- private members ---------------------------------------------

			/* vertex descriptors */
			xns::literal_map<mtl::vertex_descriptor, "default_2D",
													 "default_3D"> _descriptors;

	};

}

#endif // METAL_VERTEX_DESCRIPTOR_HPP
