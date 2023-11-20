#ifndef SIMD_HEADER
#define SIMD_HEADER

#include <xns>


#if not __has_attribute(ext_vector_type)
#	error "): SIMD: compiler does not support SIMD. :("
#endif


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	template <typename T, xns::size_t N>
	class simd final {


		// -- assertions ------------------------------------------------------

		/* arithmetic type */
		static_assert(xns::is_arithmetic<T>, "T must be arithmetic type.");

		/* minimum size */
		static_assert(N > 1, "): SIMD: N must be greater than 1. :(");


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::simd<T, N>;

			/* value type */
			using value_type = T;

			/* underlying type */
			using underlying = __attribute__((__ext_vector_type__(N))) value_type;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline constexpr simd(void) noexcept
			: _data{} {}

			/* value constructor */
			template <typename... A> requires (sizeof...(A) == N)
			inline constexpr simd(const A&... value) noexcept
			: _data{value...} {}

			/* copy constructor */
			inline constexpr simd(const self& other) noexcept
			: _data{other._data} {}

			/* move constructor */
			inline constexpr simd(self&& other) noexcept
			: simd{other} {}

			/* destructor */
			inline ~simd(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline constexpr auto operator=(const self& other) noexcept -> self& {
				_data = other._data;
				return *this;
			}

			/* move assignment operator */
			inline constexpr auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			// -- public accessors --------------------------------------------

			/* get */
			inline constexpr auto get(const xns::size_t index) const noexcept -> value_type {
				return _data[index];
			}


			// -- public modifiers --------------------------------------------

			/* set */
			inline constexpr auto set(const xns::size_t index, const value_type value) noexcept -> void {
				_data[index] = value;
			}



			// -- public arithmetic operators ---------------------------------

			/* add self */
			inline constexpr auto operator+=(const self& other) noexcept -> self& {
				_data += other._data;
				return *this;
			}

			/* add value */
			inline constexpr auto operator+=(const value_type value) noexcept -> self& {
				_data += value;
				return *this;
			}

			/* subtract self */
			inline constexpr auto operator-=(const self& other) noexcept -> self& {
				_data -= other._data;
				return *this;
			}

			/* subtract value */
			inline constexpr auto operator-=(const value_type value) noexcept -> self& {
				_data -= value;
				return *this;
			}

			/* multiply self */
			inline constexpr auto operator*=(const self& other) noexcept -> self& {
				_data *= other._data;
				return *this;
			}

			/* multiply value */
			inline constexpr auto operator*=(const value_type value) noexcept -> self& {
				_data *= value;
				return *this;
			}

			/* divide self */
			inline constexpr auto operator/=(const self& other) noexcept -> self& {
				_data /= other._data;
				return *this;
			}

			/* divide value */
			inline constexpr auto operator/=(const value_type value) noexcept -> self& {
				_data /= value;
				return *this;
			}



			// -- public unary operators --------------------------------------

			/* positive */
			inline constexpr auto operator+(void) const noexcept -> const self& {
				return *this;
			}

			/* negative */
			inline constexpr auto operator-(void) const noexcept -> self {
				return self{*this} *= -1;
			}


			// -- public binary operators -------------------------------------

			/* add self */
			inline constexpr auto operator+(const self& other) const noexcept -> self {
				return self{*this} += other;
			}

			/* add value */
			inline constexpr auto operator+(const value_type value) const noexcept -> self {
				return self{*this} += value;
			}

			/* subtract self */
			inline constexpr auto operator-(const self& other) const noexcept -> self {
				return self{*this} -= other;
			}

			/* subtract value */
			inline constexpr auto operator-(const value_type value) const noexcept -> self {
				return self{*this} -= value;
			}

			/* multiply self */
			inline constexpr auto operator*(const self& other) const noexcept -> self {
				return self{*this} *= other;
			}

			/* multiply value */
			inline constexpr auto operator*(const value_type value) const noexcept -> self {
				return self{*this} *= value;
			}

			/* divide self */
			inline constexpr auto operator/(const self& other) const noexcept -> self {
				return self{*this} /= other;
			}

			/* divide value */
			inline constexpr auto operator/(const value_type value) const noexcept -> self {
				return self{*this} /= value;
			}


		private:

			// -- private members ---------------------------------------------

			/* data */
			underlying _data;

	};

}

#endif // SIMD_HEADER
