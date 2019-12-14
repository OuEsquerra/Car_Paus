#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

	Cylinder bowl1(2, 5);
	bowl1.SetPos(0, 0, 0);
	bowl1.SetRotation(90,vec3(1,0,0));
	App->physics->AddBody(bowl1, 10);
	primitives.PushBack(&bowl1);



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 2, 0, 0);
	p.axis = true;
	p.color.r = 255;
	p.Render();
	
	//App->physics.AddBody(new Cylinder, 10.0f);

	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

