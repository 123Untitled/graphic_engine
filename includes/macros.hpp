#ifndef ENGINE_MACROS_HPP
#define ENGINE_MACROS_HPP

#define non_copyable(__objclass__) \
	__objclass__(const __objclass__&) = delete; \
	auto operator=(const __objclass__&) -> __objclass__& = delete \

#define non_movable(__objclass__) \
	__objclass__(__objclass__&&) noexcept = delete; \
	auto operator=(__objclass__&&) noexcept -> __objclass__& = delete

#define non_assignable(__objclass__) \
	non_copyable(__objclass__); \
	non_movable(__objclass__)

#define non_instanciable(__objclass__) \
	~__objclass__(void) = delete; \
	~__objclass__(void) noexcept = delete; \
	non_assignable(__objclass__)


#endif // ENGINE_MACROS_HPP
