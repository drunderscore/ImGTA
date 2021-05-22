#include "sync_scene_mod.h"
#include "natives.h"
#include "script.h"
#include "model.h"
#include <bitset>


void SyncSceneMod::Load()
{

}

void SyncSceneMod::Unload()
{

}

void SyncSceneMod::Think()
{
	if (m_constantUpdate || m_wantsUpdate)
	{
		UpdateHandleData();
		ListRunning();
		m_wantsUpdate = false;
	}
}

void SyncSceneMod::ResetData()
{
	m_phase = 0;
	m_rate = 0;
	m_looped = 0;
	m_running = 0;
	m_holdLastFrame = false;
}

void SyncSceneMod::ListRunning()
{
	m_runningList = "";
	for (int i = 0; i < 500; i++)
	{
		if (PED::IS_SYNCHRONIZED_SCENE_RUNNING(i))
			m_runningList += std::to_string(i) + ", ";
	}
}

void SyncSceneMod::UpdateHandleData()
{
	m_phase = PED::GET_SYNCHRONIZED_SCENE_PHASE(m_handleInput);
	m_rate = PED::GET_SYNCHRONIZED_SCENE_RATE(m_handleInput);
	m_looped = PED::IS_SYNCHRONIZED_SCENE_LOOPED(m_handleInput);
	m_running = PED::IS_SYNCHRONIZED_SCENE_RUNNING(m_handleInput);
	m_holdLastFrame = PED::IS_SYNCHRONIZED_SCENE_HOLD_LAST_FRAME(m_handleInput);
}

void SyncSceneMod::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Loop"))
			{
				RunOnNativeThread([=]
				{
					PED::SET_SYNCHRONIZED_SCENE_LOOPED(m_handleInput, !m_looped);
				});
			}

			if (ImGui::BeginMenu("Phase"))
			{
				ImGui::InputFloat("Phase", &m_phaseInput);
				if (ImGui::Button("Set"))
				{
					RunOnNativeThread([=]
					{
						if (m_phaseInput >= 0 && m_phaseInput <= 1)
							PED::SET_SYNCHRONIZED_SCENE_PHASE(m_handleInput, m_phaseInput);
					});
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Rate"))
			{
				ImGui::InputFloat("Rate", &m_rateInput);
				if (ImGui::Button("Set"))
				{
					RunOnNativeThread([=]
					{
						if (m_rateInput >= 0 && m_rateInput <= 2)
							PED::SET_SYNCHRONIZED_SCENE_RATE(m_handleInput, m_rateInput);
					});
				}
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Set hold last frame"))
			{
				RunOnNativeThread([=]
				{
					PED::SET_SYNCHRONIZED_SCENE_HOLD_LAST_FRAME(m_handleInput, !m_holdLastFrame);
				});
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}



bool SyncSceneMod::Draw()
{
	ImGui::SetWindowFontScale(m_menuFontSize);
	DrawMenuBar();

	ImGui::SetWindowFontScale(m_contentFontSize);

	ImGui::Checkbox("Constant Updates?", &m_constantUpdate);
	if (!m_constantUpdate)
		if (ImGui::Button("Update"))
			m_wantsUpdate = true;


	ImGui::Separator();
	if (ImGui::TreeNode("Synchronized scene"))
	{
		if (ImGui::InputInt("Handle", &m_handleInput))
		{
			if (m_handleInput < 0)
				m_handleInput = 0;
			m_wantsUpdate = true;
		}

		ImGui::Text("Phase: %.2f", m_phase);
		ImGui::Text("Rate: %.2f", m_rate);
		ImGui::Text("Is running: %d", m_running);
		ImGui::Text("Is looped: %d", m_looped);
		ImGui::Text("Hold last frame: %d", m_holdLastFrame);
		ImGui::TreePop();
	}

	ImGui::Separator();
	ImGui::Text("Running scene list: %s", m_runningList.c_str());

	return true;
}