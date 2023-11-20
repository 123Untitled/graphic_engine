#ifndef ENGINE_MESH_HPP
#define ENGINE_MESH_HPP


#include <Metal/Metal.hpp>

#include "mtl_render_command_encoder.hpp"
#include "vertex.hpp"

#include "options.hpp"
#include "mtl_buffer.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- M E S H -------------------------------------------------------------

	class mesh final {


		public:



			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::mesh;


			/* default constructor */
			inline mesh(void) noexcept
			: _vertices{}, _indexes{} {}

			/* vertex constructor */
			inline mesh(const std::vector<engine::vertex>& vertex) noexcept
			:	_vertices{vertex.size() * sizeof(engine::vertex)},
				_indexes{},
				_vcount{vertex.size()},
				_icount{0} {
				// size already set (overloaded method)
				_vertices.set_contents(vertex.data());
			}

			/* vertex 2D constructor */
			inline mesh(const std::vector<engine::vertex_2D>& vertex) noexcept
			:	_vertices{vertex.size() * sizeof(engine::vertex_2D)},
				_indexes{},
				_vcount{vertex.size()},
				_icount{0} {
				// size already set (overloaded method)
				_vertices.set_contents(vertex.data());
			}

			/* vertex 2D / index constructor */
			inline mesh(const std::vector<engine::vertex_2D>& vertex, const std::vector<unsigned int>& index) noexcept
			:	_vertices{vertex.size() * sizeof(engine::vertex_2D)},
				_indexes{index.size() * sizeof(unsigned int)},
				_vcount{vertex.size()},
				_icount{index.size()} {
				// size already set (overloaded method)
				_vertices.set_contents(vertex.data());
				_indexes.set_contents(index.data());
			}

			template <decltype(sizeof(0)) NV, decltype(NV) NI>
			inline mesh(const xns::array<engine::vertex_2D, NV>& vertices,
						const xns::array<unsigned int, NI>&      indexes) noexcept
			:   _vertices{vertices.size() * sizeof(engine::vertex_2D)},
				 _indexes{indexes.size()  * sizeof(unsigned int)},
				_vcount{vertices.size()},
				_icount{indexes.size()} {
				// size already set (overloaded method)
				_vertices.set_contents(vertices.data());
				_indexes.set_contents(indexes.data());
			}

			template <decltype(sizeof(0)) NV>
			inline mesh(const xns::array<engine::vertex_2D, NV>& vertices) noexcept
			:   _vertices{vertices.size() * sizeof(engine::vertex_2D)},
				 _indexes{},
				_vcount{vertices.size()},
				_icount{0} {
				// size already set (overloaded method)
				_vertices.set_contents(vertices.data());
			}


			/* vertex + index constructor */
			inline mesh(const engine::vpackage& vpackage) noexcept
			:	_vertices{vpackage.first.size() * sizeof(engine::vertex)},
				_indexes{vpackage.second.size() * sizeof(unsigned int)},
				_vcount{vpackage.first.size()},
				_icount{vpackage.second.size()} {

				_vertices.set_contents(vpackage.first.data());
				_indexes.set_contents(vpackage.second.data());
			}

			/* move constructor */
			inline mesh(mesh&& mesh) noexcept
			: _vertices{std::move(mesh._vertices)}, _indexes{std::move(mesh._indexes)}, _vcount{mesh._vcount}, _icount{mesh._icount} {
			}

			/* destructor */
			inline ~mesh(void) noexcept = default;



			/* render */
			inline auto render(mtl::render_command_encoder& encoder, const engine::options& opts) const noexcept -> void {

				opts.render(encoder);
				// set index buffer
				encoder.set_vertex_buffer(_vertices, 0, 0);

				if (not _indexes) {
					encoder.draw_primitives(opts.primitive(), _vcount);
				}
				else
					encoder.draw_indexed_primitives(opts.primitive(), _icount, _indexes);
			}


		private:

			// -- private members ---------------------------------------------


			/* vertex buffer */
			mtl::buffer _vertices;

			/* index buffer */
			mtl::buffer _indexes;


			/* vertex count */
			std::size_t _vcount;

			/* index count */
			std::size_t _icount;

	};


}

#endif // ENGINE_MESH_HPP
