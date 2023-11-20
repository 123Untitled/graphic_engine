#ifndef ENGINE_QUATERNIONS_HEADER
#define ENGINE_QUATERNIONS_HEADER


#include <xns>

#include <iostream>

// -- X N S  N A M E S P A C E ------------------------------------------------

namespace xns {



	template <typename T>
	auto to_radians(const T& degrees) noexcept -> T {
		constexpr auto radians = static_cast<T>(3.14159265358979323846f) / static_cast<T>(180.0f);
		return degrees * radians;
	}


	template <typename T>
	auto sqrt(const T& value) noexcept -> T {

		if (value <= 0) return 0;
		double tol = 1e-6;  // Tolérance pour la précision de la racine carrée
		double guess = value;
		double diff = 1;

		while (diff > tol) {
			double newGuess = (guess + value / guess) / 2;
			diff = abs(newGuess - guess);
			guess = newGuess;
		}
		return guess;
	}

	template <typename T, typename E>
	auto pow(const T& value, const E& exponent) noexcept -> T {
		static_assert(xns::is_arithmetic<T>, "): POW: T must be an arithmetic type :(");
		static_assert(xns::is_integral<E>,   "): POW: E must be an integral type :(");
		if (exponent == 0) return 1;
		if (exponent == 1) return value;
		T result = value;
		for (T i = 1; i < exponent; ++i)
			result *= value;
		return result;
	}

	template <typename T>
	auto cos(const T& value) noexcept -> T {
		T x = value;
		x *= 0.15915494309189535;
		x -= 0.25 + floor(x + 0.25);
		x *= 16.0 * (abs(x) - 0.5);
//#if EXTENDED_PRECISION
		x += 0.225 * x * (abs(x) - 1.0);
//#endif
		return x;
	}

	template <typename T>
	auto sin(const T& value) noexcept -> T {
		return cos(value - 1.5707963267948966);
	}

	template <typename T>
	auto tan(const T& value) noexcept -> T {
		return sin(value) / cos(value);
	}




	template <typename T, xns::size_t N>
	class vec final {

		// -- assertions ------------------------------------------------------

		/* floating point type */
		static_assert(xns::is_floating<T>, "): VEC: T must be a floating point type :(");


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = xns::vec<T, N>;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline constexpr vec(void) noexcept
			: _data{static_cast<T>(0)} {}

			/* members constructor */
			template <typename... A> requires (sizeof...(A) == N)
			inline constexpr vec(A... args) noexcept
			: _data{args...} {}

			/* copy constructor */
			inline constexpr vec(const self& other) noexcept
			: _data{other._data} {}

			/* move constructor */
			inline constexpr vec(self&& other) noexcept
			: vec{other} {}

			/* destructor */
			inline ~vec(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline constexpr auto operator=(const self& other) noexcept -> self& {
				_data = other._data;
				return *this;
			}

			/* move assignment operator */
			inline constexpr auto operator=(self&& other) noexcept -> self& {
				return operator=(other);
			}


			// -- public accessors --------------------------------------------

			// xyzw

			/* x */
			inline constexpr auto x(void) const noexcept -> const T& requires (N > 0) {
				return _data[0];
			}

			/* y */
			inline constexpr auto y(void) const noexcept -> const T& requires (N > 1) {
				return _data[1];
			}

			/* z */
			inline constexpr auto z(void) const noexcept -> const T& requires (N > 2) {
				return _data[2];
			}

			/* w */
			inline constexpr auto w(void) const noexcept -> const T& requires (N > 3) {
				return _data[3];
			}

			// uv

			/* u */
			inline constexpr auto u(void) const noexcept -> const T& requires (N > 0) {
				return _data[0];
			}

			/* v */
			inline constexpr auto v(void) const noexcept -> const T& requires (N > 1) {
				return _data[1];
			}

			// rgba

			/* r */
			inline constexpr auto r(void) const noexcept -> const T& requires (N > 0) {
				return _data[0];
			}

			/* g */
			inline constexpr auto g(void) const noexcept -> const T& requires (N > 1) {
				return _data[1];
			}

			/* b */
			inline constexpr auto b(void) const noexcept -> const T& requires (N > 2) {
				return _data[2];
			}

			/* a */
			inline constexpr auto a(void) const noexcept -> const T& requires (N > 3) {
				return _data[3];
			}


			// -- public modifiers --------------------------------------------

			// xyzw
			/* x */
			inline constexpr auto x(const T& x) noexcept -> void requires (N > 0) {
				_data[0] = x;
			}

			/* y */
			inline constexpr auto y(const T& y) noexcept -> void requires (N > 1) {
				_data[1] = y;
			}

			/* z */
			inline constexpr auto z(const T& z) noexcept -> void requires (N > 2) {
				_data[2] = z;
			}

			/* w */
			inline constexpr auto w(const T& w) noexcept -> void requires (N > 3) {
				_data[3] = w;
			}


			// uv
			/* u */
			inline constexpr auto u(const T& u) noexcept -> void requires (N > 0) {
				_data[0] = u;
			}

			/* v */
			inline constexpr auto v(const T& v) noexcept -> void requires (N > 1) {
				_data[1] = v;
			}


			// rgba
			/* r */
			inline constexpr auto r(const T& r) noexcept -> void requires (N > 0) {
				_data[0] = r;
			}

			/* g */
			inline constexpr auto g(const T& g) noexcept -> void requires (N > 1) {
				_data[1] = g;
			}

			/* b */
			inline constexpr auto b(const T& b) noexcept -> void requires (N > 2) {
				_data[2] = b;
			}

			/* a */
			inline constexpr auto a(const T& a) noexcept -> void requires (N > 3) {
				_data[3] = a;
			}




			/* length */
			/* used for normalization */
			inline constexpr auto length(void) const noexcept -> T {
				T sum = static_cast<T>(0);
				for (xns::size_t i = 0; i < N; ++i) {
					sum += _data[i] * _data[i];
				}
				return std::sqrt(sum);
			}

			/* length squared */
			/* used for normalization */
			inline constexpr auto length_squared(void) const noexcept -> T {
				T sum = static_cast<T>(0);
				for (xns::size_t i = 0; i < N; ++i) {
					sum += _data[i] * _data[i];
				}
				return sum;
			}

			/* normalize */
			inline constexpr auto normalize(void) noexcept -> self& {
				const auto length = self::length();
				for (xns::size_t i = 0; i < N; ++i) {
					_data[i] /= length;
				}
				return *this;
			}

			/* normalized */
			inline constexpr auto normalized(void) const noexcept -> self {
				return self{*this}.normalize();
			}

			/* dot product */
			inline constexpr auto dot(const self& other) const noexcept -> T {
				T sum = static_cast<T>(0);
				for (xns::size_t i = 0; i < N; ++i) {
					sum += _data[i] * other._data[i];
				}
				return sum;
			}

			/* cross product */
			inline constexpr auto cross(const self& other) const noexcept -> self {
				static_assert(N == 3, "): VEC: cross product is only defined for 3D vectors :(");
				return self{_data[1] * other._data[2] - _data[2] * other._data[1],
							_data[2] * other._data[0] - _data[0] * other._data[2],
							_data[0] * other._data[1] - _data[1] * other._data[0]};
			}

			/* angle */
			inline constexpr auto angle(const self& other) const noexcept -> T {
				return std::acos(dot(other) / (length() * other.length()));
			}

			/* distance */
			inline constexpr auto distance(const self& other) const noexcept -> T {
				return (other - *this).length();
			}

			/* distance squared */
			inline constexpr auto distance_squared(const self& other) const noexcept -> T {
				return (other - *this).length_squared();
			}

			/* lerp */
			inline constexpr auto lerp(const self& other, const T& t) const noexcept -> self {
				return self{*this} + ((other - *this) * t);
			}

			/* slerp */
			inline constexpr auto slerp(const self& other, const T& t) const noexcept -> self {
				static_assert(N == 4, "): VEC: slerp is only defined for 4D vectors :(");
				const auto angle = std::acos(dot(other) / (length() * other.length()));
				const auto sin_angle = std::sin(angle);
				return (std::sin((1.0f - t) * angle) / sin_angle) * *this + (std::sin(t * angle) / sin_angle) * other;
			}

			/* project */
			inline constexpr auto project(const self& other) const noexcept -> self {
				return other * (dot(other) / other.length_squared());
			}

			/* reflect */
			inline constexpr auto reflect(const self& normal) const noexcept -> self {
				return *this - (normal * (2.0f * dot(normal)));
			}

			/* refract */
			inline constexpr auto refract(const self& normal, const T& eta) const noexcept -> self {
				static_assert(N == 3, "): VEC: refract is only defined for 3D vectors :(");
				const auto dot = self::dot(normal);
				const auto k = static_cast<T>(1.0f - eta * eta * (1.0f - dot * dot));
				if (k < static_cast<T>(0)) {
					return self{static_cast<T>(0)};
				}
				return (*this * eta) - (normal * (eta * dot + std::sqrt(k)));
			}


			template <typename U, xns::size_t M>
			friend inline constexpr auto operator<<(std::ostream&, const xns::vec<U, M>&) noexcept -> std::ostream&;


		private:

			// -- private members ---------------------------------------------

			/* data */
			T _data[N];
	};


	// << operator
	template <typename T, xns::size_t N>
	inline constexpr auto operator<<(std::ostream& os, const xns::vec<T, N>& vec) noexcept -> std::ostream& {
		os << "vec" << N << " (";
		for (xns::size_t i = 0; i < N; ++i) {
			os << vec._data[i];
			os << (i < (N - 1) ? ", " : ")");
		}
		return os;
	}




	template <typename T>
	class quaternion final {

		// -- assertions ------------------------------------------------------

		/* floating point type */
		static_assert(xns::is_floating<T>, "): QUATERNION: T must be a floating point type :(");


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = xns::quaternion<T>;

			/* value type */
			using value_type = T;


			// -- public lifecycle --------------------------------------------

			/* constructor */
			inline constexpr quaternion(void) noexcept // identity quaternion
			: _x{0.0f}, _y{0.0f}, _z{0.0f}, _w{1.0f} {}

			/* members constructor */
			inline constexpr quaternion(value_type x,
										value_type y,
										value_type z,
										value_type w) noexcept
			: _x{x}, _y{y}, _z{z}, _w{w} {}

			/* copy constructor */
			inline constexpr quaternion(const self& other) noexcept
			: _x{other._x}, _y{other._y}, _z{other._z}, _w{other._w} {}

			/* move constructor */
			inline constexpr quaternion(self&& other) noexcept
			: quaternion{other} {}

			/* destructor */
			inline ~quaternion(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline constexpr auto operator=(const self& other) noexcept -> self& {
				_x = other._x; _y = other._y; _z = other._z; _w = other._w;
				return *this;
			}

			/* move assignment operator */
			inline constexpr auto operator=(self&& other) noexcept -> self& {
				return operator=(other);
			}


			static inline auto from_axis_angle( value_type x,
												value_type y,
												value_type z,
												value_type angle) noexcept -> self {
				const auto half_angle = angle * 0.5f;
				const auto sin_half_angle = std::sin(half_angle);
				return {x * sin_half_angle, y * sin_half_angle, z * sin_half_angle, std::cos(half_angle)};
			}


			// -- public modifiers --------------------------------------------

			/* normalize */
			inline constexpr auto normalize(void) noexcept -> self& {
				
				/* calculate length */
				const auto length = self::length();

				/* normalize */
				_x /= length;
				_y /= length;
				_z /= length;
				_w /= length;

				/* return */
				return *this;
			}

			// -- public accessors --------------------------------------------

			/* angle */
			inline constexpr auto angle(void) const noexcept -> value_type {
				return std::acos(_w) * 2.0f;
			}

			/* axis */
			/* not implemented */

			/* conjugate */
			inline constexpr auto conjugate(void) const noexcept -> self {
				return self{-_x, -_y, -_z, _w};
			}


			/* inverse */
			inline constexpr auto inverse(void) const noexcept -> self {
				return self{conjugate() * (1.0f / self::length())};
			}

			/* length */
			inline constexpr auto length(void) const noexcept -> value_type {
				return std::sqrt((_x * _x) + (_y * _y) + (_z * _z) + (_w * _w));
			}

			/* imaginary part */
			/* not implemented */



			/* to matrix */

			// -- public queries ----------------------------------------------





			// -- public arithmetic operators ---------------------------------

			/* addition operator */
			template <typename U>
			friend inline constexpr auto operator+(const xns::quaternion<U>&,
												   const xns::quaternion<U>&) noexcept -> xns::quaternion<U>;

			/* subtraction operator */
			template <typename U>
			friend inline constexpr auto operator-(const xns::quaternion<U>&,
												   const xns::quaternion<U>&) noexcept -> xns::quaternion<U>;

			/* multiplication operator */
			template <typename U>
			friend inline constexpr auto operator*(const xns::quaternion<U>&,
												   const xns::quaternion<U>&) noexcept -> xns::quaternion<U>;

			/* scalar multiplication operator */
			template <typename U>
			friend inline constexpr auto operator*(const xns::quaternion<U>&,
												   const U&) noexcept -> xns::quaternion<U>;

			/* scalar multiplication operator */
			template <typename U>
			friend inline constexpr auto operator*(const U&,
												   const xns::quaternion<U>&) noexcept -> xns::quaternion<U>;




			/* self addition operator */
			inline constexpr auto operator+=(const self& other) noexcept -> void {
				_x += other._x; _y += other._y; _z += other._z; _w += other._w;
			}

			/* self subtraction operator */
			inline constexpr auto operator-=(const self& other) noexcept -> void {
				_x -= other._x; _y -= other._y; _z -= other._z; _w -= other._w;
			}

			/* self multiplication operator */
			inline constexpr auto operator*=(const self& other) noexcept -> void {
				const value_type x = (_w * other._x) + (_x * other._w) + (_y * other._z) - (_z * other._y);
				const value_type y = (_w * other._y) + (_y * other._w) + (_z * other._x) - (_x * other._z);
				const value_type z = (_w * other._z) + (_z * other._w) + (_x * other._y) - (_y * other._x);
				const value_type w = (_w * other._w) - (_x * other._x) - (_y * other._y) - (_z * other._z);
				_x = x; _y = y; _z = z; _w = w;
			}

			/* scalar self multiplication operator */
			template <typename U>
			inline constexpr auto operator*=(const U& scalar) noexcept -> void {
				static_assert(xns::is_arithmetic<U>, "): QUATERNION: U must be an arithmetic type :(");
				_x *= scalar; _y *= scalar; _z *= scalar; _w *= scalar;
			}

			/* self division operator */
			inline constexpr auto operator/=(const self& other) noexcept -> void {
				operator*=(other.inverse());
			}

			/* scalar self division operator */
			template <typename U>
			inline constexpr auto operator/=(const U& scalar) noexcept -> void {
				static_assert(xns::is_arithmetic<U>, "): QUATERNION: U must be an arithmetic type :(");
				operator*=(1.0f / scalar);
			}



		private:

			// -- private members ---------------------------------------------

			/* real part */
			value_type _x, _y, _z;

			/* imaginary part */
			value_type _w;

	};


	// -- arithmetic operators ------------------------------------------------

	/* addition operator */
	template <typename T>
	inline constexpr auto operator+(const xns::quaternion<T>& lhs,
									const xns::quaternion<T>& rhs) noexcept -> xns::quaternion<T> {
		return xns::quaternion<T>{lhs._x + rhs._x, lhs._y + rhs._y, lhs._z + rhs._z, lhs._w + rhs._w};
	}

	/* subtraction operator */
	template <typename T>
	inline constexpr auto operator-(const xns::quaternion<T>& lhs,
									const xns::quaternion<T>& rhs) noexcept -> xns::quaternion<T> {
		return xns::quaternion<T>{lhs._x - rhs._x, lhs._y - rhs._y, lhs._z - rhs._z, lhs._w - rhs._w};
	}

	/* multiplication operator */
	template <typename T>
	inline constexpr auto operator*(const xns::quaternion<T>& lhs,
									const xns::quaternion<T>& rhs) noexcept -> xns::quaternion<T> {
		return xns::quaternion<T>{(lhs._w * rhs._x) + (lhs._x * rhs._w) + (lhs._y * rhs._z) - (lhs._z * rhs._y),
								  (lhs._w * rhs._y) + (lhs._y * rhs._w) + (lhs._z * rhs._x) - (lhs._x * rhs._z),
								  (lhs._w * rhs._z) + (lhs._z * rhs._w) + (lhs._x * rhs._y) - (lhs._y * rhs._x),
								  (lhs._w * rhs._w) - (lhs._x * rhs._x) - (lhs._y * rhs._y) - (lhs._z * rhs._z)};
	}

	/* scalar multiplication operator */
	template <typename T>
	inline constexpr auto operator*(const xns::quaternion<T>& lhs,
									const T& scalar) noexcept -> xns::quaternion<T> {
		static_assert(xns::is_arithmetic<T>, "): QUATERNION: T must be an arithmetic type :(");
		return xns::quaternion<T>{lhs._x * scalar, lhs._y * scalar, lhs._z * scalar, lhs._w * scalar};
	}

	/* scalar multiplication operator */
	template <typename T>
	inline constexpr auto operator*(const T& scalar,
									const xns::quaternion<T>& rhs) noexcept -> xns::quaternion<T> {
		return operator*(rhs, scalar);
	}


}

#endif // ENGINE_QUATERNIONS_HEADER
