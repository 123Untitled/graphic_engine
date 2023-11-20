#ifndef ENGINE_OPTIONS_HEADER
#define ENGINE_OPTIONS_HEADER


#include <Metal/Metal.hpp>
#include <mtl_render_command_encoder.hpp>


// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {


	// -- O P T I O N S -------------------------------------------------------

	class options final {


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline options(void) noexcept
			:		_type{MTL::PrimitiveTypeTriangle}, // type of primitive (draw lines, triangles, etc.)
				_fillmode{MTL::TriangleFillModeFill},  // triangle fill mode (draw filled triangles, wireframe, etc.)
				_cullmode{MTL::CullModeBack},          // cull mode (cull front, back, or both)
				 _winding{MTL::WindingClockwise}       // winding (front face is clockwise or counter-clockwise)
			{}


			// -- public accessors --------------------------------------------

			/* primitive type */
			inline auto primitive(void) const noexcept -> MTL::PrimitiveType {
				return _type;
			}

			/* triangle fill mode */
			inline auto fillmode(void) const noexcept -> MTL::TriangleFillMode {
				return _fillmode;
			}

			/* cull mode */
			inline auto cullmode(void) const noexcept -> MTL::CullMode {
				return _cullmode;
			}

			/* winding */
			inline auto winding(void) const noexcept -> MTL::Winding {
				return _winding;
			}


			// -- public modifiers ------------------------------------

			/* primitive type */
			inline auto primitive(const MTL::PrimitiveType type) noexcept -> void {
				_type = type;
			}

			/* triangle fill mode */
			inline auto fillmode(const MTL::TriangleFillMode fillmode) noexcept -> void {
				_fillmode = fillmode;
			}

			/* cull mode */
			inline auto cullmode(const MTL::CullMode cullmode) noexcept -> void {
				_cullmode = cullmode;
			}

			/* winding */
			inline auto winding(const MTL::Winding winding) noexcept -> void {
				_winding = winding;
			}


			/* render */
			inline auto render(mtl::render_command_encoder& encoder) const noexcept -> void {
				encoder.cullmode(_cullmode);
				encoder.fillmode(_fillmode);
				encoder.winding(_winding);
			}


		private:

			MTL::PrimitiveType    _type;
			MTL::TriangleFillMode _fillmode;
			MTL::CullMode         _cullmode;
			MTL::Winding          _winding;

			//MTL::DepthClipMode _depthclipmode;
			//MTL::DepthStencilDescriptor _depthstencil;
	};

}

#endif // ENGINE_OPTIONS_HEADER
