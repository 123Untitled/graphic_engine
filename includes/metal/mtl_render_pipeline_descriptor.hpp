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

			/* non-copyable class */
			non_copyable(render_pipeline_descriptor);

			/* move constructor */
			inline render_pipeline_descriptor(self&& other) noexcept
			: _descriptor{other._descriptor} {
				other._descriptor = nullptr;
			}

			/* destructor */
			inline ~render_pipeline_descriptor(void) noexcept {
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


			// -- public static methods ---------------------------------------

			/* make render pipeline descriptor */
			template <xns::basic_string_literal name>
			static inline auto make(void) -> self {

				self descriptor;

				descriptor.raster_sample(4);



				static_assert(name == "default_2D" || name == "default_3D", "invalid render pipeline descriptor name");


				if constexpr (name == "default_3D") {
					// vertex descriptor
					descriptor.vertex_descriptor<name>();
					// vertex function
					descriptor.vertex_function<"vertex_main">();
					// fragment function
					descriptor.fragment_function<"fragment_main">();
				}
				else if constexpr (name == "default_2D") {
					// vertex descriptor
					descriptor.vertex_descriptor<name>();
					// vertex function
					descriptor.vertex_function<"default_vertex_2d">();
					// fragment function
					descriptor.fragment_function<"default_fragment_2d">();
				}

				descriptor.color_attachment(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
				descriptor.depth_attachment(MTL::PixelFormat::PixelFormatDepth32Float_Stencil8);

				// enable blending
				descriptor.blending<true>();

				// blend operations
				descriptor.rgb_blend_operation  (MTL::BlendOperation::BlendOperationAdd);
				descriptor.alpha_blend_operation(MTL::BlendOperation::BlendOperationAdd);

				// blend factor
				descriptor.source_rgb_blend_factor       (MTL::BlendFactor::BlendFactorSourceAlpha);
				descriptor.source_alpha_blend_factor     (MTL::BlendFactor::BlendFactorSourceAlpha);
				descriptor.destination_rgb_blend_factor  (MTL::BlendFactor::BlendFactorOneMinusSourceAlpha);
				descriptor.destination_alpha_blend_factor(MTL::BlendFactor::BlendFactorOneMinusSourceAlpha);

				return descriptor;
			}






			// -- public conversion operators ---------------------------------

			/* underlying reference */
			inline operator MTL::RenderPipelineDescriptor&(void) const noexcept {
				return *_descriptor;
			}

			/* underlying const reference */
			inline operator const MTL::RenderPipelineDescriptor&(void) const noexcept {
				return *_descriptor;
			}

			/* underlying pointer */
			inline operator MTL::RenderPipelineDescriptor*(void) const noexcept {
				return _descriptor;
			}

			/* underlying const pointer */
			inline operator const MTL::RenderPipelineDescriptor*(void) const noexcept {
				return _descriptor;
			}


			// -- public accessors --------------------------------------------

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::RenderPipelineDescriptor& {
				return *_descriptor;
			}



		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline render_pipeline_descriptor(void)
			: _descriptor{MTL::RenderPipelineDescriptor::alloc()->init()} {
				if (_descriptor == nullptr)
					throw std::runtime_error{"failed to create render pipeline descriptor"};
			}


			// -- private modifiers -------------------------------------------

			/* set vertex descriptor */
			template <xns::basic_string_literal name>
			inline auto vertex_descriptor(void) noexcept -> void {
				_descriptor->setVertexDescriptor(mtl::vertex_descriptor_library::descriptor<name>());
			}

			/* set vertex function */
			template <xns::basic_string_literal name>
			inline auto vertex_function(void) noexcept -> void{
				_descriptor->setVertexFunction(engine::shaderlibrary::function<name>());
			}

			/* set fragment function */
			template <xns::basic_string_literal name>
			inline auto fragment_function(void) noexcept -> void {
				_descriptor->setFragmentFunction(engine::shaderlibrary::function<name>());
			}

			/* color attachment */
			inline auto color_attachment(const MTL::PixelFormat format, const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setPixelFormat(format);
			}

			/* depth attachment */
			inline auto depth_attachment(const MTL::PixelFormat format) noexcept -> void {
				_descriptor->setDepthAttachmentPixelFormat(format);
			}

			/* raster sample count */
			inline auto raster_sample(const unsigned int count) noexcept -> void {
				_descriptor->setRasterSampleCount(count);
			}

			/* blending */
			template <bool enabled>
			inline auto blending(const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setBlendingEnabled(enabled);
			}


			/* rgb blend operation */
			inline auto rgb_blend_operation(const MTL::BlendOperation operation, const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setRgbBlendOperation(operation);
			}

			/* alpha blend operation */
			inline auto alpha_blend_operation(const MTL::BlendOperation operation, const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setAlphaBlendOperation(operation);
			}

			/* source rgb blend factor */
			inline auto source_rgb_blend_factor(const MTL::BlendFactor factor, const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setSourceRGBBlendFactor(factor);
			}

			/* source alpha blend factor */
			inline auto source_alpha_blend_factor(const MTL::BlendFactor factor, const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setSourceAlphaBlendFactor(factor);
			}

			/* destination rgb blend factor */
			inline auto destination_rgb_blend_factor(const MTL::BlendFactor factor, const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setDestinationRGBBlendFactor(factor);
			}

			/* destination alpha blend factor */
			inline auto destination_alpha_blend_factor(const MTL::BlendFactor factor, const unsigned int index = 0U) noexcept -> void {
				_descriptor->colorAttachments()->object(index)->setDestinationAlphaBlendFactor(factor);
			}



			// -- private members ---------------------------------------------

			/* underlying */
			MTL::RenderPipelineDescriptor* _descriptor;

	};


	 // -- R E N D E R  P I P E L I N E  D E S C R I P T O R  L I B R A R Y ---

	class render_pipeline_descriptor_library final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::render_pipeline_descriptor_library;


			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(render_pipeline_descriptor_library);

			/* destructor */
			inline ~render_pipeline_descriptor_library(void) noexcept = default;



			// -- public static accessors -------------------------------------

			/* get vertex descriptor */
			template <xns::basic_string_literal name>
			inline static auto pipeline_descriptor(void) noexcept -> const mtl::render_pipeline_descriptor& {
				static_assert(is_valid_literal<name>, "invalid render pipeline descriptor name");
				return shared()._descriptors.template get<name>();
			}



		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline render_pipeline_descriptor_library(void)
			: _descriptors{
				mtl::render_pipeline_descriptor::make<"default_2D">(),
				mtl::render_pipeline_descriptor::make<"default_3D">()} {
			}


			// -- private static accessors ------------------------------------

			/* instance */
			inline static auto shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}


			// -- private members ---------------------------------------------

			/* vertex descriptors */
			xns::literal_map<mtl::render_pipeline_descriptor, "default_2D",
															  "default_3D"> _descriptors;


			/* is valid literal */
			template <xns::basic_string_literal name>
			static constexpr bool is_valid_literal = name == "default_2D"
												  || name == "default_3D";

	};

}

#endif // METAL_RENDER_PIPELINE_DESCRIPTOR_HPP
