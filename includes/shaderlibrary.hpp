#ifndef ENGINE_SHADERLIBRARY_HPP
#define ENGINE_SHADERLIBRARY_HPP

#include "mtl_device.hpp"
#include "mtl_library.hpp"
#include "mtl_function.hpp"

#include <iostream>
#include <unordered_map>

#include "macros.hpp"


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

			/* vertex function */
			static inline auto vertex(const std::string& name) -> mtl::function& {
				return shared()._vertexs.at(name);
			}

			/* fragment function */
			static inline auto fragment(const std::string& name) -> mtl::function& {
				return shared()._fragments.at(name);
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline shaderlibrary(void)
			: _library{"shaders/lib.metallib"} {
				_vertexs.try_emplace("vertex_main", "vertex_main", _library);
				_fragments.try_emplace("fragment_main", "fragment_main", _library);
			}


			// -- private members ---------------------------------------------

			/* library */
			mtl::library _library;

			/* vertex function hash table */
			std::unordered_map<std::string, mtl::function> _vertexs;

			/* fragment function hash table */
			std::unordered_map<std::string, mtl::function> _fragments;


	};



}

#endif // ENGINE_SHADERLIBRARY_HPP
