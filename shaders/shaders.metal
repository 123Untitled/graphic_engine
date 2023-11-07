#include <metal_stdlib>
#include <simd/simd.h>


struct vertex_in {
	metal::float3 position [[ attribute(0) ]];
	metal::float3 color    [[ attribute(1) ]];
	metal::float3 normal   [[ attribute(2) ]];
};

struct vertex_out {
	float4 position [[ position ]];
	float3 color;
};

/* vertex shader */
auto vertex vertex_main(const vertex_in           vertice    [[ stage_in  ]],
						constant metal::float4x4& projection [[ buffer(1) ]],
						constant metal::float4x4& view       [[ buffer(2) ]]) -> vertex_out {

	float4 view_position = view * float4(vertice.position, 1.0);
	float4 projection_position = projection * view_position;

	vertex_out out;
	out.position = projection_position;
	out.color = vertice.color;
	return out;
}

/* fragment shader */
auto fragment fragment_main(const vertex_out frag [[stage_in]]) -> float4 {
	return float4(frag.color, 1.0);
}


//#include <metal_stdlib>
//#include "loki.metal"
//
//using namespace metal;
//
//struct VertexIn {
//	float3 position [[ attribute(0) ]];
//	float3 color    [[ attribute(1) ]];
//	float3 normal	[[ attribute(2) ]];
//};
//
//struct VertexOut {
//	float4 position [[ position ]];
//	float3 color;
//	float3 modelPosition;
//	float3 viewPosition;
//	float3 viewDirection;
//	float3 normalSurface;
//	float depth;
//	float psize [[ point_size ]];
//};
//
//struct TypeMatrix {
//	metal::float4x4 matrix;
//};
//
//struct Material {
//	float4 color;
//	bool active;
//	bool specular;
//	bool depthFade;
//};
//
//
//
//vertex VertexOut vertex_main(	const			VertexIn		vertice			[[ stage_in ]],
//								constant		TypeMatrix&		projectMatrix	[[ buffer(1) ]],
//								constant		TypeMatrix&		viewMatrix		[[ buffer(2) ]],
//								constant		TypeMatrix&		modelMatrix		[[ buffer(3) ]],
//								constant		float3&			cameraPosition	[[ buffer(4) ]]) {
//
//
//	const float4 modelPosition	= modelMatrix.matrix * float4(vertice.position, 1.0f);
//	const float4 viewPosition	= viewMatrix.matrix * modelPosition;
//
//	// need maybe orientation camera
//	const float3 viewDirection = normalize(cameraPosition - modelPosition.xyz);
//
//	//const float3 normalSurface = metal::normalize((modelMatrix.matrix * float4(vertice.normal, 0.0)).xyz);
//	//// NEED A NORMAL MATRIX : transpose inverse of 3x3 model matrix
//	const float3 normalSurface = (modelMatrix.matrix * float4(vertice.normal, 0.0)).xyz;
//	//////////////////////////////////////////////////////////////////// - !!!!
//
//
//
//	float psize = 2.0;
//
//
//	// Calculate the distance from the camera to the object
//	const float depth = metal::distance((modelPosition).xyz, cameraPosition);
//
//	VertexOut out = {
//		//projectMatrix.matrix * viewMatrix.matrix * modelMatrix.matrix * float4(vertice.position, 1.0f),
//		//projectMatrix.matrix * viewMatrix.matrix * worldPosition,
//		projectMatrix.matrix * viewPosition,
//		vertice.color,
//		modelPosition.xyz,
//		viewPosition.xyz,
//		viewDirection,
//		normalSurface,
//		depth,
//		psize
//	};
//
//
//	return (out);
//}
//
//
//inline float getDepthOpacity(const float depth) {
//	const float minDistance = 15.0;
//	const float maxDistance = 25.0;
//    // Normalize the distance value between 0 and 1
//    const float normalizedDistance = (depth - minDistance) / (maxDistance - minDistance);
//    // Calculate the grey value based on the normalized distance
//    return ((1.0 - normalizedDistance) * 0.5);
//}
//
//
//fragment float4 fragment_main(VertexOut frag [[ stage_in ]], constant Material& material [[ buffer(1) ]], constant int& iteration [[ buffer(2) ]]) {
//
//
//
//	Loki rng = Loki(frag.position.x, frag.position.y, iteration);
//	// Use dithering to create a grainy, hand-drawn effect
//	float3 dither = { rng.rand(), rng.rand(), rng.rand() }; dither *= 0.4;
//
//
//
//	const float3 ambient = { 0.6, 0.6, 0.6 };
//	const float3 lightDirection = { 0.1, 0.3, -1.0 };
//	const float3 lightColor = { 0.6, 0.6, 0.6 };
//
//
//	const float3 normal = normalize(frag.normalSurface);
//
//	float NdotL = dot(lightDirection, normal);
//	float lightIntensity = smoothstep(0, 1.5, NdotL);
//	//float lightIntensity = max(NdotL, 0.0);
//    float3 diffuse = lightIntensity * lightColor;
//
//	float specular = 0.0;
//
//	if (material.specular) {
//		float3 reflectDir = reflect(-lightDirection, normal);
//		specular = pow(max(dot(frag.viewDirection, reflectDir), 0.0), 16);
//	}
//
//	const float alpha = material.depthFade ? getDepthOpacity(frag.depth) : material.color.w;
//
//
//	// pixel color
//	return float4(
//			// get object color
//			material.color.xyz *
//			// light shading
//			((ambient + diffuse + specular)
//			// grain noise
//			 - dither),
//			// opacity distance
//			alpha
//	);
//
//}
