#ifndef NS_MEMORY_POOL_HPP
#define NS_MEMORY_POOL_HPP

#include <Foundation/NSAutoreleasePool.hpp>
#include <iostream>

namespace ns {

	class memory_pool final {

		public:

			memory_pool(void)
			: _pool{NS::AutoreleasePool::alloc()->init()} {}

			~memory_pool(void) {
				std::cout << "memory pool destructor" << std::endl;
				if (_pool) {
					//_pool->drain();
					_pool->release();
				}
			}

			auto release(void) noexcept -> void {
				if (_pool) {
					//_pool->drain();
					_pool->release();
					_pool = nullptr;
				}
			}

		private:

			NS::AutoreleasePool* _pool;
	};
}

#endif // NS_MEMORY_POOL_HPP
