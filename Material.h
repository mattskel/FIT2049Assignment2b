/*	FIT2049 - Example Code
*	Material.h
*	Created by Mike Yeates - 2016 - Monash University
*	A material wraps up all the settings for a shader.
*	Things like plastic, wood and metal all interact with light differently.
*	These are examples of materials, and the Phong shader can replicate these
*	surfaces providing we've created a material to describe each one.
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

using namespace DirectX::SimpleMath;

// Just a collection of similar data so a struct will do

struct Material
{
	Color emissive;
	//----------------------------------- (16 byte boundary)
	Color ambient;
	//----------------------------------- (16 byte boundary)
	Color diffuse;
	//----------------------------------- (16 byte boundary)
	Color specular;
	//----------------------------------- (16 byte boundary)
	float specularPower;
	// Remember how "useTexture" is a bool in the shader?
	// A bool is only 1 byte in C and 4 in HLSL, so to maintain allignment
	// we need to use an int here
	int useTexture;
	float padding[2];
	//----------------------------------- (16 byte boundary)
};

#endif;