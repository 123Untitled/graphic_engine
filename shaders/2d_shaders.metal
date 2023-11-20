#include <simd/simd.h>
#include <metal_stdlib>


struct vertex_in {
	metal::float2 position [[ attribute(0) ]];
	//metal::float3 color    [[ attribute(1) ]];
};

struct vertex_out {
	metal::float4 position [[ position ]];
	//float3 color;
};

/* vertex shader */
auto vertex default_vertex_2d(const vertex_in vertice [[ stage_in  ]]) -> vertex_out {
	vertex_out out;
	out.position = float4(vertice.position, 0.0, 1.0);
	//out.color    = vertice.color;
	return out;
}

/* fragment shader */
auto fragment default_fragment_2d(const vertex_out vertice [[ stage_in ]]) -> metal::float4 {
	return float4(1.0, 0.0, 1.0, 1.0);
}
