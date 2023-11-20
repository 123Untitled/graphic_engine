#ifndef METAL_DEPTH_STENCIL_STATE_HEADER
#define METAL_DEPTH_STENCIL_STATE_HEADER


#include <Metal/MTLDepthStencil.hpp>
#include "mtl_device.hpp"
#include <xns>


// -- M T L  N A M E S P A C E ------------------------------------------------

namespace mtl {



	// -- D E P T H  S T E N C I L  S T A T E ---------------------------------

	class depth_stencil_state final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::depth_stencil_state;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline depth_stencil_state(void)
			: _state{nullptr} {

				MTL::DepthStencilDescriptor* descriptor = MTL::DepthStencilDescriptor::alloc()->init();
				if (descriptor == nullptr)
					throw std::runtime_error{"failed to create depth stencil descriptor"};

				descriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
				descriptor->setDepthWriteEnabled(true);

				_state = mtl::device::shared().underlying().newDepthStencilState(descriptor);
				descriptor->release();
				if (_state == nullptr)
					throw std::runtime_error{"failed to create depth stencil state"};
			}

			/* non-copyable class */
			non_copyable(depth_stencil_state);

			/* move constructor */
			inline depth_stencil_state(self&& other) noexcept
			: _state{other._state} {
				other._state = nullptr;
			}

			/* destructor */
			inline ~depth_stencil_state(void) noexcept {
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


			// -- public conversion operators ---------------------------------

			/* underlying reference */
			inline operator MTL::DepthStencilState&(void) const noexcept {
				return *_state;
			}

			/* underlying const reference */
			inline operator const MTL::DepthStencilState&(void) const noexcept {
				return *_state;
			}

			/* underlying pointer */
			inline operator MTL::DepthStencilState*(void) const noexcept {
				return _state;
			}

			/* underlying const pointer */
			inline operator const MTL::DepthStencilState*(void) const noexcept {
				return _state;
			}


		private:

			// -- private members ---------------------------------------------

			/* depth stencil state */
			MTL::DepthStencilState* _state;

	};


	// -- D E P T H  S T E N C I L  S T A T E  L I B R A R Y ------------------

	class depth_stencil_state_library final {


		public:

			// -- public type -------------------------------------------------

			/* self type */
			using self = mtl::depth_stencil_state_library;


			// -- public lifecycle --------------------------------------------

			/* non-assignable class */
			non_assignable(depth_stencil_state_library);

			/* destructor */
			inline ~depth_stencil_state_library(void) noexcept = default;




			// -- public static accessors -------------------------------------

			template <xns::basic_string_literal name>
			inline static auto state(void) noexcept -> mtl::depth_stencil_state& {
				return shared()._states.template get<name>();
			}



		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			inline depth_stencil_state_library(void)
			: _states{
				mtl::depth_stencil_state{},
				mtl::depth_stencil_state{}
			} {
			}


			// -- private static accessors ------------------------------------

			/* instance */
			inline static auto shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}


			// -- private members ---------------------------------------------

			/* render pipeline states */
			xns::literal_map<mtl::depth_stencil_state, "default", "nop"> _states;

	};


}

#endif // METAL_DEPTH_STENCIL_STATE_HEADER
