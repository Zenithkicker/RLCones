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
	if (!enableCustomCones) { return; }
	bool customEnabled = enableCustomCones.getBoolValue();
	if (ImGui::Checkbox("Enable Custom Cones", &customEnabled)) {
		enableCustomCones.setValue(customEnabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable/Disable Custom Cones. Spawn your own cones based on car location. Left Alt for small cones Right Alt for large ones");
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

}