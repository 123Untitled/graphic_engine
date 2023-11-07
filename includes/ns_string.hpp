#ifndef NS_STRING_HPP
#define NS_STRING_HPP

#include <Foundation/NSString.hpp>
#include <iostream>
#include <stdexcept>
#include <string>


// -- N S  N A M E S P A C E --------------------------------------------------

namespace ns {


	// -- S T R I N G ---------------------------------------------------------

	class string final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = ns::string;


			// -- public lifecycle --------------------------------------------

			/* string constructor */
			inline string(const std::string& str)
			: _string{NS::String::string(str.data(), NS::UTF8StringEncoding)} {
				if (_string == nullptr)
					throw std::runtime_error{"failed to create string"};
				_string->retain();
			}

			/* destructor */
			inline ~string(void) noexcept {
				if (_string == nullptr) return;
				_string->release();
			}


			// -- public conversion operators ---------------------------------

			/* underlying */
			inline operator NS::String*(void) const noexcept {
				return _string;
			}

			/* underlying */
			inline operator NS::String&(void) const noexcept {
				return *_string;
			}


		private:

			// -- private members ---------------------------------------------

			/* string */
			NS::String* _string;

	};

}

#endif // NS_STRING_HPP
