#ifndef METAL_RENDER_PIPELINE_DESCRIPTOR_HPP
#define METAL_RENDER_PIPELINE_DESCRIPTOR_HPP

#include <Metal/Metal.hpp>

#include "mtl_function.hpp"
#include "shaderlibrary.hpp"
#include "mtl_vertex_descriptor.hpp"

// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- R E N D E R  P I P E L I N E  D E S C R I P T O R -------------------

	class render_pipeline_descriptor final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::render_pipeline_descriptor;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			inline render_pipeline_descriptor(void)
			: _descriptor{MTL::RenderPipelineDescriptor::alloc()->init()} {
				if (_descriptor == nullptr)
					throw std::runtime_error{"failed to create render pipeline descriptor"};
				// this object leaks when use retain() method
				// _descriptor->retain();

				// setup descriptor

				// antialiasing
				_descriptor->setRasterSampleCount(4);

				// vertex descriptor
				mtl::vertex_descriptor vdesc;
				_descriptor->setVertexDescriptor(&vdesc.underlying());

				_descriptor->setVertexFunction(engine::shaderlibrary::vertex("vertex_main"));
				_descriptor->setFragmentFunction(engine::shaderlibrary::fragment("fragment_main"));
				_descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

				_descriptor->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float_Stencil8);

				//auto* obj = _descriptor->colorAttachments()->object(0);

				//obj->setBlendingEnabled(true);
				//obj->setRgbBlendOperation(MTL::BlendOperation::BlendOperationAdd);
				//obj->setAlphaBlendOperation(MTL::BlendOperation::BlendOperationAdd);
				//obj->setSourceRGBBlendFactor(MTL::BlendFactor::BlendFactorSourceAlpha);
				//obj->setSourceAlphaBlendFactor(MTL::BlendFactor::BlendFactorSourceAlpha);
				//obj->setDestinationRGBBlendFactor(MTL::BlendFactor::BlendFactorOneMinusSourceAlpha);
				//obj->setDestinationAlphaBlendFactor(MTL::BlendFactor::BlendFactorOneMinusSourceAlpha);
			}

			/* non-assignable class */
			non_assignable(render_pipeline_descriptor);


			/* destructor */
			inline ~render_pipeline_descriptor(void) noexcept {
				if (_descriptor == nullptr) return;
				_descriptor->release();
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::RenderPipelineDescriptor& {
				return *_descriptor;
			}


			// -- public conversion operators ---------------------------------

			/* underlying */
			inline operator MTL::RenderPipelineDescriptor&(void) const noexcept {
				return *_descriptor;
			}

			/* const underlying */
			inline operator const MTL::RenderPipelineDescriptor*(void) const noexcept {
				return _descriptor;
			}


		private:

			// -- private methods ---------------------------------------------

			/* allocate and initialize */
			static inline auto allocate(void) -> MTL::RenderPipelineDescriptor* {
				auto* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
				if (descriptor == nullptr)
					throw std::runtime_error{"failed to create render pipeline descriptor"};
				descriptor->retain();
				return descriptor;
			}


			// -- private members ---------------------------------------------

			/* underlying */
			MTL::RenderPipelineDescriptor* _descriptor;

	};

}

#endif // METAL_RENDER_PIPELINE_DESCRIPTOR_HPP
