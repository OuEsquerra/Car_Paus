#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer( bool start_enabled) : Module(start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	InitCar();
	
	throw_time = new Timer;

	state = GameLoopState::DRIVING;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	switch (state)
	{
		case GameLoopState::DRIVING:

			if (Throws == 2)
			{
				App->scene_intro->DeleteBowls();

				App->scene_intro->CreateBowls();

				Throws = 0;
			}

			Input();

			if (!App->scene_intro->FreeCamera)
			{
				//Look at car
				App->camera->LookAt(vehicle->GetPos());

				//Camera pos following car
				App->camera->Position = vec3(vehicle->GetPos().x - vehicle->vehicle->getForwardVector().x() * 10, vehicle->GetPos().y + 3, vehicle->GetPos().z - vehicle->vehicle->getForwardVector().z() * 10);
			}

			break;

		case GameLoopState::THROW:

			App->camera->LookAt(vehicle->GetPos());

			App->camera->Position = vec3(-50,10,0);

			if (throw_time->Read() > 7000)
			{
				state = GameLoopState::DRIVING;

				able_to_control = true;

				App->physics->RemoveBodyFromWorld(vehicle->GetBody());

				car_sensor->SetPos(100, 100, 100);
				
				vehicle->SetPos(0, 1, 0);

				InitCar();

				Throws++;
			}

			break;
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	car_sensor->SetPos(vehicle->GetPos().x, vehicle->GetPos().y + 2, vehicle->GetPos().z);

	char title[80];
	sprintf_s(title, " Throw %d", Throws);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2->GetSensorType() == sensorType::CONTROLS && body1->GetSensorType() == sensorType::CAR)
	{
		able_to_control = false;

		state = GameLoopState::THROW;

		throw_time->Start();

		
	}
}

void ModulePlayer::Input()
{
	if (able_to_control)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			if (vehicle->GetKmh() < 0)
			{
				brake = BRAKE_POWER;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			acceleration = MAX_ACCELERATION;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			if (vehicle->GetKmh() > 0)
			{
				brake = BRAKE_POWER;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{

			acceleration = -MAX_ACCELERATION;
		}
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
		{
			able_to_control = true;
		}
	}
}

void ModulePlayer::InitCar()
{

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 3);
	car.chassis_offset.Set(0, 1, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 30.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);

	vehicle->SetPos(0, 12, 10);


	car_sensor = new Cube(vec3(2, 1, 2), 0, true, sensorType::CAR);
	App->scene_intro->primitives.PushBack(car_sensor);
	App->camera->Position.y += vehicle->GetPos().y + 10;
	App->camera->Position.z += vehicle->GetPos().z - 15;
}