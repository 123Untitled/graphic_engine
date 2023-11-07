#ifndef ENGINE_OBJECT_HPP
#define ENGINE_OBJECT_HPP


#include "mtl_render_command_encoder.hpp"
#include "mesh.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- O B J E C T ---------------------------------------------------------

	class object final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::object;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline object(void) noexcept = default;

			/* mesh constructor */
			inline object(engine::mesh&& mesh) noexcept
			: _mesh{std::move(mesh)}, _children{} {}

			/* destructor */
			inline ~object(void) noexcept = default;




			/* render */
			inline auto render(mtl::render_command_encoder& encoder) noexcept -> void {

				// swift code
				//encoder.setVertexBytes(
				//// give a variable reference
				//&modelMatrix,
				//length: MemoryLayout<ModelMatrix>.stride,
				//// at index 1
				//index: 3
				//);
				// fragment shader material
				//encoder.setFragmentBytes(&material, length: MemoryLayout<Material>.stride, index: 1);

				_mesh.render(encoder);

				for (auto& child : _children)
					child.render(encoder);
			}


		private:

			/* mesh */
			engine::mesh _mesh;

			/* child objects */
			std::vector<engine::object> _children;

	};

}

#endif // ENGINE_OBJECT_HPP
