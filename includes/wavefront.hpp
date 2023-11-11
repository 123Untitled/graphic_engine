#ifndef ENGINE_MODEL_LOADER_HPP
#define ENGINE_MODEL_LOADER_HPP

#include "vertex.hpp"

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {




	class model_loader final {


		public:

			/* deleted default constructor */
			model_loader(void) = delete;

			/* path constructor */
			inline model_loader(const char* path) noexcept
			: _fd{-1}, _path{path} {}

			/* destructor */
			inline ~model_loader(void) noexcept {
				// close file descriptor
				if (_fd != -1)
					::close(_fd);
			}

			enum : unsigned int {
				BUFFER_SIZE = 1024
			};

			inline auto parse(void) -> void {

				_fd = ::open(_path, O_RDONLY);
				if (_fd == -1)
					throw std::runtime_error("could not open file");

				char buff[BUFFER_SIZE + 1];



				std::vector<simd::float3> positions;
				std::vector<simd::float3> normals;

				auto skip_white = [](char*& ptr) -> void {
					while (*ptr == ' ' || *ptr == '\t')
						++ptr;
				};

				auto skip_comment = [](char*& ptr) -> void {
					while (*ptr != '\0' && *ptr != '\n')
						++ptr;
					ptr += (*ptr == '\n');
				};

				auto skip_line = [](char*& ptr) -> void {
					while (*ptr == '\n')
						++ptr;
				};



				ssize_t readed = 0;

				while ((readed = ::read(_fd, buff, BUFFER_SIZE)) > 0) {

					buff[readed] = '\0';
					char* ptr = buff;

					while (*ptr != '\0') {

						//std::cout << "char: " << *ptr << std::endl;
						skip_line(ptr);

						if (*ptr == '#') {
							skip_comment(ptr);
						}

						skip_white(ptr);

						if (*ptr == 'v') {
							++ptr;

							// normal
							if (*ptr == 'n') {
								skip_white(++ptr);
								double x = 0.0;
								double y = 0.0;
								double z = 0.0;
								int ret = 0;
								ret += to_double(ptr, x);
								skip_white(ptr);
								ret += to_double(ptr, y);
								skip_white(ptr);
								ret += to_double(ptr, z);

								if (ret == 0)
									normals.emplace_back(simd::float3{(float)x, (float)y, (float)z});
								else {
									std::cerr << "error: " << ret << std::endl;
									return;
								}
								std::cout << "normal: " << x << " " << y << " " << z << std::endl;

							}
							// vertex
							else if (*ptr == ' ') {
								skip_white(++ptr);
								// position
								double x = 0.0;
								double y = 0.0;
								double z = 0.0;
								int ret = 0;
								ret += to_double(ptr, x);
								skip_white(ptr);
								ret += to_double(ptr, y);
								skip_white(ptr);
								ret += to_double(ptr, z);

								if (ret == 0)
									positions.emplace_back(simd::float3{(float)x, (float)y, (float)z});
								else {
									std::cerr << "error: " << ret << std::endl;
									return;
								}
								std::cout << "position: " << x << " " << y << " " << z << std::endl;
							}
							else {
								std::cout << "error: " << *ptr << std::endl;
							}
						}

						// face
						if (*ptr == 'f') {
							skip_comment(++ptr);
							std::cout << "face" << std::endl;
						}
						if (*ptr == 'o') {
							skip_comment(++ptr);
							std::cout << "object" << std::endl;
						}
						if (*ptr == 's') {
							skip_comment(++ptr);
							std::cout << "group" << std::endl;
						}

					}
				}


				std::vector<engine::vertex> vertices;

				for (size_t i = 0; i < positions.size(); ++i) {
				}


			}


			inline auto to_double(char*& str, double& num) -> int {

				num  = 0.0;
				double fraction = 0.0;

				double sign = +1;
				int exponent = 0;

				if (*str == '-') {
					sign = -1;
					++str; }
				else if (*str == '+')
					++str;

				if ((*str ^ '0') >= 10)
					return -1;

				while ((*str ^ '0') < 10) {
					num *= 10.0;
					num += (*str ^ '0');
					++str;
				}
				if (*str == '.') {
					++str;
					while ((*str ^ '0') < 10) {
						num *= 10.0;
						num += (*str ^ '0');
						--exponent;
						++str;
					}
				}
				while (exponent < 0) {
					num *= 0.1;
					++exponent;
				}

				num *= sign;

				return 0;
			}


			static inline auto parse(const char* path) -> void {
				model_loader loader{path};
				loader.parse();
			}


		private:

			// -- private members ---------------------------------------------

			/* descriptor */
			int _fd;

			/* path */
			const char* _path;


	};




}

#endif // ENGINE_MODEL_LOADER_HPP
