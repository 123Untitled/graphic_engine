#include <metal_stdlib>
#include <simd/simd.h>
#include "loki.metal"

using namespace metal;

struct vertex_in {
	metal::float3 position [[ attribute(0) ]];
	metal::float3 normal   [[ attribute(1) ]];
	metal::float2 texcoord [[ attribute(2) ]];
};

struct vertex_out {
	float4 position [[ position ]];
	float3 model_position;
	float3 view_position;
	float3 view_direction;
	float3 normal_surface;
};


/* vertex shader */
auto vertex vertex_main(const vertex_in           vertice    [[ stage_in  ]],
						constant metal::float4x4& projection [[ buffer(1) ]],
						constant metal::float4x4& view       [[ buffer(2) ]],
						constant metal::float4x4& model      [[ buffer(3) ]],
						constant metal::float3&   cam_pos    [[ buffer(4) ]]) -> vertex_out {


	const float4 model_position = model * metal::float4(vertice.position, 1.0);
	const float4 view_position = view * model_position;

	// need maybe orientation camera
	const float3 view_direction = metal::normalize(cam_pos - model_position.xyz);

	const float3 normal_surface = (model * metal::float4(vertice.normal, 0.0)).xyz;


	return vertex_out{
		projection * view_position,
		model_position.xyz,
		view_position.xyz,
		view_direction,
		normal_surface,
	};
}


/* fragment shader */
//auto fragment fragment_main(const vertex_out frag [[stage_in]],
//							constant unsigned int& iteration [[ buffer(1) ]]) -> float4 {
//
//	const float  amb = 0.6f;
//	const float3 ambient        = { amb, amb, amb };
//	const float3 light_direction = { +0.1, +0.3, -1.0 };
//	const float3 light_color     = { +0.6, +0.6, +0.6 };
//
//	Loki rng = Loki(frag.position.x, frag.position.y, iteration);
//	auto d = rng.rand() * 0.4;
//	metal::float3 dither = { d, d, d };
//
//
//	const float3 normal = metal::normalize(frag.normal_surface);
//
//	float NdotL = dot(light_direction, normal);
//	float light_intensity = metal::smoothstep(0, 0.5, NdotL);
//    float3 diffuse = light_intensity * light_color;
//
//
//	float3 half_vector = normalize(light_direction + frag.view_direction);
//	float NdotH = dot(normal, half_vector);
//	float specular_intensity = pow(NdotH * light_intensity, 6 * 6);
//	float specular_intensity_smooth = metal::smoothstep(0.005, 0.21, specular_intensity);
//	float3 specular = specular_intensity_smooth;
//
//
//	//float3 reflect_dir = metal::reflect(-light_direction, normal);
//	//float specular = pow(max(dot(frag.view_direction, reflect_dir), 0.0), 16);
//
//
//	auto color = float4(0.2, 0.8, 0.8, 1.0);
//
//	// pixel color
//	return float4(color.xyz * ((ambient + diffuse + specular) - dither), 1.0);
//}

/* fragment shader */
auto fragment fragment_main(const vertex_out frag [[stage_in]],
							constant metal::float4& color [[ buffer(1) ]],
							constant unsigned int& iteration [[ buffer(2) ]]) -> float4 {

	const float  amb = 0.6f;
	const float3 ambient        = { amb, amb, amb };
	const float3 light_direction = { +0.1, +0.3, -1.0 };
	const float3 light_color     = { +0.6, +0.6, +0.6 };

	Loki rng = Loki(frag.position.x, frag.position.y, iteration);
	auto d = rng.rand() * 0.6;
	metal::float3 dither = { d, d, d };


	const float3 normal = metal::normalize(frag.normal_surface);

	float NdotL = dot(light_direction, normal);
	float light_intensity = metal::smoothstep(0, 0.2, NdotL);
    float3 diffuse = light_intensity * light_color;


	float3 reflect_dir = metal::reflect(-light_direction, normal);
	float specular = pow(max(dot(frag.view_direction, reflect_dir), 0.0), 5);


	//auto color = float4(0.2, 0.8, 0.8, 1.0);


	   // Calculer la luminosité totale
    float3 total_light = ambient + diffuse + specular;
    float brightness = metal::dot(total_light, float3(0.2126, 0.7152, 0.0722)); // Luminosité perceptuelle

    // Ajuster l'intensité du bruit en fonction de la luminosité
    float noise_factor = brightness; // Plus lumineux = plus de bruit
    metal::float3 adjusted_dither = dither * noise_factor;

    // pixel color
    return float4(color.xyz * (total_light - adjusted_dither), 1.0);

	// pixel color
	//return float4(color.xyz * ((ambient + diffuse + specular) - dither), 1.0);
}





///////////////////////////////////////////////////////////////////////////////


struct VertexIn {
	float3 position [[ attribute(0) ]];
	float3 color    [[ attribute(1) ]];
	float3 normal	[[ attribute(2) ]];
};

struct VertexOut {
	float4 position [[ position ]];
	float3 color;
	float3 modelPosition;
	float3 viewPosition;
	float3 viewDirection;
	float3 normalSurface;
	float depth;
	float psize [[ point_size ]];
};

struct TypeMatrix {
	metal::float4x4 matrix;
};


struct Material {
	float4 color;
	bool active;
	bool specular;
	bool depthFade;
};


vertex VertexOut vertex_main2(	const			VertexIn		vertice			[[ stage_in ]],
								constant		TypeMatrix&		projectMatrix	[[ buffer(1) ]],
								constant		TypeMatrix&		viewMatrix		[[ buffer(2) ]],
								constant		TypeMatrix&		modelMatrix		[[ buffer(3) ]],
								constant		float3&			cameraPosition	[[ buffer(4) ]]) {


	const float4 modelPosition	= modelMatrix.matrix * float4(vertice.position, 1.0f);
	const float4 viewPosition	= viewMatrix.matrix * modelPosition;

	// need maybe orientation camera
	const float3 viewDirection = normalize(cameraPosition - modelPosition.xyz);

	//const float3 normalSurface = metal::normalize((modelMatrix.matrix * float4(vertice.normal, 0.0)).xyz);
	//// NEED A NORMAL MATRIX : transpose inverse of 3x3 model matrix
	const float3 normalSurface = (modelMatrix.matrix * float4(vertice.normal, 0.0)).xyz;
	//////////////////////////////////////////////////////////////////// - !!!!



	float psize = 2.0;


	// Calculate the distance from the camera to the object
	const float depth = metal::distance((modelPosition).xyz, cameraPosition);

	VertexOut out = {
		//projectMatrix.matrix * viewMatrix.matrix * modelMatrix.matrix * float4(vertice.position, 1.0f),
		//projectMatrix.matrix * viewMatrix.matrix * worldPosition,
		projectMatrix.matrix * viewPosition,
		vertice.color,
		modelPosition.xyz,
		viewPosition.xyz,
		viewDirection,
		normalSurface,
		depth,
		psize
	};


	return (out);
}


inline float getDepthOpacity(const float depth) {
	const float minDistance = 15.0;
	const float maxDistance = 25.0;
    // Normalize the distance value between 0 and 1
    const float normalizedDistance = (depth - minDistance) / (maxDistance - minDistance);
    // Calculate the grey value based on the normalized distance
    return ((1.0 - normalizedDistance) * 0.5);
}


fragment float4 fragment_main2(VertexOut frag [[ stage_in ]], constant Material& material [[ buffer(1) ]], constant int& iteration [[ buffer(2) ]]) {



	Loki rng = Loki(frag.position.x, frag.position.y, iteration);
	// Use dithering to create a grainy, hand-drawn effect
	metal::float3 dither = { rng.rand(), rng.rand(), rng.rand() };
				  dither *= 0.4;


	const float3 ambient        = { +0.6, +0.6, +0.6 };
	const float3 lightDirection = { +0.1, +0.3, -1.0 };
	const float3 lightColor     = { +0.6, +0.6, +0.6 };


	const float3 normal = normalize(frag.normalSurface);

	float NdotL = dot(lightDirection, normal);
	float lightIntensity = smoothstep(0, 1.5, NdotL);
	//float lightIntensity = max(NdotL, 0.0);
    float3 diffuse = lightIntensity * lightColor;

	float specular = 0.0;

	if (material.specular) {
		float3 reflectDir = reflect(-lightDirection, normal);
		specular = pow(max(dot(frag.viewDirection, reflectDir), 0.0), 16);
	}

	const float alpha = material.depthFade ? getDepthOpacity(frag.depth) : material.color.w;


	// pixel color
	return float4(
			// get object color
			material.color.xyz *
			// light shading
			((ambient + diffuse + specular)
			// grain noise
			 - dither),
			// opacity distance
			alpha
	);

}
