#ifndef ENGINE_MODEL_LOADER_HPP
#define ENGINE_MODEL_LOADER_HPP

#include "vertex.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <map>
#include <ranges>

#include <xns>


#include <simd/simd.h>

// -- E N G I N E  N A M E S P A C E ------------------------------------------

namespace engine {



	// .obj file parser
	// using lookup table state machine

	/* brief

		OBJ files do not require any sort of header,
		although it is common to begin the file with a comment line of some kind.
		Comment lines begin with a hash mark (#).
		Blank space and blank lines can be freely added to the file to aid in formatting and readability.
		Each non-blank line begins with a keyword and may be followed on the same line with the data for that keyword.
		Lines are read and processed until the end of the file.
		Lines can be logically joined with the line continuation character '\' at the end of a line.
		The following keywords may be included in an OBJ file.
		In this list, keywords are arranged by data type, and each is followed by a brief description.

		- vertex data
		* v: vertex
		* vt: texture coordinate
		* vn: normal
		* vp: parameter space vertex

		- free-form curve/surface attributes
		* deg: degree
		* bmat: basis matrix
		* step: step size
		* cstype: curve or surface type

		- elements
		* p: point
		* l: line
		* f: face
		* curv: curve
		* curv2: 2D curve
		* surf: surface

		- free-form curve/surface body statements
		* parm: parameter values
		* trim: outer trimming loop
		* hole: inner trimming loop
		* scrv: special curve
		* sp: special point
		* end: end statement

		- connectivity between free-form surfaces
		* con: connect

		- grouping
		* g: group name
		* s: smoothing group
		* mg: merging group
		* o: object name

		- display/render attributes
		* bevel: bevel interpolation
		* c_interp: color interpolation
		* d_interp: dissolve interpolation
		* lod: level of detail
		* usemtl: material name
		* mtllib: material library
		* shadow_obj: shadow casting
		* trace_obj: ray tracing
		* ctech: curve approximation technique
		* stech: surface approximation technique

	*/

	template <typename T>
	class index final {

		// -- assertions ------------------------------------------------------

		/* integral type */
		static_assert(xns::is_integral<T>, "index: T must be integral type");


		public:

			// -- public constants --------------------------------------------

			/* index kind */
			enum kind : xns::ubyte { VERTEX, TEXTURE, NORMAL, NUM_KINDS };


			// -- public types ------------------------------------------------

			/* self type */
			using self = engine::index<T>;

			/* index type */
			using type = T;

			/* index group */
			using group = xns::array<type, NUM_KINDS>;


		private:

			// -- private members ---------------------------------------------

			/* index */
			type _index;

			/* kind */
			kind _kind;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inline index(void) noexcept
			: _index{0}, _kind{VERTEX} {}

			/* constructor */
			inline index(const type index, const kind kind) noexcept
			: _index{index}, _kind{kind} {}

			/* copy constructor */
			inline index(const index& other) noexcept
			: _index{other._index}, _kind{other._kind} {}

			/* move constructor */
			inline index(index&& other) noexcept
			: index{other} {}

			/* destructor */
			inline ~index(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			inline auto operator=(const index& other) noexcept -> index& {
				_index = other._index;
				_kind  = other._kind;
				return *this;
			}

			/* move assignment operator */
			inline auto operator=(index&& other) noexcept -> index& {
				return operator=(other);
			}

			// -- public conversion operators ---------------------------------

			/* conversion to type */
			inline operator type(void) const noexcept {
				return _index;
			}


			// -- public accessors --------------------------------------------

			/* kind */
			inline auto kind(void) const noexcept -> kind {
				return _kind;
			}


			// -- F A C E -----------------------------------------------------

			class face final {

				public:

					// -- public lifecycle ------------------------------------

					/* default constructor */
					inline face(void) noexcept
					: _groups{} {}

					/* destructor */
					inline ~face(void) noexcept = default;


					// -- public modifiers --------------------------------

					/* new index group */
					inline auto add_entry(const index::type vertex,
										  const index::type texture,
										  const index::type normal) -> void {
						static unsigned int i = 0;
						_groups[i] = group{vertex, texture, normal};
					}


				private:

					/* triangle */
					xns::array<index::group, 3> _groups;
			};

	};


	// -- W A V E F R O N T ---------------------------------------------------

	class wavefront final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self       = engine::wavefront;

			/* index type */
			using index      = xns::u32;



			// -- D A T A -----------------------------------------------------

			class data final {

				private:

					struct position final {
						float x;
						float y;
						float z;
					};

					struct texcoord final {
						float u;
						float v;
					};

					struct normal final {
						float x;
						float y;
						float z;
					};

					struct face final {
						std::uint32_t v1;
						std::uint32_t t1;
						std::uint32_t n1;
						std::uint32_t v2;
						std::uint32_t t2;
						std::uint32_t n2;
						std::uint32_t v3;
						std::uint32_t t3;
						std::uint32_t n3;
					};


				public:

					enum data_kind {
						D_VERTEX,
						D_NORMAL,
						D_TEXCOORD,
						D_FACE,
						D_VINDEX,
						D_NINDEX,
						D_TINDEX,
						NUM_TARGETS
					};

					static constexpr const char* const types[NUM_TARGETS] {
						"vertex",
						"normal",
						"texcoord",
						"face",
						"vertex index",
						"normal index",
						"texture index"
					};

				/* default constructor */
				inline data(void)
				: _positions{}, _texcoords{}, _normals{}, _faces{} {}

				/* non-assignable class */
				non_assignable(data);

				/* destructor */
				inline ~data(void) noexcept = default;


				inline auto new_position(const float x, const float y, const float z) -> void {
					_positions.emplace_back(simd::float3{x, y, z});
				}

				inline auto new_texcoord(const float u, const float v) -> void {
					_texcoords.emplace_back(simd::float2{u, v});
				}

				inline auto new_normal(const float x, const float y, const float z) -> void {
					_normals.emplace_back(simd::float3{x, y, z});
				}

				inline auto new_face(const std::uint32_t v1, const std::uint32_t t1, const std::uint32_t n1,
									 const std::uint32_t v2, const std::uint32_t t2, const std::uint32_t n2,
									 const std::uint32_t v3, const std::uint32_t t3, const std::uint32_t n3) -> void {
					_faces.emplace_back(face{v1, t1, n1, v2, t2, n2, v3, t3, n3});
				}


				/* positions */
				xns::vector<simd::float3> _positions;

				/* texcoords */
				xns::vector<simd::float2> _texcoords;

				/* normals */
				xns::vector<simd::float3> _normals;

				/* faces */
				xns::vector<face> _faces;



				auto print(void) -> void {
					//std::cout << "positions: " << _positions.size() << std::endl;
					//for (auto& p : _positions)
					//	std::cout << "x: " << p.x << " y: " << p.y << " z: " << p.z << std::endl;
					//std::cout << "texcoords: " << _texcoords.size() << std::endl;
					//for (auto& t : _texcoords)
					//	std::cout << "u: " << t.u << " v: " << t.v << std::endl;
					//std::cout << "normals: " << _normals.size() << std::endl;
					//for (auto& n : _normals)
					//	std::cout << "x: " << n.x << " y: " << n.y << " z: " << n.z << std::endl;
					//std::cout << "faces: " << _faces.size() << std::endl;
					//for (auto& f : _faces) {
					//	std::cout << f.v1 << "/" << f.t1 << "/" << f.n1 << " " << f.v2 << "/" << f.t2 << "/" << f.n2 << " " << f.v3 << "/" << f.t3 << "/" << f.n3 << std::endl;
					//}
				}
			};


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			wavefront(void) = delete;

			/* path constructor */
			inline wavefront(const char* path)
			:	_fd{::open(path, O_RDONLY)},
				_path{path},
				_buffer{},
				_tr{&_transitions[0][3]}, // default state
				_i{0U},
				_kmap{},
				_integral{0U},
				_decimal{0U},
				_sign{1},
				_data{nullptr}
			{}

			/* non-assignable */
			non_assignable(wavefront);

			/* destructor */
			inline ~wavefront(void) noexcept {
				if (_fd != -1)
					::close(_fd);
			}


			/* parse */
			auto parse(data& data) -> void {
				_data = &data;
				// -- file check ----------------------------------------------

				if (_fd == -1) {
					::write(2, "wavefront: error, can't open file\n", 35);
					return;
				}

				#define ARR_SIZE CHARTYPE_SIZE * STATE_SIZE
				std::cout << "chartype size: " << CHARTYPE_SIZE << std::endl;
				std::cout << "state size: " << STATE_SIZE << std::endl;
				std::cout << "sizeof arr: " << ARR_SIZE << std::endl;
				std::cout << "sizeof trans struct: " << sizeof(transition) << std::endl;
				std::cout << "sizeof trans: " << sizeof(_transitions) * ARR_SIZE << std::endl;


				// -- main loop -----------------------------------------------


				signed_type readed = 0;

				// loop over file
				while ((readed = read()) > 0) {
					// loop over buffer
					for (_i = 0; _i < static_cast<size_type>(readed); ++_i) {
						// get chartype
						chartype c = _chartypes[static_cast<byte>(_buffer[_i])];
						// get next transition
						_tr = &(_transitions[_tr->state()][c]);
						// execute action
						(this->*_actions[_tr->action()])();
					}

					if (_tr->state() == S_ERROR) {
						const char* msg = _tr->msg();
						if (msg)
							std::cout << "\x1b[31mERROR\x1b[0m: " << msg << std::endl;
						else
							std::cout << "\x1b[31mERROR\x1b[0m: unknown error" << std::endl;
						break;
					}
					else if (_tr->state() == S_END) {
						std::cout << "\x1b[32mEND\x1b[0m" << std::endl;
						break;
					}
				}
				data.print();
			}



			static inline auto parse(const char* path) -> engine::vpackage {
				//self::data data;
				engine::vpackage vpackage;
				self{path}.parse2(vpackage);
				return vpackage;
			}



			/* temp solution */
			auto parse2(engine::vpackage& package) -> void {

				class data data;

				std::fstream file(_path, std::ios::in);
				if (not file.is_open()) {
					std::cout << "error: can't open file" << std::endl;
					return;
				}


				std::string line;
				while (std::getline(file, line)) {

					//std::cout << "line -> " << line << std::endl;

					// split by space
					std::istringstream iss(line);
					std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
												   std::istream_iterator<std::string>{}};

					if (tokens.size() == 0)
						continue;

					if (tokens[0] == "v") {
						if (tokens.size() != 4) {
							std::cout << "parsing error." << std::endl;
							return;
						}
						data.new_position(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
					}
					else if (tokens[0] == "vn") {
						if (tokens.size() != 4) {
							std::cout << "parsing error." << std::endl;
							return;
						}
						data.new_normal(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

					}
					else if (tokens[0] == "vt") {
						if (tokens.size() != 3) {
							std::cout << "parsing error." << std::endl;
							return;
						}
						data.new_texcoord(std::stof(tokens[1]), std::stof(tokens[2]));

					}
					else if (tokens[0] == "f") {
						if (tokens.size() != 4) {
							std::cout << "parsing error." << std::endl;
							return;
						}

						auto split = [](const std::string& str, const char sep) -> std::vector<std::string> {
							std::vector<std::string> tokens;
							std::string token;
							std::istringstream tokenStream(str);
							while (std::getline(tokenStream, token, sep))
								tokens.push_back(token);
							return tokens;
						};

						auto subtok1 = split(tokens[1], '/');
						auto subtok2 = split(tokens[2], '/');
						auto subtok3 = split(tokens[3], '/');


						data.new_face(std::stoi(subtok1[0]), std::stoi(subtok1[1]), std::stoi(subtok1[2]),
									  std::stoi(subtok2[0]), std::stoi(subtok2[1]), std::stoi(subtok2[2]),
									  std::stoi(subtok3[0]), std::stoi(subtok3[1]), std::stoi(subtok3[2]));

					}

					//std::cout << "line -> " << line << std::endl;
				}

				auto& vec     = package.first;
				auto& indices = package.second;

				// re-index
				for (const auto& face : data._faces) {
					vec.emplace_back(data._positions[face.v1 - 1], data._normals[face.n1 - 1], data._texcoords[face.t1 - 1]);
					vec.emplace_back(data._positions[face.v2 - 1], data._normals[face.n2 - 1], data._texcoords[face.t2 - 1]);
					vec.emplace_back(data._positions[face.v3 - 1], data._normals[face.n3 - 1], data._texcoords[face.t3 - 1]);
				}

			};


		private:

			// -- private types -----------------------------------------------

			/* size type */
			using size_type    = xns::size_t;

			/* signed type */
			using signed_type  = xns::make_signed<size_type>;

			/* byte type */
			using byte         = xns::ubyte;

			/* enum type */
			using enum_type    = xns::ubyte;

			/* action prototype */
			using action_proto = auto (wavefront::*)(void) noexcept -> void;


			/* buffer size */
			enum : size_type { BUFFER_SIZE = 200 };



			// -- forward declarations ----------------------------------------

			class transition;

			class keyword_map;




			// -- private actions ---------------------------------------------

			/* exit */
			inline auto exit(void) noexcept -> void {
				_i = BUFFER_SIZE;
			}

			/* skip */
			inline auto skip(void) noexcept -> void {}

			/* count */
			inline auto count(void) noexcept -> void {
				_kmap.add(_buffer[_i]);
			}


			/* parse keyword */
			inline auto parse_keyword(void) noexcept -> void {
				auto type = _kmap.type();

				// reset count
				if (type == K_INVALID) {
					::exit(1);
				}
				if (type == K_NOT_IMPLEMENTED) {
					// goto comment state
					_tr = &_transitions[0][HASH];
					return;
				}

				switch (type) {
					case K_V:
						_dtype = data::D_VERTEX;
						break;
					case K_VT:
						_dtype = data::D_TEXCOORD;
						break;
					case K_VN:
						_dtype = data::D_NORMAL;
						break;
					case K_F:
						_dtype = data::D_FACE;
						break;
					default:
						break;
				}
				//_data->new_entry(_dtype);
			}

			/* parse float */
			inline auto parse_float(void) noexcept -> void {

				float value = static_cast<float>(_integral);

				float decimal = .0f;
				if (_decimal) {
					const float facteur = std::pow(10, std::floor(std::log10(_decimal) + 1));
					decimal = static_cast<float>(_decimal) / facteur ? facteur : 1;
					value += decimal;
				}
				value *= _sign;
				//_data->add_value(_dtype, value);
				//_mesh->_data[_dtype].back()._values.emplace_back(value);
				std::cout << "value: " << value << std::endl;

				_integral = 0;
				_decimal  = 0;
				_sign     = 1;
			}


			/* plus */
			inline auto plus(void) noexcept -> void {
				_sign = +1;
			}

			/* minus */
			inline auto minus(void) noexcept -> void {
				_sign = -1;
			}

			/* integer */
			inline auto integer(void) noexcept -> void {
				_integral *= 10U;
				_integral += _buffer[_i] ^ '0';
			}

			/* decimal */
			inline auto decimal(void) noexcept -> void {
				_decimal *= 10U;
				_decimal += _buffer[_i] ^ '0';
			}


			/* read */
			inline auto read(void) noexcept -> signed_type {
				return ::read(_fd, _buffer, BUFFER_SIZE);
			}


			// -- private constants -------------------------------------------

			/* chartype */
			enum chartype : enum_type {
				NIL,       // '\0'
				CTRL,      // [1 - 31] | 127 (except 9, 10, 13)
				OTHER,     // ...

				SPACE,     // ' ', '\t'
				CR,        // '\r'
				LF,        // '\n'
				BACKSLASH, // '\'
				HASH,      // '#'

				KEYWORD,   // _abcdfghijlmnoprstuvwy

				EXPONENT,  // 'e' 'E'
				TWO,       // '2'

				NUMBER,    // '0' - '9' (except '2')
				POINT,     // '.'
				PLUS,      // '+'
				MINUS,     // '-'
				SLASH,     // '/'

				CHARTYPE_SIZE
			};

			/* state type */
			enum state_type : enum_type {
				S_START,
				S_ERROR,
				S_END,

				S_COMMENT,
				S_RETURN,
				S_ESCAPE,

				S_KEYWORD,


				S_AFTER_KEYWORD,

				S_BETWEEN_INDEXES,
				S_INDEX,

				S_PLUS,
				S_MINUS,
				S_INTEGER,
				S_DECIMAL,

				STATE_SIZE
			};

			/* action type */
			enum action_type : enum_type {
				A_EXIT,
				A_SKIP,
				A_COUNT,

				A_PARSE_KEYWORD,
				A_PARSE_FLOAT,

				A_PLUS,
				A_MINUS,

				A_INTEGER,
				A_DECIMAL,
			};

			static constexpr action_proto _actions[] {
				&wavefront::exit,
				&wavefront::skip,
				&wavefront::count,
				&wavefront::parse_keyword,
				&wavefront::parse_float,
				&wavefront::plus,
				&wavefront::minus,
				&wavefront::integer,
				&wavefront::decimal,
			};

			/* keyword type */
			enum keyword_type : enum_type {
				K_INVALID,         // invalid keyword
				K_NOT_IMPLEMENTED, // not implemented keyword
				K_V,               // vertex
				K_VT,              // texture coordinate
				K_VN,              // normal
				K_VP,              // parameter space vertex
				K_DEG,             // degree
				K_BMAT,            // basis matrix
				K_STEP,            // step size
				K_CSTYPE,          // curve or surface type
				K_P,               // point
				K_L,               // line
				K_F,               // face
				K_CURV,            // curve
				K_CURV2,           // 2D curve
				K_SURF,            // surface
				K_PARM,            // parameter values
				K_TRIM,            // outer trimming loop
				K_HOLE,            // inner trimming loop
				K_SCRV,            // special curve
				K_SP,              // special point
				K_END,             // end statement
				K_CON,             // connect
				K_G,               // group name
				K_S,               // smoothing group
				K_MG,              // merging group
				K_O,               // object name
				K_BEVEL,           // bevel interpolation
				K_C_INTERP,        // color interpolation
				K_LOD,             // level of detail
				K_USEMTL,          // material name
				K_MTLLIB,          // material library
				K_SHADOW_OBJ,      // shadow casting
				K_TRACE_OBJ,       // ray tracing
				K_CTECH,           // curve approximation technique
				K_STECH,           // surface approximation technique
				NUM_KEYWORDS       // number of keywords
			};


			// -- T R A N S I T I O N -----------------------------------------

			class transition final {


				public:

					/* deleted default constructor */
					transition(void) = delete;

					/* state/action constructor */
					inline constexpr transition(const  state_type state,
												const action_type action) noexcept
					: _state{state}, _action{action}, _msg{nullptr} {}

					/* state/action/msg constructor */
					inline constexpr transition(const  state_type state,
												const action_type action,
												const char* const msg) noexcept
					: _state{state}, _action{action}, _msg{msg} {}

					/* non-assignable class */
					non_assignable(transition);

					/* destructor */
					inline constexpr ~transition(void) noexcept = default;


					// -- public accessors ------------------------------------

					/* state */
					inline constexpr auto state(void) const noexcept -> state_type {
						return _state;
					}

					/* action */
					inline constexpr auto action(void) const noexcept -> action_type {
						return _action;
					}

					/* message */
					inline constexpr auto msg(void) const noexcept -> const char* {
						return _msg;
					}


				private:

					// -- private members -------------------------------------

					/* state */
					const state_type  _state;

					/* action */
					const action_type _action;

					/* message */
					const char* const _msg;
			};




			/* transitions */
			static inline /*constexpr*/ transition _transitions[STATE_SIZE][CHARTYPE_SIZE] {

				/* START */ {
					{S_END,         A_EXIT},  // NIL
					{S_ERROR,       A_EXIT, "ctrl found at start of line"},  // CTRL
					{S_ERROR,       A_EXIT, "invalid char at start of line"},  // OTHER

					{S_START,       A_SKIP},  // SPACE
					{S_RETURN,      A_SKIP},  // CR
					{S_START,       A_SKIP},  // LF
					{S_START,       A_SKIP},  // BACKSLASH // need wait LF
					{S_COMMENT,     A_SKIP},  // HASH

					{S_KEYWORD,     A_COUNT}, // KEYWORD keyword

					{S_KEYWORD,     A_COUNT}, // EXPONENT is in keyword char
					{S_KEYWORD,     A_COUNT}, // TWO is in keyword char

					{S_ERROR,       A_EXIT, "number found at start of line"},   // NUMBER
					{S_ERROR,       A_EXIT, "dot found at start of line"},   // POINT
					{S_ERROR,       A_EXIT, "sign found at start of line"},   // PLUS
					{S_ERROR,       A_EXIT, "sign found at start of line"},   // MINUS
					{S_ERROR,       A_EXIT, "slash found at start of line"},   // SLASH
				},

				/* ERROR */ {
					{S_ERROR,       A_EXIT},  // NIL
					{S_ERROR,       A_EXIT},  // CTRL
					{S_ERROR,       A_EXIT},  // OTHER

					{S_ERROR,       A_EXIT},  // SPACE
					{S_ERROR,       A_EXIT},  // CR
					{S_ERROR,       A_EXIT},  // LF
					{S_ERROR,       A_EXIT},  // BACKSLASH
					{S_ERROR,       A_EXIT},  // HASH

					{S_ERROR,       A_EXIT},  // KEYWORD

					{S_ERROR,       A_EXIT},  // EXPONENT
					{S_ERROR,       A_EXIT},  // TWO

					{S_ERROR,       A_EXIT},  // NUMBER
					{S_ERROR,       A_EXIT},  // POINT
					{S_ERROR,       A_EXIT},  // PLUS
					{S_ERROR,       A_EXIT},  // MINUS
					{S_ERROR,       A_EXIT},  // SLASH
				},

				/* END */ {
					{S_ERROR,       A_EXIT},  // NIL
					{S_ERROR,       A_EXIT},  // CTRL
					{S_ERROR,       A_EXIT},  // OTHER

					{S_ERROR,       A_EXIT},  // SPACE
					{S_ERROR,       A_EXIT},  // CR
					{S_ERROR,       A_EXIT},  // LF
					{S_ERROR,       A_EXIT},  // BACKSLASH
					{S_ERROR,       A_EXIT},  // HASH

					{S_ERROR,       A_EXIT},  // KEYWORD

					{S_ERROR,       A_EXIT},  // EXPONENT
					{S_ERROR,       A_EXIT},  // TWO

					{S_ERROR,       A_EXIT},  // NUMBER
					{S_ERROR,       A_EXIT},  // POINT
					{S_ERROR,       A_EXIT},  // PLUS
					{S_ERROR,       A_EXIT},  // MINUS
					{S_ERROR,       A_EXIT},  // SLASH
				},

				/* COMMENT */ {
					{S_END,         A_EXIT},  // NIL
					{S_ERROR,       A_EXIT, "ctrl found in comment"},  // CTRL
					{S_COMMENT,     A_SKIP},  // OTHER

					{S_COMMENT,     A_SKIP},  // SPACE
					{S_RETURN,      A_SKIP},  // CR
					{S_START,       A_SKIP},  // LF
					{S_COMMENT,     A_SKIP},  // BACKSLASH
					{S_COMMENT,     A_SKIP},  // HASH

					{S_COMMENT,     A_SKIP},  // KEYWORD

					{S_COMMENT,     A_SKIP},  // EXPONENT
					{S_COMMENT,     A_SKIP},  // TWO

					{S_COMMENT,     A_SKIP},  // NUMBER
					{S_COMMENT,     A_SKIP},  // POINT
					{S_COMMENT,     A_SKIP},  // PLUS
					{S_COMMENT,     A_SKIP},  // MINUS
					{S_COMMENT,     A_SKIP},  // SLASH
				},

				/* RETURN */ {
					{S_ERROR,       A_EXIT, "expect newline after return"},  // NIL
					{S_ERROR,       A_EXIT, "expect newline after return"},  // CTRL
					{S_ERROR,       A_EXIT, "expect newline after return"},  // OTHER

					{S_ERROR,       A_EXIT, "expect newline after return"},  // SPACE
					{S_ERROR,       A_EXIT, "expect newline after return"},  // CR
					{S_START,       A_SKIP},  // LF
					{S_ERROR,       A_EXIT, "expect newline after return"},  // BACKSLASH
					{S_ERROR,       A_EXIT, "expect newline after return"},  // HASH

					{S_ERROR,       A_EXIT, "expect newline after return"},  // KEYWORD

					{S_ERROR,       A_EXIT, "expect newline after return"},  // EXPONENT
					{S_ERROR,       A_EXIT, "expect newline after return"},  // TWO

					{S_ERROR,       A_EXIT, "expect newline after return"},  // NUMBER
					{S_ERROR,       A_EXIT, "expect newline after return"},  // POINT
					{S_ERROR,       A_EXIT, "expect newline after return"},  // PLUS
					{S_ERROR,       A_EXIT, "expect newline after return"},  // MINUS
					{S_ERROR,       A_EXIT, "expect newline after return"},  // SLASH
				},

				/* ESCAPE */ {
					{S_END,         A_EXIT},  // NIL
					{S_ERROR,       A_EXIT},  // CTRL
					{S_ERROR,       A_EXIT},  // OTHER

					{S_ESCAPE,      A_SKIP},  // SPACE (stay in escape state)
					{S_RETURN,      A_SKIP},  // CR (wait LF)
					{S_START,       A_SKIP},  // LF (issue, need to keep last state)!!!
					{S_ERROR,       A_EXIT},  // BACKSLASH
					{S_ERROR,       A_EXIT},  // HASH

					{S_ERROR,       A_EXIT},  // KEYWORD

					{S_ERROR,       A_EXIT},  // EXPONENT
					{S_ERROR,       A_EXIT},  // TWO

					{S_ERROR,       A_EXIT},  // NUMBER
					{S_ERROR,       A_EXIT},  // POINT
					{S_ERROR,       A_EXIT},  // PLUS
					{S_ERROR,       A_EXIT},  // MINUS
					{S_ERROR,       A_EXIT},  // SLASH

				},

				/* KEYWORD */ {
					{S_ERROR,       A_EXIT, "expect values after keyword"},           // NIL
					{S_ERROR,       A_EXIT, "ctrl in keyword"},           // CTRL
					{S_ERROR,       A_EXIT, "invalid keyword"},           // OTHER

					{S_AFTER_KEYWORD, A_PARSE_KEYWORD},  // SPACE
					{S_ERROR,       A_EXIT, "expect values after keyword"},           // CR
					{S_ERROR,       A_EXIT, "expect values after keyword"},           // LF
					{S_ERROR,       A_EXIT, "unknown error"},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT, "comment must begin at start of line"},           // HASH

					{S_KEYWORD,     A_COUNT},          // KEYWORD  keyword

					{S_KEYWORD,     A_COUNT},          // EXPONENT is in keyword char
					{S_KEYWORD,     A_COUNT},          // TWO      is in keyword char

					{S_ERROR,       A_EXIT, "invalid char in keyword"},           // NUMBER
					{S_ERROR,       A_EXIT, "invalid char in keyword"},           // POINT
					{S_ERROR,       A_EXIT, "invalid char in keyword"},           // PLUS
					{S_ERROR,       A_EXIT, "invalid char in keyword"},           // MINUS
					{S_ERROR,       A_EXIT, "invalid char in keyword"},           // SLASH
				},

				/* AFTER KEYWORD */ {
					{S_ERROR,       A_EXIT, "expect values after keyword"},           // NIL
					{S_ERROR,       A_EXIT, "ctrl after keyword"},           // CTRL
					{S_ERROR,       A_EXIT, "invalid char, expect number"},           // OTHER

					{S_AFTER_KEYWORD, A_SKIP},         // SPACE
					{S_ERROR,       A_EXIT, "expect values after keyword"},           // CR
					{S_ERROR,       A_EXIT, "expect values after keyword"},           // LF
					{S_ERROR,       A_EXIT, "unknown"},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT, "comment must begin at start of line"},           // HASH

					{S_ERROR,       A_EXIT, "expect number after keyword"},           // KEYWORD

					{S_ERROR,       A_EXIT, "expect number after keyword"},           // EXPONENT
					{S_INTEGER,     A_INTEGER},        // TWO

					{S_INTEGER,     A_INTEGER},        // NUMBER
					{S_DECIMAL,     A_SKIP},           // POINT (enter decimal mode)
					{S_PLUS,        A_PLUS},           // PLUS
					{S_MINUS,       A_MINUS},          // MINUS
					{S_ERROR,       A_EXIT, "expect number after keyword"},           // SLASH
				},

				/* BETWEEN INDEXES */ {
					{S_END,         A_EXIT},           // NIL
					{S_ERROR,       A_EXIT},           // CTRL
					{S_ERROR,       A_EXIT},           // OTHER

					{S_BETWEEN_INDEXES, A_SKIP},       // SPACE
					{S_RETURN,      A_SKIP},           // CR
					{S_START,       A_SKIP},           // LF
					{S_ERROR,       A_EXIT},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT},           // HASH

					{S_ERROR,       A_EXIT},           // KEYWORD

					{S_ERROR,       A_EXIT},           // EXPONENT
					{S_INDEX,       A_INTEGER},        // TWO

					{S_INDEX,       A_INTEGER},        // NUMBER
					{S_ERROR,       A_EXIT},           // POINT (enter decimal mode)
					{S_ERROR,       A_EXIT},           // PLUS
					{S_ERROR,       A_EXIT},           // MINUS
					{S_ERROR,       A_EXIT},           // SLASH
				},

				/* INDEX */ {
					{S_END,         A_EXIT},           // NIL
					{S_ERROR,       A_EXIT},           // CTRL
					{S_ERROR,       A_EXIT},           // OTHER

					{S_BETWEEN_INDEXES, A_SKIP},       // SPACE
					{S_RETURN,      A_SKIP},           // CR
					{S_START,       A_SKIP},           // LF
					{S_ERROR,       A_EXIT},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT},           // HASH

					{S_ERROR,       A_EXIT},           // KEYWORD

					{S_ERROR,       A_EXIT},           // EXPONENT
					{S_INDEX,       A_INTEGER},        // TWO

					{S_INDEX,       A_INTEGER},        // NUMBER
					{S_ERROR,       A_EXIT},           // POINT (enter decimal mode)
					{S_ERROR,       A_EXIT},           // PLUS
					{S_ERROR,       A_EXIT},           // MINUS
					{S_ERROR,       A_EXIT},           // SLASH
				},

				/* PLUS */ {
					{S_ERROR,       A_EXIT},           // NIL
					{S_ERROR,       A_EXIT},           // CTRL
					{S_ERROR,       A_EXIT},           // OTHER

					{S_ERROR,       A_EXIT},           // SPACE
					{S_ERROR,       A_EXIT},           // CR
					{S_ERROR,       A_EXIT},           // LF
					{S_ERROR,       A_EXIT},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT},           // HASH

					{S_ERROR,       A_EXIT},           // KEYWORD

					{S_ERROR,       A_EXIT},           // EXPONENT // need test
					{S_INTEGER,     A_INTEGER},        // TWO

					{S_INTEGER,     A_INTEGER},        // NUMBER
					{S_DECIMAL,     A_SKIP},           // POINT (enter decimal mode)
					{S_ERROR,       A_EXIT},           // PLUS
					{S_ERROR,       A_EXIT},           // MINUS
					{S_ERROR,       A_EXIT},           // SLASH
				},

				/* MINUS */ {
					{S_ERROR,       A_EXIT},           // NIL
					{S_ERROR,       A_EXIT},           // CTRL
					{S_ERROR,       A_EXIT},           // OTHER

					{S_ERROR,       A_EXIT},           // SPACE
					{S_ERROR,       A_EXIT},           // CR
					{S_ERROR,       A_EXIT},           // LF
					{S_ERROR,       A_EXIT},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT},           // HASH

					{S_ERROR,       A_EXIT},           // KEYWORD

					{S_ERROR,       A_EXIT},           // EXPONENT // need test
					{S_INTEGER,     A_INTEGER},        // TWO

					{S_INTEGER,     A_INTEGER},        // NUMBER
					{S_DECIMAL,     A_SKIP},           // POINT (enter decimal mode)
					{S_ERROR,       A_EXIT},           // PLUS
					{S_ERROR,       A_EXIT},           // MINUS
					{S_ERROR,       A_EXIT},           // SLASH
				},


				/* INTEGER */ {
					{S_END,         A_EXIT},           // NIL // maybe check number of float parsed
					{S_ERROR,       A_EXIT},           // CTRL
					{S_ERROR,       A_EXIT},           // OTHER

					{S_AFTER_KEYWORD, A_PARSE_FLOAT},    // SPACE // need create intermediar state between number
					{S_RETURN,      A_PARSE_FLOAT},    // CR
					{S_START,       A_PARSE_FLOAT},    // LF
					{S_ERROR,       A_EXIT},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT},           // HASH // do you validate comment at end of line ?

					{S_ERROR,       A_EXIT},           // KEYWORD

					{S_ERROR,       A_EXIT},           // EXPONENT // not implemented yet
					{S_INTEGER,     A_INTEGER},        // TWO

					{S_INTEGER,     A_INTEGER},        // NUMBER
					{S_DECIMAL,     A_SKIP},           // POINT (enter decimal mode)
					{S_ERROR,       A_EXIT},           // PLUS
					{S_ERROR,       A_EXIT},           // MINUS
					{S_ERROR,       A_EXIT},           // SLASH
				},

				/* DECIMAL */ {
					{S_END,         A_EXIT},           // NIL // maybe check number of float parsed
					{S_ERROR,       A_EXIT},           // CTRL
					{S_ERROR,       A_EXIT},           // OTHER

					{S_AFTER_KEYWORD, A_PARSE_FLOAT},    // SPACE // need create intermediar state between number
					{S_RETURN,      A_PARSE_FLOAT},    // CR
					{S_START,       A_PARSE_FLOAT},    // LF
					{S_ERROR,       A_EXIT},           // BACKSLASH // need wait LF
					{S_ERROR,       A_EXIT},           // HASH // do you validate comment at end of line ?

					{S_ERROR,       A_EXIT},           // KEYWORD

					{S_ERROR,       A_EXIT},           // EXPONENT // not implemented yet
					{S_DECIMAL,     A_DECIMAL},        // TWO

					{S_DECIMAL,     A_DECIMAL},        // NUMBER
					{S_ERROR,       A_EXIT},           // POINT (no second point)
					{S_ERROR,       A_EXIT},           // PLUS
					{S_ERROR,       A_EXIT},           // MINUS
					{S_ERROR,       A_EXIT},           // SLASH
				},
			};



			// -- static members ----------------------------------------------

			/* chartypes */
			static constexpr chartype _chartypes[256] {
				NIL,
				//[ 1 ... 3 ] = CTRL,
				CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
				SPACE,
				LF,
				CTRL, CTRL,
				CR,
				CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
				CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
				CTRL, CTRL,
				SPACE,
				OTHER, OTHER,
				HASH,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				PLUS,
				OTHER,
				MINUS,
				POINT,
				SLASH,
				NUMBER, NUMBER,
				TWO,
				NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER,
				EXPONENT,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				BACKSLASH,
				OTHER, OTHER,
				KEYWORD,
				OTHER,
				KEYWORD, KEYWORD, KEYWORD, KEYWORD,
				EXPONENT,
				KEYWORD, KEYWORD, KEYWORD, KEYWORD, KEYWORD,
				OTHER,
				KEYWORD, KEYWORD, KEYWORD, KEYWORD, KEYWORD,
				OTHER,
				KEYWORD, KEYWORD, KEYWORD, KEYWORD, KEYWORD, KEYWORD,
				OTHER,
				KEYWORD,
				OTHER, OTHER, OTHER, OTHER, OTHER,
				CTRL,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
			};



			// -- P E R F E C T  H A S H  T A B L E ---------------------------

			/* keyword map */
			class keyword_map final {

				private:

					/* constants */
					enum : size_type {
						MAX_LENGTH = 10,
						MAX_HASH   = 72
					};

					/* assumes 7-bit ASCII */
					static constexpr unsigned char _table[] {
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
						73, 73, 73, 73, 73,  0, 73, 10,  5, 10,
						 0,  0, 45, 40, 15, 73, 73, 73, 30, 30,
						 5, 25, 20, 73, 20,  0,  0,  5,  5, 73,
						73, 73, 73, 73, 73, 73, 73, 73
					};

					struct entry final {

						inline constexpr entry(void) noexcept
						: _key{nullptr}, _type{K_INVALID} {}

						inline constexpr entry(const char* const key, const keyword_type type) noexcept
						: _key{key}, _type{type} {}

						non_assignable(entry);

						const char* const _key;
						const keyword_type _type;
					};

					/* keywords */
					static inline /*constexpr*/ entry _entries[] = {
						{},
						{"s",          K_NOT_IMPLEMENTED},
						{},
						{"deg",        K_NOT_IMPLEMENTED},
						{"step",       K_NOT_IMPLEMENTED},
						{"stech",      K_NOT_IMPLEMENTED},
						{"v",          K_V},  // vertex
						{"vt",         K_VT}, // texture
						{"end",        K_NOT_IMPLEMENTED},
						{"surf",       K_NOT_IMPLEMENTED},
						{"bevel",      K_NOT_IMPLEMENTED},
						{"usemtl",     K_NOT_IMPLEMENTED},
						{"vn",         K_VN}, // normal
						{},
						{"scrv",       K_NOT_IMPLEMENTED},
						{"ctech",      K_NOT_IMPLEMENTED},
						{"cstype",     K_NOT_IMPLEMENTED},
						{},
						{"c_interp",   K_NOT_IMPLEMENTED},
						{"curv",       K_NOT_IMPLEMENTED},
						{"curv2",      K_NOT_IMPLEMENTED},
						{"p",          K_NOT_IMPLEMENTED},
						{"sp",         K_NOT_IMPLEMENTED},
						{},
						{"trim",       K_NOT_IMPLEMENTED},
						{"shadow_obj", K_NOT_IMPLEMENTED},
						{"o",          K_NOT_IMPLEMENTED},
						{"vp",         K_NOT_IMPLEMENTED},
						{},
						{"trace_obj",  K_NOT_IMPLEMENTED},
						{},
						{"l",          K_NOT_IMPLEMENTED},
						{}, {},
						{"parm",       K_NOT_IMPLEMENTED},
						{},
						{"mtllib",     K_NOT_IMPLEMENTED},
						{},
						{"con",        K_NOT_IMPLEMENTED},
						{"bmat",       K_NOT_IMPLEMENTED},
						{},
						{"g",          K_NOT_IMPLEMENTED},
						{}, {},
						{"hole",       K_NOT_IMPLEMENTED},
						{},
						{"f",          K_F},
						{}, {}, {}, {}, {}, {}, {}, {},
						{}, {}, {},
						{"lod",        K_NOT_IMPLEMENTED},
						{}, {}, {}, {}, {}, {}, {}, {},
						{}, {}, {}, {}, {},
						{"mg",         K_NOT_IMPLEMENTED},
					};


					// -- private members -------------------------------------

					/* buffer */
					char _buffer[MAX_LENGTH + 1];

					/* index */
					size_type _i;


				public:

					/* default constructor */
					inline constexpr keyword_map(void) noexcept
					: _buffer{}, _i{0} {}

					/* non-assignable class */
					non_assignable(keyword_map);

					/* destructor */
					inline ~keyword_map(void) noexcept = default;


					/* add char */
					inline constexpr auto add(const char c) noexcept -> void {
						_buffer[_i % MAX_LENGTH] = c;
						++_i;
					}


					/* get keyword type */
					//static auto type(const char*, const size_type) noexcept -> keyword_type;
					inline auto type(void) noexcept -> keyword_type {

						if (_i > MAX_LENGTH) //|| len < MIN_LENGTH)
							return K_INVALID;

						const unsigned int key = _i + ((_i == 1)
								? _table[static_cast<unsigned char>(_buffer[0])]
								: _table[static_cast<unsigned char>(_buffer[1])]
								+ _table[static_cast<unsigned char>(_buffer[0])]);

						if (key > MAX_HASH)
							return K_INVALID;

						const entry& entry = _entries[key];

						if (entry._key == nullptr)
							return K_INVALID;

						// manual strncmp (no nullptr check)
						size_type i = 0;
						while (i < _i) { // WARNING: maybe segfault !!!
							if (_buffer[i] != entry._key[i])
								return K_INVALID;
							++i;
						}
						// reset index
						_i = 0;
						return entry._key[i] == '\0' ? entry._type : K_INVALID;
					}

			};



			// -- private members ---------------------------------------------

			/* descriptor */
			int _fd;

			/* path */
			const char* _path;

			/* buffer */
			char _buffer[BUFFER_SIZE];

			/* current state */
			const transition* _tr;

			/* index */
			size_type _i;

			/* keyword map */
			keyword_map _kmap;

			/* integral */
			xns::umax _integral;

			/* decimal */
			xns::umax _decimal;

			/* sign */
			int _sign;

			/* mesh */
			data* _data;

			/* data type */
			data::data_kind _dtype;

	};
}





// -- D E V  T O O L S --------------------------------------------------------

//inline void keyword_chars(void){
//bool tab[256]={false};unsigned int i=0,j=0;
//while(engine::wavefront_parser::keywords()[i]!=nullptr){j=0;
//while(engine::wavefront_parser::keywords()[i][j]!='\0'){
//tab[(unsigned char)engine::wavefront_parser::keywords()[i][j]]=true;++j;}
//++i;}i=0;while(i<256){if(tab[i])std::cout<<(char)i<<" ";++i;}}
//
//
//inline void print_chartype(void){
//
//	unsigned int i = 0;
//
//	while (i < 256) {
//
//		// NIL 0
//		if (i == 0)
//			std::cout << "NIL,\n";
//
//		// CTRL 1 - 31, 127 (except '\r', '\n', '\t)
//		else if ((i > 0 && i < 32 && i != '\r' && i != '\n' && i != '\t') || (i == 127))
//			std::cout << "CTRL,\n";
//
//		// SPACE ' ', '\t'
//		else if (i == ' ' || i == '\t')
//			std::cout << "SPACE,\n";
//
//		// CR
//		else if (i == '\r')
//			std::cout << "CR,\n";
//
//		// LF
//		else if (i == '\n')
//			std::cout << "LF,\n";
//
//		// POINT '.'
//		else if (i == '.')
//			std::cout << "POINT,\n";
//
//		// TWO '2'
//		else if (i == '2')
//			std::cout << "TWO,\n";
//
//		// HASH '#'
//		else if (i == '#')
//			std::cout << "HASH,\n";
//
//		// EXPONENT 'e' 'E'
//		else if (i == 'e' || i == 'E')
//			std::cout << "EXPONENT,\n";
//
//		// BACKSLASH '\'
//		else if (i == '\\')
//			std::cout << "BACKSLASH,\n";
//
//		// SLASH '/'
//		else if (i == '/')
//			std::cout << "SLASH,\n";
//
//		// PLUS '+'
//		else if (i == '+')
//			std::cout << "PLUS,\n";
//
//		// MINUS '-'
//		else if (i == '-')
//			std::cout << "MINUS,\n";
//
//		// NUMBER, // '0' - '9' (except '2')
//		else if (i >= '0' && i <= '9' && i != '2')
//			std::cout << "NUMBER,\n";
//
//		// KEYWORD _abcdfghijlmnoprstuvwy (except '2' and 'e')
//		else if ([&]() -> bool {
//				const char* k = "_abcdfghijlmnoprstuvwy";
//				unsigned int c = 0;
//				while (k[c]) {
//					if (i == (unsigned)k[c])
//						return true;
//					++c;
//				}
//				return false;
//				}()) {
//			std::cout << "KEYWORD,\n";
//		}
//		// OTHER ...
//		else
//			std::cout << "OTHER,\n";
//
//		++i;
//	}
//}



template <typename T>
inline auto to_decimal(char*& str, T& num) -> int {

	static_assert(xns::is_floating<T>, "T must be a floating point type");
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


#endif // ENGINE_MODEL_LOADER_HPP
