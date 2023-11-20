#ifndef ENGINE_SHADERLIBRARY_HPP
#define ENGINE_SHADERLIBRARY_HPP

#include "mtl_device.hpp"
#include "mtl_library.hpp"
#include "mtl_function.hpp"

#include <iostream>
#include <unordered_map>

#include <xns>


// -- E N G I N E -------------------------------------------------------------

namespace engine {


	// -- S H A D E R L I B R A R Y ---------------------------------------------

	class shaderlibrary final {


		public:

			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(shaderlibrary);

			/* destructor */
			~shaderlibrary(void) noexcept = default;



			// -- public static accessors -------------------------------------

			/* instance */
			static auto shared(void) -> shaderlibrary& {
				static shaderlibrary instance;
				return instance;
			}

			// -- public accessors --------------------------------------------

			/* get function */
			template <xns::basic_string_literal name>
			static inline auto function(void) -> const mtl::function& {
				return shared()._functions.get<name>();
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline shaderlibrary(void)
			: _library{"shaders/lib.metallib"},
			  _functions{
				mtl::function{"default_vertex_2d", _library},
				mtl::function{"default_fragment_2d", _library},
				mtl::function{"vertex_main", _library},
				mtl::function{"fragment_main", _library}
			  } {
			}


			// -- private members ---------------------------------------------

			/* library */
			mtl::library _library;

			/* map */
			xns::literal_map<mtl::function, "default_vertex_2d",
											"default_fragment_2d",
											"vertex_main",
											"fragment_main"> _functions;

	};



}

#endif // ENGINE_SHADERLIBRARY_HPP
