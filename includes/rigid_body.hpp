#ifndef ENGINE_RIGID_BODY_HEADER
#define ENGINE_RIGID_BODY_HEADER

#include "transform.hpp"
#include "simd.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {



	class verlet final {

		public:


		private:

			// -- private members ---------------------------------------------

			/* position */
			::simd::float3 _position;

			/* previous position */
			::simd::float3 _previous;

			/* acceleration */
			::simd::float3 _acceleration;

			/* mass */
			float _mass;

	};


	// -- utility physics functions -------------------------------------------


	//        force = mass * acceleration
	// acceleration = force / mass
	// acceleration = velocity / time
	//     velocity = Δposition / Δtime

	inline auto force(const float mass, const ::simd::float3& acceleration) noexcept -> ::simd::float3 {
		return mass * acceleration;
	}

	inline auto acceleration(const float mass, const ::simd::float3& force) noexcept -> ::simd::float3 {
		return force / mass;
	}

	inline auto acceleration(const ::simd::float3& velocity, const float time) noexcept -> ::simd::float3 {
		return velocity / time;
	}

	inline auto _velocity(const ::simd::float3& position, const float time) noexcept -> ::simd::float3 {
		return position / time;
	}



	template <decltype(sizeof(0)) N>
	class transform final {

		// -- assertions ------------------------------------------------------

		/* require 2, 3 or 4 dimensions */
		static_assert(N == 2 || N == 3 || N == 4, "): TRANSFORM: dimension must be 2, 3 or 4");


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::transform<N>;

			/* position type */
			using position = engine::simd<float, N>;

			/* rotation type */
			using rotation = engine::simd<float, N>;


		private:

			// -- private members ---------------------------------------------

			/* position */
			position _position;

			/* rotation */
			::simd::quatf _rotation;

	};

	class mass_inertia final {

		public:


			auto compute_force(void) const noexcept -> ::simd::float3 {
				return ::simd::float3{0.0f, _mass * -9.81f, 0.0f};
			}

		private:


			// -- private constants -------------------------------------------

			enum {
				EARTH, MOON, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO,
				NUM_PLANETS
			};


			/* gravity */
			static constexpr float _gravity[NUM_PLANETS] = {
				9.81f, 1.62f, 3.711f, 24.79f, 10.44f, 8.87f, 11.15f, 0.62f
			};


			// -- private members ---------------------------------------------

			/* mass */
			float _mass;

			/* inertia */
			::simd::float3x3 _inertia;

	};


	class velocity final {

		public:

		private:

			// -- private members ---------------------------------------------

			/* linear */
			::simd::float3 _linear;

			/* angular */
			::simd::float3 _angular;

	};



	// -- R I G I D  B O D Y --------------------------------------------------

	class rigid_body final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::rigid_body;



			/* update */
			auto update(void) noexcept -> void {
			}

		private:

			/* apply force */
			auto apply_force(const ::simd::float3& force) noexcept -> void {
			}

			/* update position */
			auto update_position(const float time) noexcept -> void {
			}

			/* update velocity */
			auto update_velocity(const float time) noexcept -> void {
			}

			/* detect collision */
			auto detect_collision(void) noexcept -> void {
			}

			/* solve constraints */
			auto solve_constraints(void) noexcept -> void {
			}



			// -- private members ---------------------------------------------

			/* transform */
			engine::transform<2> _transform;

			/* mass inertia */
			engine::mass_inertia _mass_inertia;

			/* velocity */
			engine::velocity _velocity;

	};



}

#endif // ENGINE_RIGID_BODY_HEADER
