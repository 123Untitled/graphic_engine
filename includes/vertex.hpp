#ifndef ENGINE_VERTEX_HPP
#define ENGINE_VERTEX_HPP


#include <simd/simd.h>
#include <iostream>
#include <vector>

#include <xns>


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {

	/* forward declaration */
	class vertex;

	/* vertex vector */
	using vertices = std::vector<engine::vertex>;

	/* index vector */
	using indexes = std::vector<unsigned int>;

	/* vertex package */
	using vpackage = std::pair<engine::vertices, engine::indexes>;


	// -- V E R T E X ---------------------------------------------------------

	class vertex final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::vertex;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline vertex(void) noexcept
			: _position{0.0f, 0.0f, 0.0f},
				_normal{0.0f, 0.0f, 0.0f},
			   _texture{0.0f, 0.0f} {}

			/* position constructor */
			inline vertex(const float px, const float py, const float pz) noexcept
			: _position{px, py, pz},
				_normal{0.0f, 0.0f, 0.0f},
			   _texture{0.0f, 0.0f} {}

			/* position (simd) constructor */
			inline vertex(const simd::float3& position) noexcept
			: _position{position},
				_normal{0.0f, 0.0f, 0.0f},
			   _texture{0.0f, 0.0f} {}

			inline vertex(const simd::float3& position,
						  const simd::float3& normal,
						  const simd::float2& texture) noexcept
			: _position{position},
				_normal{normal},
			   _texture{texture} {}

			/* position + normal constructor */
			inline vertex(const float px, const float py, const float pz,
						  const float nx, const float ny, const float nz) noexcept
			: _position{px, py, pz},
				_normal{nx, ny, nz},
			   _texture{0.0f, 0.0f} {}

			/* position + normal + texture constructor */
			inline vertex(const float px, const float py, const float pz,
						  const float nx, const float ny, const float nz,
						  const float tx, const float ty) noexcept
			: _position{px, py, pz},
				_normal{nx, ny, nz},
			   _texture{tx, ty} {}

			/* copy constructor */
			inline vertex(const self& other) noexcept
			: _position{other._position},
				_normal{other._normal},
			   _texture{other._texture} {}

			/* move constructor */
			inline vertex(self&& other) noexcept
			: vertex{other} {}

			/* destructor */
			inline ~vertex(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline auto operator=(const self& other) noexcept -> self& {
				_position = other._position;
				_normal   = other._normal;
				_texture  = other._texture;
				return *this;
			}

			/* move assignment operator */
			inline auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			// -- public accessors --------------------------------------------

			/* position */
			inline auto position(void) const noexcept -> const simd::float3& {
				return _position;
			}

			/* normal */
			inline auto normal(void) const noexcept -> const simd::float3& {
				return _normal;
			}

			/* texture */
			inline auto texture(void) const noexcept -> const simd::float2& {
				return _texture;
			}


			/* position x */
			inline auto px(void) const noexcept -> float {
				return _position.x;
			}

			/* position y */
			inline auto py(void) const noexcept -> float {
				return _position.y;
			}

			/* position z */
			inline auto pz(void) const noexcept -> float {
				return _position.z;
			}


			/* normal x */
			inline auto nx(void) const noexcept {
				return _normal.x;
			}

			/* normal y */
			inline auto ny(void) const noexcept {
				return _normal.y;
			}

			/* normal z */
			inline auto nz(void) const noexcept {
				return _normal.z;
			}


			/* texture u */
			inline auto tu(void) const noexcept {
				return _texture.x;
			}

			/* texture v */
			inline auto tv(void) const noexcept {
				return _texture.y;
			}


			// -- public modifiers --------------------------------------------

			/* position */
			inline auto position(const simd::float3& position) noexcept -> void {
				_position = position;
			}

			/* position */
			inline auto position(const float px,
								 const float py,
								 const float pz) noexcept -> void {
				_position = simd::float3{px, py, pz};
			}

			/* normal */
			inline auto normal(const simd::float3& normal) noexcept -> void {
				_normal = normal;
			}

			/* normal */
			inline auto normal(const float nx,
							   const float ny,
							   const float nz) noexcept -> void {
				_normal = simd::float3{nx, ny, nz};
			}

			/* texture */
			inline auto texture(const simd::float2& texture) noexcept -> void {
				_texture = texture;
			}

			/* texture */
			inline auto texture(const float tu,
								const float tv) noexcept -> void {
				_texture = simd::float2{tu, tv};
			}


			/* position x */
			inline auto px(const float px) noexcept -> void {
				_position.x = px;
			}

			/* position y */
			inline auto py(const float py) noexcept -> void {
				_position.y = py;
			}

			/* position z */
			inline auto pz(const float pz) noexcept -> void {
				_position.z = pz;
			}


			/* normal x */
			inline auto nx(const float nx) noexcept -> void {
				_normal.x = nx;
			}

			/* normal y */
			inline auto ny(const float ny) noexcept -> void {
				_normal.y = ny;
			}

			/* normal z */
			inline auto nz(const float nz) noexcept -> void {
				_normal.z = nz;
			}


			/* texture u */
			inline auto tu(const float tu) noexcept -> void {
				_texture.x = tu;
			}

			/* texture v */
			inline auto tv(const float tv) noexcept -> void {
				_texture.y = tv;
			}


		private:

			// -- private members ---------------------------------------------

			/* position */
			simd::float3 _position;

			/* normal */
			simd::float3 _normal;

			/* texture */
			simd::float2 _texture;

	};




	// -- 2 D  V E R T E X ----------------------------------------------------

	class vertex_2D final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::vertex_2D;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline constexpr vertex_2D(void) noexcept
			: _position{0.0f, 0.0f} {
			}

			/* position constructor */
			inline constexpr vertex_2D(const float px, const float py) noexcept
			: _position{px, py} {
			}

			/* position (simd) constructor */
			inline vertex_2D(const simd::float2& position) noexcept
			: _position{position} {
			}


			/* copy constructor */
			inline vertex_2D(const self& other) noexcept
			: _position{other._position} {
			}

			/* move constructor */
			inline vertex_2D(self&& other) noexcept
			: vertex_2D{other} {}

			/* destructor */
			inline ~vertex_2D(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline constexpr auto operator=(const self& other) noexcept -> self& {
				_position = other._position;
				return *this;
			}

			/* move assignment operator */
			inline constexpr auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			// -- public accessors --------------------------------------------

			/* const position */
			inline constexpr auto position(void) const noexcept -> const simd::float2& {
				return _position;
			}


		private:


			// -- private members ---------------------------------------------

			/* position */
			simd::float2 _position;

	};


}

#endif // ENGINE_VERTEX_HPP
