#include "pch.h"
#include "RLCones.h"
#include "JSONFileParser.h"
#include "Course.h"
using namespace std;

BAKKESMOD_PLUGIN(RLCones, "This is the RLCones description", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLCones::onLoad()
{
	_globalCvarManager = cvarManager;
	RegisterCvars();
	RegisterNotifiers();
	RegisterHookEvents();	
}

void RLCones::OnFreeplayLoad(std::string eventName)
{
	gameWrapper->RegisterDrawable(std::bind(&RLCones::Render, this, std::placeholders::_1));	
	LoadCourseFileList();
}

void RLCones::OnFreeplayDestroy(std::string eventName)
{
	gameWrapper->UnregisterDrawables();
}

void RLCones::onUnload()
{
	LOG("Hello World onUnLoad!");
}

void RLCones::RegisterCvars()
{
	_globalCvarManager->registerCvar("rlcones_enabled", "0");
	_globalCvarManager->registerCvar("rlcones_boostpad_standard_render_enabled", "0");
	_globalCvarManager->registerCvar("rlcones_boostpad_custom_render_enabled", "0");
	_globalCvarManager->registerCvar("rlcones_boostpad_collision_force", "1000", "The amount of force to apply when the ball collides with the boost pad", true, true, 0, true, 10000);
	_globalCvarManager->registerCvar("rlcones_forceMode", "1", "stuff");	
	_globalCvarManager->registerCvar("rlcones_gate_distance_from_car", "400", "The gate distance from the car. Press G to spawn", true, true, 270, true, 600);
}

void RLCones::RegisterNotifiers()
{
	//Place the ball on top of the player
	_globalCvarManager->registerNotifier("rlcones_ballontop", [&gw = this->gameWrapper](std::vector<std::string> commands) {
		CVarWrapper isEnabled = _globalCvarManager->getCvar("rlcones_enabled");
		if (isEnabled.IsNull() || !isEnabled.getBoolValue() || !gw->IsInFreeplay())
			return;

		ServerWrapper tutorial = gw->GetGameEventAsServer();

		if (tutorial.IsNull() || tutorial.GetGameBalls().Count() == 0)
			return;

		BallWrapper ball = tutorial.GetGameBalls().Get(0);
		CarWrapper car = tutorial.GetGameCar();
		if (ball.IsNull() || car.IsNull())
			return;
		Vector playerVelocity = car.GetVelocity();
		Vector addToBall = Vector(playerVelocity.X, playerVelocity.Y, 170);

		addToBall.X = max(min(20.0f, addToBall.X), -20.0f);//maybe limit the X a bit more
		addToBall.Y = max(min(30.0f, addToBall.Y), -30.0f);

		ball.SetLocation(car.GetLocation() + addToBall);
		ball.SetVelocity(playerVelocity);
	}, "Spawns the ball on top of your car", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);


	//load custom course
	_globalCvarManager->registerNotifier("rlcones_load_course", [&gw = this->gameWrapper, &bpm = this->_bPadManager, &_selectedCourse = this->selectedCourse](std::vector<std::string> commands) {
		CVarWrapper isEnabled = _globalCvarManager->getCvar("rlcones_enabled");
		if (isEnabled.IsNull() || !isEnabled.getBoolValue() || !gw->IsInFreeplay())
			return;

		//must disable custom boost pad rendering first
		CVarWrapper cvarBoostpadCustomIsEnabled = _globalCvarManager->getCvar("rlcones_boostpad_custom_render_enabled");
		if (cvarBoostpadCustomIsEnabled.IsNull() || cvarBoostpadCustomIsEnabled.getBoolValue())
			return;

		if (NULL == _selectedCourse) 
		{
			LOG("Please Select Course");
			return;
		}			

		try 
		{
			JSONFileParser jsonFileParser = JSONFileParser();
			std::string s = _selectedCourse;
			LOG("loading course file: " + s);

			std::string filePath = gw->GetDataFolder().string() + "/RLCones/" + _selectedCourse;
			json data = jsonFileParser.ReadFile(filePath);
			if (NULL == data)
			{
				LOG("Could not read file: " + filePath);
				_selectedCourse = NULL;
				return;
			}
			Course course = Course(data);
			bpm.LoadCourse(course);

			//enable custom cones
			CVarWrapper enableCustomCones = _globalCvarManager->getCvar("rlcones_boostpad_custom_render_enabled");
			enableCustomCones.setValue(true);
		}
		catch (std::exception& e) {
			LOG("{}", e.what());
		}

	}, "Load Course From File", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);


	//unload course
	_globalCvarManager->registerNotifier("rlcones_unload_course", [&gw = this->gameWrapper, &bpm = this->_bPadManager, &_selectedCourse = this->selectedCourse](std::vector<std::string> commands) {
		CVarWrapper isEnabled = _globalCvarManager->getCvar("rlcones_enabled");
		if (isEnabled.IsNull() || !isEnabled.getBoolValue() || !gw->IsInFreeplay())
			return;

		//disable custom cones
		CVarWrapper enableCustomCones = _globalCvarManager->getCvar("rlcones_boostpad_custom_render_enabled");
		enableCustomCones.setValue(false);

		//clear selected course
		_selectedCourse = NULL;

		//clear spawns
		bpm.ClearCustomSpawns();		

	}, "Unload Course", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);

	//save custom course to file
	_globalCvarManager->registerNotifier("rlcones_save_course", [&gw = this->gameWrapper, &bpm = this->_bPadManager, &fileList = this->_custombPadFileList](std::vector<std::string> commands) {
		CVarWrapper isEnabled = _globalCvarManager->getCvar("rlcones_enabled");
		if (isEnabled.IsNull() || !isEnabled.getBoolValue() || !gw->IsInFreeplay())
			return;

		//must disable custom boost pad rendering first
		CVarWrapper cvarBoostpadCustomIsEnabled = _globalCvarManager->getCvar("rlcones_boostpad_custom_render_enabled");
		if (cvarBoostpadCustomIsEnabled.IsNull() || cvarBoostpadCustomIsEnabled.getBoolValue())
			return;

		JSONFileParser jsonFileParser = JSONFileParser();
		int fileCount = fileList.size();
		std::string filePath = gw->GetDataFolder().string() + "/RLCones/BoostPads" + std::to_string(fileCount) + ".json";
		json fileData = bpm.CreateCustomConesJson();
		jsonFileParser.WriteFile(filePath, fileData);


	}, "Save Course To File", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);
}

void RLCones::RegisterHookEvents()
{
	//this runs 120 times per second
	gameWrapper->HookEvent("Function TAGame.GameObserver_TA.GetBall",
		[this](std::string eventName) {
			CVarWrapper cvarGlobalIsEnabled = _globalCvarManager->getCvar("rlcones_enabled");
			if (cvarGlobalIsEnabled.IsNull() || !cvarGlobalIsEnabled.getBoolValue())
				return;			

			CVarWrapper cvarForceMode = _globalCvarManager->getCvar("rlcones_forceMode");
			int forceModeInt = 1;
			if (!cvarForceMode.IsNull())
				forceModeInt = cvarForceMode.getIntValue();

			//is this how to convert?
			char forceModChar = forceModeInt;

			CVarWrapper cvarForceAmount = _globalCvarManager->getCvar("rlcones_boostpad_collision_force");
			float forceAmount = 1000;
			if (!cvarForceAmount.IsNull())
				forceAmount = cvarForceAmount.getFloatValue();

			GetBall(forceModChar, forceAmount);			
		});

	gameWrapper->HookEvent("Function TAGame.Mutator_Freeplay_TA.Init", bind(&RLCones::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", bind(&RLCones::OnFreeplayDestroy, this, std::placeholders::_1));
	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GameViewportClient_TA.HandleKeyPress", std::bind(&RLCones::OnKeyPressed, this, placeholders::_1, placeholders::_2, placeholders::_3));
}

void RLCones::LoadCourseFileList() 
{
	std::string filesDirectory = gameWrapper->GetDataFolder().string() + "/RLCones";
	if (!fs::exists(filesDirectory)) 
	{
		fs::create_directory(filesDirectory);
		return;
	}

	_custombPadFileList.clear();

	for (const auto& entry : fs::directory_iterator(filesDirectory)) 
	{
		_custombPadFileList.push_back(entry.path().filename().string());
	}		
}

void RLCones::GetBall(unsigned char forceMode, float forceAmount)
{
	RunBallCollisionChecks(forceMode, forceAmount);
}

void RLCones::RunBallCollisionChecks(unsigned char forceMode, float forceAmount) {
	if (!gameWrapper->IsInFreeplay())
		return;
	ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
	BallWrapper ball = tutorial.GetGameBalls().Get(0);
	if (tutorial.IsNull() || ball.IsNull())
		return;
	
	CVarWrapper cvarBoostpadStandardIsEnabled = _globalCvarManager->getCvar("rlcones_boostpad_standard_render_enabled");
	CVarWrapper cvarBoostpadCustomIsEnabled = _globalCvarManager->getCvar("rlcones_boostpad_custom_render_enabled");

	if (!cvarBoostpadStandardIsEnabled.IsNull() && cvarBoostpadStandardIsEnabled.getBoolValue()) {
		for (int i = 0; i < _bPadManager.boostPadsCount; i++) {
			HandleBallPadCollision(ball, _bPadManager._standardCylinder2Boostpads[i], forceMode, forceAmount);
		}
	}

	if (!cvarBoostpadCustomIsEnabled.IsNull() && cvarBoostpadCustomIsEnabled.getBoolValue()) {
		for (std::vector<Cylinder2BoostPad>::iterator it = _bPadManager._customCylinder2Boostpads.begin(); it != _bPadManager._customCylinder2Boostpads.end(); ++it)
		{
			HandleBallPadCollision(ball, *it, forceMode, forceAmount);
		}
	}
}

void RLCones::HandleBallPadCollision(BallWrapper& ball, Cylinder2BoostPad& pad, unsigned char forceMode, float forceAmount)
{
	if (_collisionManager.AreSpheresColliding(ball.GetLocation(), ball.GetRadius(), pad._boostPad.GetPosition(), pad._boostPad.GetRadius()))
	{
		Vector force = Vector(0, 0, forceAmount);
		ball.AddForce(force, forceMode);
	}
}

void RLCones::Render(CanvasWrapper canvas)
{
	CVarWrapper cvarGlobalIsEnabled = _globalCvarManager->getCvar("rlcones_enabled");
	if (cvarGlobalIsEnabled.IsNull() || !cvarGlobalIsEnabled.getBoolValue())
		return;

	CameraWrapper camera = gameWrapper->GetCamera();
	if (camera.IsNull())
		return;

	RT::Frustum frust{ canvas, camera };
	Vector up = Vector(0, 0, 1);

	CVarWrapper cvarBoostpadStandardIsEnabled = _globalCvarManager->getCvar("rlcones_boostpad_standard_render_enabled");
	CVarWrapper cvarBoostpadCustomIsEnabled = _globalCvarManager->getCvar("rlcones_boostpad_custom_render_enabled");

	if (!cvarBoostpadStandardIsEnabled.IsNull() && cvarBoostpadStandardIsEnabled.getBoolValue()) 
	{
		for (int i = 0; i < _bPadManager.boostPadsCount; i++)
		{
			Cylinder2BoostPad pad = _bPadManager._standardCylinder2Boostpads[i];			
			pad.Draw(canvas, frust);
		}
	}
	if (!cvarBoostpadCustomIsEnabled.IsNull() && cvarBoostpadCustomIsEnabled.getBoolValue()) 
	{
		for (std::vector<Cylinder2BoostPad>::iterator it = _bPadManager._customCylinder2Boostpads.begin(); it != _bPadManager._customCylinder2Boostpads.end(); ++it) 
		{			
			Cylinder2BoostPad pad = *it;
			pad.Draw(canvas, frust);
		}
	}
}

void RLCones::OnKeyPressed(ActorWrapper aw, void* params, std::string eventName)
{
	CVarWrapper cvarGlobalIsEnabled = _globalCvarManager->getCvar("rlcones_enabled");
	if (cvarGlobalIsEnabled.IsNull() || !cvarGlobalIsEnabled.getBoolValue())
		return;

	CVarWrapper cvarBoostpadCustomIsEnabled = _globalCvarManager->getCvar("rlcones_boostpad_custom_render_enabled");
	if (cvarBoostpadCustomIsEnabled.IsNull() || !cvarBoostpadCustomIsEnabled.getBoolValue())
		return;

	KeyPressParams* keyPressData = (KeyPressParams*)params;
	if (keyPressData->EventType != EInputEvent::Released)
		return;

	ServerWrapper sw = gameWrapper->GetGameEventAsServer();
	if (sw.IsNull())
		return;
	CarWrapper cw = sw.GetGameCar();
	if (cw.IsNull())
		return;

	if (keyPressData->Key.Index == gameWrapper->GetFNameIndexByString("LeftAlt")) 
	{
		_bPadManager.SpawnCustomCylinder2BoostPad(cw.GetLocation());
	}
	else if (keyPressData->Key.Index == gameWrapper->GetFNameIndexByString("RightAlt"))
	{
		_bPadManager.SpawnCustomCylinder2BoostPad(cw.GetLocation(),true);
	}
	else if (keyPressData->Key.Index == gameWrapper->GetFNameIndexByString("G"))
	{
		CVarWrapper cvarDistFromCar = _globalCvarManager->getCvar("rlcones_gate_distance_from_car");		
		float gateDistFromCar = 400;
		if (!cvarDistFromCar.IsNull())
			gateDistFromCar = cvarDistFromCar.getFloatValue();

		_bPadManager.SpawnCustomCylinder2Gate(cw.GetLocation(), gateDistFromCar, true);
	}
}

