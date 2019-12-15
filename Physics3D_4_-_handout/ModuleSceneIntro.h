#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"

class Primitive;

class  PhysBody3D;
class Cube;
class vec3;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	bool CleanUp();

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);
	Cube* left_hinge;
	Cube* right_hinge;

public:

	Cube* CreateCircuitWall(vec3 dimensions, vec3 pos, vec3 rotation, float angle);

	p2DynArray<Primitive*> primitives;
	Cube* controls_sensor;
};
