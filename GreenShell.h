/*
 * The greenshell object cannot change direction unless it hits a wall
 */

#ifndef GREEN_SHELL_H
#define GREEN_SHELL_H

#include "Shell.h"
#include <vector>

class GreenShell : public Shell {
public:
	GreenShell(Mesh* mesh,
		Shader* shader,
		Texture* texture,
		Material* mat,
		Vector3 position,
		Vector3 direction);
	void Update(float timestep);

};

#endif