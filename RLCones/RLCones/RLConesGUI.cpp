#include "pch.h"
#include "RLCones.h"

void RLCones::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string RLCones::GetPluginName() {
	return "RLCones Plugin";
}

void RLCones::RenderSettings() {	

	//Global Enable/Disable
	CVarWrapper cvarGlobalEnable = cvarManager->getCvar("rlcones_enabled");
	if (!cvarGlobalEnable) { return; }
	bool globalEnabled = cvarGlobalEnable.getBoolValue();
	if (ImGui::Checkbox("Enable plugin", &globalEnabled)) {
		cvarGlobalEnable.setValue(globalEnabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable / Disable Plugin");
	}

	if (!globalEnabled)
		return;

	//Standard Cones Enable/Disable
	CVarWrapper enableStandardBoost = cvarManager->getCvar("rlcones_boostpad_standard_render_enabled");
	if (!enableStandardBoost) { return; }
	bool standardEnabled = enableStandardBoost.getBoolValue();
	if (ImGui::Checkbox("Enable Standard Cones", &standardEnabled)) {
		enableStandardBoost.setValue(standardEnabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable/Disable Standard Cones");
	}


	//Custom Cones Enable/Disable
	CVarWrapper enableCustomCones = cvarManager->getCvar("rlcones_boostpad_custom_render_enabled");
	bool customIsEnabled = enableCustomCones.getBoolValue();


	//When Custom Course Loaded
	//Show an unload button to clear spawns
	if (customIsEnabled)
	{
		//Show Unload button
		if (ImGui::Button("Unload Course")) {
			gameWrapper->Execute([this](GameWrapper* gw) {
				cvarManager->executeCommand("rlcones_unload_course");
			});
		}
	}

	//When Custom Course UnLoaded
	//Allow loading a new course file 
	if (!customIsEnabled)
	{
		const char* selectedDisplayText = selectedCourse;
		if (NULL == selectedDisplayText)
			selectedDisplayText = "Select Course...";
		
		//Course File Loading
		if (ImGui::BeginCombo("", selectedDisplayText))
		{
			for (int i = 0; i < _custombPadFileList.size(); ++i) 
			{
				char* indexItem = _custombPadFileList.at(i).data();
				const bool isSelected = (selectedCourse == indexItem);
				if (ImGui::Selectable(indexItem, isSelected))
				{
					selectedCourse = indexItem;
				}

				// Set the initial focus when opening the combo
				// (scrolling + keyboard navigation focus)
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}


		if (ImGui::Button("Load Course")) {
			gameWrapper->Execute([this](GameWrapper* gw) {
				cvarManager->executeCommand("rlcones_load_course");
			});
		}
	}

	//BallOnTop
	if (ImGui::Button("Ball On Top")) {
		gameWrapper->Execute([this](GameWrapper* gw) {
			cvarManager->executeCommand("rlcones_ballontop");
		});
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Put the ball on top of your car");
	}
	
	//ForceAmount
	CVarWrapper cvarForce = cvarManager->getCvar("rlcones_boostpad_collision_force");
	if (!cvarForce) { return; }
	float forceAmt = cvarForce.getFloatValue();
	if (ImGui::SliderFloat("Boostpad Collision Force", &forceAmt, 0.0, 10000)) {
		cvarForce.setValue(forceAmt);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverText = "force amount is " + std::to_string(forceAmt);
		ImGui::SetTooltip(hoverText.c_str());
	}

	//ForceMode
	static int fm = 0;
	CVarWrapper cvarForceMode = cvarManager->getCvar("rlcones_forceMode");
	if (!cvarForceMode) { return; }
	int forceModeInt = cvarForceMode.getIntValue();
	fm = forceModeInt;
	ImGui::Text("Force Mode:");										
	ImGui::SameLine();	
	if (ImGui::RadioButton("Acceleration", fm == 1)) 
	{ 
		cvarForceMode.setValue(1);
	}	
	ImGui::SameLine();
	if (ImGui::RadioButton("Impulse", fm == 2)) 
	{
		cvarForceMode.setValue(2);
	}


	//Gate Distance From Car
	CVarWrapper cvarGateDist = cvarManager->getCvar("rlcones_gate_distance_from_car");
	if (!cvarGateDist) { return; }
	float gateDist = cvarGateDist.getFloatValue();
	if (ImGui::SliderFloat("Gate Distance From Car", &gateDist, 270, 600)) {
		cvarGateDist.setValue(gateDist);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverText = "gate distance is " + std::to_string(gateDist);
		ImGui::SetTooltip(hoverText.c_str());
	}

	
}