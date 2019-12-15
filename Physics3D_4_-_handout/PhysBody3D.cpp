#include "PhysBody3D.h"
#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ---------------------------------------------------------
PhysBody3D::PhysBody3D()
	: body(nullptr)
	, colShape(nullptr)
	, motionState(nullptr)
	, parentPrimitive(nullptr)
	, collision_listeners()
{
	
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	if (HasBody() == true)
	{
		App->physics-> RemoveBodyFromWorld(body);
		delete body;
		delete colShape;
		delete motionState;
	}
}

sensorType	PhysBody3D::GetSensorType()const
{
	return sensor_type;
}

void PhysBody3D::SetBody(Sphere* primitive, float mass, bool sensor, sensorType sensor_type)
{
	SetBody(new btSphereShape(primitive->GetRadius()),primitive, mass,sensor, sensor_type);
}

void PhysBody3D::SetBody(Cube* primitive, float mass, bool sensor ,sensorType sensor_type)
{
	btVector3 tmp = { primitive->GetSize().x ,primitive->GetSize().y ,primitive->GetSize().z };

	SetBody(new btBoxShape( tmp / 2 ),primitive, mass,sensor, sensor_type);
}

void PhysBody3D::SetBody(Cylinder* primitive, float mass, bool sensor, sensorType sensor_type)
{
	btVector3 tmp = { primitive->GetHeight() / 2, primitive->GetRadius(), 0.0f };

	SetBody(new btCylinderShape(tmp),primitive, mass,sensor, sensor_type);
}

bool PhysBody3D::HasBody() const
{
	return body != nullptr;
}

btRigidBody * PhysBody3D::GetBody() const
{
	return body;
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if (HasBody() == false)
		return;

	body->getWorldTransform().getOpenGLMatrix(matrix);
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if (HasBody() == false)
		return;

	btTransform trans;
	trans.setFromOpenGLMatrix(matrix);
	body->setWorldTransform(trans);
	body->activate();
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	if (HasBody() == false)
		return;

	btTransform trans = body->getWorldTransform();
	trans.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(trans);
	body->activate();
}

void PhysBody3D::SetSpeed(vec3 speed)
{
	Stop();
	Push(speed);
}

void PhysBody3D::Push(vec3 force)
{
	if (HasBody())
	{
		body->activate();
		body->applyCentralForce(btVector3(force.x, force.y, force.z));
	}
}

void PhysBody3D::Stop()
{
	if (HasBody())
		body->clearForces();
}

void PhysBody3D::SetBody(btCollisionShape * shape, Primitive* parent, float mass, bool sensor, sensorType sensor_type)
{
	assert(HasBody() == false);

	parentPrimitive = parent;

	colShape = shape;

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&parent->transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);

	body = new btRigidBody(rbInfo);

	body->setUserPointer(this);
	
	
	


	if (sensor)
	{
		body->setCollisionFlags( btCollisionObject::CF_NO_CONTACT_RESPONSE); //body->getCollisionFlags() |
	}

	if (sensor)
	{
		this->collision_listeners.add(App->player);
	}

	this->sensor_type = sensor_type;

	App->physics->AddBodyToWorld(body);
}
