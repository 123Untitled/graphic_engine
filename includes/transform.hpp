#ifndef ENGINE_TRANSFORM_HEADER
#define ENGINE_TRANSFORM_HEADER

#include <simd/simd.h>
#include "matrix.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- T R A N S F O R M ---------------------------------------------------

	//class transform final {
	//
	//	public:
	//
	//		// -- public type -------------------------------------------------
	//
	//		/* self type */
	//		using self = engine::transform;
	//
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//		/* default constructor */
	//		inline transform(void) noexcept
	//		: _position{0.0f, 0.0f, 0.0f},
	//		  _rotation{0.0f, 0.0f, 0.0f},
	//			 _scale{1.0f, 1.0f, 1.0f} {}
	//
	//		/* copy constructor */
	//		inline transform(const self& other) noexcept
	//		: _position{other._position}, _rotation{other._rotation}, _scale{other._scale} {}
	//
	//		/* move constructor */
	//		inline transform(transform&& other) noexcept
	//		: transform{other} {}
	//
	//		/* destructor */
	//		inline ~transform(void) noexcept = default;
	//
	//
	//		// -- public assignment operators ---------------------------------
	//
	//		/* copy assignment operator */
	//		inline auto operator=(const self& other) noexcept -> self& {
	//			_position = other._position;
	//			_rotation = other._rotation;
	//			   _scale = other._scale;
	//			return *this;
	//		}
	//
	//		/* move assignment operator */
	//		inline auto operator=(self&& other) noexcept -> self& {
	//			return self::operator=(other);
	//		}
	//
	//
	//		// -- public accessors --------------------------------------------
	//
	//		/* position */
	//		inline auto position(void) noexcept -> simd::float3& {
	//			return _position;
	//		}
	//
	//		/* const position */
	//		inline auto position(void) const noexcept -> const simd::float3& {
	//			return _position;
	//		}
	//
	//		/* rotation */
	//		inline auto rotation(void) noexcept -> simd::float3& {
	//			return _rotation;
	//		}
	//
	//		/* const rotation */
	//		inline auto rotation(void) const noexcept -> const simd::float3& {
	//			return _rotation;
	//		}
	//
	//		/* scale */
	//		inline auto scale(void) noexcept -> simd::float3& {
	//			return _scale;
	//		}
	//
	//		/* const scale */
	//		inline auto scale(void) const noexcept -> const simd::float3& {
	//			return _scale;
	//		}
	//
	//
	//		// -- public modifiers --------------------------------------------
	//
	//		/* scale */
	//		inline auto scale(const float x, const float y, const float z) noexcept -> void {
	//			_scale = simd::float3{x, y, z};
	//		}
	//
	//		/* scale */
	//		inline auto scale(const float factor) noexcept -> void {
	//			_scale = simd::float3{factor, factor, factor};
	//		}
	//
	//
	//		// -- public methods ----------------------------------------------
	//
	//		/* update */
	//		inline auto update(void) noexcept -> void {
	//			    _matrix.reset();
	//			_matrix.translate(_position);
	//			   _matrix.rotate(_rotation);
	//			    _matrix.scale(_scale);
	//		}
	//
	//
	//
	//	private:
	//
	//		// -- private members ---------------------------------------------
	//
	//		/* position */
	//		simd::float3 _position;
	//
	//		/* rotation */
	//		simd::float3 _rotation;
	//
	//		/* scale */
	//		simd::float3 _scale;
	//
	//		/* matrix */
	//		engine::matrix _matrix;
	//
	//};



}

#endif // ENGINE_TRANSFORM_HEADER
