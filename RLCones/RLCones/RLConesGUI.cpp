#include "pch.h"
#include "RLCones.h"

void RLCones::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string RLCones::GetPluginName() {
	return "RLCones Plugin";
}

void RLCones::RenderSettings()
{
	CVarWrapper enableCustomCreateCones = _globalCvarManager->getCvar("rlcones_boostpad_custom_create_enabled");
	bool customCreateIsEnabled = enableCustomCreateCones.getBoolValue();

	//Global Enable/Disable
	CVarWrapper cvarGlobalEnable = cvarManager->getCvar("rlcones_enabled");
	if (!cvarGlobalEnable) { return; }
	bool globalEnabled = cvarGlobalEnable.getBoolValue();

	//tabs
	if (ImGui::BeginTabBar("My Tabs"))
	{
		if (ImGui::BeginTabItem("Settings"))
		{
			RenderSettingsTab(cvarGlobalEnable);
			ImGui::EndTabItem();
		}

		if (globalEnabled)
		{
			if (ImGui::BeginTabItem("Course Controls"))
			{
				RenderCourseControlsTab(customCreateIsEnabled);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Interaction Controls"))
			{
				RenderInteractionControlsTab();
				ImGui::EndTabItem();
			}
		}

		ImGui::EndTabBar();
	}
}

void RLCones::RenderSettingsTab(CVarWrapper& cvarGlobalEnable)
{
	//spacing
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	bool globalEnabled = cvarGlobalEnable.getBoolValue();

	if (ImGui::Checkbox("Enable plugin", &globalEnabled)) {
		cvarGlobalEnable.setValue(globalEnabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable / Disable Plugin");
	}

	//spacing
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	if (globalEnabled)
	{
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
	}
}

void RLCones::RenderCourseControlsTab(bool customCreateIsEnabled)
{
	//spacing
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	if (customCreateIsEnabled)
	{
		ImGui::Text("Use Left Alt to spawn normal cone. Use Right Alt to spawn big cone. Use G to spawn a gate.  Click Save Course to save course to file.");
		ImGui::Text("The slider below controls how far apart the gate cones are from each other");
		//spacing
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//Gate Distance From Car
		CVarWrapper cvarGateDist = cvarManager->getCvar("rlcones_gate_distance_from_car");
		if (cvarGateDist) 
		{
			float gateDist = cvarGateDist.getFloatValue();
			if (ImGui::SliderFloat("Gate Distance From Car", &gateDist, 270, 600)) {
				cvarGateDist.setValue(gateDist);
			}
			if (ImGui::IsItemHovered()) {
				std::string hoverText = "gate distance is " + std::to_string(gateDist);
				ImGui::SetTooltip(hoverText.c_str());
			}
		}		

		//Show Save
		if (ImGui::Button("Save Course")) {
			gameWrapper->Execute([this](GameWrapper* gw) {
				cvarManager->executeCommand("rlcones_create_course_save");
				});
		}

		ImGui::SameLine();

		//Show Cancel
		if (ImGui::Button("Cancel")) {
			gameWrapper->Execute([this](GameWrapper* gw) {
				cvarManager->executeCommand("rlcones_create_course_cancel");
				});
		}
	}
	else
	{

		//Custom Cones Enable/Disable
		CVarWrapper enableCustomCones = cvarManager->getCvar("rlcones_boostpad_custom_render_enabled");
		bool customIsEnabled = enableCustomCones.getBoolValue();

		//When Custom Course Loaded
		//Show an unload button to clear spawns
		if (customIsEnabled)
		{
			ImGui::Text("Course Is Currently Loaded.  Click 'Unload Course' to load a new course.");

			//spacing
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			//Show Unload button
			if (ImGui::Button("Unload Course")) {
				gameWrapper->Execute([this](GameWrapper* gw) {
					cvarManager->executeCommand("rlcones_unload_course");
					});
			}
		}

		//When Custom Course UnLoaded
		//Allow loading a new course file 
		//Allow creating a new course file
		if (!customIsEnabled)
		{
			if (ImGui::Button("Create Course")) {
				gameWrapper->Execute([this](GameWrapper* gw) {
					cvarManager->executeCommand("rlcones_create_course_enable");
					});
			}

			//spacing
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			int fileListSize = _custombPadFileList.size();
			if (fileListSize > 0) {
				const char* selectedDisplayText = selectedCourse;
				if (NULL == selectedDisplayText)
					selectedDisplayText = "Select Course...";

				//spacing
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				//Course File Loading
				if (ImGui::BeginCombo("  ", selectedDisplayText))
				{
					for (int i = 0; i < fileListSize; ++i)
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

				//spacing
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				if (ImGui::Button("Load Selected Course")) {
					gameWrapper->Execute([this](GameWrapper* gw) {
						cvarManager->executeCommand("rlcones_load_course");
						});
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete Selected Course")) {
					gameWrapper->Execute([this](GameWrapper* gw) {
						cvarManager->executeCommand("rlcones_delete_course");
						});
				}
			}
		}
	}
}

void RLCones::RenderInteractionControlsTab()
{
	//spacing
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	//BallOnTop
	if (ImGui::Button("Ball On Top")) {
		gameWrapper->Execute([this](GameWrapper* gw) {
			cvarManager->executeCommand("rlcones_ballontop");
			});
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Put the ball on top of your car");
	}

	//spacing
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

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

	//spacing
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

}
