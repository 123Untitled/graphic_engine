#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP

#include "time.hpp"
#include "mtl_render_command_encoder.hpp"
#include "event.hpp"
#include "screen.hpp"
#include "matrix.hpp"
#include "game_object.hpp"

//#include <unordered_set>

			/* jump */
			//inline auto jump(void) noexcept -> void {
			//	static bool  is_jumping = false;
			//	static float jump_height = 1.0f;  // Hauteur maximale du saut
			//	static float gravity = 9.81f;    // Accélération due à la gravité
			//	static float vertical_speed = 0.0f; // Vitesse verticale actuelle
			//	static simd::float3 horizontal_speed = {0.0f, 0.0f, 0.0f}; // Vitesse horizontale actuelle
			//
			//	// assume position.y = 2.0f is the ground
			//
			//	if (engine::event::is_pressed(engine::event::key::SPACE)) {
			//		if (!is_jumping) {
			//
			//			// get current position and velocity
			//			is_jumping = true;
			//			vertical_speed = std::sqrt(2.0f * gravity * jump_height);
			//
			//			horizontal_speed.x = _direction.x * _speed;
			//			horizontal_speed.z = _direction.y * _speed;
			//		}
			//
			//		std::cout << "vertical speed: " << vertical_speed << std::endl;
			//	}
			//
			//	if (is_jumping) {
			//
			//		_position.y += vertical_speed * engine::time::delta();
			//		vertical_speed -= gravity * engine::time::delta;
			//		if (_position.y <= 2.0f) {
			//			_position.y = 2.0f;
			//			is_jumping = false;
			//		}
			//		std::cout << "vertical speed: " << vertical_speed << std::endl;
			//	}
			//
			//
			//}



// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


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
			    _position{simd::float3{0.0f, 2.0f, 0.0f}},
			    _rotation{simd::float3{0.0f, 0.0f, 0.0f}},
			   _direction{simd::float2{0.0f, 0.0f}},
			       _speed{10.0f},
			         _fov{90.0f},
					 _velocity{0.0f, 0.0f, 0.0f},
					 _acceleration{0.0f, 0.0f, 0.0f},
					 _gravity{0.0f, 0.0f, 0.0f},
					 _jumping{false} {

				//_cameras.insert(this);
				update_projection();
			}

			/* non-copyable class */
			non_copyable(camera);

			/* move constructor */
			inline camera(self&& other) noexcept
			: _projection{std::move(other._projection)},
					_view{std::move(other._view)},
				_position{std::move(other._position)},
				_rotation{std::move(other._rotation)},
			   _direction{std::move(other._direction)},
				   _speed{std::move(other._speed)},
					 _fov{std::move(other._fov)} {
				  //_cameras.insert(this);
			}

			/* destructor */
			inline ~camera(void) noexcept {
				//_cameras.erase(this);
			}


			// -- public methods ----------------------------------------------

			/* render */
			inline auto render(mtl::render_command_encoder& encoder) -> void {
				encoder.set_vertex_bytes(&_projection,   sizeof(_projection), 1);
				encoder.set_vertex_bytes(&(_view.get()), sizeof(_view),       2);
				encoder.set_vertex_bytes(&_position,     sizeof(_position),   4);
			}


			// -- public modifiers --------------------------------------------

			/* update */
			inline auto update(void) noexcept -> void {

				if (engine::event::is_pressed(engine::event::key::LOWER_J))
					engine::event::mouse().accelerate_x(-10.0);

				if (engine::event::is_pressed(engine::event::key::LOWER_L))
					engine::event::mouse().accelerate_x(+10.0);

				if (engine::event::is_pressed(engine::event::key::LOWER_K))
					engine::event::mouse().accelerate_y(+10.0);

				if (engine::event::is_pressed(engine::event::key::LOWER_I))
					engine::event::mouse().accelerate_y(-10.0);

				if (engine::event::is_pressed(engine::event::key::LOWER_T))
					decrease_fov();

				if (engine::event::is_pressed(engine::event::key::LOWER_G))
					increase_fov();


				update_rotation();
				update_direction();
				update_position();
				update_view();
				update_projection();
			}


			/* projection */
			inline auto projection(void) const noexcept -> const simd::float4x4& {
				return _projection;
			}

			/* position */
			inline auto position(void) const noexcept -> const simd::float3& {
				return _position;
			}

			/* rotation */
			inline auto rotation(void) const noexcept -> const simd::float3& {
				return _rotation;
			}

			/* direction */
			inline auto direction(void) const noexcept -> const simd::float2& {
				return _direction;
			}

			/* view */
			inline auto view(void) const noexcept -> const simd::float4x4& {
				return _view.get();
			}


		private:


			// -- private methods ---------------------------------------------

			/* increase fov */
			inline auto increase_fov(void) noexcept -> void {
				_fov += _fov < 180.0f ? 1.0f : 0.0f;

			}

			/* decrease fov */
			inline auto decrease_fov(void) noexcept -> void {
				_fov -= _fov > 1.0f ? 1.0f : 0.0f;
			}

			/* update projection */
			inline auto update_projection(void) noexcept -> void {

				const float ys = 1 / std::tan(((_fov / 180.0f) * M_PI) * 0.5f);
				const float xs = ys / engine::screen::ratio();
				const float zs = 1000.0f / (0.01f - 1000.0f);
				const float zt = zs * 0.1f;

				_projection = matrix_float4x4{
					simd::float4{+xs,   0,   0,   0},
					simd::float4{  0, +ys,   0,   0},
					simd::float4{  0,   0, -zs,   1},
					simd::float4{  0,   0,  zt,   0}
				};
			}


			/* update rotation */
			inline auto update_rotation(void) noexcept -> void {
				_rotation.y = engine::event::mouse().x_axis();
				_rotation.x = engine::event::mouse().y_axis();
			}

			/* update direction */
			inline auto update_direction(void) noexcept -> void {
				_direction.x = std::sin(_rotation.y);
				_direction.y = std::cos(_rotation.y);
			}


			/* update position */
			inline auto update_position(void) noexcept -> void {

				const bool front = engine::event::is_pressed(engine::event::key::LOWER_E);
				const bool back  = engine::event::is_pressed(engine::event::key::LOWER_D);
				const bool left  = engine::event::is_pressed(engine::event::key::LOWER_S);
				const bool right = engine::event::is_pressed(engine::event::key::LOWER_F);

				simd::float3 movement = {0.0f, 0.0f, 0.0f};

				if (front) {
					movement.x -= _direction.x;
					movement.z += _direction.y;
				}
				if (back) {
					movement.x += _direction.x;
					movement.z -= _direction.y;
				}
				if (left) {
					movement.x -= _direction.y;
					movement.z -= _direction.x;
				}
				if (right) {
					movement.x += _direction.y;
					movement.z += _direction.x;
				}

				if ((right || left) && (front || back)) {
					movement.x *= 0.70710678118f;
					movement.z *= 0.70710678118f;
					// what is this number? 1 / sqrt(2)
					// this is to normalize the movement
				}

				_position.x += movement.x * _speed * engine::time::delta();
				_position.z += movement.z * _speed * engine::time::delta();

			}

			/* update view */
			inline auto update_view(void) noexcept -> void {

				_view.reset();
				// matrix multiply
				_view.rotate(_rotation);
				_view.translate(simd::float3{-_position.x, -_position.y, -_position.z});
			}



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

			/* field of view */
			float _fov;


			// -- physics -----------------------------------------------------

			/* velocity */
			simd::float3 _velocity;

			/* acceleration */
			simd::float3 _acceleration;

			/* gravity */
			simd::float3 _gravity;

			/* jump */
			bool _jumping;




			// -- private static members --------------------------------------

			/* camera list */
			//static inline std::unordered_set<engine::camera*> _cameras{};

	};


	// -- R A Y  C A S T ------------------------------------------------------

	class ray_cast final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::ray_cast;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline ray_cast(void) noexcept;

			/* camera constructor */
			inline ray_cast(const engine::camera& camera) noexcept
			: _origin{},
			  _direction{} {


				// assume these points are in center of screen
				//auto xpos = engine::screen::width() / 2.0f;
				//auto ypos = engine::screen::height() / 2.0f;


				//float xNDC = 2.0f * 0.5f - 1.0f; // ce qui équivaut à 0.0f
				//float yNDC = 1.0f - 2.0f * 0.5f; // ce qui équivaut également à 0.0f
				//float xNDC = (2.0f * xpos) / engine::screen::width() - 1.0f;
				//float yNDC = 1.0f - (2.0f * ypos) / engine::screen::height();
				float xNDC = 0.0f;
				float yNDC = 0.0f;

				simd::float4 clip = simd::float4{xNDC, yNDC, -1.0f, 1.0f};

				simd::float4 eye = simd::inverse(camera.projection()) * clip;
				eye = simd::float4{eye.x, eye.y, -1.0f, 0.0f};

				simd::float4 world = simd::inverse(camera.view()) * eye;
				simd::float3 ray = simd::normalize(simd::float3{world.x, world.y, world.z});

				_origin = camera.position();
				_direction = ray;

			}

			/* non-copyable class */
			non_copyable(ray_cast);



			/* intersection */
			inline auto intersection(const engine::game_object& object) const noexcept -> bool {

				const auto& transform = object.transform();

				const simd::float3& center = transform.position();
				float radius = 1.0f * transform.scale().x;

				simd::float3 oc = _origin - center;
				float a = simd::dot(_direction, _direction);
				float b = 2.0f * simd::dot(oc, _direction);
				float c = simd::dot(oc, oc) - radius * radius;
				float discriminant = b * b - 4 * a * c;

				return discriminant >= 0;

			}




		private:

			// -- private members ---------------------------------------------

			/* origin */
			simd::float3 _origin;

			/* direction */
			simd::float3 _direction;

	};


}

#endif // ifndef ENGINE_CAMERA_HPP
