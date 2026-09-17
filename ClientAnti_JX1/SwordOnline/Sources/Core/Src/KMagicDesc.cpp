#include "KCore.h"

#include "KEngine.h"
#include "KMagicAttrib.h"
#include "KMagicDesc.h"
#include "GameDataDef.h"
#include "KFaction.h"

#define		MAGICDESC_FILE		"\\settings\\MagicDesc.Ini"
extern const char * g_MagicID2String(int nAttrib);
const char MAGIC_ATTRIB_STRING[magic_end + 1][100] = 
{
	/* Same order as enum MAGIC_ATTRIB (jx9tn); generated, see KMagicAttrib.h. */
	"skill_begin",	// 0
	"skill_cost_v",	// 1
	"skill_costtype_v",	// 2
	"skill_mintimepercast_v",	// 3
	"skill_misslenum_v",	// 4
	"skill_misslesform_v",	// 5
	"skill_param1_v",	// 6
	"skill_param2_v",	// 7
	"skill_skillexp_v",	// 8
	"skill_waittime",	// 9
	"skill_mintimepercastonhorse_v",	// 10
	"skill_appendskill",	// 11
	"skill_eventskilllevel",	// 12
	"skill_end",	// 13
	"missle_begin",	// 14
	"missle_movekind_v",	// 15
	"missle_speed_v",	// 16
	"missle_lifetime_v",	// 17
	"missle_height_v",	// 18
	"missle_damagerange_v",	// 19
	"missle_radius_v",	// 20
	"missle_missrate",	// 21
	"missle_hitcount",	// 22
	"missle_reserve3",	// 23
	"missle_reserve4",	// 24
	"missle_reserve5",	// 25
	"missle_end",	// 26
	"item_begin",	// 27
	"weapondamagemin_v",	// 28
	"weapondamagemax_v",	// 29
	"armordefense_v",	// 30
	"durability_v",	// 31
	"requirestr",	// 32
	"requiredex",	// 33
	"requirevit",	// 34
	"requireeng",	// 35
	"requirelevel",	// 36
	"requireseries",	// 37
	"requiresex",	// 38
	"requiremenpai",	// 39
	"weapondamageenhance_p",	// 40
	"armordefenseenhance_p",	// 41
	"requirementreduce_p",	// 42
	"indestructible_b",	// 43
	"item_usagetime_v",	// 44
	"req_levelortranslife",	// 45
	"require_translife",	// 46
	"require_fortune_value",	// 47
	"item_reserve5",	// 48
	"item_reserve6",	// 49
	"item_reserve7",	// 50
	"item_reserve8",	// 51
	"item_reserve9",	// 52
	"item_reserve10",	// 53
	"item_end",	// 54
	"damage_begin",	// 55
	"attackrating_v",	// 56
	"attackrating_p",	// 57
	"ignoredefense_p",	// 58
	"physicsdamage_v",	// 59
	"colddamage_v",	// 60
	"firedamage_v",	// 61
	"lightingdamage_v",	// 62
	"poisondamage_v",	// 63
	"magicdamage_v",	// 64
	"physicsenhance_p",	// 65
	"steallife_p",	// 66
	"stealmana_p",	// 67
	"stealstamina_p",	// 68
	"knockback_p",	// 69
	"deadlystrike_p",	// 70
	"fatallystrike_p",	// 71
	"stun_p",	// 72
	"addskillexp1",	// 73
	"addskillexp2",	// 74
	"seriesdamage_p",	// 75
	"damage_reserve4",	// 76
	"damage_reserve5",	// 77
	"damage_reserve6",	// 78
	"damage_reserve7",	// 79
	"damage_reserve8",	// 80
	"damage_reserve9",	// 81
	"damage_reserve10",	// 82
	"damage_end",	// 83
	"normal_begin",	// 84
	"lifemax_v",	// 85
	"lifemax_p",	// 86
	"life_v",	// 87
	"lifereplenish_v",	// 88
	"manamax_v",	// 89
	"manamax_p",	// 90
	"mana_v",	// 91
	"manareplenish_v",	// 92
	"staminamax_v",	// 93
	"staminamax_p",	// 94
	"stamina_v",	// 95
	"staminareplenish_v",	// 96
	"strength_v",	// 97
	"dexterity_v",	// 98
	"vitality_v",	// 99
	"energy_v",	// 100
	"poisonres_p",	// 101
	"fireres_p",	// 102
	"lightingres_p",	// 103
	"physicsres_p",	// 104
	"coldres_p",	// 105
	"freezetimereduce_p",	// 106
	"burntimereduce_p",	// 107
	"poisontimereduce_p",	// 108
	"poisondamagereduce_v",	// 109
	"stuntimereduce_p",	// 110
	"fastwalkrun_p",	// 111
	"visionradius_p",	// 112
	"fasthitrecover_v",	// 113
	"allres_p",	// 114
	"attackspeed_v",	// 115
	"castspeed_v",	// 116
	"meleedamagereturn_v",	// 117
	"meleedamagereturn_p",	// 118
	"rangedamagereturn_v",	// 119
	"rangedamagereturn_p",	// 120
	"addphysicsdamage_v",	// 121
	"addfiredamage_v",	// 122
	"addcolddamage_v",	// 123
	"addlightingdamage_v",	// 124
	"addpoisondamage_v",	// 125
	"addphysicsdamage_p",	// 126
	"slowmissle_b",	// 127
	"changecamp_b",	// 128
	"physicsarmor_v",	// 129
	"coldarmor_v",	// 130
	"firearmor_v",	// 131
	"poisonarmor_v",	// 132
	"lightingarmor_v",	// 133
	"damage2addmana_p",	// 134
	"lucky_v",	// 135
	"steallifeenhance_p",	// 136
	"stealmanaenhance_p",	// 137
	"stealstaminaenhance_p",	// 138
	"allskill_v",	// 139
	"metalskill_v",	// 140
	"woodskill_v",	// 141
	"waterskill_v",	// 142
	"fireskill_v",	// 143
	"earthskill_v",	// 144
	"knockbackenhance_p",	// 145
	"deadlystrikeenhance_p",	// 146
	"stunenhance_p",	// 147
	"badstatustimereduce_v",	// 148
	"manashield_p",	// 149
	"adddefense_v",	// 150
	"adddefense_p",	// 151
	"fatallystrikeenhance_p",	// 152
	"lifepotion_v",	// 153
	"manapotion_v",	// 154
	"physicsresmax_p",	// 155
	"coldresmax_p",	// 156
	"fireresmax_p",	// 157
	"lightingresmax_p",	// 158
	"poisonresmax_p",	// 159
	"allresmax_p",	// 160
	"coldenhance_p",	// 161
	"fireenhance_p",	// 162
	"lightingenhance_p",	// 163
	"poisonenhance_p",	// 164
	"magicenhance_p",	// 165
	"attackratingenhance_v",	// 166
	"attackratingenhance_p",	// 167
	"addphysicsmagic_v",	// 168
	"addcoldmagic_v",	// 169
	"addfiremagic_v",	// 170
	"addlightingmagic_v",	// 171
	"addpoisonmagic_v",	// 172
	"fatallystrikeres_p",	// 173
	"statusimmunity_b",	// 174
	"expenhance_v",	// 175
	"expenhance_p",	// 176
	"seriesres_p",	// 177
	"seriesenhance_p",	// 178
	"createnpc",	// 179
	"autoattacknpc",	// 180
	"dynamicmagicshield_v",	// 181
	"nomovespeed",	// 182
	"execscript",	// 183
	"changefeature1",	// 184
	"changefeature2",	// 185
	"stealfeature",	// 186
	"addstealfeatureskill",	// 187
	"lucky_v_partner",	// 188
	"listen_msg",	// 189
	"lifereplenish_p",	// 190
	"ignoreskill_p",	// 191
	"returnskill_p",	// 192
	"poisondamagereturn_v",	// 193
	"poisondamagereturn_p",	// 194
	"autoreplyskill",	// 195
	"autoattackskill",	// 196
	"autorescueskill",	// 197
	"autodeathskill",	// 198
	"randmove",	// 199
	"hide",	// 200
	"ignorenegativestate_p",	// 201
	"poison2decmana_p",	// 202
	"staticmagicshield_v",	// 203
	"staticmagicshield_p",	// 204
	"returnres_p",	// 205
	"add120skillexpenhance_p",	// 206
	"dec_percasttimehorse",	// 207
	"dec_percasttime",	// 208
	"enhance_709_auto",	// 209
	"enhance_708_life_p",	// 210
	"enhance_93_life_v",	// 211
	"enhance_711_auto",	// 212
	"enhance_714_auto",	// 213
	"enhance_717_auto",	// 214
	"enhance_723_miss_p",	// 215
	"showpersoninfo",	// 216
	"ignoredamage",	// 217
	"sorbdamage_p",	// 218
	"anti_hitrecover",	// 219
	"anti_stuntimereduce_p",	// 220
	"anti_poisonres_p",	// 221
	"anti_fireres_p",	// 222
	"anti_lightingres_p",	// 223
	"anti_physicsres_p",	// 224
	"anti_coldres_p",	// 225
	"block_rate",	// 226
	"enhancehit_rate",	// 227
	"poisonres_yan_p",	// 228
	"lightingres_yan_p",	// 229
	"fireres_yan_p",	// 230
	"physicsres_yan_p",	// 231
	"coldres_yan_p",	// 232
	"lifemax_yan_v",	// 233
	"lifemax_yan_p",	// 234
	"manamax_yan_v",	// 235
	"manamax_yan_p",	// 236
	"sorbdamage_yan_p",	// 237
	"fastwalkrun_yan_p",	// 238
	"attackspeed_yan_v",	// 239
	"castspeed_yan_v",	// 240
	"allres_yan_p",	// 241
	"anti_maxres_p",	// 242
	"skill_enhance",	// 243
	"magicdamage_p",	// 244
	"fasthitrecover_yan_v",	// 245
	"five_elements_enhance_v",	// 246
	"five_elements_resist_v",	// 247
	"manareplenish_p",	// 248
	"add_damage_p",	// 249
	"forbit_attack",	// 250
	"frozen_action",	// 251
	"forbit_takemedicine",	// 252
	"invincibility",	// 253
	"not_add_pkvalue_p",	// 254
	"add_boss_damage",	// 255
	"pk_punish_weaken",	// 256
	"pk_punish_enhance",	// 257
	"anti_poisontimereduce_p",	// 258
	"do_hurt_p",	// 259
	"anti_do_hurt_p",	// 260
	"do_stun_p",	// 261
	"anti_do_stun_p",	// 262
	"anti_physicsres_yan_p",	// 263
	"anti_poisonres_yan_p",	// 264
	"anti_coldres_yan_p",	// 265
	"anti_fireres_yan_p",	// 266
	"anti_lightingres_yan_p",	// 267
	"anti_allres_yan_p",	// 268
	"anti_sorbdamage_yan_p",	// 269
	"anti_block_rate",	// 270
	"anti_enhancehit_rate",	// 271
	"autocastskill",	// 272
	"transfigure",	// 273
	"oncastskill",	// 274
	"enhancehiteffect_rate",	// 275
	"me2metaldamage_p",	// 276
	"metal2medamage_p",	// 277
	"me2wooddamage_p",	// 278
	"wood2medamage_p",	// 279
	"me2waterdamage_p",	// 280
	"water2medamage_p",	// 281
	"me2firedamage_p",	// 282
	"fire2medamage_p",	// 283
	"me2earthdamage_p",	// 284
	"earth2medamage_p",	// 285
	"meleedamagereturnmana_p",	// 286
	"rangedamagereturnmana_p",	// 287
	"reduceskillcd1",	// 288
	"reduceskillcd2",	// 289
	"reduceskillcd3",	// 290
	"clearallcd",	// 291
	"addblockrate",	// 292
	"walkrunshadow",	// 293
	"",	// 294
	"candetonate1",	// 295
	"candetonate2",	// 296
	"candetonate3",	// 297
	"manatoskill_enhance",	// 298
	"melee_returnres_p",	// 299
	"range_returnres_p",	// 300
	"forbit_usetownportal",	// 301
	"normal_end",	// 302
	"skillexpbegin",	// 303
	"addskilldamage1",	// 304
	"addskilldamage2",	// 305
	"addskilldamage3",	// 306
	"addskilldamage4",	// 307
	"addskilldamage5",	// 308
	"addskilldamage6",	// 309
	"skill_attackradius",	// 310
	"skill_startevent",	// 311
	"skill_flyevent",	// 312
	"skill_collideevent",	// 313
	"skill_vanishedevent",	// 314
	"skill_dohurt",	// 315
	"skill_bymissle",	// 316
	"skill_showevent",	// 317
	"skill_desc",	// 318
	"",	// 319
	"",	// 320
	"",	// 321
	"",	// 322
	"skillexpend",	// 323
	"missle_exp_begin",	// 324
	"missle_range",	// 325
	"missle_dmginterval",	// 326
	"missle_zspeed",	// 327
	"missle_ablility",	// 328
	"missle_param",	// 329
	"missle_wait",	// 330
	"missle_fly",	// 331
	"missle_collide",	// 332
	"missle_vanish",	// 333
	"missle_exp_rev1",	// 334
	"missle_exp_rev2",	// 335
	"missle_exp_rev3",	// 336
	"missle_exp_rev4",	// 337
	"missle_exp_rev5",	// 338
	"missle_exp_end",	// 339
	"magic_end",	// 340
};



KMagicDesc	g_MagicDesc;
KMagicDesc::KMagicDesc()
{
	m_szDesc[0] = 0;
}

KMagicDesc::~KMagicDesc()
{
}

BOOL KMagicDesc::Init()
{
//	g_SetFilePath("\\");
	return (m_IniFile.Load(MAGICDESC_FILE));
}

const char* KMagicDesc::GetDesc(void *pData)
{
	
	char	szTempDesc[256];
	char*	pTempDesc = szTempDesc;
	int		i = 0;

	ZeroMemory(m_szDesc, sizeof(m_szDesc));
	
	if (!pData)
		return NULL;

	KMagicAttrib* pAttrib = (KMagicAttrib *)pData;

	const char	*pszKeyName = g_MagicID2String(pAttrib->nAttribType);
	m_IniFile.GetString("Descript", pszKeyName, "", szTempDesc, sizeof(szTempDesc));
	/* Sau thuoc tinh addskilldamage1..6 duoc DU LIEU khai voi tien to
	   "skill_" (ca ban ta, ban6 lan voz2 deu vay) trong khi bang ten noi bo
	   cua nguon 2003 ghi tran. Doi chieu ca bang: 197/211 ten khop san,
	   dung 6 ten nay lech tien to - nen thu them mot lan thay vi sua du lieu.
	   Thieu buoc nay thi mo ta tra ve rong va dong "Tang sat thuong [ten
	   chieu]" bien mat khong dau vet. */
	if (!szTempDesc[0])
	{
		char szKhac[64];
		if (strlen(pszKeyName) + 7 < sizeof(szKhac))
		{
			strcpy(szKhac, "skill_");
			strcat(szKhac, pszKeyName);
			m_IniFile.GetString("Descript", szKhac, "", szTempDesc, sizeof(szTempDesc));
		}
	}
	while(*pTempDesc)
	{
		if (*pTempDesc == '#')
		{
			int	nDescAddType = 0;
			/* Ban goc viet *pTempDesc + 3: do la gia tri cua '#' cong 3 (= '&'),
			   khong bao gio bang '+' - nen chu "Tang"/"Giam" cua #d chua tung
			   hien ra lan nao. */
			switch(*(pTempDesc + 3))
			{
			case '+':
				nDescAddType = 1;
				break;
			default:
				nDescAddType = 0;
				break;
			}
			int nValue = 0;
			
			switch(*(pTempDesc + 2))
			{
			case '1':
				nValue = pAttrib->nValue[0];
				break;
			case '2':
				nValue = pAttrib->nValue[1];
				break;
			case '3':
				nValue = pAttrib->nValue[2];
				break;
			default:
				nValue = pAttrib->nValue[0];
				break;
			}
			switch(*(pTempDesc+1))
			{
			case 'm':		// ÃÅÅÉ
				if (nValue >= 0 && nValue < MAX_FACTION)	// ten hien thi; chi so ngoai bang thi bo trong
				{
					strcat(m_szDesc, g_Faction.m_sAttribute[nValue].m_szShowName);
					i += strlen(g_Faction.m_sAttribute[nValue].m_szShowName);
				}
				break;
			case 's':		// ÎåÐÐ
				switch(nValue)
				{
				case series_metal:
					strcat(m_szDesc, "HÖ Kim");
					break;
				case series_wood:
					strcat(m_szDesc, "HÖ Méc");
					break;
				case series_water:
					strcat(m_szDesc, "HÖ Thñy");
					break;
				case series_fire:
					strcat(m_szDesc, "HÖ Háa");
					break;
				case series_earth:
					strcat(m_szDesc, "HÖ Thæ\n");
					break;
				default:
					strcat(m_szDesc, "V« HÖ");
					break;
				}
				i += 4;
				break;
			case 'k':		// ÏûºÄÀàÐÍ
				switch(nValue)
				{
				case 0:
					strcat(m_szDesc, "Néi Lùc");
					break;
				case 1:
					strcat(m_szDesc, "Sinh Lùc");
					break;
				case 2:
					strcat(m_szDesc, "ThÓ Lùc");
					break;
				case 3:
					strcat(m_szDesc, "TiÒn");
					break;
				default:
					strcat(m_szDesc, "Néi Lùc");
					break;
				}
				i += 4;
				break;
			case 'd':		// Êý×Ö
				{
					//if (nValue == 0)	// ÊýÖµÎªÁãµÄ»°£¬²»ÏÔÊ¾
					{
					//	return NULL;
					}

					/* KHONG tu them chu "Tang"/"Giam" nua: du lieu Viet hoa da viet
					   san chu do trong chinh nhan ("Tang tan cong chi mang: #d1+"),
					   them lan nua thanh "Tang tan cong chi mang: Tang80". So am van
					   ra dau tru qua sprintf("%d").
					   Ban goc khong bao gio chay nhanh nay - switch ben tren thieu mot
					   cap ngoac nen nDescAddType luon 0 - vi vay du lieu moi duoc viet
					   theo cach tu ke chu san. */
					char	szMsg[16];
					sprintf(szMsg, "%d", nValue);
					strcat(m_szDesc, szMsg);
					i += strlen(szMsg);
				}
				break;
			/* Hai the RIENG cua ta. Du lieu 8.x goi HAI so vao mot o nValue:
			     autoreplyskill  nValue[0] = idChieu*256 + capChieu
			                     nValue[2] = (giay*18)*256 + tyLe%
			   (doc tu script/skill/advancedskill.lua). Ban 8.x tra bang chi so
			   #d7/#d9/#f6 ma engine 2003 khong hieu - ca ba deu roi vao nhanh
			   default nen in RA CUNG MOT SO 5121 ba lan.
			     #p<i> = nValue[i] & 0xFF        (cap chieu, ty le %)
			     #q<i> = nValue[i] >> 8, /18     (khung hinh -> giay) */
			case 'p':
				{
					char	szMsg[16];
					sprintf(szMsg, "%d", nValue & 0xff);
					strcat(m_szDesc, szMsg);
					i += strlen(szMsg);
				}
				break;
			case 'q':
				{
					char	szMsg[16];
					sprintf(szMsg, "%d", (nValue >> 8) / 18);
					strcat(m_szDesc, szMsg);
					i += strlen(szMsg);
				}
				break;
			case 'x':		// ÐÔ±ð
				if (nValue)
					strcat(m_szDesc, "N÷ ");
				else
					strcat(m_szDesc, "Nam");
				i += 4;
				break;
			case 'l':		// ¼¼ÄÜ
				{
					char	szMsg[32];
					if (nValue > 0)
					{
						ISkill* pSkill =  g_SkillManager.GetSkill(nValue, 1);
						sprintf(szMsg, "<color=HGreen>[ %s ]<color>", pSkill->GetSkillName());
					}
					else
						sprintf(szMsg, "%s", "vâ c«ng vèn cã");
					strcat(m_szDesc, szMsg);
					i += strlen(szMsg);
				}
				break;
			default:
				break;
			}
			/* Ky tu thu tu chi thuoc ve the khi no la dau '+'/'-'. Nhay 4 vo dieu
			   kien thi "[#l1]" bi an mat dau ']' - ra "[[ ten chieu " mot ben. */
			if (*(pTempDesc + 3) == '+' || *(pTempDesc + 3) == '-')
				pTempDesc += 4;
			else
				pTempDesc += 3;
		}
		else
		{
			m_szDesc[i] = *pTempDesc;
			pTempDesc++;
			i++;
		}
	}
	return m_szDesc;
}

const char * g_MagicID2String(int nAttrib)
{
	if ((nAttrib < 0) || nAttrib >= magic_end) return MAGIC_ATTRIB_STRING[magic_end];
	return 	MAGIC_ATTRIB_STRING[nAttrib];
}

int	g_String2MagicID(char * szMagicAttribName)
{
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return -1;

	//nValue2 µ±ÖµÎª-1Ê±ÎªÓÀ¾ÃÐÔ×´Ì¬£¬0Îª·Ç×´Ì¬£¬ÆäËüÖµÎªÓÐÊ±Ð§ÐÔ×´Ì¬Ä§·¨Ð§¹û
	//ÐèÒª½«×´Ì¬Êý¾ÝÓë·Ç×´Ì¬Êý¾Ý·ÖÀë³öÀ´£¬·ÅÈëÏàÓ¦µÄÊý×éÄÚ£¬²¢¼ÇÂ¼×ÜÊýÁ¿
	
	for (int i  = 0 ; i < magic_end; i ++)
	{
		if (!strcmp(szMagicAttribName, g_MagicID2String(i)))
			return i;
	}
	return -1;
}

