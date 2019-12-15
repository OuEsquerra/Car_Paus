#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;
class Cube;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 10.0f * DEGTORAD
#define BRAKE_POWER 500.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer( bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	bool able_to_control = true;

	Cube* car_sensor;

	float camera_offset;
};