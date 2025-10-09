
#ifndef COREUSENAMEDEF_H
#define COREUSENAMEDEF_H

#define		MAX_PLAYER_IN_ACCOUNT			3

#define		SETTING_PATH					"\\settings"

//---------------------------- npc res Ïà¹Ø ------------------------------
#define		RES_INI_FILE_PATH				"\\settings\\npcres"
#define		RES_NPC_STYLE_PATH				"\\spr\\npcres\\style"
#define		RES_SOUND_FILE_PATH				"sound"

#define		NPC_RES_KIND_FILE_NAME			"\\settings\\npcres\\ÈËÎïÀàÐÍ.txt"
#define		NPC_NORMAL_RES_FILE				"\\settings\\npcres\\ÆÕÍ¨npc×ÊÔ´.txt"
#define		NPC_NORMAL_SPRINFO_FILE			"\\settings\\npcres\\ÆÕÍ¨npc×ÊÔ´ÐÅÏ¢.txt"
#define		STATE_MAGIC_TABLE_NAME			"\\settings\\npcres\\×´Ì¬Í¼ÐÎ¶ÔÕÕ±í.txt"
#define		PLAYER_RES_SHADOW_FILE			"\\settings\\npcres\\Ö÷½Ç¶¯×÷ÒõÓ°¶ÔÓ¦±í.txt"
#define		NPC_RES_SHADOW_FILE				"ÆÕÍ¨npc¶¯×÷ÒõÓ°¶ÔÓ¦±í.txt"
#define		PLAYER_SOUND_FILE				"\\settings\\npcres\\Ö÷½Ç¶¯×÷ÉùÒô±í.txt"
#define		NPC_SOUND_FILE					"\\settings\\npcres\\npc¶¯×÷ÉùÒô±í.txt"

#define		NPC_ACTION_NAME					"npc¶¯×÷±í.txt"
#define		ACTION_FILE_NAME				"¶¯×÷±àºÅ±í.txt"

#define		PLAYER_MENU_STATE_RES_FILE		"\\settings\\npcres\\½çÃæ×´Ì¬ÓëÍ¼ÐÎ¶ÔÕÕ±í.txt"
#define		PLAYER_INSTANT_SPECIAL_FILE		"\\settings\\npcres\\Ë²¼äÌØÐ§.txt"

#define		SPR_INFO_NAME					"ÐÅÏ¢"
#define		KIND_NAME_SECT					"ÈËÎïÀàÐÍ"
#define		KIND_NAME_SPECIAL				"ÌØÊânpc"
#define		KIND_NAME_NORMAL				"ÆÕÍ¨npc"
#define		KIND_FILE_SECT1					"²¿¼þËµÃ÷ÎÄ¼þÃû"
#define		KIND_FILE_SECT2					"ÎäÆ÷ÐÐÎª¹ØÁª±í1"
#define		KIND_FILE_SECT3					"ÎäÆ÷ÐÐÎª¹ØÁª±í2"
#define		KIND_FILE_SECT4					"¶¯×÷ÌùÍ¼Ë³Ðò±í"
#define		KIND_FILE_SECT5					"×ÊÔ´ÎÄ¼þÂ·¾­"

//--------------------------- player ÃÅÅÉÏà¹Ø ------------------------------
// ×¢£ºÏÂÃæÕâ¸öÉè¶¨ÎÄ¼þ±ØÐë°´ ½ð ½ð Ä¾ Ä¾ Ë® Ë® »ð »ð ÍÁ ÍÁ µÄË³ÐòÅÅÁÐ
#define		FACTION_FILE					"\\settings\\faction\\MonPhai.ini"

//--------------------------- player PKÏà¹Ø ------------------------------
#define		defPK_PUNISH_FILE				"\\settings\\npc\\player\\PKPunish.txt"

//--------------------------- player °ï»áÏà¹Ø ------------------------------
#define		defPLAYER_TONG_PARAM_FILE		"\\settings\\tong\\TongSet.ini"


//----------------------------- ÁÄÌìÏà¹Ø ------------------------------
#define		CHAT_PATH						"\\chat"
#define		CHAT_TEAM_INFO_FILE_NAME		"Team.cht"
#define		CHAT_CHANNEL_INFO_FILE_NAME		"Channel.cht"

#define		CHAT_CHANNEL_NAME_ALL			"TÊt c¶"
#define		CHAT_CHANNEL_NAME_SCREEN		"GÇn"
#define		CHAT_CHANNEL_NAME_SINGLE		"MËt"
#define		CHAT_CHANNEL_NAME_TEAM			"Nhãm"
#define		CHAT_CHANNEL_NAME_FACTION		"Ph¸i"
#define		CHAT_CHANNEL_NAME_TONG			"Bang"
#define		CHAT_CHANNEL_NAME_SCREENSINGLE	"Phßng"
#define		CHAT_CHANNEL_NAME_SYSTEM		"HÖ Thèng"

#define		CHAT_TAKE_CHANNEL_NAME_TEAM			"Nhãm"
#define		CHAT_TAKE_CHANNEL_NAME_FACTION		"Ph¸i"
#define		CHAT_TAKE_CHANNEL_NAME_TONG			"Bang"
#define		CHAT_TAKE_CHANNEL_NAME_SCREENSINGLE	"Phßng"

//---------------------------- player ÊýÖµÏà¹Ø ------------------------------
#define		PLAYER_LEVEL_EXP_FILE			"\\settings\\npc\\player\\level_exp.txt"
#define		PLAYER_LEVEL_ADD_FILE			"\\settings\\npc\\player\\level_add.txt"
#define		PLAYER_LEVEL_LEAD_EXP_FILE		"\\settings\\npc\\player\\level_lead_exp.txt"
#define		BASE_ATTRIBUTE_FILE_NAME		"\\settings\\npc\\player\\NewPlayerBaseAttribute.ini"
#define		PLAYER_PK_RATE_FILE				"\\settings\\npc\\PKRate.ini"
#define		PLAYER_BASE_VALUE				"\\settings\\npc\\player\\BaseValue.ini"

// 00ÄÐ½ð 01Å®½ð 02ÄÐÄ¾ 03Å®Ä¾ 04ÄÐË® 05Å®Ë® 06ÄÐ»ð 07Å®»ð 08ÄÐÍÁ 09Å®ÍÁ
#define		NEW_PLAYER_INI_FILE_NAME		"\\settings\\npc\\player\\NewPlayerIni%02d.ini"

//---------------------------- player ½»Ò×Ïà¹Ø ------------------------------
#define		BUYSELL_FILE					"\\settings\\buysell.txt"
#define		GOODS_FILE						"\\settings\\goods.txt"

//----------------------------- ÎïÆ·Ïà¹Ø ------------------------------
#define		CHANGERES_MELEE_FILE			"\\settings\\item\\MeleeRes.txt"
#define		CHANGERES_RANGE_FILE			"\\settings\\item\\RangeRes.txt"
#define		CHANGERES_ARMOR_FILE			"\\settings\\item\\ArmorRes.txt"
#define		CHANGERES_HELM_FILE				"\\settings\\item\\HelmRes.txt"
#define		CHANGERES_HORSE_FILE			"\\settings\\item\\HorseRes.txt"
#define		CHANCERES_GOLD_FILE				"\\settings\\item\\GolditemRes.txt"
#define		ITEM_ABRADE_FILE				"\\settings\\item\\AbradeRate.ini"

//--------------- npc skill missles Éè¶¨ÎÄ¼þ£¬ÓÃÓÚÉú³ÉÄ£°å -------------
#define		SKILL_SETTING_FILE				"\\settings\\Skills.txt"
#define		MISSLES_SETTING_FILE			"\\settings\\Missles.txt"
#define		NPC_SETTING_FILE				"\\settings\\NpcS.txt"
#define		NPC_GOLD_TEMPLATE_FILE			"\\settings\\npc\\NpcGoldTemplate.txt"

//---------------------------- object Ïà¹Ø ------------------------------
#define		OBJ_DATA_FILE_NAME		"\\settings\\obj\\ObjData.txt"
#define		MONEY_OBJ_FILE_NAME		"\\settings\\obj\\MoneyObj.txt"
#define		OBJ_NAME_COLOR_FILE		"\\settings\\obj\\ObjNameColor.ini"

//---------------------------- Ð¡µØÍ¼Ïà¹Ø -------------------------------
#define		defLITTLE_MAP_SET_FILE	"\\Ui\\Default\\Ð¡µØÍ¼ÑÕÉ«.ini"

//----------------------------- ÉùÒôÏà¹Ø --------------------------------
#define		defINSTANT_SOUND_FILE	"\\settings\\SoundList.txt"
#define		defMUSIC_SET_FILE		"\\settings\\music\\MusicSet.txt"
#define		defMUSIC_FIGHT_SET_FILE	"\\settings\\music\\MusicFightSet.ini"

//------------------------------------------------------------------------
#define NPC_LEVELSCRIPT_FILENAME		"\\script\\npclevelscript\\npclevelscript.lua"
#define NPC_TEMPLATE_BINFILEPATH		"\\settings"
#define NPC_TEMPLATE_BINFILE			"NpcTemplate.Bin"
#define WEAPON_PHYSICSSKILLFILE			"\\settings\\ÎäÆ÷ÎïÀí¹¥»÷¶ÔÕÕ±í.txt"				
#define WEAPON_PARTICULARTYPE			"ÏêÏ¸Àà±ð"
#define	WEAPON_DETAILTYPE				"¾ßÌåÀà±ð"
#define	WEAPON_SKILLID					"¶ÔÓ¦ÎïÀí¼¼ÄÜ±àºÅ"
//-----------------------------------------------------------------------
#define	WORLD_WAYPOINT_TABFILE			"\\settings\\WayPoint.txt"
#define WORLD_STATION_TABFILE			"\\settings\\Station.txt"
#define WORLD_STATIONPRICE_TABFILE		"\\settings\\StationPrice.txt"
#define WORLD_WAYPOINTPRICE_TABFILE		"\\settings\\WayPointPrice.txt"
#define WORLD_DOCK_TABFILE				"\\settings\\Wharf.txt"
#define WORLD_DOCKPRICE_TABFILE			"\\settings\\WharfPrice.txt"
#define STRINGRESOURSE_TABFILE			"\\settings\\StringResource.txt"
#define PLAYER_RANK_SETTING_TABFILE		"\\settings\\RankSetting.txt"	
#define QUESTITEM_TABFILE				"\\settings\\item\\questkey.txt"
#define MINEITEM_TABFILE				"\\settings\\item\\mine.txt"

//---------------------------------ÈÎÎñ--------------------------
#define TASK_MISSION_SETTING_TABFILE	"\\settings\\task\\missions.txt"



//---------------------------- ÏûÏ¢Ïà¹Ø ------------------------------
// ÌØ±ð×¢Òâ£ºÒÔÏÂ×Ö·û´®³¤¶È²»ÄÜ³¬¹ý32×Ö½Ú£¬°üÀ¨ %d %s µÈ½ÓÊÕ¾ßÌåÄÚÈÝÒÔºóµÄ³¤¶È
#define		MSG_GET_EXP						"B¹n nhËn ®­îc %d kinh nghiÖm!"
#define		MSG_DEC_EXP						"B¹n bÞ mÊt %d kinh nghiÖm!"
#define		MSG_LEVEL_UP					"§¼ng cÊp ®¹t %d cÊp"
#define		MSG_LEADER_LEVEL_UP				"Tµi l·nh ®¹o ®¹t %d cÊp"
#define		MSG_GET_ATTRIBUTE_POINT			"B¹n cã %d ®iÓm tiÒm n¨ng"
#define		MSG_GET_SKILL_POINT				"B¹n cã %d ®iÓm tiÒm n¨ng"
#define		MSG_GET_ATTRIBUTE_SKILL_POINT	"B¹n cã %d ®iÓm tiÒm n¨ng %d ®iÓm kü n¨ng"

#define		MSG_TEAM_AUTO_REFUSE_INVITE		"Tù ®éng tõ chèi lêi mêi trong nhãm!"
#define		MSG_TEAM_NOT_AUTO_REFUSE_INVITE	"Tù ®éng hiÓn thÞ lêi mêi trong nhãm!"
#define		MSG_TEAM_SEND_INVITE			"Nhãm göi lêi mêi ®Õn %s"
#define		MSG_TEAM_GET_INVITE				"%s mêi nhãm b¹n!"
#define		MSG_TEAM_REFUSE_INVITE			"%s tõ chèi lêi mêi nhãm cña b¹n!"
#define		MSG_TEAM_CREATE					"T¹o nhãm thµnh c«ng!"
#define		MSG_TEAM_CREATE_FAIL			"Tao nhãm thÊt b¹i!"
#define		MSG_TEAM_CANNOT_CREATE			"Kh«ng thÓ t¹o nhãm!"
#define		MSG_TEAM_TARGET_CANNOT_ADD_TEAM	"§· ë nhãm kh¸c kh«ng thÓ t¹o nhãm"
#define		MSG_TEAM_OPEN					"Nhãm cña b¹n ®­îc phÐp tiÕp nhËn thµnh viªn míi"
#define		MSG_TEAM_CLOSE					"Nhãm cña b¹n kh«ng ®­îc phÐp tiÕp nhËn thµnh viªn"
#define		MSG_TEAM_ADD_MEMBER				"%s ®· ®­îc thªm vµo nhãm!"
#define		MSG_TEAM_SELF_ADD				"B¹n ®· tham gia nhãm cña %s "
#define		MSG_TEAM_DISMISS_CAPTAIN		"B¹n ®· gi¶i t¸n nhãm!"
#define		MSG_TEAM_DISMISS_MEMBER			"% ®· rêi khái nhãm!"
#define		MSG_TEAM_KICK_ONE				"%s bÞ ®uæi khái nhãm!"
#define		MSG_TEAM_BE_KICKEN				"B¹n ®· bÞ ®uæi khái nhãm!"
#define		MSG_TEAM_APPLY_ADD				"%s ®· gia nhËp nhãm!"
#define		MSG_TEAM_APPLY_ADD_SELF_MSG		"B¹n ®·  vµo nhãm %s !"
#define		MSG_TEAM_LEAVE					"%s §· rêi khái nhãm!"
#define		MSG_TEAM_LEAVE_SELF_MSG			"B¹n ®· rêi khái nhãm %s !"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL1	"Bæ nhiÖm ®éi tr­ëng thÊt b¹i!"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL2	"%s tµi l·nh ®¹o kh«ng ®ñ"
#define		MSG_TEAM_CHANGE_CAPTAIN_FAIL3	"Kh«ng thÓ chuyÓn cho ng­êi míi ch¬i!"
#define		MSG_TEAM_CHANGE_CAPTAIN			"%s ®­îc bæ nhiÖm lµm ®éi tr­ëng"
#define		MSG_TEAM_CHANGE_CAPTAIN_SELF	"B¹n ®­îc %s bæ nhiÖm lµm ®éi tr­ëng!"

#define		MSG_CHAT_APPLY_ADD_FRIEND		"Ban vµ %s trë thµnh b¹n bÌ!"
#define		MSG_CHAT_FRIEND_HAD_IN			"%s ®· ®­îc thªm vµo danh s¸ch b¹n bÌ cña b¹n!"
#define		MSG_CHAT_GET_FRIEND_APPLY		"%s trë thµnh b¹n bÌ cña b¹n!"
#define		MSG_CHAT_REFUSE_FRIEND			"%s tõ chèi yªu cÇu kÕt b¹n cña b¹n!"
#define		MSG_CHAT_ADD_FRIEND_FAIL		"Thªm %s vµo danh s¸ch b¹n bÌ thÊt b¹i!"
#define		MSG_CHAT_ADD_FRIEND_SUCCESS		"Thªm %s vµo danh s¸ch b¹n bÌ thµnh c«ng"
#define		MSG_CHAT_CREATE_TEAM_FAIL1		"T¹o nhãm kh«ng thµnh c«ng!"
#define		MSG_CHAT_CREATE_TEAM_FAIL2		"Tªn nhãm ®· ®­îc sö dông!"
#define		MSG_CHAT_CREATE_TEAM_FAIL3		"Nhãm ®· ®¹t ®Õn sç l­îng tèi ®a!"
#define		MSG_CHAT_RENAME_TEAM_FAIL		"§æi tªn nhãm kh«ng thµnh c«ng!"
#define		MSG_CHAT_DELETE_TEAM_FAIL1		"Xo¸ nhãm kh«ng thµnh c«ng!"
#define		MSG_CHAT_DELETE_TEAM_FAIL2		"Nhãm nµy kh«ng ®­îc xo¸!"
#define		MSG_CHAT_FRIEND_ONLINE			"%s ®· Online!"
#define		MSG_CHAT_FRIEND_OFFLINE			"%s ®· Offline!"
#define		MSG_CHAT_DELETE_FRIEND			"B¹n ®· xo¸ quan hÖ b¹n bÌ víi %s"
#define		MSG_CHAT_DELETED_FRIEND			"%s ®· xo¸ quan hÖ b¹n bÌ víi b¹n"
#define		MSG_CHAT_MSG_FROM_FRIEND		"%s ®· göi tin nh¾n nµy"
#define		MSG_CHAT_FRIEND_NOT_NEAR		"%s kh«ng cã trong trß ch¬i!"
#define		MSG_CHAT_FRIEND_NOT_ONLINE		"%s kh«ng cã trªn m¹ng!"
#define		MSG_CHAT_TAR_REFUSE_SINGLE_TALK	"%s kh«ng ®¨ng ký kªnh ch¸t riªng!"

#define		MSG_SHOP_NO_ROOM				"Hµnh trang kh«ng ®ñ chæ!"
#define		MSG_SHOP_NO_MONEY				"B¹n kh«ng ®ñ tiÒn"
#define		MSG_SHOP_NO_XU					"B¹n kh«ng cã ®ñ xu!"

#define		MSG_NPC_NO_MANA					"Néi lùc kh«ng ®ñ"
#define		MSG_NPC_NO_STAMINA				"ThÓ lùc kh«ng ®ñ"
#define		MSG_NPC_NO_LIFE					"Sinh lùc kh«ng ®ñ"
#define		MSG_NPC_DEATH					"%s ®· vÒ thµnh d­ìng søc!"

#define		MSG_OBJ_CANNOT_PICKUP			"B¹n kh«ng thÓ lÊy trang bÞ cña ng­êi kh¸c!"
#define		MSG_MONEY_CANNOT_PICKUP			"B¹n kh«ng thÓ lÊy tiÒn cña ng­êi kh¸c!"
#define		MSG_OBJ_TOO_FAR					"VËt phÈm qu¸ xa kh«ng thÓ lÊy!"
#define		MSG_DEC_MONEY					"B¹n bÞ mÊt %d l­îng !"
#define		MSG_EARN_MONEY					"B¹n nhËn ®­îc %d l­îng !"
#define		MSG_EARN_XU						"B¹n nhËn ®­îc %d xu!"
#define		MSG_DEATH_LOSE_ITEM				"B¹n ®· bÞ mÊt %s"
#define		MSG_ADD_ITEM					"B¹n nhËn ®­îc %s !"
#define		MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE	"§· cã vËt phÈm cïng lo¹i!"
#define		MSG_CAN_NOT_VIEW_ITEM			"Kh«ng thÓ xem trang bÞ cña ng­êi nµy!"
#define		MSG_ITEM_DAMAGED				"Trang bÞ %s ®· bÞ háng råi!"
#define		MSG_ITEM_NEARLY_DAMAGED			"Trang bÞ %s s¾p háng h·y ®Õn thî rÌn ®Ó söa chöa l¹i!"

#define		MSG_TRADE_STATE_OPEN			"B©y giê b¹n cã thÓ giao dÞch !"
#define		MSG_TRADE_STATE_CLOSE			"§ãng giao dÞch!"
#define		MSG_TRADE_SELF_LOCK				"Kho¸ giao dÞch"
#define		MSG_TRADE_SELF_UNLOCK			"Më kho¸ giao dÞch!"
#define		MSG_TRADE_DEST_LOCK				"%s ®· kho¸ giao dÞch !"
#define		MSG_TRADE_DEST_UNLOCK			"%s ®· më kho¸ giao dÞch !"
#define		MSG_TRADE_SUCCESS				"B¹n giao dÞch víi %s thµnh c«ng!"
#define		MSG_TRADE_FAIL					"B¹n giao dÞch víi %s thÊt b¹i"
#define		MSG_TRADE_SELF_ROOM_FULL		"Hµnh trang chñ b¹n kh«ng ®ñ chæ trèng!"
#define		MSG_TRADE_DEST_ROOM_FULL		"Hµnh trang %s kh«ng ®ñ chæ"
#define		MSG_TRADE_SEND_APPLY			"Göi lêi mêi giao dÞch ®Õn %s"
#define		MSG_TRADE_GET_APPLY				"%s göi lêi mêi giao dÞch ®Õn víi b¹n!"
#define		MSG_TRADE_REFUSE_APPLY			"%s tõ chèi lêi mêi cña b¹n!"
#define		MSG_TRADE_TASK_ITEM				"VËt phÈm nhiÖm vô kh«ng thÓ giao dÞch!"

#define		MSG_PK_NORMAL_FLAG_OPEN			"B¹n ®ang trong tr¹ng th¸i chiÕn ®Êu"
#define		MSG_PK_NORMAL_FLAG_CLOSE		"B¹n ®ang trong tr¹ng th¸i luyÖn c«ng"
#define		MSG_PK_NORMAL_FLAG_DS			"B¹n ®ang trong tr¹ng th¸i ®å s¸t!"
#define		MSG_PK_VALUE					"TrÞ sè PK hiÖn t¹i lµ %d !"
#define		MSG_PK_ERROR_1					"B¹n ®ang trong tr¹ng th¸i phi chiÕn ®Êu!"
#define		MSG_PK_ERROR_2					"Ch÷ tr¾ng kh«ng thÓ cõu s¸t!"
#define		MSG_PK_ERROR_3					"B¹n ®ang t×m hiÓu víi nh÷ng ng­êi kh¸c!"
#define		MSG_PK_ERROR_4					"B¹n ®ang cõu s¸t víi ng­êi kh¸c!"
#define		MSG_PK_ERROR_5					"B¹n kh«ng thÓ tÊn c«ng ng­êi míi!"
#define		MSG_PK_ERROR_6					"§èi thñ ®ang trogn t×nh tr¹ng phi chiÕn ®Êu!"
#define		MSG_PK_ERROR_7					"Kh«ng thÕ chiÕn ®Êu"
#define		MSG_PK_ENMITY_SUCCESS_1			"%s tiÕn hµnh cõu s¸t"
#define		MSG_PK_ENMITY_SUCCESS_2			"10 gi©y tiÕn hµnh cõu s¸t!"
#define		MSG_PK_ENMITY_CLOSE				"Tr¹ng th¸i cõu s¸t kÕt thóc..."
#define		MSG_PK_ENMITY_OPEN				"B¾t ®Çu tiÕn hµnh cõu s¸t..."

#define		MSG_TONG_CREATE_ERROR01			"Tªn bang héi qu¸ dµi"
#define		MSG_TONG_CREATE_ERROR02			"Lçi bang héi"
#define		MSG_TONG_CREATE_ERROR03			"Thµnh viªn bang héi kh«ng thÓ t¹o mét bang héi míi"
#define		MSG_TONG_CREATE_ERROR04			"S¸t thñ míi ®­îc phÐp t¹o bang héi!"
#define		MSG_TONG_CREATE_ERROR05			"Sè ng­êi lËp bang kh«ng ®ñ!"
#define		MSG_TONG_CREATE_ERROR06			"Tµi l·nh ®¹o kh«ng ®ñ!"
#define		MSG_TONG_CREATE_ERROR07			"LËp bang cÇn %d l­îng"
#define		MSG_TONG_CREATE_ERROR08			"T©n thñ kh«ng thÓ lËp bang"
#define		MSG_TONG_CREATE_ERROR09			"Lçi bang héi 1101"
#define		MSG_TONG_CREATE_ERROR10			"Sè tiÒn ®Ó lËp bang kh«ng ®ñ!"
#define		MSG_TONG_CREATE_ERROR11			"Tªn bang nµy ®· ®­îc sö dông!"
#define		MSG_TONG_APPLY_CREATE			"§ång ý t¹o bang héi!"
#define		MSG_TONG_CREATE_SUCCESS			"T¹o bang héi thµnh c«ng!"
#define		MSG_TONG_APPLY_ADD				"B¹n xin gia nhËp bang héi!"
#define		MSG_TONG_APPLY_ADD_ERROR1		"Thµnh viªn bang héi kh«ng thÓ gia nhËp bang héi kh¸c!"
#define		MSG_TONG_APPLY_ADD_ERROR2		"S¸t thñ míi cã thÓ gia nhËp bang héi!"
#define		MSG_TONG_APPLY_ADD_ERROR3		"Nhãm kh«ng thÓ gia nhËp bang héi!"
#define		MSG_TONG_REFUSE_ADD				"%s tõ chèi gia nhËp bang héi!"
#define		MSG_TONG_ADD_SUCCESS			"Gia nhËp bang héi thµnh c«ng"
#define		MSG_TONG_CANNOT_LEAVE1			"Thµnh viªn chÝnh thøc kh«ng thÓ rêi khái bang"
#define		MSG_TONG_CANNOT_LEAVE2			"Tr­ëng l·o kh«ng thÓ rêi khái bang!"
#define		MSG_TONG_BE_KICKED				"B¹n bÞ ®uæi ra khái bang!"
#define		MSG_TONG_LEAVE_SUCCESS			"Rêi khái bang héi thµnh c«ng"
#define		MSG_TONG_LEAVE_FAIL				"Rêi khái bang héi thÊt b¹i!"
#define		MSG_TONG_CHANGE_AS_MASTER		"B¹n ®­îc bæ nhiÖm lµm bang chñ!"
#define		MSG_TONG_CHANGE_AS_MEMBER		"B©y giê b¹n trë thµnh thµnh viªn b×nh th­êng!"
#define		MSG_EXPAND_STORE_BOX			"B¹n ch­a mua chøc n¨ng nµy. H·y ®Õn Ba L¨ng HuyÖn gÆp ThÈm Cöu (188,198) ®Ó mua chøc n¨ng nµy."

#define		MSG_NPC_CANNOT_RIDE				"B¹n qu¸ mÖt mái ,kh«ng thÓ liªn tôc lªn xuèng ngùa!"
#define		MSG_COMP_FAILED					"B¾t ®Çu chÕ t¹o"
#define		MSG_COMP_SUCCESS				"ChÕ t¹o trang bÞ thµnh c«ng!"
#define		MSG_COMP_ERITEM					"B¹n kh«ng cã ®ñ nguyªn liÖu!"


#define		MESSAGE_SYSTEM_ANNOUCE_HEAD		"C«ng bè"

enum enumMSG_ID
{
	enumMSG_ID_NONE = 0,
	enumMSG_ID_TEAM_KICK_One,
	enumMSG_ID_TEAM_DISMISS,
	enumMSG_ID_TEAM_LEAVE,
	enumMSG_ID_TEAM_REFUSE_INVITE,
	enumMSG_ID_TEAM_SELF_ADD,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL,
	enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2,
	enumMSG_ID_OBJ_CANNOT_PICKUP,
	enumMSG_ID_OBJ_TOO_FAR,
	enumMSG_ID_DEC_MONEY,
	enumMSG_ID_TRADE_SELF_ROOM_FULL,
	enumMSG_ID_TRADE_DEST_ROOM_FULL,
	enumMSG_ID_TRADE_REFUSE_APPLY,
	enumMSG_ID_TRADE_TASK_ITEM,
	enumMSG_ID_GET_ITEM,
	enumMSG_ID_ITEM_DAMAGED,
	enumMSG_ID_MONEY_CANNOT_PICKUP,
	enumMSG_ID_CANNOT_ADD_TEAM,
	enumMSG_ID_TARGET_CANNOT_ADD_TEAM,
	enumMSG_ID_PK_ERROR_1,
	enumMSG_ID_PK_ERROR_2,
	enumMSG_ID_PK_ERROR_3,
	enumMSG_ID_PK_ERROR_4,
	enumMSG_ID_PK_ERROR_5,
	enumMSG_ID_PK_ERROR_6,
	enumMSG_ID_PK_ERROR_7,
	enumMSG_ID_DEATH_LOSE_ITEM,
	enumMSG_ID_TONG_REFUSE_ADD,
	enumMSG_ID_TONG_BE_KICK,
	enumMSG_ID_TONG_LEAVE_SUCCESS,
	enumMSG_ID_TONG_LEAVE_FAIL,
	enumMSG_ID_TONG_CHANGE_AS_MASTER,
	enumMSG_ID_TONG_CHANGE_AS_MEMBER,
	enumMSG_ID_RIDE_CANNOT,
	enumMSG_ID_COMP_FAILED,
	enumMSG_ID_COMP_SUCCESS,
	enumMSG_ID_COMP_ERITEM,
	enumMSG_ID_NUM,
};

//---------------------------- Êó±êÖ¸ÕëÏà¹Ø ------------------------------
#define		MOUSE_CURSOR_NORMAL				0
#define		MOUSE_CURSOR_FIGHT				1
#define		MOUSE_CURSOR_DIALOG				2
#define		MOUSE_CURSOR_PICK				3
#define		MOUSE_CURSOR_USE				8
#endif
