#pragma once
#include "mod.h"
#include "types.h"

class CutsceneMod : public Mod
{
public:
    CutsceneMod(bool supportGlobals) : Mod( "Cutscene", true, supportGlobals)
    {
        m_iWindowFlags = ImGuiWindowFlags_MenuBar;
        m_startPos.x = 0;
        m_startPos.y = 0;
        m_startPos.z = 0;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:
    char m_szRequestedCutscene[256] = "";
    void DrawMenuBar();

    bool m_bHasCutsceneLoaded = false;
    bool m_bHasCutsceneFinished = false;
	bool m_bWasCutsceneSkipped = false;
    bool m_bIsCutscenePlaying = false;
    bool m_bIsCutsceneActive = false;

	bool m_bCanRequestAssetsForCutsceneEntity = false;
	bool m_bHasCutsceneCutThisFrame = false;

	int m_iCutsceneId = 0;
    int m_iCutsceneTime = 0;
    int m_iCutsceneTotalDuration = 0;
    int m_iCutsceneSectionPlaying = 0;

	int m_loaded_cutscene_id = -1;

	char m_audioVariableName[256] = "INOUT";
	float m_audioVariableValue = 0;

	int m_param0 = 0;
	int m_param1 = 0;

    // ImGui inputs
    unsigned int m_iCutsceneRequestFlags = 8;
    unsigned int m_iCutsceneStartFlags = 0;
    char m_szCutsceneInput[256] = "";

    Vector3 m_startPos;
};