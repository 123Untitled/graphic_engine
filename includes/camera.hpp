#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP

#include <simd/simd.h>
#include "time.hpp"
#include "mtl_render_command_encoder.hpp"
#include "keyboard.hpp"



// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	class matrix final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::matrix;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline matrix(void) noexcept
			: _matrix{matrix_identity_float4x4} {}


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


	// -- C A M E R A ---------------------------------------------------------

	class camera final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::camera;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline camera(void) noexcept
			: _projection{matrix_identity_float4x4},
			  _view{},
			  _position{simd::float3{0.0f, 0.0f, 0.0f}},
			  _rotation{simd::float3{0.0f, 0.0f, 0.0f}},
			  _direction{simd::float2{0.0f, 0.0f}},
			  _speed{5.0f},
			  _sensitivity{2.0f},
			  _fov{60.0f} {
				  update_projection();
			  }

			/* non-assignable class */
			non_assignable(camera);

			/* destructor */
			inline ~camera(void) noexcept = default;

			static inline auto m_front(void) noexcept -> void {
				shared().move_front();
			}

			static inline auto m_back(void) noexcept -> void {
				shared().move_back();
			}

			static inline auto m_left(void) noexcept -> void {
				shared().move_left();
			}

			static inline auto m_right(void) noexcept -> void {
				shared().move_right();
			}


			static inline auto shared(void) noexcept -> self& {
				static engine::camera camera;
				return camera;
			}

			// -- public methods ----------------------------------------------

			/* render */
			inline auto render(mtl::render_command_encoder& encoder) -> void {
				encoder.set_vertex_bytes(&_projection,   sizeof(_projection), 1);
				encoder.set_vertex_bytes(&(_view.get()), sizeof(_view),       2);
			}


			// -- public modifiers --------------------------------------------

			/* increase fov */
			inline auto increase_fov(void) noexcept -> void {
				_fov += _fov < 180.0f ? 1.0f : 0.0f;
				update_projection();
				std::cout << "fov: " << _fov << std::endl;
			}

			/* decrease fov */
			inline auto decrease_fov(void) noexcept -> void {
				_fov -= _fov > 1.0f ? 1.0f : 0.0f;
				update_projection();
				std::cout << "fov: " << _fov << std::endl;
			}

			/* update projection */
			inline auto update_projection(void) noexcept -> void {
				constexpr float ratio = 16.0f / 9.0f;

				const float ys = 1 / std::tan(((_fov / 180.0f) * M_PI) * 0.5f);
				const float xs = ys / ratio;
				const float zs = 1000.0f / (0.01f - 10000.0f);
				const float zt = zs * 0.1f;

				_projection = matrix_float4x4{
					simd::float4{+xs,   0,   0,   0},
					simd::float4{  0, +ys,   0,   0},
					simd::float4{  0,   0, -zs,   1},
					simd::float4{  0,   0,  zt,   0}
				};
			}

			/* update direction */
			inline auto update_direction(void) noexcept -> void {
				_direction.x = std::sin(_rotation.y)
					* _speed * engine::time::delta;
				_direction.y = std::cos(_rotation.y)
					* _speed * engine::time::delta;
			}

			/* update view */
			inline auto update_view(void) noexcept -> void {
				_view.reset();

				_view.xrotate(_rotation.x);
				_view.yrotate(_rotation.y);
				_view.zrotate(_rotation.z);
				_view.translate(simd::float3{-_position.x, -_position.y, -_position.z});
			}

			/* update */
			inline auto update(void) noexcept -> void {
				update_direction();

				if (engine::event_manager::is_pressed(engine::key::LOWER_E))
					move_front();
				if (engine::event_manager::is_pressed(engine::key::LOWER_D))
					move_back();
				if (engine::event_manager::is_pressed(engine::key::LOWER_S))
					move_left();
				if (engine::event_manager::is_pressed(engine::key::LOWER_F))
					move_right();
				if (engine::event_manager::is_pressed(engine::key::LOWER_J))
					watch_left();
				if (engine::event_manager::is_pressed(engine::key::LOWER_L))
					watch_right();
				if (engine::event_manager::is_pressed(engine::key::LOWER_I))
					watch_up();
				if (engine::event_manager::is_pressed(engine::key::LOWER_K))
					watch_down();


				update_view();
			}

			/* move front */
			inline auto move_front(void) noexcept -> void {
				_position.x -= _direction.x;
				_position.z += _direction.y;
				std::cout << "move front [x: " << _position.x << ", z: " << _position.z << "]" << std::endl;
			}

			/* move back */
			inline auto move_back(void) noexcept -> void {
				_position.x += _direction.x;
				_position.z -= _direction.y;
				std::cout << "move back [x: " << _position.x << ", z: " << _position.z << "]" << std::endl;
			}

			/* move left */
			inline auto move_left(void) noexcept -> void {
				_position.x -= _direction.y;
				_position.z -= _direction.x; // maybe minus
				std::cout << "move left [x: " << _position.x << ", z: " << _position.z << "]" << std::endl;
			}

			/* move right */
			inline auto move_right(void) noexcept -> void {
				_position.x += _direction.y;
				_position.z += _direction.x; // maybe plus
				std::cout << "move right [x: " << _position.x << ", z: " << _position.z << "]" << std::endl;
			}

			/* watch up */
			inline auto watch_up(void) noexcept -> void {
				_rotation.x += _sensitivity * engine::time::delta;
				std::cout << "watch up [x: " << _rotation.x << "]" << std::endl;
			}

			/* watch down */
			inline auto watch_down(void) noexcept -> void {
				_rotation.x -= _sensitivity * engine::time::delta;
				std::cout << "watch down [x: " << _rotation.x << "]" << std::endl;
			}

			/* watch left */
			inline auto watch_left(void) noexcept -> void {
				_rotation.y += _sensitivity * engine::time::delta;
				_rotation.y -= _rotation.y / M_PI > 2.0f ? M_PI * 2 : 0.0f;
				std::cout << "watch left [y: " << _rotation.y << "]" << std::endl;
			}

			/* watch right */
			inline auto watch_right(void) noexcept -> void {
				_rotation.y -= _sensitivity * engine::time::delta;
				_rotation.y += _rotation.y / M_PI < -1.0f ? M_PI * 2 : 0.0f;
				std::cout << "watch right [y: " << _rotation.y << "]" << std::endl;
			}


		private:

			// -- private members ---------------------------------------------

			/* projection matrix */
			simd::float4x4 _projection;

			/* view matrix */
			engine::matrix _view;

			/* position */
			simd::float3 _position;

			/* rotation */
			simd::float3 _rotation;

			/* direction */
			simd::float2 _direction;

			/* speed */
			float _speed;

			/* sensitivity */
			float _sensitivity;

			/* field of view */
			float _fov;

	};

}

#endif // ifndef ENGINE_CAMERA_HPP
