#ifndef ENGINE_VERTEX_HPP
#define ENGINE_VERTEX_HPP


#include <simd/simd.h>
#include <iostream>


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- V E R T E X ---------------------------------------------------------

	class vertex final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::vertex;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline vertex(void) noexcept = delete;

			/* position constructor */
			inline vertex(const float x, const float y, const float z) noexcept
			: _vertex{{simd::float3{x, y, z}},
					  {simd::float3{1.0f, 0.0f, 1.0f}},
					  {simd::float3{0.0f, 0.0f, 0.0f}}} {}

			/* position + color constructor */
			inline vertex(const float x, const float y, const float z,
						  const float r, const float g, const float b) noexcept
			: _vertex{{simd::float3{x, y, z}},
					  {simd::float3{r, g, b}},
					  {simd::float3{0.0f, 0.0f, 0.0f}}} {}

			/* position + color + normal constructor */
			inline vertex(const float px, const float py, const float pz,
						  const float cr, const float cg, const float cb,
						  const float nx, const float ny, const float nz) noexcept
			: _vertex{{simd::float3{px, py, pz}},
					  {simd::float3{cr, cg, cb}},
					  {simd::float3{nx, ny, nz}}} {}

			/* copy constructor */
			inline vertex(const self& other) noexcept
			: _vertex{{other._vertex[POSITION]},
					  {other._vertex[COLOR]},
					  {other._vertex[NORMAL]}} {}

			/* move constructor */
			inline vertex(self&& other) noexcept
			: vertex{other} {}

			/* destructor */
			inline ~vertex(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline auto operator=(const self& other) noexcept -> self& {
				_vertex[POSITION] = other._vertex[POSITION];
				_vertex[COLOR]    = other._vertex[COLOR];
				_vertex[NORMAL]   = other._vertex[NORMAL];
				return *this;
			}

			/* move assignment operator */
			inline auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			auto print(void) noexcept {
				for (unsigned int i = 0; i < MAX_ATTRIBUTES; ++i)
					std::cout << _vertex[i].x << ", " << _vertex[i].y << ", " << _vertex[i].z << std::endl;
			}


		private:

			// -- private constants -------------------------------------------

			enum : unsigned int {
				POSITION, COLOR, NORMAL,
				MAX_ATTRIBUTES
			};

			// -- private members ---------------------------------------------

			/* vertex */
			simd::float3 _vertex[MAX_ATTRIBUTES];

	};

}

#endif // ENGINE_VERTEX_HPP
