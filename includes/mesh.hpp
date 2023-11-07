#ifndef ENGINE_MESH_HPP
#define ENGINE_MESH_HPP


#include <Metal/Metal.hpp>

#include "mtl_render_command_encoder.hpp"
#include "vertex.hpp"

#include "mtl_buffer.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- M E S H -------------------------------------------------------------

	class mesh final {


		public:


			// -- O P T I O N S -----------------------------------------------

			class options final {


				public:

					// -- public lifecycle ------------------------------------

					/* default constructor */
					inline options(void) noexcept
					:		_type{MTL::PrimitiveTypeTriangle},
						_fillmode{MTL::TriangleFillModeFill},
						_cullmode{MTL::CullModeBack},
						 _winding{MTL::WindingClockwise} {}


					// -- public accessors ------------------------------------

					/* primitive type */
					inline auto primitive(void) const noexcept -> MTL::PrimitiveType {
						return _type;
					}

					/* triangle fill mode */
					inline auto fillmode(void) const noexcept -> MTL::TriangleFillMode {
						return _fillmode;
					}

					/* cull mode */
					inline auto cullmode(void) const noexcept -> MTL::CullMode {
						return _cullmode;
					}

					/* winding */
					inline auto winding(void) const noexcept -> MTL::Winding {
						return _winding;
					}


					// -- public modifiers ------------------------------------

					/* primitive type */
					inline auto primitive(const MTL::PrimitiveType type) noexcept -> void {
						_type = type;
					}

					/* triangle fill mode */
					inline auto fillmode(const MTL::TriangleFillMode fillmode) noexcept -> void {
						_fillmode = fillmode;
					}

					/* cull mode */
					inline auto cullmode(const MTL::CullMode cullmode) noexcept -> void {
						_cullmode = cullmode;
					}

					/* winding */
					inline auto winding(const MTL::Winding winding) noexcept -> void {
						_winding = winding;
					}


				private:

					MTL::PrimitiveType    _type;
					MTL::TriangleFillMode _fillmode;
					MTL::CullMode         _cullmode;
					MTL::Winding          _winding;
					//MTL::Winding _winding;
					//MTL::DepthClipMode _depthclipmode;
					//MTL::DepthStencilDescriptor _depthstencil;

			};


			/* default constructor */
			inline mesh(void) noexcept
			: _opts{}, _vertex{}, _index{} {}

			/* vertex constructor */
			inline mesh(const std::vector<engine::vertex>& vertex) noexcept
			: _opts{}, _vertex{vertex.size() * sizeof(engine::vertex)}, _vcount{vertex.size()}, _index{}, _icount{0} {
				// size already set (overloaded method)
				_vertex.set_contents(vertex.data());
			}

			/* move constructor */
			inline mesh(mesh&& mesh) noexcept
			: _opts{std::move(mesh._opts)}, _vertex{std::move(mesh._vertex)}, _index{std::move(mesh._index)} {}

			/* destructor */
			inline ~mesh(void) noexcept = default;


			/* render */
			inline auto render(mtl::render_command_encoder& encoder) -> void {

				// set index buffer

				// set cull mode
				encoder.cullmode(_opts.cullmode());
				encoder.fillmode(_opts.fillmode());
				// set front facing
				encoder.winding(_opts.winding());


				encoder.set_vertex_buffer(_vertex, 0, 0);

				encoder.draw_primitives(_opts.primitive(), _vcount);


				//if (not _index)
					//encoder.drawPrimitives(_opts.primitive(), 0, 0, 0);
				//else
					//encoder.drawIndexedPrimitives(_opts.primitive(), _index.length() / sizeof(unsigned int), MTL::IndexType::UInt32, _index, 0);

//drawIndexedPrimitives(	MTL::PrimitiveType primitiveType,
//						NS::UInteger indexCount,
//						MTL::IndexType indexType,
//						const MTL::Buffer* indexBuffer,
//						NS::UInteger indexBufferOffset,
//						NS::UInteger instanceCount)

				//encoder.drawIndexedPrimitives(_opt.primitive(), _index.length() / sizeof(unsigned int), MTL::IndexType::UInt32, _index, 0);

			}

		private:

			// -- private members ---------------------------------------------

			/* options */
			options _opts;

			/* vertex buffer */
			mtl::buffer _vertex;

			/* vertex count */
			std::size_t _vcount;

			/* index buffer */
			mtl::buffer _index;

			/* index count */
			std::size_t _icount;

	};


}




#endif // ENGINE_MESH_HPP
