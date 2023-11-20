#ifndef METAL_RENDER_COMMAND_ENCODER_HPP
#define METAL_RENDER_COMMAND_ENCODER_HPP

#include <Metal/Metal.hpp>

#include "mtl_command_buffer.hpp"
#include "mtl_render_pass_descriptor.hpp"
#include "mtl_render_pipeline_state.hpp"
#include "mtl_depth_stencil_state.hpp"
#include "mtl_buffer.hpp"


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- R E N D E R  C O M M A N D  E N C O D E R ---------------------------

	class render_command_encoder final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::render_command_encoder;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline render_command_encoder(void) noexcept
			: _encoder{nullptr} {}

			/* command / descriptor constructor */
			inline render_command_encoder(const mtl::command_buffer& command, mtl::render_pass_descriptor& descriptor)
			: _encoder{command.underlying().renderCommandEncoder(&descriptor.underlying())} {
				if (_encoder == nullptr)
					throw std::runtime_error{"failed to create render command encoder"};
				_encoder->retain();
			}

			/* destructor */
			inline ~render_command_encoder(void) noexcept {
				if (_encoder == nullptr) return;
				_encoder->release();
			}

			/* end encoding */
			inline void end_encoding(void) const noexcept {
				_encoder->endEncoding();
			}



			/* set render pipeline state */
			inline auto set_render_pipeline_state(const mtl::render_pipeline_state& state) noexcept -> void {
				_encoder->setRenderPipelineState(state);
			}

			/* set depth stencil state */
			inline auto set_depth_stencil_state(const mtl::depth_stencil_state& state) noexcept -> void {
				_encoder->setDepthStencilState(state);
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::RenderCommandEncoder& {
				return *_encoder;
			}


			// -- public modifiers --------------------------------------------

			/* cullmode */
			inline void cullmode(const MTL::CullMode mode) noexcept {
				_encoder->setCullMode(mode);
			}

			/* triangle fill mode */
			inline void fillmode(const MTL::TriangleFillMode mode) noexcept {
				_encoder->setTriangleFillMode(mode);
			}

			/* front facing winding */
			inline void winding(const MTL::Winding winding) noexcept {
				_encoder->setFrontFacingWinding(winding);
			}

			/* set vertex buffer */
			inline void set_vertex_buffer(const mtl::buffer& buffer, const std::size_t offset, const std::size_t index) noexcept {
				_encoder->setVertexBuffer(buffer, offset, index);
			}

			/* set vertex bytes */
			inline void set_vertex_bytes(const void* bytes, const std::size_t length, const std::size_t index) noexcept {
				_encoder->setVertexBytes(bytes, length, index);
			}

			/* draw primitives */
			inline void draw_primitives(const MTL::PrimitiveType type, const std::size_t count) noexcept {
				_encoder->drawPrimitives(type, static_cast<std::size_t>(0U), count);
			}

			/* draw indexed primitives */
			inline void draw_indexed_primitives(const MTL::PrimitiveType type, const std::size_t count, const mtl::buffer& buffer) noexcept {
				_encoder->drawIndexedPrimitives(type, count, MTL::IndexTypeUInt32, buffer, static_cast<std::size_t>(0U));
			}

			/* set fragment bytes */
			inline void set_fragment_bytes(const void* bytes, const std::size_t length, const std::size_t index) noexcept {
				_encoder->setFragmentBytes(bytes, length, index);
			}




		private:

			// -- private members ---------------------------------------------

			/* render command encoder */
			MTL::RenderCommandEncoder* _encoder;

	};


}

#endif // METAL_RENDER_COMMAND_ENCODER_HPP
