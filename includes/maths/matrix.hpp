#ifndef ENGINE_MATRIX_HEADER
#define ENGINE_MATRIX_HEADER

#include <simd/simd.h>


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- M A T R I X ---------------------------------------------------------

	class matrix final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::matrix;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline matrix(void) noexcept
			: _matrix{matrix_identity_float4x4} {}

			/* copy constructor */
			inline matrix(const self& other) noexcept
			: _matrix{other._matrix} {}

			/* move constructor */
			inline matrix(self&& other) noexcept
			: _matrix{std::move(other._matrix)} {}


			// -- public assignment operators ---------------------------------

			/* copy assignment */
			inline auto operator=(const self& other) noexcept -> self& {
				_matrix = other._matrix;
				return *this;
			}

			/* move assignment */
			inline auto operator=(self&& other) noexcept -> self& {
				_matrix = std::move(other._matrix);
				return *this;
			}

			/* matrix assignment operator */
			inline auto operator=(const simd::float4x4& matrix) noexcept -> self& {
				_matrix = matrix;
				return *this;
			}


			/* translate */
			inline auto translate(const simd::float3& direction) noexcept -> void {

				simd::float4x4 matrix = matrix_identity_float4x4;
				matrix.columns[3].x = direction.x;
				matrix.columns[3].y = direction.y;
				matrix.columns[3].z = direction.z;
				_matrix = matrix_multiply(_matrix, matrix);
			}

			/* scale */
			inline auto scale(const simd::float3& scale) noexcept -> void {

				simd::float4x4 matrix = matrix_identity_float4x4;
				matrix.columns[0].x = scale.x;
				matrix.columns[1].y = scale.y;
				matrix.columns[2].z = scale.z;
				_matrix = matrix_multiply(_matrix, matrix);
			}

			/* xrotate */
			inline auto xrotate(const float angle) noexcept -> void {

				/*
				   1  0  0  0
				   0  c -s  0
				   0  s  c  0
				   0  0  0  1
				*/

				simd::float4x4 matrix = matrix_identity_float4x4;
				const float c = std::cos(angle);
				const float s = std::sin(angle);
				matrix.columns[1].y = +c;
				matrix.columns[2].z = +c;
				matrix.columns[2].y = -s;
				matrix.columns[1].z = +s;
				_matrix = matrix_multiply(_matrix, matrix);
			}

			/* yrotate */
			inline auto yrotate(const float angle) noexcept -> void {

				/*
				   c  0  s  0
				   0  1  0  0
				  -s  0  c  0
				   0  0  0  1
				*/

				simd::float4x4 matrix = matrix_identity_float4x4;
				const float c = std::cos(angle);
				const float s = std::sin(angle);
				matrix.columns[0].x = +c;
				matrix.columns[2].z = +c;
				matrix.columns[0].z = -s;
				matrix.columns[2].x = +s;
				_matrix = matrix_multiply(_matrix, matrix);
			}

			/* zrotate */
			inline auto zrotate(const float angle) noexcept -> void {

				/*
				   c -s  0  0
				   s  c  0  0
				   0  0  1  0
				   0  0  0  1
				*/

				simd::float4x4 matrix = matrix_identity_float4x4;
				const float c = std::cos(angle);
				const float s = std::sin(angle);
				matrix.columns[0].x = +c;
				matrix.columns[1].y = +c;
				matrix.columns[0].y = -s;
				matrix.columns[1].x = +s;
				_matrix = matrix_multiply(_matrix, matrix);
			}

			/* rotate */
			inline auto rotate(const simd::float3& rotation) noexcept -> void {
				xrotate(rotation.x);
				yrotate(rotation.y);
				zrotate(rotation.z);
			}

			/* multiply */
			inline auto multiply(const self& other) noexcept -> void {
				_matrix = matrix_multiply(_matrix, other._matrix);
			}

			/* reset */
			inline auto reset(void) noexcept -> void {
				_matrix = matrix_identity_float4x4;
			}



			// -- public accessors --------------------------------------------

			/* underlying */
			inline auto get(void) const noexcept -> const simd::float4x4& {
				return _matrix;
			}


		private:

			// -- private members ---------------------------------------------

			/* matrix */
			simd::float4x4 _matrix;

	};

}

#endif // ENGINE_MATRIX_HEADER
