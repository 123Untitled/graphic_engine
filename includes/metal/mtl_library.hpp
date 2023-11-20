#ifndef METAL_LIBRARY_HPP
#define METAL_LIBRARY_HPP


#include <Metal/Metal.hpp>

#include "mtl_device.hpp"
#include "ns_string.hpp"


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- L I B R A R Y -------------------------------------------------------

	class library final {


		public:

			// -- public lifecycle --------------------------------------------

			/* path constructor */
			template <decltype(sizeof(0)) N>
			inline library(const char (&path)[N])
			: _library{nullptr} {
				_library = mtl::device::underlying().newLibrary(ns::string{path}, nullptr);
				if (_library == nullptr)
					throw std::runtime_error("failed to create shader library");
				_library->retain();
			}

			/* destructor */
			inline ~library(void) noexcept {
				if (_library == nullptr) return;
				_library->release();
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::Library& {
				return *_library;
			}



		private:

			// -- private members ---------------------------------------------

			/* library */
			MTL::Library* _library;

	};

}

#endif // METAL_LIBRARY_HPP

