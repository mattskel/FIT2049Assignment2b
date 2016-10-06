/*	FIT2049 - Example Code
*	Texture.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class stores the required information for a single texture.
*	It is also used to load textures from a number of different file formats
*	It uses the functions from the DirectX Tool Kit to achieve this.
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include "Direct3D.h"

#include <map>

class Texture
{
private:
	Texture(LPCWSTR filename, const char* identifier, ID3D11Resource* texture, ID3D11ShaderResourceView* textureView);

	LPCWSTR m_filename;	

	// Not using filename to identify a mesh so we can easily rename and not have to change lots in our code
	// Note these must be unique as we use them as keys in our map
	const char* m_identifier;

	ID3D11Resource* m_texture;					//The texture resource, can be used for procedurally modifying the texture
	ID3D11ShaderResourceView* m_textureView;	//The texture view, this is passed into our Shaders and the Sprite Batch

	// Static manager helpers
	// We keep track of every texture created so we can fetch textures anywhere in our game
	typedef std::map<std::string, Texture*> TextureMap;
	static TextureMap textureMap;
	
public:
	
	~Texture();	//Destructor

	//Accessors
	ID3D11Resource* GetTexture() { return m_texture; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureView; }
	LPCWSTR GetFilename() { return m_filename; }
	const char* GetIdentifier() { return m_identifier; }

	// Static manager helpers
	static bool LoadFromFile(LPCWSTR filename, const char* identifier, Direct3D* direct3D);
	static Texture* GetTexture(const char* identifier);
	static void ReleaseTexture(Texture* texture);
	static void ReleaseAllTextures();

};

#endif