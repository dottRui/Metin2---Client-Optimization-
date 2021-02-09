/*
	Developed by .Rui
	Version 0.0.1
	Last revision: --/--/--
*/

#include "Stdafx.h"
#include "PythonPlayerSettingsModule.h"
#include "PythonNetworkStream.h"
#include "PythonPlayer.h"
#include "PythonBackground.h"
#include "PythonNetworkStream.h"

#include "../EterLib/ResourceManager.h"
#include "../EterPack/EterPackManager.h"
#include "../GameLib/RaceManager.h"

#include "boost/algorithm/string.hpp"


#define DISABLE_LANGUAGE_SKILL

void CPythonPlayerSettingsModule::Load()
{
	__RegisterDungeonMapNames();
	__RegisterTitleNames();
	__RegisterNameColors();
	__RegisterTitleColors();
	__RegisterEmotionIcons();

	__LoadGameSound();
	__LoadGameEffects();
	__LoadGameInit();
	__LoadEmoticons();
	__LoadRaceHeight();
#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
	__RegistLeaderGradeName();
#endif
#ifdef ENABLE_LOAD_PROPERTY
	CPythonBackground::Instance().CreateProperty();
#endif

	for (const auto &[raceIndex, fileName] : vecRaceData)
	{
		CRaceManager::Instance().CreateRace(raceIndex);
		CRaceManager::Instance().SelectRace(raceIndex);

		switch (raceIndex)
		{
		case NPlayerData::MAIN_RACE_WARRIOR_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "warrior_m.msm");
			__LoadGameWarriorEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_WARRIOR_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "warrior_w.msm");
			__LoadGameWarriorEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_ASSASSIN_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "assassin_m.msm");
			__LoadGameAssassinEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_ASSASSIN_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "assassin_w.msm");
			__LoadGameAssassinEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_SURA_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "sura_m.msm");
			__LoadGameSuraEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_SURA_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "sura_w.msm");
			__LoadGameSuraEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_SHAMAN_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "shaman_m.msm");
			__LoadGameShamanEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_SHAMAN_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "shaman_w.msm");
			__LoadGameShamanEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;
		}
	}

	__LoadGameNPC();
}

void CPythonPlayerSettingsModule::__RegisterCacheMotionData(CRaceData *pRaceData, const uint16_t wMotionMode, const uint16_t wMotionIndex, const char *c_szFileName, const uint8_t byPercentage)
{
	const char *c_szFullFileName = CRaceManager::Instance().GetFullPathFileName(c_szFileName);
	const CGraphicThing *pkMotionThing = pRaceData->RegisterMotionData(wMotionMode, wMotionIndex, c_szFullFileName, byPercentage);

	if (pkMotionThing)
		CResourceManager::Instance().LoadStaticCache(pkMotionThing->GetFileName());
}

void CPythonPlayerSettingsModule::__SetIntroMotions(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "intro/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_GENERAL);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_INTRO_WAIT, "wait.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_INTRO_SELECTED, "selected.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_INTRO_NOT_SELECTED, "not_selected.msa");
}

void CPythonPlayerSettingsModule::__SetGeneralMotions(CRaceData *pRaceData, const uint16_t wMotionMode, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName(c_rstrFolderName.c_str());
	pRaceData->RegisterMotionMode(wMotionMode);

	for (const auto &[motionIndex, fileName, percentage] : vecGeneralMotions)
		__RegisterCacheMotionData(pRaceData, wMotionMode, motionIndex, fileName, percentage);
}

void CPythonPlayerSettingsModule::__LoadComboRace(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName)
{
    const BYTE RaceIndex = RaceToJob(pRaceData->GetRace());

    CRaceManager::Instance().SetPathName(c_rstrFolderName.c_str());

    pRaceData->RegisterMotionMode(wMotionMode);

    if (RaceIndex == NRaceData::JOB_WARRIOR)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, "wait.msa", 50);
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, "wait_1.msa", 50);
    }
    else if (RaceIndex == NRaceData::JOB_ASSASSIN)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, "wait.msa", 70);
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, "wait_1.msa", 30);
    }
    else if (RaceIndex == NRaceData::JOB_SURA)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, "wait.msa");
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, "damage.msa");
    }
    else if (RaceIndex == NRaceData::JOB_SHAMAN)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, "wait.msa");
    }

    __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WALK, "walk.msa");
    __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_RUN, "run.msa");
    __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, "damage.msa", 50);
    __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, "damage_1.msa", 50);
    __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE_BACK, "damage_2.msa", 50);
    __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE_BACK, "damage_3.msa", 50);

    if (wMotionMode == CRaceMotionData::MODE_BOW)
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack.msa");
	else
	{
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa");
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa");
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa");
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_4, "combo_04.msa");
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_5, "combo_05.msa");
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_6, "combo_06.msa");
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_7, "combo_07.msa");
	}
      if (wMotionMode == CRaceMotionData::MODE_DUALHAND_SWORD)
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_8, "combo_08.msa");

    __LoadCombo(pRaceData, wMotionMode);
}


void CPythonPlayerSettingsModule::__LoadCombo(CRaceData *pRaceData, const uint16_t wMotionMode)
{

        if (wMotionMode == CRaceMotionData::MODE_BOW)
        {
            pRaceData->ReserveComboAttack(CRaceMotionData::MODE_BOW, COMBO_TYPE_1, 1);
            pRaceData->RegisterComboAttack(CRaceMotionData::MODE_BOW, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
            return;
        }

        pRaceData->ReserveComboAttack(wMotionMode, COMBO_TYPE_1, 4);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_4, CRaceMotionData::NAME_COMBO_ATTACK_4);


        pRaceData->ReserveComboAttack(wMotionMode, COMBO_TYPE_2, 5);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_4, CRaceMotionData::NAME_COMBO_ATTACK_5);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_5, CRaceMotionData::NAME_COMBO_ATTACK_7);

        pRaceData->ReserveComboAttack(wMotionMode, COMBO_TYPE_3, 6);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_4, CRaceMotionData::NAME_COMBO_ATTACK_5);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_5, CRaceMotionData::NAME_COMBO_ATTACK_6);
		if (wMotionMode == CRaceMotionData::MODE_DUALHAND_SWORD)
			pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_6, CRaceMotionData::NAME_COMBO_ATTACK_8);
		else
			pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_6, CRaceMotionData::NAME_COMBO_ATTACK_4);
}

void CPythonPlayerSettingsModule::__LoadGameWarriorEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	pRaceData->SetMotionRandomWeight(CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WAIT, 0, 70);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack.msa", 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack_1.msa", 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	for (int i = 0; i < CPythonSkill::SKILL_EFFECT_COUNT; ++i)
	{
		std::string END_STRING = "";
		if (i != 0)
			END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("samyeon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("palbang" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("jeongwi" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("geomgyeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("tanhwan" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("gihyeol" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("gigongcham" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("gyeoksan" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("daejin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("cheongeun" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("geompung" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("noegeom" + END_STRING + ".msa").c_str());

		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}

	__LoadFishing(pRaceData, c_rstrFolderName);
	__LoadGuildSkill(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait.msa", 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_1.msa", 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_2.msa", 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, "walk.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, "run.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, "dead.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, "skill_charge.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_onehand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa");
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, "skill_splash.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_twohand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa", 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa", 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa", 1);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, "skill_splash.msa");

	__LoadWedding(pRaceData, c_rstrFolderName);
	__LoadActions(pRaceData, c_rstrFolderName);
    __LoadComboRace(pRaceData, CRaceMotionData::MODE_ONEHAND_SWORD, c_rstrFolderName + "onehand_sword/");
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_TWOHAND_SWORD, c_rstrFolderName + "twohand_sword/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, "equip_right_hand");
    pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_ACCE, "Bip01 Spine2");
}

void CPythonPlayerSettingsModule::__LoadGameAssassinEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	pRaceData->SetMotionRandomWeight(CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WAIT, 0, 70);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack.msa", 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack_1.msa", 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	for (int i = 0; i < CPythonSkill::SKILL_EFFECT_COUNT; ++i)
	{
		std::string END_STRING = "";
		if (i != 0)
			END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("amseup" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("gungsin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("charyun" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("eunhyeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("sangong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("seomjeon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("yeonsa" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("gwangyeok" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("hwajo" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("gyeonggong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("dokgigung" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("seomgwang" + END_STRING + ".msa").c_str());
		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}

	__LoadFishing(pRaceData, c_rstrFolderName);
	__LoadGuildSkill(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait.msa", 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_1.msa", 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_2.msa", 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, "walk.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, "run.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, "dead.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, "skill_charge.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_onehand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa");
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, "skill_splash.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_dualhand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa");
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, "skill_splash.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_bow/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_BOW);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_WAIT, "wait.msa", 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_WAIT, "wait_1.msa", 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_WAIT, "wait_2.msa", 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_RUN, "run.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_DAMAGE, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_DEAD, "dead.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_SKILL + 123, "skill_splash.msa");
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_BOW, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BOW, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	__LoadWedding(pRaceData, c_rstrFolderName);
	__LoadActions(pRaceData, c_rstrFolderName);
    __LoadComboRace(pRaceData, CRaceMotionData::MODE_ONEHAND_SWORD, c_rstrFolderName + "onehand_sword/");
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_DUALHAND_SWORD, c_rstrFolderName + "dualhand_sword/");
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_BOW, c_rstrFolderName + "bow/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, "equip_right");
	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON_LEFT, "equip_left");
    pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_ACCE, "Bip01 Spine2");
}

void CPythonPlayerSettingsModule::__LoadGameSuraEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack.msa", 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack_1.msa", 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);


	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	for (int i = 0; i < CPythonSkill::SKILL_EFFECT_COUNT; ++i)
	{
		std::string END_STRING = "";
		if (i != 0)
			END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("swaeryeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("yonggwon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("gwigeom" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("gongpo" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("jumagap" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("pabeop" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("maryeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("hwayeom" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("muyeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("heuksin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("tusok" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("mahwan" + END_STRING + ".msa").c_str());
		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}
	__LoadGuildSkill(pRaceData, c_rstrFolderName);
	__LoadFishing(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait.msa", 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_1.msa", 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_2.msa", 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, "walk.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, "run.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, "dead.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, "skill_charge.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 123, "skill_splash.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_onehand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa");
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");

	__LoadWedding(pRaceData, c_rstrFolderName);

	__LoadActions(pRaceData, c_rstrFolderName);
        __LoadComboRace(pRaceData, CRaceMotionData::MODE_ONEHAND_SWORD, c_rstrFolderName + "onehand_sword/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, "equip_right");
    pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_ACCE, "Bip01 Spine2");
}

void CPythonPlayerSettingsModule::__LoadGameShamanEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack.msa", 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, "attack_1.msa", 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 1, "bipabu.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 2, "yongpa.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 3, "paeryong.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 4, "hosin_target.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 5, "boho_target.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 6, "gicheon_target.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 16, "noejeon.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 17, "byeorak.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 18, "pokroe.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 19, "jeongeop_target.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 20, "kwaesok_target.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 21, "jeungryeok_target.msa");

	std::vector<int> skillList{ 1, 2, 3 };

	for (const auto& i : skillList)
	{
		std::string END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("bipabu" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("yongpa" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("paeryong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("hosin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("boho" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("gicheon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("noejeon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("byeorak" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("pokroe" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("jeongeop" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("kwaesok" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("jeungryeok" + END_STRING + ".msa").c_str());
		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}

	__LoadGuildSkill(pRaceData, c_rstrFolderName);
	__LoadFishing(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait.msa", 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_1.msa", 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, "wait_2.msa", 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, "walk.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, "run.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, "damage.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, "dead.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, "skill_charge.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 123, "skill_splash.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_fan/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_FAN);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa");
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_bell/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_BELL);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_COMBO_ATTACK_1, "combo_01.msa", 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_COMBO_ATTACK_2, "combo_02.msa", 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_COMBO_ATTACK_3, "combo_03.msa", 1);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_SKILL + 121, "skill_wildattack.msa");

	__LoadWedding(pRaceData, c_rstrFolderName);
	__LoadActions(pRaceData, c_rstrFolderName);
        __LoadComboRace(pRaceData, CRaceMotionData::MODE_FAN, c_rstrFolderName + "fan/");
        __LoadComboRace(pRaceData, CRaceMotionData::MODE_BELL, c_rstrFolderName + "Bell/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, "equip_right");
	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON_LEFT, "equip_left");
    pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_ACCE, "Bip01 Spine2");
}

void CPythonPlayerSettingsModule::__LoadGameRace(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	if (!pRaceData->LoadRaceData(c_rstrFolderName.c_str()))
	{
		TraceError("Failed to load race data : %s\n", c_rstrFolderName.c_str());
	}
}

void CPythonPlayerSettingsModule::__LoadFishing(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "fishing/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_FISHING);

	for (const auto &[motionIndex, fileName] : vecFishing)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_FISHING, motionIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadWedding(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "wedding/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_WEDDING_DRESS);

	for (const auto &[motionIndex, fileName] : vecWeddingList)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_WEDDING_DRESS, motionIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadGuildSkill(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_GENERAL);

	for (const auto &[motionIndex, fileName] : vecGuildSkill)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, motionIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadActions(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "action/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_GENERAL);

	for (const auto &[motionIndex, fileName] : vecActionsList)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, motionIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadGameSound()
{
	for (const auto &[useSoundIndex, fileName] : vecUseSoundFileName)
		CPythonItem::Instance().SetUseSoundFileName(useSoundIndex, fileName);

	for (const auto &[dropSoundIndex, fileName] : vecDropSoundFileName)
		CPythonItem::Instance().SetDropSoundFileName(dropSoundIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadGameEffects()
{
	for (const auto& [effectName, effectType, effectFolder, effectCache] : vecEffectList)
		CInstanceBase::RegisterEffect(effectName, effectType, effectFolder, effectCache);

	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_EXP, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_yellow_small2.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_HP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_small.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_HP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_big.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_warrior_small.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_small.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_big.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK1, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_1.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK2, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_2.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK3, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_3.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK4, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_4.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK5, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_5.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK6, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_6.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK_XMAS, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_xmas.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_CHAIN_LIGHTNING, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/pc/shaman/effect/pokroe.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_HP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_smallest.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SKILL_MUYEONG, CFlyingManager::INDEX_FLY_TYPE_AUTO_FIRE, "d:/ymir work/pc/sura/effect/muyeong_fly.msf");
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_QUIVER_ATTACK_NORMAL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/pc/assassin/effect/arrow_02.msf");
}

void CPythonPlayerSettingsModule::__LoadEmoticons()
{
	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 0, "", "d:/ymir work/effect/etc/emoticon/sweat.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(sweat)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 1, "", "d:/ymir work/effect/etc/emoticon/money.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(money)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 2, "", "d:/ymir work/effect/etc/emoticon/happy.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(happy)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 3, "", "d:/ymir work/effect/etc/emoticon/love_s.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(love_s)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 4, "", "d:/ymir work/effect/etc/emoticon/love_l.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(love_l)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 5, "", "d:/ymir work/effect/etc/emoticon/angry.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(angry)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 6, "", "d:/ymir work/effect/etc/emoticon/aha.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(aha)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 7, "", "d:/ymir work/effect/etc/emoticon/gloom.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(gloom)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 8, "", "d:/ymir work/effect/etc/emoticon/sorry.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(sorry)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 9, "", "d:/ymir work/effect/etc/emoticon/!_mix_back.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(!)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 10, "", "d:/ymir work/effect/etc/emoticon/question.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(?)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 11, "", "d:/ymir work/effect/etc/emoticon/fish.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(fish)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 12, "", "d:/ymir work/effect/etc/emoticon/charging.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(charging)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 13, "", "d:/ymir work/effect/etc/emoticon/nosay.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(nosay)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 14, "", "d:/ymir work/effect/etc/emoticon/weather1.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(weather1)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 15, "", "d:/ymir work/effect/etc/emoticon/weather2.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(weather2)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 16, "", "d:/ymir work/effect/etc/emoticon/weather3.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(weather3)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 17, "", "d:/ymir work/effect/etc/emoticon/hungry.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(hungry)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 18, "", "d:/ymir work/effect/etc/emoticon/siren.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(siren)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 19, "", "d:/ymir work/effect/etc/emoticon/letter.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(letter)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 20, "", "d:/ymir work/effect/etc/emoticon/call.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(call)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 21, "", "d:/ymir work/effect/etc/emoticon/celebration.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(celebration)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 22, "", "d:/ymir work/effect/etc/emoticon/alcohol.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(alcohol)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 23, "", "d:/ymir work/effect/etc/emoticon/busy.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(busy)");

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 24, "", "d:/ymir work/effect/etc/emoticon/whirl.mse", true);
	CPythonNetworkStream::Instance().RegisterEmoticonString("(whirl)");
}

void CPythonPlayerSettingsModule::__LoadGameInit()
{
	CInstanceBase::SetDustGap(250);
	CInstanceBase::SetHorseDustGap(500);
	CPythonPlayer::Instance().RegisterEffect(CPythonPlayer::EFFECT_PICK, "d:/ymir work/effect/etc/click/click.mse", true);
}

void CPythonPlayerSettingsModule::__LoadRaceHeight()
{
	for (size_t index = 0; index < raceHeight.size(); ++index)
		CRaceManager::Instance().SetRaceHeight(raceHeight[index].raceVnum, raceHeight[index].height);
}

void CPythonPlayerSettingsModule::__LoadGameNPC()
{
	using namespace boost::algorithm;

	CMappedFile file;
	const VOID *pvData;
	if (!CEterPackManager::Instance().Get(file, NPC_LIST_FILE_NAME, &pvData))
	{
		TraceError("CPythonPlayerSettingsModule::LoadGameNPC(fileName=%s) - Load Error", NPC_LIST_FILE_NAME);
		return;
	}

	CMemoryTextFileLoader fileLoader;
	fileLoader.Bind(file.Size(), pvData);

	CTokenVector kTokenVector;

	for (size_t i = 0; i < fileLoader.GetLineCount(); ++i)
	{
		if (!fileLoader.SplitLineByTab(i, &kTokenVector))
			continue;

		const uint32_t& vnum = atoi(kTokenVector[0].c_str());
		std::string& name = kTokenVector[1];
		trim(name);

		if (vnum)
		{
			CRaceManager::Instance().RegisterRaceName(vnum, name.c_str());
		}
		else
		{
			std::string& src = kTokenVector[2];
			trim(src);
			CRaceManager::Instance().RegisterRaceSrcName(name, src);
		}
	}
}

void CPythonPlayerSettingsModule::__RegisterDungeonMapNames()
{
	CPythonBackground::Instance().RegisterDungeonMapName("metin2_map_spiderdungeon");
	CPythonBackground::Instance().RegisterDungeonMapName("metin2_map_monkeydungeon");
	CPythonBackground::Instance().RegisterDungeonMapName("metin2_map_monkeydungeon_02");
	CPythonBackground::Instance().RegisterDungeonMapName("metin2_map_monkeydungeon_03");
	CPythonBackground::Instance().RegisterDungeonMapName("metin2_map_deviltower1");
}

#ifdef ENABLE_GENDER_ALIGNMENT
void CPythonPlayerSettingsModule::__RegisterTitleNames()
{
	uint8_t gender_index = 0;
	for (const auto &[gender_male, gender_female] : titleNameList) {
		CInstanceBase::RegisterTitleName(gender_index, gender_female, gender_male);
		++gender_index;
	}
}
#else
void CPythonPlayerSettingsModule::__RegisterTitleNames()
{
	for (size_t index = 0; index < titleNameList.size(); ++index)
	{
		CInstanceBase::RegisterTitleName(index, titleNameList[index]);
	}
}
#endif

#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
void CPythonPlayerSettingsModule::__RegistLeaderGradeName()
{
	const size_t leaderGradeNameSize = leaderGradeName.size();
	for (size_t index = 0; index != leaderGradeNameSize; ++index)
		CInstanceBase::RegisterGuildLeaderGradeName(index, leaderGradeName[index]);
}
#endif

void CPythonPlayerSettingsModule::__RegisterNameColors()
{
	for (const auto &[index, rgbTuple] : characterNameColors)
	{
		const auto &[r, g, b] = rgbTuple;
		CInstanceBase::RegisterNameColor(index, r, g, b);
	}
}

void CPythonPlayerSettingsModule::__RegisterTitleColors()
{
	for (const auto &[index, rgbTuple] : characterTitleColors)
	{
		const auto &[r, g, b] = rgbTuple;
		CInstanceBase::RegisterTitleColor(index, r, g, b);
	}
}

void CPythonPlayerSettingsModule::__RegisterEmotionIcons()
{
	for (const auto& [index, imagePath] : emotionIcons)
	{
		CGraphicImage* pImage = (CGraphicImage*)CResourceManager::Instance().GetResourcePointer(imagePath);
		m_kMap_iEmotionIndex_pkIconImage.insert(std::make_pair(index, pImage));
	}
}

void CPythonPlayerSettingsModule::RegisterSkills()
{
	CPythonNetworkStream& net = CPythonNetworkStream::Instance();
	RegisterSkills(net.GetMainActorRace(), net.GetMainActorSkillGroup(), net.GetMainActorEmpire());
}

void CPythonPlayerSettingsModule::RegisterSkills(uint32_t race, uint8_t skill_group, uint8_t empire)
{
	const uint32_t playerRace = race;
	const uint32_t playerSkillGroup = skill_group;
	const uint8_t playerJob = RaceToJob(playerRace);
	const uint32_t playerEmpire = empire;

	// Player Skills
	if (skillIndexes.find(playerJob) != skillIndexes.end() &&
		skillIndexes.at(playerJob).find(skill_group) != skillIndexes.at(playerJob).end())
	{
		std::vector<uint8_t> playerSkills = skillIndexes.at(playerJob).at(playerSkillGroup);

		for (size_t slot = 0; slot < playerSkills.size(); ++slot)
		{
			CPythonPlayer::Instance().SetSkill(slot + 1, playerSkills[slot]);
		}
	}

	//Horse Skills
	if (horseSkillIndexes.find(playerJob) != horseSkillIndexes.end()) {
		const auto v_horseSkills = horseSkillIndexes.at(playerJob);
		for (size_t horseSkillSlot = 0; horseSkillSlot < v_horseSkills.size(); ++horseSkillSlot)
			CPythonPlayer::Instance().SetSkill(horseSkillSlot + 121, v_horseSkills[horseSkillSlot]);
	}
	
	// Support Skills
	const auto supportSkills = supportSkillIndexes;
	for (size_t slot = 0; slot < supportSkills.size(); ++slot)
	{
		CPythonPlayer::Instance().SetSkill(slot + 100 + 1, supportSkills[slot]);
	}

	#ifndef DISABLE_LANGUAGE_SKILL
	// Language Skills
	if (playerEmpire)
	{
		for (size_t i = 0; i < 3; ++i)
			if ((i+1) != playerEmpire)
				CPythonPlayer::Instance().SetSkill(107+i, c_iSkillIndex_Language1+i);
	}
	#endif

	// Passive Guild Skills
	auto passiveSkills = guildSkills.at("PASSIVE");
	for (size_t j = 0; j < passiveSkills.size(); ++j)
		CPythonPlayer::Instance().SetSkill(200 + j, passiveSkills[j]);

	// Active Guild Skills
	auto activeSkills = guildSkills.at("ACTIVE");
	for (size_t k = 0; k < activeSkills.size(); ++k)
		CPythonPlayer::Instance().SetSkill(210 + k, activeSkills[k]);
}

int32_t CPythonPlayerSettingsModule::GetCharacterTitleColor(uint8_t index)
{
	const auto &[r, g, b] = characterTitleColors.at(index);
	return CPythonGraphic::Instance().GenerateColor(r, g, b, 1.0);
}

bool CPythonPlayerSettingsModule::RegisterGuildBuildingList(const char* filepath)
{
	CMappedFile File;
	LPCVOID pData;

	if (!CEterPackManager::Instance().Get(File, filepath, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	PyObject* pyGuildData = PyList_New(0);

	CTokenVector TokenVector;
	for (size_t i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLineByTab(i, &TokenVector))
			continue;

		std::string& strVnum = TokenVector[GUIILD_BUILDING_TOKEN_VNUM];

		if (strVnum.find_first_not_of("0123456789") != std::string::npos)
			continue;

		if (TokenVector.size() < GUIILD_BUILDING_LIMIT_TOKEN_COUNT)
		{
			TraceError("RegisterGuildBuildingList(%s) - StrangeLine in %d TokenVector size too long: %d", filepath, i, TokenVector.size());
			continue;
		}

		const int32_t& id = atoi(strVnum.c_str());
		const std::string_view type = TokenVector[GUIILD_BUILDING_TOKEN_TYPE];
		const char* name = TokenVector[GUIILD_BUILDING_TOKEN_NAME].c_str();
		const char* localName = TokenVector[GUIILD_BUILDING_TOKEN_LOCAL_NAME].c_str();
		const int32_t& xRotLimit = atoi(TokenVector[GUIILD_BUILDING_TOKEN_X_ROT_LIMIT].c_str());
		const int32_t& yRotLimit = atoi(TokenVector[GUIILD_BUILDING_TOKEN_Y_ROT_LIMIT].c_str());
		const int32_t& zRotLimit = atoi(TokenVector[GUIILD_BUILDING_TOKEN_Z_ROT_LIMIT].c_str());
		const int32_t& price = atoi(TokenVector[GUIILD_BUILDING_TOKEN_PRICE].c_str());
		const std::string_view material = TokenVector[GUIILD_BUILDING_TOKEN_MATERIAL];
		const int32_t& enableType = atoi(TokenVector[GUIILD_BUILDING_TOKEN_ENABLE_FLAG].c_str());

		if (enableType == 0)
			continue;

		// Register material
		const std::string& folderName = m_buildingTypeToFolder[type];

		CRaceManager::Instance().RegisterRaceSrcName(name, folderName);
		CRaceManager::Instance().RegisterRaceName(id, name);

		// Create building material List
		std::vector<std::string> materialVector;
		boost::split(materialVector, material, boost::is_any_of("/"));

		PyObject* pyMaterialList = PyList_New(3);

		for (size_t j = 0; j < GUILD_MATERIAL_NUM; ++j)
		{
			PyList_SetItem(pyMaterialList, j, Py_BuildValue("s", "0"));
		}

		for (const std::string& itemPair : materialVector)
		{
			std::vector<std::string> itemVector;
			boost::split(itemVector, itemPair, boost::is_any_of(","));

			if (itemVector.size() != 2)
				continue;

			const int32_t& vnum = atoi(itemVector[0].c_str());
			const std::string& count = itemVector[1];

			const uint8_t index = getGuildMaterialIndex(vnum);

			if (index == -1)
			{
				TraceError("Found strange guild material: %d", vnum);
				continue;
			}

			PyList_SetItem(pyMaterialList, index, Py_BuildValue("s", count.c_str()));
		}

		// Create guild data dict and append to list
		PyObject* pyGuildDataItem = PyDict_New();
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "VNUM"), Py_BuildValue("i", id));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "TYPE"), Py_BuildValue("s", type.data()));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "NAME"), Py_BuildValue("s", name));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "LOCAL_NAME"), Py_BuildValue("s", localName));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "X_ROT_LIMIT"), Py_BuildValue("i", xRotLimit));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "Y_ROT_LIMIT"), Py_BuildValue("i", yRotLimit));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "Z_ROT_LIMIT"), Py_BuildValue("i", zRotLimit));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "PRICE"), Py_BuildValue("i", price));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "MATERIAL"), Py_BuildValue("O", pyMaterialList));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "SHOW"), Py_BuildValue("i", enableType != 2));
		PyList_Append(pyGuildData, pyGuildDataItem);
	}

	CPythonNetworkStream::Instance().SendPythonData(pyGuildData, "BINARY_SetGuildBuildingList");
	return true;
}
