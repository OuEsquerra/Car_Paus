#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"

class Primitive;

class  PhysBody3D;
class Cube;
class vec3;
class Cylinder;


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

	void CreateBowls();
	void DeleteBowls();
private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

	Cube* left_hinge;
	Cube* right_hinge;

	
public:

	Cube* CreateCircuitWall(vec3 dimensions, vec3 pos, vec3 rotation, float angle);

	p2DynArray<Primitive*> primitives;

	Cube* controls_sensor;

	Cylinder* bowl1;
	Cylinder* bowl2;
	Cylinder* bowl3;
	Cylinder* bowl4;
	Cylinder* bowl5;
	Cylinder* bowl6;
	Cylinder* bowl7;
	Cylinder* bowl8;
	Cylinder* bowl9;
	Cylinder* bowl10;

	

	float bowl_height = 2.f;

	float bowl_radius = 2.f;
	//p2List<Cylinder*> bowls;

	bool FreeCamera = false;
};
