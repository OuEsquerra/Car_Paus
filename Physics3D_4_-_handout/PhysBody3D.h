#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "glmath.h"

class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class Module;

class Sphere;
class Cube;
class Cylinder;
class Plane;

enum class sensorType
{
	CAR,
	CONTROLS,
	UNKNOWN

};


class Primitive;

class PhysBody3D
{
public:
	PhysBody3D();
	~PhysBody3D();

	void SetBody(Sphere* primitive, float mass,bool sensor=false, sensorType sensor_type = sensorType::UNKNOWN);
	void SetBody(Cube* primitive, float mass, bool sensor = false, sensorType sensor_type = sensorType::UNKNOWN);
	void SetBody(Cylinder* primitive, float mass, bool sensor = false, sensorType sensor_type = sensorType::UNKNOWN);
	bool HasBody() const;
	btRigidBody* GetBody() const;
	sensorType	GetSensorType()const;

	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);

	void SetSpeed(vec3 speed);
	void Push(vec3 force);
	void Stop();

private:
	void SetBody(btCollisionShape* shape, Primitive* parent, float mass, bool sensor = false ,sensorType sensor_type = sensorType::UNKNOWN);

	sensorType sensor_type;
	btRigidBody* body;
	btCollisionShape* colShape;
	btDefaultMotionState* motionState;
public:
	Primitive* parentPrimitive;
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__