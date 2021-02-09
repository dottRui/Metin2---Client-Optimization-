#pragma once

#include "PythonItem.h"
#include "PythonSkill.h"
#include "PythonPlayer.h"
#include "InstanceBase.h"
#include "../GameLib/FlyingObjectManager.h"
#include "GameType.h"
#include "../GameLib/RaceData.h"
#include "../GameLib/RaceMotionData.h"
#include "../GameLib/RaceManager.h"

#include <thread>
#include <map>

typedef std::array<uint8_t, 3> RGB;

static const char* NPC_LIST_FILE_NAME = "npclist.txt";
static bool loaded = false;

class CPythonPlayerSettingsModule : public CSingleton<CPythonPlayerSettingsModule>
{
public:
	CPythonPlayerSettingsModule() = default;
	virtual ~CPythonPlayerSettingsModule() = default;
	void Load();

public:
	void RegisterSkills();
	void RegisterSkills(uint32_t race, uint8_t skill_group, uint8_t empire);

	void __RegisterCacheMotionData(CRaceData* pRaceData, const uint16_t wMotionMode, const uint16_t wMotionIndex, const char* c_szFileName, const uint8_t byPercentage = 100);
	void __SetIntroMotions(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __SetGeneralMotions(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName);

	void __LoadComboRace(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName);

	void __LoadCombo(CRaceData* pRaceData, const uint16_t wMotionMode);
	void __LoadGameWarriorEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGameAssassinEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGameSuraEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGameShamanEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGameRace(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadFishing(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadWedding(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadActions(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGuildSkill(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadWeaponMotion(CRaceData* pRaceData, WORD wMotionMode, const std::string& c_rstrFolderName);

	void __RegisterDungeonMapNames();
	void __RegisterTitleNames();
	void __RegisterNameColors();
	void __RegisterTitleColors();
	void __RegisterEmotionIcons();
	#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
	void __RegistLeaderGradeName();
	#endif

public:
	bool RegisterGuildBuildingList(const char* filepath);
	const char* GetLocaleString(const char* key);
	int32_t GetCharacterTitleColor(uint8_t index);

private:
	enum EGuildBuildingListToken
	{
		GUIILD_BUILDING_TOKEN_VNUM,
		GUIILD_BUILDING_TOKEN_TYPE,
		GUIILD_BUILDING_TOKEN_NAME,
		GUIILD_BUILDING_TOKEN_LOCAL_NAME,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_1,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_2,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_3,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_4,
		GUIILD_BUILDING_TOKEN_X_ROT_LIMIT,
		GUIILD_BUILDING_TOKEN_Y_ROT_LIMIT,
		GUIILD_BUILDING_TOKEN_Z_ROT_LIMIT,
		GUIILD_BUILDING_TOKEN_PRICE,
		GUIILD_BUILDING_TOKEN_MATERIAL,
		GUIILD_BUILDING_TOKEN_NPC,
		GUIILD_BUILDING_TOKEN_GROUP,
		GUIILD_BUILDING_TOKEN_DEPEND_GROUP,
		GUIILD_BUILDING_TOKEN_ENABLE_FLAG,
		GUIILD_BUILDING_LIMIT_TOKEN_COUNT,
	} EGuildBuildingListToken;

	std::map<std::string_view, std::string> m_buildingTypeToFolder = 
	{
		{"HEADQUARTER", "headquarter"},
		{"FACILITY", "facility"},
		{"OBJECT", "object"},
		{"WALL", "fence"},
	};

public:
	void __LoadGameSound();
	void __LoadGameEffects();
	void __LoadEmoticons();
	void __LoadRaceHeight();
	void __LoadGameInit();
	void __LoadGameNPC();

public:
	std::thread m_loadThread;
	bool m_loadedThread = false;

	void startLoadThread()
	{
		if (!m_loadedThread)
		{
			m_loadThread = std::thread(&CPythonPlayerSettingsModule::Load, this);
			m_loadedThread = true;
		}
	}

	void joinLoadThread()
	{
		if (m_loadThread.joinable())
		{
			m_loadThread.join();
		}
	}
};

typedef std::pair<DWORD, std::string> TSoundFileName;
static const std::array<TSoundFileName, CPythonItem::USESOUND_NUM - 1> vecUseSoundFileName =
{ {
	{CPythonItem::USESOUND_DEFAULT, "sound/ui/drop.wav"},
	{CPythonItem::USESOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav"},
	{CPythonItem::USESOUND_ARMOR, "sound/ui/equip_metal_armor.wav"},
	{CPythonItem::USESOUND_BOW, "sound/ui/equip_bow.wav"},
	{CPythonItem::USESOUND_WEAPON, "sound/ui/equip_metal_weapon.wav"},
	{CPythonItem::USESOUND_POTION, "sound/ui/eat_potion.wav"},
	{CPythonItem::USESOUND_PORTAL, "sound/ui/potal_scroll.wav"},
} };

static const std::array<TSoundFileName, CPythonItem::DROPSOUND_NUM> vecDropSoundFileName =
{ {
		{CPythonItem::DROPSOUND_DEFAULT, "sound/ui/drop.wav"},
		{CPythonItem::DROPSOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav"},
		{CPythonItem::DROPSOUND_ARMOR, "sound/ui/equip_metal_armor.wav"},
		{CPythonItem::DROPSOUND_BOW, "sound/ui/equip_bow.wav"},
		{CPythonItem::DROPSOUND_WEAPON, "sound/ui/equip_metal_weapon.wav"},
} };

const constexpr uint8_t GENERAL_MOTION_NUM = 13;
typedef std::tuple<DWORD, const char*, const uint8_t> TGMotionList;
constexpr std::array<TGMotionList, GENERAL_MOTION_NUM> vecGeneralMotions =
{ {
		{CRaceMotionData::NAME_WAIT, "wait.msa", 100},
		{CRaceMotionData::NAME_WALK, "walk.msa", 100},
		{CRaceMotionData::NAME_RUN, "run.msa", 100},
		{CRaceMotionData::NAME_DAMAGE, "damage.msa", 50},
		{CRaceMotionData::NAME_DAMAGE, "damage_1.msa", 50},
		{CRaceMotionData::NAME_DAMAGE_BACK, "damage_2.msa", 50},
		{CRaceMotionData::NAME_DAMAGE_BACK, "damage_3.msa", 50},
		{CRaceMotionData::NAME_DAMAGE_FLYING, "damage_flying.msa", 100},
		{CRaceMotionData::NAME_STAND_UP, "falling_stand.msa", 100},
		{CRaceMotionData::NAME_DAMAGE_FLYING_BACK, "back_damage_flying.msa", 100},
		{CRaceMotionData::NAME_STAND_UP_BACK, "back_falling_stand.msa", 100},
		{CRaceMotionData::NAME_DEAD, "dead.msa", 100},
		{CRaceMotionData::NAME_DIG, "dig.msa", 100},
} };

const constexpr uint8_t WEDDING_MOTION_NUM = 3;
typedef std::pair<DWORD, const char*> TWeddingList;
constexpr std::array<TWeddingList, WEDDING_MOTION_NUM> vecWeddingList =
{ {
		{CRaceMotionData::NAME_WAIT, "wait.msa"},
		{CRaceMotionData::NAME_WALK, "walk.msa"},
		{CRaceMotionData::NAME_RUN, "walk.msa"},
} };

const constexpr uint8_t ACTION_MOTION_NUM = 39;
typedef std::pair<DWORD, const char*> TActionsList;
constexpr std::array<TActionsList, ACTION_MOTION_NUM> vecActionsList =
{ {
	{CRaceMotionData::NAME_CLAP, "clap.msa"},
	{CRaceMotionData::NAME_CHEERS_1, "cheers_1.msa"},
	{CRaceMotionData::NAME_CHEERS_2, "cheers_2.msa"},
	{CRaceMotionData::NAME_DANCE_1, "dance_1.msa"},
	{CRaceMotionData::NAME_DANCE_2, "dance_2.msa"},
	{CRaceMotionData::NAME_DANCE_3, "dance_3.msa"},
	{CRaceMotionData::NAME_DANCE_4, "dance_4.msa"},
	{CRaceMotionData::NAME_DANCE_5, "dance_5.msa"},
	{CRaceMotionData::NAME_DANCE_6, "dance_6.msa"},
	{CRaceMotionData::NAME_CONGRATULATION, "congratulation.msa"},
	{CRaceMotionData::NAME_FORGIVE, "forgive.msa"},
	{CRaceMotionData::NAME_ANGRY, "angry.msa"},
	{CRaceMotionData::NAME_ATTRACTIVE, "attractive.msa"},
	{CRaceMotionData::NAME_SAD, "sad.msa"},
	{CRaceMotionData::NAME_SHY, "shy.msa"},
	{CRaceMotionData::NAME_CHEERUP, "cheerup.msa"},
	{CRaceMotionData::NAME_BANTER, "banter.msa"},
	{CRaceMotionData::NAME_JOY, "joy.msa"},
	{CRaceMotionData::NAME_FRENCH_KISS_WITH_WARRIOR, "french_kiss_with_warrior.msa"},
	{CRaceMotionData::NAME_FRENCH_KISS_WITH_ASSASSIN, "french_kiss_with_assassin.msa"},
	{CRaceMotionData::NAME_FRENCH_KISS_WITH_SURA, "french_kiss_with_sura.msa"},
	{CRaceMotionData::NAME_FRENCH_KISS_WITH_SHAMAN, "french_kiss_with_shaman.msa"},
	{CRaceMotionData::NAME_KISS_WITH_WARRIOR, "kiss_with_warrior.msa"},
	{CRaceMotionData::NAME_KISS_WITH_ASSASSIN, "kiss_with_assassin.msa"},
	{CRaceMotionData::NAME_KISS_WITH_SURA, "kiss_with_sura.msa"},
	{CRaceMotionData::NAME_KISS_WITH_SHAMAN, "kiss_with_shaman.msa"},
	{CRaceMotionData::NAME_SLAP_HIT_WITH_WARRIOR, "slap_hit.msa"},
	{CRaceMotionData::NAME_SLAP_HIT_WITH_ASSASSIN, "slap_hit.msa"},
	{CRaceMotionData::NAME_SLAP_HIT_WITH_SURA, "slap_hit.msa"},
	{CRaceMotionData::NAME_SLAP_HIT_WITH_SHAMAN, "slap_hit.msa"},
	{CRaceMotionData::NAME_SLAP_HURT_WITH_WARRIOR, "slap_hurt.msa"},
	{CRaceMotionData::NAME_SLAP_HURT_WITH_ASSASSIN, "slap_hurt.msa"},
	{CRaceMotionData::NAME_SLAP_HURT_WITH_SURA, "slap_hurt.msa"},
	{CRaceMotionData::NAME_SLAP_HURT_WITH_SHAMAN, "slap_hurt.msa"},
	{CRaceMotionData::NAME_DANCE_7, "dance_7.msa"},
	{CRaceMotionData::NAME_DOZE, "doze.msa"},
	{CRaceMotionData::NAME_EXERCISE, "exercise.msa"},
	{CRaceMotionData::NAME_PUSHUP, "pushup.msa"},
	{CRaceMotionData::NAME_SELFIE, "selfie.msa"}
} };

typedef std::tuple<uint32_t, const char*, const char*, bool> TEffectList;
static const std::vector<TEffectList> vecEffectList =
{
	{ CInstanceBase::EFFECT_GYEONGGONG_BOOM, "Bip01", "d:/ymir work/effect/hit/gyeonggong_boom.mse", true},
	{ CInstanceBase::EFFECT_SPAWN_APPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_appear.mse", true},
	{ CInstanceBase::EFFECT_SPAWN_DISAPPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_die.mse", true},
	{ CInstanceBase::EFFECT_FLAME_ATTACK, "equip_right_hand", "d:/ymir work/effect/hit/blow_flame/flame_3_weapon.mse", true},
	{ CInstanceBase::EFFECT_FLAME_HIT, "", "d:/ymir work/effect/hit/blow_flame/flame_3_blow.mse", true},
	{ CInstanceBase::EFFECT_FLAME_ATTACH, "", "d:/ymir work/effect/hit/blow_flame/flame_3_body.mse", true},
	{ CInstanceBase::EFFECT_ELECTRIC_ATTACK, "equip_right", "d:/ymir work/effect/hit/blow_electric/light_1_weapon.mse", true},
	{ CInstanceBase::EFFECT_ELECTRIC_HIT, "", "d:/ymir work/effect/hit/blow_electric/light_1_blow.mse", true},
	{ CInstanceBase::EFFECT_ELECTRIC_ATTACH, "", "d:/ymir work/effect/hit/blow_electric/light_1_body.mse", true},
	{ CInstanceBase::EFFECT_LEVELUP, "", "d:/ymir work/effect/etc/levelup_1/level_up.mse", true},
	{ CInstanceBase::EFFECT_SKILLUP, "", "d:/ymir work/effect/etc/skillup/skillup_1.mse", true},
	{ CInstanceBase::EFFECT_EMPIRE+1, "Bip01", "d:/ymir work/effect/etc/empire/empire_A.mse", true},
	{ CInstanceBase::EFFECT_EMPIRE+2, "Bip01", "d:/ymir work/effect/etc/empire/empire_B.mse", true},
	{ CInstanceBase::EFFECT_EMPIRE+3, "Bip01", "d:/ymir work/effect/etc/empire/empire_C.mse", true},
	{ CInstanceBase::EFFECT_WEAPON+1, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_sword_loop.mse", true},
	{ CInstanceBase::EFFECT_WEAPON+2, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_spear_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+0, "Bip01", "locale/pt/effect/gm.mse", true},
	{ CInstanceBase::EFFECT_BOSS, "", "d:/ymir work/effect/etc/boss.mse", true},
	{ CInstanceBase::EFFECT_MINI_BOSS, "", "d:/ymir work/effect/etc/mini_boss.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_POISON, "Bip01", "d:/ymir work/effect/hit/blow_poison/poison_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_SLOW, "", "d:/ymir work/effect/affect/slow.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_STUN, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_CHEONGEUN, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_GYEONGGONG, "", "d:/ymir work/pc/assassin/effect/gyeonggong_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_GWIGEOM, "Bip01 R Finger2", "d:/ymir work/pc/sura/effect/gwigeom_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_GONGPO, "", "d:/ymir work/pc/sura/effect/fear_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_JUMAGAP, "", "d:/ymir work/pc/sura/effect/jumagap_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_HOSIN, "", "d:/ymir work/pc/shaman/effect/3hosin_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_BOHO, "", "d:/ymir work/pc/shaman/effect/boho_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_KWAESOK, "", "d:/ymir work/pc/shaman/effect/10kwaesok_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_HEUKSIN, "", "d:/ymir work/pc/sura/effect/heuksin_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_MUYEONG, "", "d:/ymir work/pc/sura/effect/muyeong_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_GICHEON, "Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_JEUNGRYEOK, "Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_PABEOP, "Bip01 Head", "d:/ymir work/pc/sura/effect/pabeop_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_FALLEN_CHEONGEUN, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", true},
	{ CInstanceBase::EFFECT_REFINED +1, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7.mse", true},
	{ CInstanceBase::EFFECT_REFINED +2, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8.mse", true},
	{ CInstanceBase::EFFECT_REFINED +3, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9.mse", true},
	{ CInstanceBase::EFFECT_REFINED +4, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7_b.mse", true},
	{ CInstanceBase::EFFECT_REFINED +5, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_8_b.mse", true},
	{ CInstanceBase::EFFECT_REFINED +6, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_9_b.mse", true},
	{ CInstanceBase::EFFECT_REFINED +7, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7_f.mse", true},
	{ CInstanceBase::EFFECT_REFINED +8, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8_f.mse", true},
	{ CInstanceBase::EFFECT_REFINED +9, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9_f.mse", true},
	{ CInstanceBase::EFFECT_REFINED +10, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +11, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +12, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +13, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +14, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_8_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +15, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_9_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +16, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_7.mse", true},
	{ CInstanceBase::EFFECT_REFINED +17, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_8.mse", true},
	{ CInstanceBase::EFFECT_REFINED +18, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_9.mse", true},
	{ CInstanceBase::EFFECT_REFINED +19, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-4-2-1.mse", true},
	{ CInstanceBase::EFFECT_REFINED +20, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-4-2-2.mse", true},
	{ CInstanceBase::EFFECT_REFINED +21, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-5-1.mse", true},
	{ CInstanceBase::EFFECT_REFINED +22, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-5_jin.mse", true},
	{ CInstanceBase::EFFECT_REFINED +23, "Bip01", "D:/ymir work/pc/common/effect/armor/acc_01.mse", true},
	{ CInstanceBase::EFFECT_REFINED +24, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_7th_01.mse", true},
	{ CInstanceBase::EFFECT_REFINED +25, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7th.mse", true},
	{ CInstanceBase::EFFECT_REFINED +26, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7th_b.mse", true},
	{ CInstanceBase::EFFECT_REFINED +27, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7th_f.mse", true},
	{ CInstanceBase::EFFECT_REFINED +28, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7th_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +29, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7th_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED +30, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/element02_fire.mse", true},
	{ CInstanceBase::EFFECT_OWN_OFFLINE_SHOP, "", "D:/ymir work/effect/npc2/p_shop_my_shop.mse", true},
	{CInstanceBase::EFFECT_DUST, "", "d:/ymir work/effect/etc/dust/dust.mse", true},
	{CInstanceBase::EFFECT_HORSE_DUST, "", "d:/ymir work/effect/etc/dust/running_dust.mse", true},
	{CInstanceBase::EFFECT_HIT, "", "d:/ymir work/effect/hit/blow_1/blow_1_low.mse", true},
	{CInstanceBase::EFFECT_HPUP_RED, "", "d:/ymir work/effect/etc/recuperation/drugup_red.mse", true},
	{CInstanceBase::EFFECT_SPUP_BLUE, "", "d:/ymir work/effect/etc/recuperation/drugup_blue.mse", true},
	{CInstanceBase::EFFECT_SPEEDUP_GREEN, "", "d:/ymir work/effect/etc/recuperation/drugup_green.mse", true},
	{CInstanceBase::EFFECT_DXUP_PURPLE, "", "d:/ymir work/effect/etc/recuperation/drugup_purple.mse", true},
	{CInstanceBase::EFFECT_AUTO_HPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_red.mse", true},
	{CInstanceBase::EFFECT_AUTO_SPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_blue.mse", true},
	{CInstanceBase::EFFECT_RAMADAN_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item1.mse", true},
	{CInstanceBase::EFFECT_HALLOWEEN_CANDY_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item2.mse", true},
	{CInstanceBase::EFFECT_HAPPINESS_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item3.mse", true},
	{CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item4.mse", true},
	{CInstanceBase::EFFECT_MAGIC_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item5.mse", true},
	{CInstanceBase::EFFECT_ACCE_SUCESS_ABSORB, "", "d:/ymir work/effect/etc/buff/buff_item6.mse", true},
	{CInstanceBase::EFFECT_ACCE_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item7.mse", true},
	{CInstanceBase::EFFECT_EASTER_CANDY_EQIP, "", "d:/ymir work/effect/etc/buff/buff_item8.mse", true},
	{CInstanceBase::EFFECT_PENETRATE, "Bip01", "d:/ymir work/effect/hit/gwantong.mse", true},
	{CInstanceBase::EFFECT_FIRECRACKER, "", "d:/ymir work/effect/etc/firecracker/newyear_firecracker.mse", true},
	{CInstanceBase::EFFECT_SPIN_TOP, "", "d:/ymir work/effect/etc/firecracker/paing_i.mse", true},
	{CInstanceBase::EFFECT_SELECT, "", "d:/ymir work/effect/etc/click/click_select.mse", true},
	{CInstanceBase::EFFECT_TARGET, "", "d:/ymir work/effect/etc/click/click_glow_select.mse", true},
	{CInstanceBase::EFFECT_STUN, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun.mse", true},
	{CInstanceBase::EFFECT_CRITICAL, "Bip01 R Hand", "d:/ymir work/effect/hit/critical.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_TARGET, "", "d:/ymir work/effect/affect/damagevalue/target.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_NOT_TARGET, "", "d:/ymir work/effect/affect/damagevalue/nontarget.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE, "", "d:/ymir work/effect/affect/damagevalue/damage.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE2, "", "d:/ymir work/effect/affect/damagevalue/damage_1.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_POISON, "", "d:/ymir work/effect/affect/damagevalue/poison.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_MISS, "", "d:/ymir work/effect/affect/damagevalue/miss.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_TARGETMISS, "", "d:/ymir work/effect/affect/damagevalue/target_miss.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_CRITICAL, "", "d:/ymir work/effect/affect/damagevalue/critical.mse", true},
	{CInstanceBase::EFFECT_DAMAGE_FIRE, "", "d:/ymir work/effect/affect/damagevalue/burn.mse", true},
	{CInstanceBase::EFFECT_PERCENT_DAMAGE1, "", "d:/ymir work/effect/hit/percent_damage1.mse", true},
	{CInstanceBase::EFFECT_PERCENT_DAMAGE2, "", "d:/ymir work/effect/hit/percent_damage2.mse", true},
	{CInstanceBase::EFFECT_PERCENT_DAMAGE3, "", "d:/ymir work/effect/hit/percent_damage3.mse", true},
	{CInstanceBase::EFFECT_THUNDER_AREA, "", "d:/ymir work/effect/monster/light_emissive3.mse", false},
	{CInstanceBase::EFFECT_THUNDER, "", "d:/ymir work/effect/monster/yellow_tigerman_24_1.mse", false},
	{CInstanceBase::EFFECT_HEAL, "", "d:/ymir work/pc/shaman/effect/jeongeop_2.mse", false},
	{CInstanceBase::EFFECT_CAPE_OF_COURAGE, "", "d:/ymir work/effect/etc/buff/buff_item9.mse", false},
	{CInstanceBase::EFFECT_CHOCOLATE_PENDANT, "", "d:/ymir work/effect/etc/buff/buff_item10.mse", false},
	{CInstanceBase::EFFECT_PEPSI_EVENT, "", "d:/ymir work/effect/etc/buff/buff_item11.mse", false},
	{CInstanceBase::EFFECT_DRAGONLAIR_STONE_UNBEATABLE_1, "", "d:/ymir work/effect/monster2/redd_moojuk.mse", false},
	{CInstanceBase::EFFECT_DRAGONLAIR_STONE_UNBEATABLE_2, "", "d:/ymir work/effect/monster2/redd_moojuk.mse", false},
	{CInstanceBase::EFFECT_DRAGONLAIR_STONE_UNBEATABLE_3, "", "d:/ymir work/effect/monster2/redd_moojuk_blue.mse", false},
	{CInstanceBase::EFFECT_DRAGONLAIR_STONE_UNBEATABLE_4, "", "d:/ymir work/effect/monster2/redd_moojuk_green.mse", false},
	{CInstanceBase::EFFECT_BATTLE_POTION, "", "d:/ymir work/effect/etc/buff/buff_item12.mse", false},
	{CInstanceBase::EFFECT_NAZAR_PENDANT, "", "d:/ymir work/effect/etc/buff/buff_item13.mse", false},
	{CInstanceBase::EFFECT_GUARDIAN_PENDANT, "", "d:/ymir work/effect/etc/buff/buff_item14.mse", false},
	{CInstanceBase::EFFECT_FLOWER_EVENT, "", "d:/ymir work/effect/etc/buff/buff_item15_flower.mse", false},
	{CInstanceBase::EFFECT_GEM_PENDANT, "", "d:/ymir work/effect/etc/buff/buff_item16.mse", false},
};

const constexpr uint8_t INTRO_MOTION_NUM = 3;
typedef std::pair<DWORD, const char*> TIMotionList;
constexpr std::array<TIMotionList, INTRO_MOTION_NUM> vecIntroMotions =
{ {
		{CRaceMotionData::NAME_INTRO_WAIT, "wait.msa"},
		{CRaceMotionData::NAME_INTRO_SELECTED, "selected.msa"},
		{CRaceMotionData::NAME_INTRO_NOT_SELECTED, "not_selected.msa"},
} };

const constexpr uint8_t FISHING_MOTION_NUM = 8;
typedef std::pair<DWORD, const char*> TFishing;
constexpr std::array<TFishing, FISHING_MOTION_NUM> vecFishing =
{ {
		{CRaceMotionData::NAME_WAIT, "wait.msa"},
		{CRaceMotionData::NAME_WALK, "walk.msa"},
		{CRaceMotionData::NAME_FISHING_THROW, "throw.msa"},
		{CRaceMotionData::NAME_FISHING_WAIT, "fishing_wait.msa"},
		{CRaceMotionData::NAME_FISHING_STOP, "fishing_cancel.msa"},
		{CRaceMotionData::NAME_FISHING_REACT, "fishing_react.msa"},
		{CRaceMotionData::NAME_FISHING_CATCH, "fishing_catch.msa"},
		{CRaceMotionData::NAME_FISHING_FAIL, "fishing_fail.msa"},
} };

const constexpr uint8_t GUILD_SKILL_MOTION_NUM = 6;
typedef std::pair<DWORD, const char*> TGuildSkill;
constexpr std::array<TGuildSkill, GUILD_SKILL_MOTION_NUM> vecGuildSkill =
{ {
		{CRaceMotionData::NAME_SKILL + 107, "guild_yongsinuipi.msa"},
		{CRaceMotionData::NAME_SKILL + 108, "guild_yongsinuichukbok.msa"},
		{CRaceMotionData::NAME_SKILL + 109, "guild_seonghwigap.msa"},
		{CRaceMotionData::NAME_SKILL + 110, "guild_gasokhwa.msa"},
		{CRaceMotionData::NAME_SKILL + 111, "guild_yongsinuibunno.msa"},
		{CRaceMotionData::NAME_SKILL + 112, "guild_jumunsul.msa"},
} };

typedef std::pair<DWORD, const char*> TRaceData;
constexpr std::array<TRaceData, NPlayerData::MAIN_RACE_MAX_NUM> vecRaceData =
{ {
		{NPlayerData::MAIN_RACE_WARRIOR_M, "d:/ymir work/pc/warrior/"},
		{NPlayerData::MAIN_RACE_WARRIOR_W, "d:/ymir work/pc2/warrior/"},
		{NPlayerData::MAIN_RACE_ASSASSIN_W, "d:/ymir work/pc/assassin/"},
		{NPlayerData::MAIN_RACE_ASSASSIN_M, "d:/ymir work/pc2/assassin/"},
		{NPlayerData::MAIN_RACE_SURA_M, "d:/ymir work/pc/sura/"},
		{NPlayerData::MAIN_RACE_SURA_W, "d:/ymir work/pc2/sura/"},
		{NPlayerData::MAIN_RACE_SHAMAN_W, "d:/ymir work/pc/shaman/"},
		{NPlayerData::MAIN_RACE_SHAMAN_M, "d:/ymir work/pc2/shaman/"},
} };

enum ComboType
{
	COMBO_TYPE_1,
	COMBO_TYPE_2,
	COMBO_TYPE_3,
};

enum ComboIndex
{
	COMBO_INDEX_1,
	COMBO_INDEX_2,
	COMBO_INDEX_3,
	COMBO_INDEX_4,
	COMBO_INDEX_5,
	COMBO_INDEX_6,
};

constexpr uint8_t GUILD_MATERIAL_NUM = 3;

enum EGuildMaterialItems
{
	MATERIAL_STONE_ID = 90010,
	MATERIAL_LOG_ID,
	MATERIAL_PLYWOOD_ID,
};

static const uint8_t getGuildMaterialIndex(const int32_t& vnum)
{
	switch (vnum)
	{
	case MATERIAL_STONE_ID:
	case MATERIAL_LOG_ID:
	case MATERIAL_PLYWOOD_ID:
		return vnum - 90010;
	}

	return -1;
}

#ifdef ENABLE_GENDER_ALIGNMENT
typedef std::map<const char*, const char*> TTitleNameList;
static const TTitleNameList titleNameList{
	{"Cavaleiro", "Amazona"},
	{"Nobre", "Nobre"},
	{"Bom", "Boa"},
	{"Amigável", "Amigável"},
	{"Neutro", "Neutra"},
	{"Agressivo", "Agressiva"},
	{"Fraudulento", "Fraudulenta"},
	{"Malicioso", "Maliciosa"},
	{"Cruel", "Cruel"}
};
#else
static const std::array<const char*, 9> titleNameList{ "PVP_LEVEL0", "PVP_LEVEL1", "PVP_LEVEL2", "PVP_LEVEL3", "PVP_LEVEL4", "PVP_LEVEL5", "PVP_LEVEL6", "PVP_LEVEL7", "PVP_LEVEL8"};
#endif

#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
static const std::array<const char*, 2> leaderGradeName{"[Líder] ", "[General]"};
#endif

typedef std::map<uint8_t, RGB> TCharacterColorMap;
static const TCharacterColorMap characterNameColors
{
	{ CInstanceBase::NAMECOLOR_NORMAL_PC, { 255, 215, 76 }},
	{ CInstanceBase::NAMECOLOR_NORMAL_NPC, { 122, 231, 93 }},
	{ CInstanceBase::NAMECOLOR_NORMAL_MOB, { 235, 22, 9 }},
	{ CInstanceBase::NAMECOLOR_PVP, { 238, 54, 223 }},
	{ CInstanceBase::NAMECOLOR_PK, { 180, 100, 0 }},
	{ CInstanceBase::NAMECOLOR_PARTY, { 128, 192, 255 }},
	{ CInstanceBase::NAMECOLOR_WARP, { 136, 218, 241 }},
	{ CInstanceBase::NAMECOLOR_WAYPOINT, { 255, 255, 255 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_MOB, { 235, 22, 9 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_NPC, { 122, 231, 93 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_PC + 1, { 157, 0, 0 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_PC + 2, { 222, 160, 47 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_PC + 3, { 23, 30, 138 }},
	{ CInstanceBase::NAMECOLOR_PET, { 122, 231, 93 }},
	{ CInstanceBase::NAMECOLOR_OFFLINESHOP, { 128, 192, 255 }},
	{ CInstanceBase::NAMECOLOR_STONE, { 255, 255, 255 }},
	{ CInstanceBase::NAMECOLOR_MINI_BOSS, { 102, 102, 255 }},
	{ CInstanceBase::NAMECOLOR_BOSS, { 255, 204, 153 }},
};

static const TCharacterColorMap characterTitleColors
{
	{ 0, { 0, 204, 255 }},
	{ 1, { 0, 144, 255 }},
	{ 2, { 92, 110, 255 }},
	{ 3, { 155, 155, 255 }},
	{ 4, { 255, 255, 255 }},
	{ 5, { 207, 117, 0 }},
	{ 6, { 235, 83, 0 }},
	{ 7, { 227, 0, 0 }},
	{ 8, { 255, 0, 0 }},
	{ 9, { 220, 20, 60 }},
};

static const std::map<uint8_t, const char*> emotionIcons =
{
	{ CPythonPlayer::EMOTION_CLAP, "d:/ymir work/ui/game/windows/emotion_clap.sub" },
	{ CPythonPlayer::EMOTION_CHEERS_1, "d:/ymir work/ui/game/windows/emotion_cheers_1.sub" },
	{ CPythonPlayer::EMOTION_CHEERS_2, "d:/ymir work/ui/game/windows/emotion_cheers_2.sub" },
	{ CPythonPlayer::EMOTION_DANCE_1, "icon/action/dance1.tga" },
	{ CPythonPlayer::EMOTION_DANCE_2, "icon/action/dance2.tga" },
	{ CPythonPlayer::EMOTION_CONGRATULATION, "icon/action/congratulation.tga" },
	{ CPythonPlayer::EMOTION_FORGIVE, "icon/action/forgive.tga" },
	{ CPythonPlayer::EMOTION_ANGRY, "icon/action/angry.tga" },
	{ CPythonPlayer::EMOTION_ATTRACTIVE, "icon/action/attractive.tga" },
	{ CPythonPlayer::EMOTION_SAD, "icon/action/sad.tga" },
	{ CPythonPlayer::EMOTION_SHY, "icon/action/shy.tga" },
	{ CPythonPlayer::EMOTION_CHEERUP, "icon/action/cheerup.tga" },
	{ CPythonPlayer::EMOTION_BANTER, "icon/action/banter.tga" },
	{ CPythonPlayer::EMOTION_JOY, "icon/action/joy.tga" },
	{ CPythonPlayer::EMOTION_DANCE_1, "icon/action/dance1.tga" },
	{ CPythonPlayer::EMOTION_DANCE_2, "icon/action/dance2.tga" },
	{ CPythonPlayer::EMOTION_DANCE_3, "icon/action/dance3.tga" },
	{ CPythonPlayer::EMOTION_DANCE_4, "icon/action/dance4.tga" },
	{ CPythonPlayer::EMOTION_DANCE_5, "icon/action/dance5.tga" },
	{ CPythonPlayer::EMOTION_DANCE_6, "icon/action/dance6.tga" },
	{ CPythonPlayer::EMOTION_KISS, "d:/ymir work/ui/game/windows/emotion_kiss.sub" },
	{ CPythonPlayer::EMOTION_FRENCH_KISS, "d:/ymir work/ui/game/windows/emotion_french_kiss.sub" },
	{ CPythonPlayer::EMOTION_SLAP, "d:/ymir work/ui/game/windows/emotion_slap.sub" },
	{ CPythonPlayer::EMOTION_PUSHUP, "icon/action/pushup.tga" },
	{ CPythonPlayer::EMOTION_DANCE_7, "icon/action/dance7.tga" },
	{ CPythonPlayer::EMOTION_EXERCISE, "icon/action/exercise.tga" },
	{ CPythonPlayer::EMOTION_DOZE, "icon/action/doze.tga" },
	{ CPythonPlayer::EMOTION_SELFIE, "icon/action/selfie.tga" },
	{ CPythonPlayer::EMOTION_CHARGING, "icon/action/charging.tga" },
	{ CPythonPlayer::EMOTION_NOSAY, "icon/action/nosay.tga" },
	{ CPythonPlayer::EMOTION_WEATHER_1, "icon/action/weather1.tga" },
	{ CPythonPlayer::EMOTION_WEATHER_2, "icon/action/weather2.tga" },
	{ CPythonPlayer::EMOTION_WEATHER_3, "icon/action/weather3.tga" },
	{ CPythonPlayer::EMOTION_HUNGRY, "icon/action/hungry.tga" },
	{ CPythonPlayer::EMOTION_SIREN, "icon/action/siren.tga" },
	{ CPythonPlayer::EMOTION_LETTER, "icon/action/letter.tga" },
	{ CPythonPlayer::EMOTION_CALL, "icon/action/call.tga" },
	{ CPythonPlayer::EMOTION_CELEBRATION, "icon/action/celebration.tga" },
	{ CPythonPlayer::EMOTION_ALCOHOL, "icon/action/alcohol.tga" },
	{ CPythonPlayer::EMOTION_BUSY, "icon/action/busy.tga" },
	{ CPythonPlayer::EMOTION_WHIRL, "icon/action/whirl.tga" }
};

typedef struct SRaceHeightRegist { int raceVnum; double height; } TRaceHeightRegist;
static const std::vector<TRaceHeightRegist> raceHeight =
{
	{20029, 180.00},
	{20030, 180.00},
	{20101, 220.00},
	{20102, 220.00},
	{20103, 220.00},
	{20104, 220.00},
	{20105, 220.00},
	{20106, 220.00},
	{20107, 220.00},
	{20108, 220.00},
	{20109, 220.00},
	{30000, 110.00},
	{30006, 95.00},
	{30007, 95.00},
	{30008, 95.00},
	{20110, 140.00},
	{20111, 140.00},
	{20112, 160.00},
	{20113, 160.00},
	{20114, 160.00},
	{20115, 140.00},
	{20116, 140.00},
	{20117, 160.00},
	{20118, 160.00},
	{20119, 180.00},
	{20120, 160.00},
	{20121, 160.00},
	{20122, 160.00},
	{20123, 160.00},
	{20124, 160.00},
	{20125, 160.00},
	{20126, 160.00},
	{20201, 140.00},
	{20202, 140.00},
	{20203, 160.00},
	{20204, 160.00},
	{20205, 160.00},
	{20206, 140.00},
	{20207, 160.00},
	{20208, 160.00},
	{20209, 140.00},
	{20210, 140.00},
	{20211, 160.00},
	{20212, 160.00},
	{20213, 160.00},
	{20214, 160.00},
	{20215, 160.00},
	{20216, 160.00},
	{20217, 160.00},
	{20218, 160.00},
	{20219, 160.00},
	{20220, 160.00},
	{20221, 160.00},
	{20222, 160.00},
	{20223, 160.00},
	{20224, 160.00},
	{20225, 160.00},
	{20226, 160.00},
	{20227, 160.00},
	{20228, 160.00},
	{20229, 160.00},
	{20230, 160.00},
	{20231, 160.00},
	{20232, 160.00},
	{20233, 160.00},
	{20234, 160.00},
	{20235, 160.00},
	{20236, 160.00},
	{20237, 160.00},
	{20238, 160.00},
	{20239, 160.00},
	{20240, 160.00},
	{20241, 160.00},
	{20242, 160.00},
	{20243, 160.00},
	{20244, 180.00},
	{20245, 180.00},
	{20246, 230.00},
	{20247, 230.00},
	{20248, 220.00},
	{20249, 220.00},
	{20250, 160.00},
	{20251, 160.00},
	{20252, 220.00},
	{20253, 160.00},
	{20254, 160.00},
	{20255, 160.00},
	{20256, 160.00},
	{20257, 160.00},
	{20258, 160.00},
	{20259, 160.00},
	{20260, 160.00},
	{20261, 160.00},
	{20262, 160.00},
	{20263, 160.00},
	{20264, 160.00},
	{20265, 160.00},
	{20266, 160.00},
	{2307, 550.00},
	{3391, 360.00},
	{3891, 335.00},
	{3390, 295.00},
	{3913, 550.00},
	{2493, 300.00},
	{34023, 55.00},
	{34024, 55.00},
	{34030, 200.00},
	{34001, 250.00},
	{34003, 250.00},
	{34008, 70.00},
	{34011, 70.00},
	{34012, 70.00},
	{34007, 70.00},
	{34019, 70.00},
	{34016, 250.00},
	{34020, 70.00},
	{34027, 140.00},
	{34031, 215.00},
	{34032, 250.00},
	{34033, 250.00},
	{34055, 250.00},
	{34056, 250.00},
	{34057, 250.00},
	{34062, 250.00},
	{34063, 250.00},
	{34090, 250.00},
	{34091, 250.00},
	{34095, 250.00},
	{34096, 250.00},
	{34085, 100.00},
	{34086, 110.00},
	{34087, 110.00},
	{34088, 110.00},
	{34097, 160.00},
	{34098, 160.00},
	{34099, 160.00},
	{34041, 60.00},
	{34042, 60.00},
	{34045, 55.00},
	{34046, 55.00},
	{34073, 70.00},
	{34074, 70.00},
	{34075, 70.00},
	{34076, 70.00},
	{34080, 70.00},
	{34081, 70.00},
	{34082, 70.00},
	{34083, 70.00},
	{34036, 180.00},
	{34037, 180.00}
};

static const std::map<uint8_t, std::map<uint8_t, std::vector<uint8_t>>> skillIndexes{
	{ NRaceData::JOB_WARRIOR, {
		{1, {1, 2, 3, 4, 5, 6, 0, 0, 137, 0, 138, 0, 139, 0}},
		{2, {16, 17, 18, 19, 20, 21, 0, 0, 137, 0, 138, 0, 139, 0}},
	}},
	{ NRaceData::JOB_ASSASSIN, {
		{1, {31, 32, 33, 34, 35, 36, 0, 0, 137, 0, 138, 0, 139, 0, 140}},
		{2, {46, 47, 48, 49, 50, 51, 0, 0, 137, 0, 138, 0, 139, 0, 140}},
	}},
	{ NRaceData::JOB_SURA, {
		{1, {61, 62, 63, 64, 65, 66, 0, 0, 137, 0, 138, 0, 139, 0}},
		{2, {76, 77, 78, 79, 80, 81, 0, 0, 137, 0, 138, 0, 139, 0}},
	}},
	{ NRaceData::JOB_SHAMAN, {
		{1, {91, 92, 93, 94, 95, 96, 0, 0, 137, 0, 138, 0, 139, 0}},
		{2, {106, 107, 108, 109, 110, 111, 0, 0, 137, 0, 138, 0, 139, 0}},
	}},
};

constexpr std::array<uint8_t, 12> supportSkillIndexes{
	{201, 202, 203, 204, 205, 206, 122, 121, 124, 130, 131, 0},
};

static const std::map<uint8_t, std::vector<uint8_t>> horseSkillIndexes{
	{ NRaceData::JOB_WARRIOR, {137, 138, 139, 0, 0, 0} },
	{ NRaceData::JOB_ASSASSIN, {137, 138, 139, 140, 0, 0} },
	{ NRaceData::JOB_SURA, {137, 138, 139, 0, 0, 0} },
	{ NRaceData::JOB_SHAMAN, {137, 138, 139, 0, 0, 0} }
};

static const std::map < std::string, std::vector<uint8_t>> guildSkills{
	{ "PASSIVE", { 151 }},
	{ "ACTIVE", { 152, 153, 154, 155, 156, 157 }},
};
