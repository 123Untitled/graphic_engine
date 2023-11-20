#ifndef METAL_RENDER_PIPELINE_STATE_HPP
#define METAL_RENDER_PIPELINE_STATE_HPP

#include <Metal/Metal.hpp>

#include "mtl_render_pipeline_descriptor.hpp"
#include "mtl_device.hpp"


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {


	// -- R E N D E R  P I P E L I N E  S T A T E -----------------------------

	class render_pipeline_state final {

		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::render_pipeline_state;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline render_pipeline_state(void) noexcept
			: _state{nullptr} {}

			/* render pipeline descriptor constructor */
			inline render_pipeline_state(const mtl::render_pipeline_descriptor& descriptor)
			: _state{nullptr} {

				NS::Error* error = nullptr;
						  _state = mtl::device::underlying().newRenderPipelineState(descriptor, &error);

				if (_state == nullptr) {
					if (error)
						std::cout << error->code() << ": " << error->localizedDescription()->utf8String() << std::endl;
					throw std::runtime_error{"failed to create render pipeline state"};
				}
			}

			/* non-copyable class */
			non_copyable(render_pipeline_state);

			/* move constructor */
			inline render_pipeline_state(self&& other) noexcept
			: _state{other._state} {
				other._state = nullptr;
			}

			/* destructor */
			inline ~render_pipeline_state(void) noexcept {
				if (_state == nullptr) return;
				_state->release();
			}


			// -- public assignment operators ---------------------------------

			/* move assignment operator */
			inline auto operator=(self&& other) noexcept -> self& {
				if (this == &other) return *this;
				if (_state != nullptr) _state->release();
				_state = other._state;
				other._state = nullptr;
				return *this;
			}



			// -- public accessors --------------------------------------------

			/* underlying */
			inline auto underlying(void) const noexcept -> MTL::RenderPipelineState& {
				return *_state;
			}


			// -- public conversion operators ---------------------------------

			/* underlying reference */
			inline operator MTL::RenderPipelineState&(void) const noexcept {
				return *_state;
			}

			/* underlying const reference */
			inline operator const MTL::RenderPipelineState&(void) const noexcept {
				return *_state;
			}

			/* underlying pointer */
			inline operator MTL::RenderPipelineState*(void) const noexcept {
				return _state;
			}

			/* underlying const pointer */
			inline operator const MTL::RenderPipelineState*(void) noexcept {
				return _state;
			}


		private:

			// -- private members ---------------------------------------------

			/* underlying */
			MTL::RenderPipelineState* _state;

	};



	// -- R E N D E R  P I P E L I N E  S T A T E  L I B R A R Y --------------

	class render_pipeline_state_library final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::render_pipeline_state_library;


			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(render_pipeline_state_library);

			/* destructor */
			inline ~render_pipeline_state_library(void) noexcept = default;




			// -- public static accessors -------------------------------------

			template <xns::basic_string_literal name>
			inline static auto state(void) noexcept -> mtl::render_pipeline_state& {
				return shared()._states.template get<name>();
			}



		private:


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline render_pipeline_state_library(void)
			: _states{
				//mtl::render_pipeline_state{},
				mtl::render_pipeline_descriptor_library::pipeline_descriptor<"default_2D">(),
				mtl::render_pipeline_descriptor_library::pipeline_descriptor<"default_3D">()} {
			}


			// -- private static accessors ------------------------------------

			/* instance */
			inline static auto shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}


			// -- private members ---------------------------------------------

			/* render pipeline states */
			xns::literal_map<mtl::render_pipeline_state, "default_2D",
														 "default_3D"> _states;

	};

}

#endif // ifndef METAL_RENDER_PIPELINE_STATE_HPP
