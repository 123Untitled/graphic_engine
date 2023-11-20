#ifndef ENGINE_TIME_HPP
#define ENGINE_TIME_HPP

#include <xns>


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- T I M E -------------------------------------------------------------

	class time final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::time;


			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(time);

			/* destructor */
			~time(void) noexcept = default;


			// -- public static accessors -------------------------------------

			/* delta */
			static inline auto delta(void) noexcept -> const float& {
				return self::shared()._delta;
			}


			// -- public static modifiers -------------------------------------

			/* update */
			static inline auto update(void) noexcept -> void {
				auto& instance  = self::shared();
				instance._start = ::CFAbsoluteTimeGetCurrent();
				instance._delta = instance._start - instance._last;
				instance._last  = instance._start;
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			time(void) noexcept
			: _delta{0.0f}, _last{0.0f}, _start{0.0f} {
			}


			// -- private static accessors ------------------------------------

			/* shared */
			static inline auto shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}


			// -- private members ---------------------------------------------

			/* delta */
			float _delta;

			/* last */
			::CFAbsoluteTime _last;

			/* start */
			::CFAbsoluteTime _start;

	};


}

#endif // ENGINE_TIME_HPP
