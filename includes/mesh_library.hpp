#ifndef ENGINE_MESH_LIBRARY_HEADER
#define ENGINE_MESH_LIBRARY_HEADER

#include "mesh.hpp"
#include "wavefront.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- M E S H  L I B R A R Y ----------------------------------------------

	class mesh_library final {


		public:

			// -- public constants --------------------------------------------

			enum mesh_type : unsigned int {
				GRID,
				CUBE,
				PLANE,
				SPHERE,
				CYLINDER,
				CONE,
				TORUS,
				CUBOID,
				ICOSAHEDRON,
				OCTAHEDRON,
				NUM_MESHES
			};


			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::mesh_library;

			/* floating point type */
			using float_type = float;


			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(mesh_library);

			/* destructor */
			inline ~mesh_library(void) noexcept = default;


			// -- public static accessors -------------------------------------

			/* get mesh */
			inline static auto get(const mesh_type index) noexcept -> engine::mesh& {
				return self::shared()._meshes[index];
			}

			/* get cube */
			inline static auto cube(void) noexcept -> engine::mesh& {
				return shared()._meshes[CUBE];
			}



		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline mesh_library(void) noexcept
			: _meshes{
				{},
				engine::mesh{engine::wavefront::parse("assets/cube.obj").first},
			} {}


			// -- private instance --------------------------------------------

			/* singleton instance */
			static auto shared(void) -> self& {
				static self instance;
				return instance;
			}


			// -- private members ---------------------------------------------

			/* meshes */
			engine::mesh _meshes[NUM_MESHES];

	};

}

#endif // ENGINE_MESH_LIBRARY_HEADER
