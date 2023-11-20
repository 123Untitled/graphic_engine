#ifndef ENGINE_SCENE_HEADER
#define ENGINE_SCENE_HEADER

#include "camera.hpp"
#include "mtl_render_command_encoder.hpp"
#include "mesh_library.hpp"
#include "wavefront.hpp"
#include "mesh.hpp"
#include "game_object.hpp"


/* will be moved in the class to avoid recreating the mesh every frame */

inline auto create_floor() -> engine::mesh& {

//	print("createFloor");
	float from = -1;
	float to   = +1;
	float y    = 0;
	float s    = 0.1f;

	std::uint32_t i = 0;

	engine::vpackage pack;

	for (float z = from; z < to; z += s) {
		for (float x = from; x < to; x += s) {

			pack.first.emplace_back(x, y, z);
			pack.first.emplace_back(x + s, y, z);
			pack.first.emplace_back(x, y, z + s);
			pack.first.emplace_back(x + s, y, z + s);

			pack.second.emplace_back(i + 0);
			pack.second.emplace_back(i + 1);
			pack.second.emplace_back(i + 1);
			pack.second.emplace_back(i + 3);
			pack.second.emplace_back(i + 3);
			pack.second.emplace_back(i + 2);
			pack.second.emplace_back(i + 2);
			pack.second.emplace_back(i + 0);

			i += 4;

		}

	}


	static engine::mesh mesh{pack};

	return mesh;
}


inline auto create_cuboid(const float width, const float height, const float depth) -> engine::mesh& {

	engine::vpackage pack;

	auto& vec = pack.first;

	// remap to 0.0 - 1.0
	//auto max = std::max({width, height, depth});

	float x = width  / 2;
	float y = height / 2;
	float z = depth  / 2;


	// FRONT
	vec.emplace_back(-x, -y, -z, +0.0, +0.0, -1.0);
	vec.emplace_back(+x, -y, -z, +0.0, +0.0, -1.0);
	vec.emplace_back(+x, +y, -z, +0.0, +0.0, -1.0);
	vec.emplace_back(-x, +y, -z, +0.0, +0.0, -1.0);
	// BACK
	vec.emplace_back(+x, -y, +z, +0.0, +0.0, +1.0);
	vec.emplace_back(-x, -y, +z, +0.0, +0.0, +1.0);
	vec.emplace_back(-x, +y, +z, +0.0, +0.0, +1.0);
	vec.emplace_back(+x, +y, +z, +0.0, +0.0, +1.0);
	// UP
	vec.emplace_back(-x, +y, -z, +0.0, +1.0, +0.0);
	vec.emplace_back(+x, +y, -z, +0.0, +1.0, +0.0);
	vec.emplace_back(+x, +y, +z, +0.0, +1.0, +0.0);
	vec.emplace_back(-x, +y, +z, +0.0, +1.0, +0.0);
	// DOWN
	vec.emplace_back(-x, -y, +z, +0.0, -1.0, +0.0);
	vec.emplace_back(+x, -y, +z, +0.0, -1.0, +0.0);
	vec.emplace_back(+x, -y, -z, +0.0, -1.0, +0.0);
	vec.emplace_back(-x, -y, -z, +0.0, -1.0, +0.0);
	// LEFT
	vec.emplace_back(-x, -y, +z, -1.0, +0.0, +0.0);
	vec.emplace_back(-x, -y, -z, -1.0, +0.0, +0.0);
	vec.emplace_back(-x, +y, -z, -1.0, +0.0, +0.0);
	vec.emplace_back(-x, +y, +z, -1.0, +0.0, +0.0);
	// RIGHT
	vec.emplace_back(+x, -y, -z, +1.0, +0.0, +0.0);
	vec.emplace_back(+x, -y, +z, +1.0, +0.0, +0.0);
	vec.emplace_back(+x, +y, +z, +1.0, +0.0, +0.0);
	vec.emplace_back(+x, +y, -z, +1.0, +0.0, +0.0);

	auto& idxs = pack.second;

	idxs = {
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10, 11,  8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	// inverse order for back cullmode

	idxs = {
		2,  1,  0,  0,  3,  2,
		6,  5,  4,  4,  7,  6,
		10,  9,  8,  8, 11, 10,
		14, 13, 12, 12, 15, 14,
		18, 17, 16, 16, 19, 18,
		22, 21, 20, 20, 23, 22

	};



	static engine::mesh mesh{pack};

	return mesh;
}




// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- S C E N E -----------------------------------------------------------

	class scene final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::scene;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline scene(void)
			: _camera{}, _objects{}, _floor{}, _cuboid{} {


				_cuboid.set_mesh(create_cuboid(8.0f, 19.0f, 3.0f));
				_cuboid.material().color(0.1f, 0.1f, 0.1f, 1.0f);
				_cuboid.transform().position().y = 9.5f;
				//_cuboid.options().cullmode(MTL::CullModeFront);


				for (auto& f : _floor) {
					f.set_mesh(create_floor());
					f.options().primitive(MTL::PrimitiveTypeLine);
				}

				_floor[0].transform().scale(30.0f);

				_floor[1].transform().rotation().z = 1.5708f;
				_floor[1].transform().position().x = 1.0f;
				_floor[1].transform().position().y = 1.0f;

				_floor[2].transform().rotation().z = 1.5708f;
				_floor[2].transform().position().x = -1.0f;
				_floor[2].transform().position().y = 1.0f;

				_floor[3].transform().rotation().x = 1.5708f;
				_floor[3].transform().position().z = 1.0f;
				_floor[3].transform().position().y = 1.0f;

				_floor[4].transform().rotation().x = 1.5708f;
				_floor[4].transform().position().z = -1.0f;
				_floor[4].transform().position().y = 1.0f;

				_floor[5].transform().position().y = 2.0f;





				for (std::size_t i = 1; i < 6;++i)
					_floor[0].add_child(_floor[i]);







				
				_objects.emplace_back();
				_objects.back().set_mesh(engine::mesh_library::cube());


				_objects.back().transform().position().y = 1.0f;
				_objects.back().transform().position().z = 5.0f;
				_objects.back().transform().position().x = 5.0f;



				_objects.emplace_back();
				_objects.back().set_mesh(engine::mesh_library::cube());

				_objects.back().transform().position().y = 4.0f;
				_objects.back().transform().position().z = 3.0f;
				_objects.back().transform().position().x = 2.0f;


				_objects.emplace_back();
				_objects.back().set_mesh(engine::mesh_library::cube());

				_objects.back().transform().position().y = 4.0f;
				_objects.back().transform().position().z = 3.0f;
				_objects.back().transform().position().x = 2.0f;
				_objects.back().options().fillmode(MTL::TriangleFillModeLines);
				_objects.back().transform().scale(1.001f);
				_objects.back().material().color(0.1f, 0.1f, 0.1f, 1.0f);

				//_objects.front().add_child(_objects.back());

			}

			/* non-copyable class */
			non_copyable(scene);

			/* move constructor */
			inline scene(self&& other) noexcept
			{
				// not implemented
			}

			/* destructor */
			inline ~scene(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* render */
			inline auto render(mtl::render_command_encoder& encoder) noexcept -> void {

				static float angle = 0.0f;
				static float angle2 = 0.0f;

				for (auto& object : _objects) {
					object.transform().rotation().y = angle;
					object.transform().rotation().x = angle2;
				}

				angle += 0.008f;
				angle2 += 0.005f;

				_camera.update();

				for (auto& object : _objects)
					object.update();

				_cuboid.update();

				engine::ray_cast ray{_camera};

				int i = 0;
				for (auto& o : _objects) {
					if (i == 2) {
						break;
					}
					++i;
					if (ray.intersection(o)) {
						o.material().color(0.3f, 0.3f, 0.8f, 1.0f);
						//o.transform().scale(1.2f);
					}
					else {
						o.material().color(1.0f, 1.0f, 1.0f, 1.0f);
						//o.transform().scale(1.0f);
					}
				}


				_floor[0].update();


				//_objects.front().update();

				_camera.render(encoder);

				 _floor[0].render(encoder);

				 _cuboid.render(encoder);

				 for (auto& object : _objects)
				 	object.render(encoder);
				 //_objects.front().render(encoder);
			}


		private:

			// -- private members ---------------------------------------------

			/* camera */
			engine::camera _camera;

			std::vector<engine::game_object> _objects;
			engine::game_object _floor[6];

			engine::game_object _cuboid;


	};

}

#endif // ENGINE_SCENE_HEADER
