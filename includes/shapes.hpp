#ifndef ENGINE_SHAPES_HEADER
#define ENGINE_SHAPES_HEADER

#include "vertex.hpp"
#include "mesh.hpp"

#include <xns>


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {





	template <xns::size_t segments>
	inline auto make_circle(const float radius) -> engine::mesh {

		std::vector<engine::vertex_2D> vertices;
		std::vector<unsigned int> indexes;

		//xns::array<engine::vertex_2D, segments + 1> vertices;

		// -- vertices --------------------------------------------------------

		// center
		vertices.push_back(engine::vertex_2D{0.0f, 0.0f});

		// circumference
		for (unsigned int i = 0; i < segments; ++i) {
			const float angle = 2.0f * M_PI * (i / static_cast<float>(segments));
			vertices.push_back(engine::vertex_2D{radius * std::cos(angle), radius * std::sin(angle)});
		}

		std::cout << "segments: " << segments << std::endl;
		std::cout << "size: " << vertices.size() << std::endl;

		// -- indexes ---------------------------------------------------------

		// inverse index order
		for (unsigned int i = 0; i < segments; ++i) {
			indexes.push_back(0);
			indexes.push_back((i + 1) % segments + 1);
			indexes.push_back(i + 1);
		}

		return engine::mesh{vertices, indexes};
	}


	class shapes final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::shapes;


			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(shapes);

			/* destructor */
			inline ~shapes(void) noexcept = default;


			// -- public static accessors -------------------------------------

			/* get mesh */
			template <xns::basic_string_literal name>
			static inline auto get(void) -> const engine::mesh& {
				return engine::shapes::shared()._shapes.get<name>();
			}


		private:

			// -- private static accessors ------------------------------------

			/* instance */
			static inline auto shared(void) -> const engine::shapes& {
				static engine::shapes instance;
				return instance;
			}

			/* make square */
			static inline auto make_square(void) -> engine::mesh {

				constexpr xns::array<engine::vertex_2D, 4> vertices {
					engine::vertex_2D{-1.0f, -1.0f},
					engine::vertex_2D{-1.0f, +1.0f},
					engine::vertex_2D{+1.0f, +1.0f},
					engine::vertex_2D{+1.0f, -1.0f}
				};

				constexpr xns::array<unsigned int, 6> indexes {
					0, 1, 3, 3, 1, 2
				};

				return engine::mesh{vertices, indexes};
			}

			/* make triangle */
			static inline auto make_triangle(void) -> engine::mesh {
				constexpr xns::array<engine::vertex_2D, 4> vertices {
					engine::vertex_2D{-1.0f, -1.0f},
					engine::vertex_2D{+0.0f, +1.0f},
					engine::vertex_2D{+1.0f, -1.0f}
				};
				return engine::mesh{vertices};
			}



			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline shapes(void) noexcept
			: _shapes{
				make_circle<320>(1.0f),
				self::make_square(),
				self::make_triangle()} {
			}



			// -- private members ---------------------------------------------

			/* circle mesh */
			xns::literal_map<const engine::mesh, "circle", "square", "triangle"> _shapes;

	};





}

#endif // ENGINE_SHAPES_HEADER
