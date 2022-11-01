#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "version.h"
#include "Objects/Frustum.h"
#include "Objects/Cylinder2.h"
#include "BoostPadManager.h"
#include "CollisionManager.h"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

struct FNameStruct {
	int Index;
	int Number;
};

struct KeyPressParams {
	int ControllerID;
	FNameStruct Key;
	unsigned char EventType;
	float AmountDepressed;
	unsigned int bGamepad;
	unsigned int ReturnValue;
};

enum EInputEvent
{
	Pressed = 0,
	Released = 1,
	Repeat = 2,
	DoubleClick = 3,
	Axis = 4
};

class RLCones: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow
{
	BoostPadManager _bPadManager = BoostPadManager();
	CollisionManager _collisionManager = CollisionManager();
	virtual void onLoad();
	virtual void onUnload();
	virtual void RegisterCvars();
	virtual void RegisterNotifiers();
	virtual void RegisterHookEvents();
	virtual void RunBallCollisionChecks(unsigned char forceMode, float forceAmount);
	virtual void GetBall(unsigned char forceMode, float forceAmount);
	virtual void HandleBallPadCollision(BallWrapper& ball, BoostPad& pad, unsigned char forceMode, float forceAmount);	
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
	void Render(CanvasWrapper canvas);
	void OnFreeplayLoad(std::string eventName);
	void OnFreeplayDestroy(std::string eventName);
	void OnKeyPressed(ActorWrapper aw, void* params, std::string eventName);
};

