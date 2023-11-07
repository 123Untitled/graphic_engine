#ifndef METAL_VERTEX_DESCRIPTOR_HPP
#define METAL_VERTEX_DESCRIPTOR_HPP


#include <stdexcept>
#include <vertex.hpp>
#include <Metal/MTLVertexDescriptor.hpp>
#include <simd/simd.h>


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- V E R T E X  D E S C R I P T O R ------------------------------------

	class vertex_descriptor final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::vertex_descriptor;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline vertex_descriptor(void)
			: _descriptor{MTL::VertexDescriptor::alloc()->init()} {
				if (_descriptor == nullptr)
					throw std::runtime_error{"failed to allocate vertex descriptor"};

				// position
				_descriptor->attributes()->object(0)->setFormat(MTL::VertexFormat::VertexFormatFloat3);
				_descriptor->attributes()->object(0)->setBufferIndex(0);
				_descriptor->attributes()->object(0)->setOffset(0);

				// color
				_descriptor->attributes()->object(1)->setFormat(MTL::VertexFormat::VertexFormatFloat3);
				_descriptor->attributes()->object(1)->setBufferIndex(0);
				_descriptor->attributes()->object(1)->setOffset(sizeof(simd::float3));

				// normal
				_descriptor->attributes()->object(2)->setFormat(MTL::VertexFormat::VertexFormatFloat3);
				_descriptor->attributes()->object(2)->setBufferIndex(0);
				_descriptor->attributes()->object(2)->setOffset(sizeof(simd::float3) * 2);

				_descriptor->layouts()->object(0)->setStride(sizeof(engine::vertex));
			}


			/* destructor */
			inline ~vertex_descriptor(void) noexcept {
				if (_descriptor == nullptr) return;
				_descriptor->release();
			}


			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::VertexDescriptor& {
				return *_descriptor;
			}


		private:

			// -- private members ---------------------------------------------

			/* vertex descriptor */
			MTL::VertexDescriptor* _descriptor;


	};

}

#endif // METAL_VERTEX_DESCRIPTOR_HPP
