#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	controls_sensor = new Cube(vec3(25, 1, 2),0,true,sensorType::CONTROLS);
	primitives.PushBack(controls_sensor);
	controls_sensor->SetPos(-65.f, 2.f, 0.f);


	CreateCircuitWall(vec3(60.0f, 4.0f, 1.0f), vec3(10.0f, 2.0f, 20.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f);

	CreateCircuitWall(vec3(56.0f, 4.0f, 1.0f), vec3(-10.0f, 2.0f, 18.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f);
	
	CreateCircuitWall(vec3(20.0f, 4.0f, 1.0f), vec3(0.0f, 2.0f, -10.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f);

	CreateCircuitWall(vec3(50.0f, 4.0f, 1.0f), vec3(0.0f, 2.0f, 60.0f), vec3(0.0f, 1.0f, 0.0f), 50.0f);

	CreateCircuitWall(vec3(25.0f, 4.0f, 1.0f), vec3(-18.0f, 2.0f, 55.0f), vec3(0.0f, 1.0f, 0.0f), 50.0f);

	CreateCircuitWall(vec3(60.0f, 4.0f, 1.0f), vec3(-20.0f, 2.0f, 80.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f);

	CreateCircuitWall(vec3(70.0f, 4.0f, 1.0f), vec3(-57.0f, 2.0f, 63.0f), vec3(0.0f, 1.0f, 0.0f), -50.0f);
	
	CreateCircuitWall(vec3(40.0f, 4.0f, 1.0f), vec3(-40.0f, 2.0f, 50.0f), vec3(0.0f, 1.0f, 0.0f), -50.0f);


	Cube* left_wall = CreateCircuitWall(vec3(100.0f, 4.0f, 1.0f), vec3(-53.0f, 2.0f,-14.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f);
	Cube* right_wall =	CreateCircuitWall(vec3(100.0f, 4.0f, 1.0f), vec3(-78.0f, 2.0f, -14.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f);
	
	left_hinge = CreateCircuitWall(vec3(100.0f, 2.5f, 1.0f), vec3(-53.0f, 2.0f, -14.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f);
	App->physics->AddConstraintHinge(*left_wall, *left_hinge, btVector3(-50.0f, 0.0f, 0.0f), btVector3(-50.0f, 0.0f, 0.0f), btVector3(0.0f, 1.0f, 0.0f), btVector3(0.0f, 1.0f, 0.0f));
	
	
	CreateCircuitWall(vec3(30.0f, 4.0f, 1.0f), vec3(-67.0f, 2.0f, -65.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f);


	CreateBowls();
	
	return ret;
}

Cube* ModuleSceneIntro::CreateCircuitWall(vec3 dimensions, vec3 pos, vec3 rotation, float angle) {
	Cube* wall = new Cube(dimensions, 0.0f);
	wall->SetPos(pos.x, pos.y, pos.z);
	wall->SetRotation(angle, rotation);
	wall->color = Aqua;
	primitives.PushBack(wall);

	return wall;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		DebugSpawnPrimitive(new Sphere());
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		DebugSpawnPrimitive(new Cube());
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		DebugSpawnPrimitive(new Cylinder());
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->body.Push(vec3((float)(std::rand() % 500) - 250, 500, (float)(std::rand() % 500) - 250));

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//TODO: NEW CODE
		//A snippet of new code that may be useful for you. Nothing to do here really


		//Get a vector indicating the direction from the camera viewpoint to the "mouse"
		const vec2 mousePos(((float)App->input->GetMouseX() / (float)App->window->Width()) * 2.f - 1.f,
			-((float)App->input->GetMouseY() / (float)App->window->Height()) * 2.f + 1.f);
		const vec4 rayEye = inverse(App->renderer3D->ProjectionMatrix) * vec4(mousePos.x, mousePos.y, -1.f, 1.f);
		const vec4 rayWorld(inverse(App->camera->GetViewMatrix()) * vec4(rayEye.x, rayEye.y, -1.f, 0.f));

		vec3 Dir(rayWorld.x, rayWorld.y, rayWorld.z);
		//Cast a ray from the camera, in the "mouse" direction
		PhysBody3D* body = App->physics->RayCast(App->camera->Position, Dir);
		if (body)
		{
			//Change the color of the clicked primitive
			body->parentPrimitive->color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
		}
	}
}

void ModuleSceneIntro::DebugSpawnPrimitive(Primitive * p)
{
	primitives.PushBack(p);
	p->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	p->body.collision_listeners.add(this);
	p->body.Push(-App->camera->Z * 1000.f);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		FreeCamera = !FreeCamera;
	}


	if (App->debug == true)
		HandleDebugInput();

	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Update();

	left_hinge->body.GetBody()->applyTorque(btVector3(500.0f, 0.0f, 500.0f));

	bowl1->body.GetBody()->activate(true);
	bowl2->body.GetBody()->activate(true);
	bowl3->body.GetBody()->activate(true);
	bowl4->body.GetBody()->activate(true);
	bowl5->body.GetBody()->activate(true);
	bowl6->body.GetBody()->activate(true);
	bowl7->body.GetBody()->activate(true);
	bowl8->body.GetBody()->activate(true);
	bowl9->body.GetBody()->activate(true);
	bowl10->body.GetBody()->activate(true);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		if(!primitives[n]->sensor)
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);

	body1->parentPrimitive->color = color;
	body2->parentPrimitive->color = color;

}

void ModuleSceneIntro::CreateBowls()
{
	//bowl_height,bowl_radious

	bowl1 = new Cylinder(bowl_height, bowl_radius);
	bowl1->SetPos(-65, 1, -50);
	primitives.PushBack(bowl1);

	bowl2 = new Cylinder(bowl_height, bowl_radius);
	bowl2->SetPos(-60, 1, -50);
	primitives.PushBack(bowl2);

	bowl2 = new Cylinder(bowl_height, bowl_radius);
	bowl2->SetPos(-70, 1, -50);
	primitives.PushBack(bowl2);

	bowl3 = new Cylinder(bowl_height, bowl_radius);
	bowl3->SetPos(-65, 1, -40);
	primitives.PushBack(bowl3);

	bowl4 = new Cylinder(bowl_height, bowl_radius);
	bowl4->SetPos(-60, 1, -40);
	primitives.PushBack(bowl4);

	bowl5 = new Cylinder(bowl_height, bowl_radius);
	bowl5->SetPos(-70, 1, -40);
	primitives.PushBack(bowl5);

	bowl6 = new Cylinder(bowl_height, bowl_radius);
	bowl6->SetPos(-65, 1, -45);
	primitives.PushBack(bowl6);

	bowl7 = new Cylinder(bowl_height, bowl_radius);
	bowl7->SetPos(-70, 1, -45);
	primitives.PushBack(bowl7);

	bowl8 = new Cylinder(bowl_height, bowl_radius);
	bowl8->SetPos(-60, 1, -45);
	primitives.PushBack(bowl8);

	bowl9 = new Cylinder(bowl_height, bowl_radius);
	bowl9->SetPos(-65, 1, -35);
	primitives.PushBack(bowl9);

	bowl10 = new Cylinder(bowl_height, bowl_radius);
	bowl10->SetPos(-65, 1, -30);
	primitives.PushBack(bowl10);

}
void ModuleSceneIntro::DeleteBowls()
{
	App->physics->RemoveBodyFromWorld(bowl1->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl2->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl3->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl4->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl5->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl6->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl7->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl8->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl9->body.GetBody());
	App->physics->RemoveBodyFromWorld(bowl10->body.GetBody());
}