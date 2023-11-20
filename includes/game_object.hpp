#ifndef ENGINE_GAME_OBJECT_HEADER
#define ENGINE_GAME_OBJECT_HEADER


#include "mesh.hpp"
#include "options.hpp"
#include "matrix.hpp"
#include "mtl_render_command_encoder.hpp"


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {



	// -- T R A N S F O R M  C O M P O N E N T --------------------------------

	class transform final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::transform;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline transform(void) noexcept
			: _position{0.0f, 0.0f, 0.0f},
			  _rotation{0.0f, 0.0f, 0.0f},
				 _scale{1.0f, 1.0f, 1.0f} {}

			/* copy constructor */
			inline transform(const self& other) noexcept
			: _position{other._position}, _rotation{other._rotation}, _scale{other._scale} {}

			/* move constructor */
			inline transform(transform&& other) noexcept
			: transform{other} {}

			/* destructor */
			inline ~transform(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline auto operator=(const self& other) noexcept -> self& {
				_position = other._position;
				_rotation = other._rotation;
				   _scale = other._scale;
				return *this;
			}

			/* move assignment operator */
			inline auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			// -- public accessors --------------------------------------------

			/* position */
			inline auto position(void) noexcept -> simd::float3& {
				return _position;
			}

			/* const position */
			inline auto position(void) const noexcept -> const simd::float3& {
				return _position;
			}

			/* rotation */
			inline auto rotation(void) noexcept -> simd::float3& {
				return _rotation;
			}

			/* const rotation */
			inline auto rotation(void) const noexcept -> const simd::float3& {
				return _rotation;
			}

			/* scale */
			inline auto scale(void) noexcept -> simd::float3& {
				return _scale;
			}

			/* const scale */
			inline auto scale(void) const noexcept -> const simd::float3& {
				return _scale;
			}


			// -- public modifiers --------------------------------------------

			/* scale */
			inline auto scale(const float x, const float y, const float z) noexcept -> void {
				_scale = simd::float3{x, y, z};
			}

			/* scale */
			inline auto scale(const float factor) noexcept -> void {
				_scale = simd::float3{factor, factor, factor};
			}


			// -- public methods ----------------------------------------------

			/* update */
			inline auto update(void) noexcept -> void {
				    _matrix.reset();
				_matrix.translate(_position);
				   _matrix.rotate(_rotation);
				    _matrix.scale(_scale);
			}

			/* update from parent */
			inline auto update(const self& parent) noexcept -> void {
				    _matrix.reset();
					_matrix = parent._matrix;
				_matrix.translate(_position);
				   _matrix.rotate(_rotation);
				    _matrix.scale(_scale);
			}

			/* render */
			inline auto render(mtl::render_command_encoder& encoder) const noexcept -> void {
				encoder.set_vertex_bytes(&(_matrix.get()), sizeof(_matrix), 3);
			}


		private:

			// -- private members ---------------------------------------------

			/* position */
			simd::float3 _position;

			/* rotation */
			simd::float3 _rotation;

			/* scale */
			simd::float3 _scale;

			/* matrix */
			engine::matrix _matrix;

	};


	// -- P H Y S I C S  C O M P O N E N T ------------------------------------

	class physics final {

		public:

		private:

	};


	// -- M A T E R I A L  C O M P O N E N T ----------------------------------

	class material final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = engine::material;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline material(void) noexcept
			: _color{1.0f, 1.0f, 1.0f, 1.0f} {}

			/* copy constructor */
			inline material(const self& other) noexcept
			: _color{other._color} {}

			/* move constructor */
			inline material(self&& other) noexcept
			: material{other} {}

			/* destructor */
			inline ~material(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline auto operator=(const self& other) noexcept -> self& {
				_color = other._color;
				return *this;
			}

			/* move assignment operator */
			inline auto operator=(self&& other) noexcept -> self& {
				return self::operator=(other);
			}


			// -- public accessors --------------------------------------------

			/* color */
			inline auto color(void) noexcept -> simd::float4& {
				return _color;
			}


			// -- public modifiers --------------------------------------------

			/* color */
			inline auto color(const float r, const float g, const float b, const float a) noexcept -> void {
				_color = simd::float4{r, g, b, a};
			}

			/* color */
			inline auto color(const float r, const float g, const float b) noexcept -> void {
				_color = simd::float4{r, g, b, 1.0f};
			}


			// -- public methods ----------------------------------------------

			/* render */
			inline auto render(mtl::render_command_encoder& encoder) const noexcept -> void {
				encoder.set_fragment_bytes(&_color, sizeof(_color), 1);
			}


		private:

			// -- private members ---------------------------------------------

			/* color */
			simd::float4 _color;

	};



	// -- G A M E  O B J E C T ------------------------------------------------

	class game_object final {

		public:

			/* default constructor */
			inline game_object(void)
			: _opts{}, _mesh{nullptr}, _transform{}, _physics{}, _material{},
			  _parent{nullptr}, _children{} {}


			/* update */
			inline auto update(void) noexcept -> void {
				if (not _parent)
					_transform.update();
				else
					_transform.update(_parent->transform());

				for (auto& child : _children) {
					child->update();
				}
			}

			/* add child */
			inline auto add_child(engine::game_object& child) noexcept -> void {
				child._parent = this;
				_children.emplace_back(&child);
			}


			/* render */
			inline auto render(mtl::render_command_encoder& encoder) const noexcept -> void {
				_material.render(encoder);
				_transform.render(encoder);
					 _mesh->render(encoder, _opts);

				for (auto& child : _children) {
					child->render(encoder);
				}
			}

			/* set mesh */
			inline auto set_mesh(engine::mesh& mesh) noexcept -> void {
				_mesh = &mesh;
			}


			// -- public accessors --------------------------------------------

			/* options */
			inline auto options(void) noexcept -> engine::options& {
				return _opts;
			}

			/* const options */
			inline auto options(void) const noexcept -> const engine::options& {
				return _opts;
			}

			/* transform */
			inline auto transform(void) noexcept -> engine::transform& {
				return _transform;
			}

			/* const transform */
			inline auto transform(void) const noexcept -> const engine::transform& {
				return _transform;
			}

			/* mesh */
			inline auto mesh(void) noexcept -> engine::mesh& {
				return *_mesh;
			}

			/* const mesh */
			inline auto mesh(void) const noexcept -> const engine::mesh& {
				return *_mesh;
			}

			/* physics */
			inline auto physics(void) noexcept -> engine::physics& {
				return _physics;
			}

			/* const physics */
			inline auto physics(void) const noexcept -> const engine::physics& {
				return _physics;
			}

			/* material */
			inline auto material(void) noexcept -> engine::material& {
				return _material;
			}

			/* const material */
			inline auto material(void) const noexcept -> const engine::material& {
				return _material;
			}


		private:

			// -- private members ---------------------------------------------

			/* options */
			engine::options _opts;

			/* mesh */
			engine::mesh* _mesh;

			/* transform */
			engine::transform _transform;

			/* physics */
			engine::physics _physics;

			/* material */
			engine::material _material;


			/* parent */
			engine::game_object* _parent;

			/* children */
			std::vector<engine::game_object*> _children;



	};

}

#endif // ENGINE_GAME_OBJECT_HEADER
