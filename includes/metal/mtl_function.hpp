#ifndef METAL_FUNCTION_HPP
#define METAL_FUNCTION_HPP

#include <Metal/Metal.hpp>

#include "mtl_library.hpp"
#include "macros.hpp"

#include <string>

// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- F U N C T I O N -----------------------------------------------------

	class function final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::function;



			// -- public lifecycle --------------------------------------------

			/* name / library constructor */
			inline function(const std::string& name, mtl::library& library)
			: _function{library.underlying().newFunction(NS::String::string(name.data(), NS::UTF8StringEncoding))} {
				if (_function == nullptr)
					throw std::runtime_error{"failed to create function"};
				_function->retain();
			}

			/* non-copyable class */
			non_copyable(function);

			/* move constructor */
			inline function(self&& other) noexcept
			: _function{other._function} {
				other._function = nullptr;
			}

			/* destructor */
			inline ~function(void) noexcept {
				if (_function == nullptr) return;
				_function->release();
			}


			// -- public assignment operators ---------------------------------

			/* move assignment operator */
			inline auto operator=(self&& other) noexcept -> self& {
				if (this == &other) return *this;
				if (_function != nullptr) _function->release();
				_function = other._function;
				other._function = nullptr;
				return *this;
			}


			// -- public conversion operators ---------------------------------

			/* underlying */
			inline operator MTL::Function*(void) const noexcept {
				return _function;
			}

			/* underlying */
			inline operator MTL::Function&(void) const noexcept {
				return *_function;
			}

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::Function& {
				return *_function;
			}

		private:

			// -- private members ---------------------------------------------

			/* underlying */
			MTL::Function* _function;

	};

}

#endif // METAL_FUNCTION_HPP
