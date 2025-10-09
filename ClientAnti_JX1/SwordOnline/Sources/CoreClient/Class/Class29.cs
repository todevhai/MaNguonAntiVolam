using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using CoreClient;
using RGiesecke.DllExport;

internal class Class29
{
	public static Class29 m_Class29;

	private uint uint_0;

	private uint uint_1;

	private uint uint_2;

	private uint uint_3;

	private uint uint_4;

	private uint uint_5;

	private uint uint_6;

	private Process process_0;

	public Class36 class36_0;

	private Class12 class12_0;

	private Class10 class10_0;

	public JsonAccChinh jsonAccChinh_0;

	private bool bool_0;

	private int int_0;

	public IntPtr intptr_0;

	private int int_1;

	private int int_2;

	private string string_0;

	private string string_1;

	public bool bool_1;

	public int int_3;

	public int int_4;

	private bool bool_2;

	private int int_5;

	private Class30 class30_0;

	private int int_6;

	public int int_7;

	public int int_8;

	public ArrayList arrayList_0;

	private int int_9;

	private int int_10;

	private int int_11;

	private int int_12;

	private int int_13;

	private int int_14;

	private int int_15;

	private int int_16;

	private int int_17;

	private ArrayList arrayList_1;

	private Struct20 struct20_0;

	private Struct20 struct20_1;

	private ArrayList arrayList_2;

	private ArrayList arrayList_3;

	private int int_18;

	private Struct20 struct20_2;

	private Struct20 struct20_3;

	private int int_19;

	private bool bool_3;

	private bool bool_4;

	private int int_20;

	private bool bool_5;

	public int int_21;

	public Dictionary<string, string> dictionary_0;

	public int int_22;

	public bool bool_6;

	public int int_23;

	public int int_24;

	public int int_25;

	public int int_26;

	internal static object object_0;

	public Class29(Process process_1, int int_27)
	{
		m_Class29 = this;
		class12_0 = new Class12();
		int_1 = 1;
		string_1 = "";
		int_3 = 400;
		int_4 = 300;
		int_5 = 100;
		int_14 = 1;
		arrayList_1 = new ArrayList();
		arrayList_2 = new ArrayList();
		arrayList_3 = new ArrayList();
		bool_3 = true;
		int_20 = 1;
		dictionary_0 = new Dictionary<string, string>();
		int_22 = 1;

		process_0 = process_1;
		class36_0 = new Class36(process_0);
		if (process_0.MainWindowHandle.ToInt32() == 0)
			intptr_0 = Class49.smethod_23(process_0);
		else
			intptr_0 = process_0.MainWindowHandle;
		uint_1 = class36_0.method_3(19682624u);
		uint_2 = class36_0.method_3(9324788u);
		uint_3 = uint_2 + 35632;
		uint_4 = uint_3 + 276;
		int_1 = (int)class36_0.method_3(uint_4);
		if (int_27 <= 0)
			uint_0 = (uint)(uint_1 + int_1 * 70584);
		else
			uint_0 = (uint)(uint_1 + int_27 * 70584);
		uint_5 = class36_0.method_3(20189584u);
		uint_6 = class36_0.method_3(9324788u) + 36076;
		method_119();
		if (Class49.smethod_26(method_105()) == "N/A")
			string_0 = method_22();
		else
			string_0 = Class49.smethod_26(method_105());
		string_1 = Class18.smethod_4(method_33().MainModule.FileName);
	}

	public void method_0(Class30 class30_1)
	{
		class30_0 = class30_1;
	}

	public void method_1(JsonAccChinh jsonAccChinh_1)
	{
		jsonAccChinh_0 = jsonAccChinh_1;
	}

	public string method_2()
	{
		return string_1;
	}

	public void method_3(int int_27, bool bool_7)
	{
		bool_2 = bool_7;
		int_5 = int_27;
	}

	public bool method_4()
	{
		return bool_2;
	}

	public int method_5()
	{
		return int_5;
	}

	public void method_6(bool bool_7)
	{
		bool_1 = bool_7;
	}

	public bool method_7()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.isDoingMacDinh;
		return bool_1;
	}

	public int method_8()
	{
		return int_3;
	}

	public void method_9(int int_27)
	{
		int_3 = int_27;
	}

	public int method_10()
	{
		return int_4;
	}

	public void method_11(int int_27)
	{
		int_4 = int_27;
	}

	public void method_12(int int_27, ArrayList arrayList_4)
	{
		if (Class49.smethod_31() > int_7)
		{
			int_7 = Class49.smethod_31();
			int_8 = int_27;
			arrayList_0 = arrayList_4;
		}
	}

	public JsonAccChinh method_13()
	{
		return jsonAccChinh_0;
	}

	public IntPtr method_14()
	{
		return intptr_0;
	}

	[DllExport(CallingConvention = CallingConvention.StdCall)]
	public static uint Class29_method_15()
	{
		return m_Class29.method_15();
	}
	public uint method_15()
	{
		int_1 = (int)class36_0.method_3(uint_4);
		uint_0 = (uint)(uint_1 + int_1 * 70584);
		return uint_0;
	}

	public int method_16()
	{
		int num = (int)class36_0.method_3(6906976u);
		return (int)class36_0.method_3((uint)(num + 520));
	}

	[DllExport(CallingConvention = CallingConvention.StdCall)]
	public static int Class29_method_17()
	{
		return m_Class29.method_17();
	}
	public int method_17()
	{
		//if (int_6 == 2)
		//	return jsonAccChinh_0.playerIdx;
		int_1 = (int)class36_0.method_3(uint_4);
		return int_1;
	}

	public int method_18()
	{
		return int_6;
	}

	public void method_19(int int_27)
	{
		int_6 = int_27;
	}

	public void method_20(int int_27, int int_28, int int_29)
	{
		int_12 = int_27;
		int_13 = int_28;
		int_14 = int_29;
		int_16 = method_130();
	}

	public int method_21()
	{
		Struct20 @struct = method_118(method_17());
		int num = method_145(@struct.x, @struct.y, int_12, int_13);
		int num2 = method_130();
		if (num2 - int_16 > 30)
			return 0;
		if (method_102() != int_14 && int_11 != int_14)
		{
			int_11 = int_14;
			int_9 = num2;
		}
		else if (method_102() != int_14 && int_11 == int_14)
		{
			if (num2 - int_9 > 50)
			{
				int_9 = num2;
				int_11 = method_102();
				return 1;
			}
		}
		else if (method_102() == int_14 && method_102() != 1)
		{
			int_9 = num2;
			int_11 = method_102();
		}
		if (num >= 120 && int_15 != num)
		{
			int_10 = num2;
			int_15 = num;
		}
		else if (num >= 120)
		{
			if (num >= 120 && int_15 >= 120 && num2 - int_10 > 50)
			{
				int_10 = num2;
				int_15 = method_102();
				return 2;
			}
		}
		else
		{
			int_10 = num2;
			int_15 = num;
		}
		return 0;
	}

	public string method_22()
	{
		if (method_146(115))
			return "CB";
		if (method_146(116))
			return "CB";
		if (method_146(130))
			return "CB";
		if (!method_146(357))
		{
			if (!method_146(359))
			{
				if (!method_146(4))
				{
					if (!method_146(6))
					{
						if (method_146(8))
							return "TL";
						if (method_146(16))
							return "TL";
						if (!method_146(273))
						{
							if (method_146(318))
								return "TL";
							if (!method_146(319))
							{
								if (!method_146(321))
								{
									if (method_146(95))
										return "TY";
									if (!method_146(97))
									{
										if (method_146(109))
											return "TY";
										if (!method_146(114))
										{
											if (!method_146(111))
											{
												if (!method_146(336))
												{
													if (method_146(337))
														return "TY";
													if (!method_146(131))
													{
														if (!method_146(132))
														{
															if (method_146(150))
																return "TN";
															if (method_146(362))
																return "TN";
															if (!method_146(361))
															{
																if (method_146(151))
																	return "VĐ";
																if (method_146(152))
																	return "VĐ";
																if (method_146(166))
																	return "VĐ";
																if (!method_146(365))
																{
																	if (method_146(368))
																		return "VĐ";
																	if (!method_146(167))
																	{
																		if (method_146(168))
																			return "CL";
																		if (method_146(275))
																			return "CL";
																		if (!method_146(372))
																		{
																			if (method_146(375))
																				return "CL";
																			if (!method_146(392))
																			{
																				if (method_146(393))
																					return "CL";
																				if (!method_146(77))
																				{
																					if (!method_146(79))
																					{
																						if (method_146(86))
																							return "NM";
																						if (method_146(89))
																							return "NM";
																						if (method_146(92))
																							return "NM";
																						if (method_146(252))
																							return "NM";
																						if (method_146(380))
																							return "NM";
																						if (method_146(328))
																							return "NM";
																						if (!method_146(60))
																						{
																							if (!method_146(62))
																							{
																								if (!method_146(67))
																								{
																									if (!method_146(68))
																									{
																										if (!method_146(75))
																										{
																											if (!method_146(390))
																											{
																												if (!method_146(353))
																												{
																													if (!method_146(355))
																													{
																														if (!method_146(43))
																														{
																															if (!method_146(45))
																															{
																																if (!method_146(48))
																																{
																																	if (method_146(351))
																																		return "ĐM";
																																	if (method_146(342))
																																		return "ĐM";
																																	if (!method_146(339))
																																	{
																																		if (!method_146(302))
																																		{
																																			if (method_146(23))
																																				return "TVB";
																																			if (!method_146(24))
																																			{
																																				if (method_146(26))
																																					return "TVB";
																																				if (method_146(40))
																																					return "TVB";
																																				if (method_146(42))
																																					return "TVB";
																																				if (method_146(36))
																																					return "TVB";
																																				if (method_146(323))
																																					return "TVB";
																																				if (method_146(324))
																																					return "TVB";
																																				if (!method_146(325))
																																					return Class49.smethod_26(method_105());
																																				return "TVB";
																																			}
																																			return "TVB";
																																		}
																																		return "ĐM";
																																	}
																																	return "ĐM";
																																}
																																return "ĐM";
																															}
																															return "ĐM";
																														}
																														return "ĐM";
																													}
																													return "NĐ";
																												}
																												return "NĐ";
																											}
																											return "NĐ";
																										}
																										return "NĐ";
																									}
																									return "NĐ";
																								}
																								return "NĐ";
																							}
																							return "NĐ";
																						}
																						return "NĐ";
																					}
																					return "NM";
																				}
																				return "NM";
																			}
																			return "CL";
																		}
																		return "CL";
																	}
																	return "CL";
																}
																return "VĐ";
															}
															return "TN";
														}
														return "TN";
													}
													return "TN";
												}
												return "TY";
											}
											return "TY";
										}
										return "TY";
									}
									return "TY";
								}
								return "TL";
							}
							return "TL";
						}
						return "TL";
					}
					return "TL";
				}
				return "TL";
			}
			return "CB";
		}
		return "CB";
	}

	public int method_23()
	{
		return (int)class36_0.method_3(uint_3 + 34196);
	}

	public int method_24()
	{
		return (int)class36_0.method_3(uint_3 + 20352);
	}

	public string method_25()
	{
		int num = (int)class36_0.method_3(uint_3 + 34204);
		return class36_0.method_2((uint)(num + 48), 32, bool_0: true);
	}

	public void method_26(Class10 class10_1)
	{
		class10_0 = class10_1;
		int_2 = class10_1.int_84;
	}

	public void method_27(int int_27)
	{
		int_17 = int_27;
	}

	public int method_28()
	{
		return int_17;
	}

	public void method_29(int int_27)
	{
		int_0 = int_27;
	}

	public int method_30()
	{
		return int_0;
	}

	public uint method_31()
	{
		return uint_0;
	}

	public void method_32()
	{
		class36_0.method_0();
	}

	public Process method_33()
	{
		return process_0;
	}

	public bool method_34()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.isBungItem;
		return bool_0;
	}

	public int method_35()
	{
		return (int)class36_0.method_3(uint_3 + 22800);
	}

	public void method_36(bool bool_7)
	{
		bool_0 = bool_7;
	}

	public int method_37()
	{
		return (int)class36_0.method_3(uint_3 + 19856);
	}

	public int method_38()
	{
		return (int)class36_0.method_3(uint_3 + 316);
	}

	public int method_39(int int_27)
	{
		uint num = class36_0.method_3(uint_3 + 19992);
		return (int)class36_0.method_3((uint)(num + int_27 * 4));
	}

	public int method_40()
	{
		uint num = class36_0.method_3(7200896u);
		return (int)class36_0.method_3(num + 444);
	}

	public int method_41(int int_27)
	{
		uint num = class36_0.method_3(method_15() + 256);
		int num2 = 0;
		while (true)
		{
			if (num2 < 50)
			{
				if (class36_0.method_3(num + 16) == (uint)int_27)
					break;
				num = class36_0.method_3(num + 8);
				num2++;
				continue;
			}
			return -1;
		}
		return (int)class36_0.method_3(num + 24);
	}

	public int method_42()
	{
		return (int)class36_0.method_3(uint_3 + 19828);
	}

	public int method_43()
	{
		return (int)class36_0.method_3(uint_3 + 20260);
	}

	public int method_44()
	{
		return (int)class36_0.method_3(uint_3 + 20264);
	}

	public int method_45()
	{
		return (int)class36_0.method_3(uint_3 + 19884);
	}

	public int method_46()
	{
		return (int)Math.Floor((decimal)((int)(class36_0.method_3(uint_3 + 19884) + class36_0.method_3(uint_3 + 19856)) / 10000));
	}

	public int method_47()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.npcExist;
		return (int)class36_0.method_4(method_15() + 20);
	}

	public int method_48(int int_27)
	{
		if (int_6 == 2 && jsonAccChinh_0.playerIdx == int_27)
			return jsonAccChinh_0.npcExist;
		return (int)class36_0.method_4((uint)(uint_1 + int_27 * 70584 + 20L));
	}

	public int method_49()
	{
		return (int)class36_0.method_3(method_15() + 264);
	}

	public int method_50()
	{
		return 1;
	}

	public int method_51()
	{
		return (int)class36_0.method_4(uint_3 + 24976);
	}

	public uint method_52()
	{
		return class36_0.method_3(6894212u);
	}

	public string method_53()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.name;
		return class36_0.method_2(method_15() + 4877, 32, bool_0: true);
	}

	public string method_54(int int_27)
	{
		return class36_0.method_2((uint)(uint_1 + int_27 * 70584 + 4877L), 32, bool_0: true);
	}

	public int method_55(int int_27)
	{
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 32L));
	}

	public int method_56(int int_27)
	{
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 60L));
	}

	public string method_57(int int_27)
	{
		//return class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 32L)) switch
		//{
		//	1u => "Méc",
		//	2u => "Thñy",
		//	3u => "Háa",
		//	4u => "Thæ",
		//	0u => "Kim",
		//	_ => "Kh«ng biÕt",
		//};
		return "Kh«ng biÕt";
	}

	public string method_58()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.name;
		return class36_0.method_2(7205808u, 32, bool_0: true);
	}

	public int method_59()
	{
		return (int)class36_0.method_3(20138196u);
	}

	public int method_60()
	{
		int num = (int)class36_0.method_3(7212920u);
		int num2 = (int)class36_0.method_3(6911520u);
		return (int)class36_0.method_3(6911516u) - (num - num2) / 1000;
	}

	public string method_61()
	{
		int num = (int)class36_0.method_3(7212920u);
		int num2 = (int)class36_0.method_3(6911520u);
		int num3 = (int)class36_0.method_3(6911516u);
		if (num3 != 0)
		{
			int num4 = num3 - (num - num2) / 1000;
			int num5 = num4 / 60;
			return string.Concat(str2: (num4 % 60).ToString(), str0: num5.ToString(), str1: ":");
		}
		return "0:0";
	}

	public int method_62()
	{
		return (int)class36_0.method_3(6911524u);
	}

	public int method_63()
	{
		return (int)class36_0.method_3(6911528u);
	}

	public int method_64()
	{
		return (int)class36_0.method_3(23185948u);
	}

	public int method_65()
	{
		return (int)class36_0.method_3(6595332u);
	}

	public int method_66()
	{
		uint num = class36_0.method_3(6911400u);
		return (int)class36_0.method_3(num + 4);
	}

	public int method_67()
	{
		uint num = class36_0.method_3(6911400u);
		return (int)class36_0.method_3(num + 8);
	}

	public int method_68()
	{
		uint num = class36_0.method_3(6911400u);
		return (int)class36_0.method_3(num + 12);
	}

	public int method_69()
	{
		uint num = class36_0.method_3(6911400u);
		return (int)class36_0.method_3(num + 16);
	}

	public int method_70()
	{
		uint num = class36_0.method_3(6911400u);
		return (int)class36_0.method_3(num + 56);
	}

	public int method_71()
	{
		uint num = class36_0.method_3(6911400u);
		return (int)class36_0.method_3(num + 52);
	}

	public int method_72()
	{
		return (int)class36_0.method_3(6595336u);
	}

	public int method_73()
	{
		return (int)class36_0.method_3(uint_3 + 24896);
	}

	public int method_74()
	{
		return (int)class36_0.method_3(uint_3 + 24900);
	}

	public string method_75()
	{
		return class36_0.method_2(uint_3 + 24904, 32, bool_0: true);
	}

	public int method_76(int int_27)
	{
		if (class36_0.method_3((uint)(9324556 + int_27 * 1272 + 192)) == 1)
			return 1;
		return 0;
	}

	public string method_77(int int_27)
	{
		return class36_0.method_2((uint)(9324556 + int_27 * 1272 + 24), 32, bool_0: true);
	}

	public int method_78(int int_27)
	{
		if (int_27 == 178)
			return 1;
		if (class36_0.method_3((uint)(9324556 + int_27 * 1272 + 124)) == 3 && class36_0.method_3((uint)(9324556 + int_27 * 1272 + 188)) != 1 && class36_0.method_3((uint)(9324556 + int_27 * 1272 + 156)) == 1)
		{
			if (class36_0.method_3((uint)(9324556 + int_27 * 1272 + 60)) == 3)
				return 0;
			return 1;
		}
		return 0;
	}

	public int method_79()
	{
		return (int)class36_0.method_3(uint_3 + 36);
	}

	public int method_80()
	{
		return (int)class36_0.method_3(uint_3 + 44);
	}

	public uint method_81()
	{
		return class36_0.method_3(uint_3 + 25036);
	}

	public int method_82()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.maxMp;
		if ((int)class36_0.method_3(method_15() + 70408) > (int)class36_0.method_3(method_15() + 70412))
			return (int)class36_0.method_3(method_15() + 70408);
		return (int)class36_0.method_3(method_15() + 70412);
	}

	public int method_83()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.curMp;
		return (int)class36_0.method_3(method_15() + 4184);
	}

	public bool method_84()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.CheckAttackNoTarget;
		if ((int)class36_0.method_3(method_15() + 236) > 4 && (int)class36_0.method_3(method_15() + 5916) < 1000 && method_104() == 0)
			return true;
		return false;
	}

	public int method_85()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.xPos;
		return (int)class36_0.method_3(method_15() + 5920);
	}

	public int method_86()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.yPos;
		return (int)class36_0.method_3(method_15() + 5924);
	}

	public int method_87()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.curHp;
		return (int)class36_0.method_3(method_15() + 4168);
	}

	public int method_88(int int_27)
	{
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 4168L));
	}

	public int method_89()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.maxHp;
		if ((int)class36_0.method_3(method_15() + 70404) <= (int)class36_0.method_3(method_15() + 70400))
			return (int)class36_0.method_3(method_15() + 70400);
		return (int)class36_0.method_3(method_15() + 70404);
	}

	public int method_90()
	{
		return (int)class36_0.method_3(method_15() + 70432);
	}

	public int method_91()
	{
		return (int)class36_0.method_3(method_15() + 70440);
	}

	public int method_92(int int_27)
	{
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 70400L));
	}

	public int method_93(int int_27)
	{
		if (int_6 == 2 && jsonAccChinh_0.playerIdx == int_27)
			return jsonAccChinh_0.npcColor;
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 232L));
	}

	public int method_94(int int_27)
	{
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 228L));
	}

	public int method_95(int int_27)
	{
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 36L));
	}

	public int method_96(int int_27)
	{
		if (int_6 == 2 && jsonAccChinh_0.playerIdx == int_27)
			return jsonAccChinh_0.attackKind;
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 5536L));
	}

	public int method_97(int int_27)
	{
		return (int)class36_0.method_4((uint)(uint_1 + int_27 * 70584 + 136L));
	}

	public int method_98(int int_27)
	{
		if (int_6 == 2 && jsonAccChinh_0.playerIdx == int_27)
			return jsonAccChinh_0.npcIdx;
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584));
	}

	public int method_99(int int_27)
	{
		return (int)class36_0.method_4((uint)(uint_1 + int_27 * 70584 + 6144L));
	}

	public int method_100()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.fightMode;
		return (int)class36_0.method_4(method_15() + 5476);
	}

	public int method_101()
	{
		return (int)class36_0.method_4(method_15() + 6144);
	}

	public int method_102()
	{
		return (int)class36_0.method_3(method_15() + 236);
	}

	public int method_103(int int_27)
	{
		return (int)class36_0.method_4((uint)(uint_1 + int_27 * 70584 + 236L));
	}

	public int method_104()
	{
		if (int_6 == 2)
			return jsonAccChinh_0.peopleIdx;
		return (int)class36_0.method_3(method_15() + 5020);
	}

	public int method_105()
	{
		return (int)class36_0.method_4(method_15() + 48) + int_2;
	}

	public int method_106()
	{
		return (int)class36_0.method_3(uint_3 + 196);
	}

	public string method_107()
	{
		string result = "0:0";
		uint num = 0u;
		uint num2 = 0u;
		int num3 = 100;
		uint num4 = class36_0.method_3(6906896u);
		uint num5 = num4 + 7736;
		num = num4 + 7740;
		num = class36_0.method_3(num);
		uint num6 = class36_0.method_3(num5);
		for (int num7 = (int)(num - 1); num7 >= 0; num7--)
		{
			num2 = class36_0.method_3(num6 + (uint)(4 * num7));
			if (class36_0.method_3(num2 + 36) == 33554432)
			{
				num3 = (int)class36_0.method_3(num2 + 24);
				Match match = Regex.Match(class36_0.method_1(num2 + 39, num3 - 10, bool_0: true), ".*?tÝch lòy cña 2 phe lµ (.*?):(\\d+).*?$", RegexOptions.IgnoreCase);
				if (match.Success)
				{
					result = match.Groups[1].Value.Trim() + "-" + match.Groups[2].Value.Trim();
					break;
				}
			}
		}
		return result;
	}

	public string method_108()
	{
		uint num = 0u;
		uint num2 = 0u;
		int num3 = 100;
		uint num4 = class36_0.method_3(6906896u) + 7096;
		num = class36_0.method_3(6906896u) + 7100;
		num = class36_0.method_3(num);
		uint num5 = class36_0.method_3(num4);
		int num6 = (int)(num - 1);
		num2 = class36_0.method_3(num5 + (uint)(4 * num6));
		num3 = (int)class36_0.method_4(num2 + 24);
		return class36_0.method_1(num2 + 39, num3 - 10, bool_0: true);
	}

	public int method_109(string string_2, int int_27)
	{
		int result = -1;
		for (int i = 1; i < 256; i++)
		{
			class36_0.method_3((uint)(uint_1 + i * 70584 + 20L));
			if (class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) != 0)
			{
				class36_0.method_3((uint)(uint_1 + i * 70584 + 4168L));
				string text = class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), int_27, bool_0: true);
				if (text.Contains(string_2) && text.Length > 1)
				{
					result = method_93(i);
					break;
				}
			}
		}
		return result;
	}

	public ArrayList method_110(int int_27)
	{
		ArrayList arrayList = new ArrayList();
		arrayList.Clear();
		for (int i = 1; i < 256; i++)
		{
			class36_0.method_3((uint)(uint_1 + i * 70584 + 20L));
			if (class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) != 0)
			{
				class36_0.method_3((uint)(uint_1 + i * 70584 + 4168L));
				string text = class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), int_27, bool_0: true);
				if (!arrayList.Contains(text) && text.Length > 1)
					arrayList.Add(text);
			}
		}
		return arrayList;
	}

	public int method_111()
	{
		return (int)class36_0.method_3(6809096u) / 16;
	}

	public int method_112()
	{
		return (int)class36_0.method_3(6809100u) / 16;
	}

	public int method_113(string string_2)
	{
		int result = 0;
		for (int i = 1; i < 256; i++)
		{
			if (i == method_17())
				continue;
			class36_0.method_3((uint)(uint_1 + i * 70584 + 20L));
			if (class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 0)
				continue;
			class36_0.method_3((uint)(uint_1 + i * 70584 + 4168L));
			if (class36_0.method_1((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true).Equals(string_2))
			{
				struct20_3 = method_118(i);
				struct20_2 = method_118(method_17());
				if (method_145(struct20_2.x, struct20_2.y, struct20_3.x, struct20_3.y) < 600)
					result = i;
				break;
			}
		}
		return result;
	}

	public int method_114(string string_2)
	{
		int result = 0;
		for (int i = 1; i < 256; i++)
		{
			if (i != method_17() && class36_0.method_1((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true).Contains(string_2))
			{
				struct20_3 = method_118(i);
				struct20_2 = method_118(method_17());
				if (method_145(struct20_2.x, struct20_2.y, struct20_3.x, struct20_3.y) < 600)
					result = i;
				break;
			}
		}
		return result;
	}

	public int method_115(string string_2)
	{
		if (class10_0.method_4().method_129() != method_129())
			return 0;
		int result = 0;
		for (int i = 1; i < 256; i++)
		{
			if (i == method_17())
				continue;
			class36_0.method_3((uint)(uint_1 + i * 70584 + 20L));
			if (class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 0)
				continue;
			class36_0.method_3((uint)(uint_1 + i * 70584 + 4168L));
			if (class36_0.method_1((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true).Equals(string_2))
			{
				struct20_3 = method_118(i);
				struct20_2 = method_118(method_17());
				if (method_145(struct20_2.x, struct20_2.y, struct20_3.x, struct20_3.y) < 600 && method_145(class10_0.method_4().method_118(class10_0.method_4().method_17()).x, class10_0.method_4().method_118(class10_0.method_4().method_17()).y, struct20_3.x, struct20_3.y) < 300)
					result = i;
				break;
			}
		}
		return result;
	}

	public int method_116(int int_27)
	{
		int result = 0;
		for (int i = 1; i < 256; i++)
		{
			uint num = 0u;
			num = (uint)(uint_1 + i * 70584);
			if ((int)class36_0.method_4(num + 20) >= 0 && class36_0.method_4(num + 36) != 0 && (int)class36_0.method_3(num + 4168) >= 0 && class36_0.method_3(num) == (uint)int_27)
			{
				result = i;
				break;
			}
		}
		return result;
	}

	public int method_117(int int_27)
	{
		int result = 0;
		for (int i = 1; i < 256; i++)
		{
			if (i != method_17())
			{
				uint num = 0u;
				num = (uint)(uint_1 + i * 70584);
				if ((int)class36_0.method_4(num + 20) >= 0 && class36_0.method_4(num + 36) != 0 && (int)class36_0.method_3(num + 4168) >= 0 && class36_0.method_3(num + 232) != 4 && class36_0.method_3(num + 232) == method_93(method_17()) && class36_0.method_3(num) == (uint)int_27)
				{
					result = i;
					break;
				}
			}
		}
		return result;
	}

	public Struct20 method_118(int int_27)
	{
		if (int_6 == 2 && jsonAccChinh_0.playerIdx == int_27)
			return new Struct20(jsonAccChinh_0.pNx, jsonAccChinh_0.pNy);
		uint num = 0u;
		num = (uint)(uint_1 + int_27 * 70584);
		int num2 = (int)class36_0.method_3(num + 4776);
		int num3 = (int)class36_0.method_3(num + 4788);
		uint num4 = class36_0.method_4(num + 5508);
		int num5 = (int)class36_0.method_3(num + 4776 + 4);
		int num6 = (int)class36_0.method_3(num + 4788 + 4);
		int num7 = (int)class36_0.method_4(num + 5508 + 2);
		int num8 = (int)(num4 * 512) + num2 * 32 + num3 / 1024;
		int num9 = num7 * 1024 + num5 * 32 + num6 / 1024;
		return new Struct20(num8, num9);
	}

	public void method_119()
	{
		arrayList_1.Clear();
		arrayList_1.Add("Chủ tiệm");
		arrayList_1.Add("Hậu Cần");
		arrayList_1.Add("hậu cần");
		arrayList_1.Add("Hiệu Thuốc");
		arrayList_1.Add("Quân Nhu");
		arrayList_1.Add("Quân nhu");
		arrayList_1.Add("quân nhu");
		arrayList_1.Add("Chủ dược điếm");
		arrayList_1.Add("Bán thuốc");
		arrayList_1.Add("Ông chủ dược điếm");
		arrayList_1.Add("Tùy Quân dược Y");
		arrayList_1.Add("Tống quân quân nhu quan");
		arrayList_1.Add("Kim quân quân nhu quan");
		arrayList_1.Add("Ngô Thần Y");
	}

	public bool method_120(string string_2)
	{
		bool result = false;
		for (int i = 0; i < arrayList_1.Count; i++)
		{
			if (string_2.Contains(arrayList_1[i].ToString()))
				result = true;
		}
		return result;
	}

	public int method_121(int int_27)
	{
		int result = 0;
		for (int i = 1; i < 256; i++)
		{
			if (i == method_17())
				continue;
			uint num = 0u;
			num = (uint)(uint_1 + i * 70584);
			if ((int)class36_0.method_4(num + 20) < 0 || class36_0.method_4(num + 24) == 0 || class36_0.method_4(num + 36) == 1)
				continue;
			string string_ = class12_0.method_2(class36_0.method_2(num + 4877, 32, bool_0: true), 2, 5);
			if (method_120(string_))
			{
				if (int_27 <= 0)
				{
					result = i;
					break;
				}
				Struct20 @struct = method_118(i);
				Struct20 struct2 = method_118(method_17());
				if (method_145(struct2.x, struct2.y, @struct.x, @struct.y) < int_27)
				{
					result = i;
					break;
				}
			}
		}
		return result;
	}

	public int method_122(string string_2, int int_27)
	{
		int result = 0;
		for (int i = 1; i < 256; i++)
		{
			if (i == method_17())
				continue;
			uint num = 0u;
			num = (uint)(uint_1 + i * 70584);
			if ((int)class36_0.method_4(num + 20) < 0 || class36_0.method_4(num + 24) == 0 || class36_0.method_4(num + 36) == 1)
				continue;
			string text = class12_0.method_2(class36_0.method_2(num + 4877, 32, bool_0: true), 2, 5);
			if (text.Contains(string_2) && (!string_2.Equals("Xa phu") || (!text.Contains("Xa phu công thành") && !text.Contains("Xa phu Công Thành"))))
			{
				if (int_27 <= 0)
				{
					result = i;
					break;
				}
				Struct20 @struct = method_118(i);
				Struct20 struct2 = method_118(method_17());
				if (method_145(struct2.x, struct2.y, @struct.x, @struct.y) < int_27)
				{
					result = i;
					break;
				}
			}
		}
		return result;
	}

	public bool method_123(int int_27)
	{
		if (method_48(int_27) != 0)
		{
			if (class10_0.method_4().method_102() != 21 && class10_0.method_4().method_102() != 10)
			{
				if (method_100() != class10_0.method_4().method_100())
					return false;
				if (method_129() != class10_0.method_4().method_129())
					return false;
				if (int_27 != 0)
				{
					if (method_98(int_27) != class10_0.method_4().method_98(class10_0.method_4().method_17()))
						return false;
					struct20_0 = method_118(method_17());
					struct20_1 = method_118(int_27);
					if (method_145(struct20_0.x, struct20_0.y, struct20_1.x, struct20_1.y) > 400 && method_102() == 1)
						return false;
					return true;
				}
				struct20_0 = method_118(method_17());
				struct20_1 = class10_0.method_4().method_118(class10_0.method_4().method_17());
				if (method_145(struct20_0.x, struct20_0.y, struct20_1.x, struct20_1.y) <= 400)
					return false;
				return true;
			}
			return false;
		}
		return false;
	}

	public bool method_124(int int_27)
	{
		if (int_27 != 0)
		{
			if (method_48(int_27) != 0)
			{
				if (method_54(int_27).Equals(class10_0.string_6))
					return true;
				return false;
			}
			return false;
		}
		return false;
	}

	public ArrayList method_125(int int_27)
	{
		ArrayList arrayList = new ArrayList();
		arrayList.Clear();
		for (int i = 1; i < 256; i++)
		{
			if (i == method_17())
				continue;
			uint num = class36_0.method_4((uint)((int)(uint_1 + i * 70584) + 36));
			int num2 = (int)class36_0.method_4((uint)((int)(uint_1 + i * 70584) + 20));
			int num3 = (int)class36_0.method_3((uint)((int)(uint_1 + i * 70584) + 4168));
			if (num == (uint)int_27 && num3 > 0 && num2 > 0)
			{
				Struct20 @struct = method_118(i);
				Struct20 struct2 = method_118(method_17());
				if (method_145(struct2.x, struct2.y, @struct.x, @struct.y) < 600)
					arrayList.Add(class36_0.method_2((uint)((int)(uint_1 + i * 70584) + 4877), 32, bool_0: true));
			}
		}
		return arrayList;
	}

	public ArrayList method_126()
	{
		ArrayList arrayList = new ArrayList();
		arrayList.Clear();
		for (int i = 1; i < 256; i++)
		{
			uint num = class36_0.method_4((uint)((int)(uint_1 + i * 70584) + 36));
			int num2 = (int)class36_0.method_4((uint)((int)(uint_1 + i * 70584) + 20));
			int num3 = (int)class36_0.method_3((uint)((int)(uint_1 + i * 70584) + 4168));
			if ((int)num > 1 && num3 > 0 && num2 > 0)
				arrayList.Add(class36_0.method_2((uint)((int)(uint_1 + i * 70584) + 4877), 32, bool_0: true));
		}
		return arrayList;
	}

	public string method_127()
	{
		return class36_0.method_2(19899808u, 32, bool_0: true);
	}

	public string method_128()
	{
		return class36_0.method_2(uint_3 + 25040, 8, bool_0: true);
	}

	public int method_129()
	{
		//if (int_6 == 2)
			//return jsonAccChinh_0.mapId;
		return (int)class36_0.method_3(6907596u);
	}

	public int method_130()
	{
		return (int)class36_0.method_3(20187736u);
	}

	public int method_131()
	{
		return (int)class36_0.method_3(7212920u);
	}

	public int method_132()
	{
		return (int)class36_0.method_3(6879272u);
	}

	public int method_133()
	{
		uint num = class36_0.method_3(6879272u);
		return int.Parse(class36_0.method_2(num, 1, bool_0: true));
	}

	public int method_134()
	{
		return (int)class36_0.method_3(6879284u);
	}

	public int method_135()
	{
		return (int)class36_0.method_3(6879308u);
	}

	public int method_136()
	{
		return (int)class36_0.method_3(6879312u);
	}

	public uint method_137()
	{
		return class36_0.method_3(6906976u);
	}

	public int method_138()
	{
		return (int)class36_0.method_3(6347808u);
	}

	public int method_139()
	{
		return (int)class36_0.method_3(6880684u);
	}

	public int method_140()
	{
		return (int)class36_0.method_3(6879696u);
	}

	public int method_141()
	{
		return (int)class36_0.method_3(6879284u);
	}

	public int method_142()
	{
		return (int)class36_0.method_3(6869160u);
	}

	public int method_143()
	{
		return (int)class36_0.method_3(uint_3 + 60);
	}

	public string method_144()
	{
		return class36_0.method_2(7205676u, 32, bool_0: true);
	}

	public int method_145(int int_27, int int_28, int int_29, int int_30)
	{
		int num = int_29 - int_27;
		int num2 = int_30 - int_28;
		return (int)Math.Sqrt(num * num + num2 * num2);
	}

	public bool method_146(int int_27)
	{
		int num = 0;
		while (true)
		{
			if (num < 24)
			{
				int num2 = (int)class36_0.method_3((uint)(method_15() + 668 + num * 48));
				if (class36_0.method_3((uint)(method_15() + 664 + num * 48)) == (uint)int_27 && num2 > 0)
					break;
				num++;
				continue;
			}
			return false;
		}
		return true;
	}

	public ArrayList method_147()
	{
		ArrayList arrayList = new ArrayList();
		//for (int i = 0; i < 24; i++)
		//{
		//	int num = (int)class36_0.method_3((uint)(method_15() + 668 + i * 48));
		//	int num2 = (int)class36_0.method_3((uint)(method_15() + 664 + i * 48));
		//	if (num > 0 && num2 > 0 && class36_0.method_3((uint)(9324556 + num2 * 1272 + 60)) != 3)
		//	{
		//		string text = class12_0.method_2(method_77(num2), 2, 5);
		//		arrayList.Add(new Struct19(num2, num, text));
		//	}
		//}
		return arrayList;
	}

	public ArrayList method_148()
	{
		ArrayList arrayList = new ArrayList();
		//for (int i = 0; i < 24; i++)
		//{
		//	int num = (int)class36_0.method_3((uint)(method_15() + 668 + i * 48));
		//	int num2 = (int)class36_0.method_3((uint)(method_15() + 664 + i * 48));
		//	if (num > 0 && num2 > 0 && method_78(num2) == 1)
		//	{
		//		string text = class12_0.method_2(method_77(num2), 2, 5);
		//		arrayList.Add(new Struct19(num2, num, text));
		//	}
		//}
		return arrayList;
	}

	public bool method_149(string string_2)
	{
		bool result = false;
		for (int i = 0; i < arrayList_2.Count; i++)
		{
			if (string_2.Contains(arrayList_2[i].ToString()))
				result = true;
		}
		return result;
	}

	public ArrayList method_150()
	{
		arrayList_2.Clear();
		for (int i = 0; i < 320; i++)
		{
			uint num = (uint)(uint_6 + i * 20);
			uint num2 = (uint)(uint_6 + i * 20 + 4L);
			if (class36_0.method_3(num2) != 3)
				continue;
			int num3 = (int)class36_0.method_3(num);
			uint num4 = (uint)(uint_5 + num3 * 1404 + 4L);
			int num5 = (int)class36_0.method_3(num4);
			if (num5 == 1 || num5 == 6)
			{
				uint num6 = (uint)(uint_5 + num3 * 1404 + 48L);
				string text = class36_0.method_2(num6, 32, bool_0: true);
				if (!method_149(text))
					arrayList_2.Add(text);
			}
		}
		return arrayList_2;
	}

	public ArrayList method_151()
	{
		arrayList_2.Clear();
		for (int i = 0; i < 320; i++)
		{
			uint num = (uint)(uint_6 + i * 20);
			uint num2 = (uint)(uint_6 + i * 20 + 4L);
			if (class36_0.method_3(num2) != 3)
				continue;
			int num3 = (int)class36_0.method_3(num);
			uint num4 = (uint)(uint_5 + num3 * 1404 + 28L);
			uint num5 = (uint)(uint_5 + num3 * 1404 + 32L);
			int num6 = (int)class36_0.method_3(num5);
			if ((int)class36_0.method_3(num4) <= 1 && num6 <= 1)
			{
				uint num7 = (uint)(uint_5 + num3 * 1404 + 48L);
				string text = class36_0.method_2(num7, 32, bool_0: true);
				if (!method_149(text))
					arrayList_2.Add(text);
			}
		}
		return arrayList_2;
	}

	public int method_152(string string_2)
	{
		int num = class10_0.string_1.IndexOf(string_2);
		if (num == -1)
			return 10;
		return num;
	}

	public int method_153(int int_27)
	{
		int num = class10_0.string_0.IndexOf(int_27.ToString());
		if (num == -1)
			return 7;
		return num;
	}

	public bool method_154()
	{
		bool result = true;
		for (int i = 0; i < class10_0.arrayList_0.Count; i++)
		{
			if (!arrayList_3.Contains(class10_0.arrayList_0[i]))
			{
				result = false;
				break;
			}
		}
		return result;
	}

	public bool method_155(int int_27)
	{
		bool result = false;
		arrayList_3.Clear();
		arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11652L)));
		arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11808L)));
		arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11964L)));
		arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12120L)));
		arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12276L)));
		arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12432L)));
		if ((int)arrayList_3[0] > 0 && (int)arrayList_3[1] > 0 && (int)arrayList_3[2] > 0 && (int)arrayList_3[3] > 0 && (int)arrayList_3[4] > 0 && (int)arrayList_3[5] > 0)
		{
			if (method_154())
				result = true;
		}
		else
			result = method_154();
		return result;
	}

	public bool method_156(int int_27, int int_28)
	{
		bool result = false;
		//arrayList_3.Clear();
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11652L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11808L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11964L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12120L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12276L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12432L)));
		//if ((int)arrayList_3[0] > 0 && (int)arrayList_3[1] > 0 && (int)arrayList_3[2] > 0 && (int)arrayList_3[3] > 0 && (int)arrayList_3[4] > 0 && (int)arrayList_3[5] > 0)
		//{
		//	if (method_154())
		//		result = true;
		//}
		//else
		//{
		//	for (int i = 0; i < arrayList_3.Count; i++)
		//	{
		//		if (Class49.smethod_19((int)arrayList_3[i]) == int_28)
		//		{
		//			result = true;
		//			break;
		//		}
		//	}
		//}
		return result;
	}

	public int method_157(int int_27)
	{
        int num = 0;
		return num;
		//arrayList_3.Clear();
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11652L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11808L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 11964L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12120L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12276L)));
		//arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 12432L)));
		//for (int i = 0; i < class10_0.arrayList_0.Count; i++)
		//{
		//	if (!arrayList_3.Contains(class10_0.arrayList_0[i]))
		//	{
		//		num = (int)class10_0.arrayList_0[i];
		//		break;
		//	}
		//}
		//return Class49.smethod_21(num, string_0);
	}

	public int method_158(int int_27)
	{
		return (int)class36_0.method_3((uint)(uint_1 + int_27 * 70584 + 20L));
	}

	public bool method_159()
	{
		return bool_5;
	}

	public int method_160(string string_2, int int_27)
	{
		if (!class10_0.dictionary_2.ContainsKey(string_2))
			return int_27;
		return class10_0.dictionary_2[string_2];
	}

	public void method_161(int int_27)
	{
		if (class10_0.int_115 <= method_59() - int_18)
		{
			int_18 = method_59();
			string text = (method_118(int_27).x / 256).ToString();
			string text2 = (method_118(int_27).y / 512).ToString();
			string text3 = method_54(int_27).ToString();
			int num = method_88(int_27) / method_92(int_27) * 100;
			string text4 = "(lvd)";
			if (num < 50 && num > 10)
				text4 = "(chd)";
			else if (num <= 10)
			{
				text4 = "(hod)";
			}
			_ = text3 + "(hÖ " + method_57(int_27) + ") HP:" + num + "% " + text4 + ", ë " + method_127() + " täa ®é: " + text + "/" + text2;
		}
	}

	public int method_162(int int_27, int int_28, int int_29)
	{
		int num = 0;
		//int int_30 = class10_0.int_50;
		//bool flag = false;
		//bool flag2 = false;
		//bool_5 = false;
		//int_23 = 0;
		//int_24 = 0;
		//int_25 = 0;
		//int_21 = 0;
		//int num2;
		//if (int_20 == 1)
		//{
		//	num2 = class10_0.int_50;
		//	int_19 = class10_0.int_50;
		//}
		//else
		//{
		//	num2 = class10_0.int_49;
		//	int_19 = class10_0.int_49;
		//}
		//bool flag3 = false;
		//bool flag4 = false;
		//int num3 = class10_0.int_52 - 1;
		//int num4 = 4;
		//int num5 = class10_0.int_53 - 1;
		//if (int_27 == 0 && class10_0.bool_32)
		//{
		//	flag3 = true;
		//	int_20 = 1;
		//}
		//if (int_29 <= 1)
		//	struct20_2 = method_118(method_17());
		//else
		//	struct20_2 = method_118(int_29);
		//for (int i = 1; i < 256; i++)
		//{
		//	if (i == method_17() || (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 20L)) < 1 || (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 4168L)) <= 0)
		//		continue;
		//	int num6 = (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 232L));
		//	if (num6 == 1 && class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 1)
		//		int_23++;
		//	else if (num6 == 2 && class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 1)
		//	{
		//		int_24++;
		//	}
		//	else if (num6 == 3 && class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 1)
		//	{
		//		int_25++;
		//	}
		//	else if (num6 == 5)
		//	{
		//		int_21 = i;
		//		int_26++;
		//	}
		//	if (class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) != (uint)int_27)
		//	{
		//		if (!class10_0.bool_49)
		//		{
		//			if (!class10_0.bool_86)
		//				continue;
		//			if (method_129() == 959)
		//			{
		//				string text = class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true);
		//				if (text.Contains("Hoµng Kim") || text.Contains("Huy Hoµng"))
		//					bool_5 = true;
		//			}
		//		}
		//		else if (class36_0.method_3((uint)(uint_1 + i * 70584 + 70400L)) > 500000)
		//		{
		//			if (num6 == method_93(method_17()))
		//				continue;
		//			flag = true;
		//		}
		//	}
		//	if (int_27 == 1 && !flag && !bool_5)
		//	{
		//		if ((class10_0.bool_56 && class36_0.method_3((uint)(uint_1 + i * 70584 + 236L)) == 3 && method_104() != i) || class36_0.method_3((uint)(uint_1 + i * 70584 + 5476L)) == 0 || (class10_0.bool_80 && (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 32L)) < class10_0.int_80) || class36_0.method_3((uint)(uint_1 + i * 70584 + 5476L)) == 0)
		//			continue;
		//		int num7 = (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 5536L));
		//		if (num6 == 0)
		//			continue;
		//		if (method_93(method_17()) != 4)
		//		{
		//			if (i != int_28 && class10_0.int_71 != 3 && num6 == method_93(method_17()) && method_129() != 998 && !class10_0.bool_69)
		//			{
		//				if (!class10_0.bool_44 || int_28 > 0 || !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)))
		//					continue;
		//			}
		//			else if (i != int_28 && class10_0.int_71 != 3 && num6 == method_93(method_17()) && method_129() != 998 && class10_0.bool_69)
		//			{
		//				string item = class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), 6, bool_0: true);
		//				if (class10_0.bool_44)
		//				{
		//					if ((int_28 > 0 && !class10_0.arrayList_2.Contains(item)) || (!class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && !class10_0.arrayList_2.Contains(item)))
		//						continue;
		//				}
		//				else if (!class10_0.arrayList_2.Contains(item))
		//				{
		//					continue;
		//				}
		//			}
		//			else if (i != int_28 && class10_0.int_71 != 3 && num6 != method_93(method_17()) && method_129() != 998 && class10_0.bool_69)
		//			{
		//				string item = class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), 6, bool_0: true);
		//				if (!class10_0.bool_44)
		//				{
		//					if (!class10_0.arrayList_2.Contains(item) && ((class10_0.bool_50 && method_96(method_17()) == 0 && num7 < 2) || (!class10_0.bool_59 && method_96(method_17()) != 2 && num7 < 1)))
		//						continue;
		//				}
		//				else if ((num7 == 0 && !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && !class10_0.arrayList_2.Contains(item)) || (!class10_0.arrayList_2.Contains(item) && !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && ((class10_0.bool_50 && method_96(method_17()) == 0 && num7 < 2) || (!class10_0.bool_59 && method_96(method_17()) != 2 && num7 < 1))))
		//				{
		//					continue;
		//				}
		//			}
		//			else if (class10_0.int_71 == 3 || method_129() == 998)
		//			{
		//				string item = method_128();
		//				if (class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), item.Length, bool_0: true).Equals(item))
		//					continue;
		//			}
		//		}
		//		if (i != int_28 && class10_0.int_71 != 3 && !class10_0.bool_69 && ((method_96(method_17()) != 2 && num7 < 1 && (!class10_0.bool_44 || !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)))) || (class10_0.bool_50 && method_96(method_17()) == 0 && num7 < 2 && (!class10_0.bool_44 || !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true))))))
		//			continue;
		//	}
		//	else if (num6 == method_93(method_17()) && !bool_5)
		//	{
		//		continue;
		//	}
		//	struct20_3 = method_118(i);
		//	int_30 = method_145(struct20_2.x, struct20_2.y, struct20_3.x, struct20_3.y);
		//	if (int_29 > 1)
		//	{
		//		if (!class10_0.bool_48)
		//		{
		//			if (int_30 >= class10_0.int_72 + class10_0.int_51 - 10)
		//				continue;
		//		}
		//		else if (int_30 >= class10_0.int_72 + 400)
		//		{
		//			continue;
		//		}
		//	}
		//	if (int_30 >= int_19)
		//	{
		//		if (!flag)
		//		{
		//			if (bool_5)
		//				bool_5 = false;
		//			else if (flag2)
		//			{
		//				flag2 = false;
		//			}
		//		}
		//		else
		//			flag = false;
		//	}
		//	else
		//	{
		//		if ((class10_0.bool_68 && class10_0.arrayList_1.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), 6, bool_0: true))) || (class10_0.bool_70 && class10_0.arrayList_3.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true))))
		//			continue;
		//		if (class10_0.bool_48)
		//		{
		//			arrayList_3.Clear();
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 11652L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 11808L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 11964L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 12120L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 12276L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 12432L)));
		//			if (method_154() && !bool_6)
		//				continue;
		//			if (bool_6 && !method_154())
		//				bool_6 = false;
		//		}
		//		if (!class10_0.bool_45 && class10_0.bool_57 && !bool_5 && class10_0.method_4().method_104() > 0 && class10_0.method_4().method_98(class10_0.method_4().method_104()) == i && class10_0.method_4().method_102() > 4)
		//		{
		//			num = i;
		//			break;
		//		}
		//		if (num6 == method_93(method_17()))
		//		{
		//			int num8 = class30_0.method_13(method_17(), i);
		//			if (class10_0.bool_44)
		//			{
		//				if (int_28 == 0)
		//				{
		//					string text2 = class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true);
		//					if (!dictionary_0.ContainsValue(text2) && method_73() == 0 && class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && num8 != 8)
		//					{
		//						if (!dictionary_0.ContainsKey(method_53()))
		//							dictionary_0.Add(method_53(), text2);
		//						else
		//							dictionary_0[method_53()] = text2;
		//						class30_0.method_74("Revenge('" + text2 + "')");
		//						continue;
		//					}
		//					if (num8 != 8)
		//						continue;
		//				}
		//				else if (num8 != 8)
		//				{
		//					continue;
		//				}
		//			}
		//			else if (num8 != 8)
		//			{
		//				continue;
		//			}
		//		}
		//		else if (class10_0.bool_44 && class36_0.method_3((uint)(uint_1 + i * 70584 + 5536L)) == 0 && method_96(method_17()) != 2 && class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)))
		//		{
		//			class30_0.method_18(2);
		//			continue;
		//		}
		//		if (class10_0.bool_141)
		//		{
		//			if (!class10_0.arrayList_4.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true).Trim()))
		//			{
		//				if (flag2)
		//					continue;
		//			}
		//			else
		//			{
		//				flag2 = true;
		//				if (int_30 < num2 || !flag3 || !flag4)
		//				{
		//					num2 = int_30;
		//					num = i;
		//					flag3 = true;
		//					if (!flag4)
		//					{
		//						flag4 = true;
		//						num4 = method_160(class36_0.method_2((uint)(uint_1 + i * 70584 + 5328L), 32, bool_0: true).Trim(), 4);
		//					}
		//				}
		//			}
		//		}
		//		if (class10_0.int_71 == 2 && class10_0.bool_123)
		//		{
		//			int num9 = method_160(class36_0.method_2((uint)(uint_1 + i * 70584 + 5328L), 32, bool_0: true).Trim(), 4);
		//			if (num9 > num4 || num9 < 0)
		//				continue;
		//			num4 = num9;
		//		}
		//		if (int_30 < class10_0.int_49 && !flag3 && int_20 == 1 && !bool_5)
		//		{
		//			if (!class10_0.bool_31)
		//			{
		//				if (!class10_0.bool_32)
		//				{
		//					int_20 = 0;
		//					flag3 = true;
		//					num2 = class10_0.int_49;
		//					int_19 = class10_0.int_49;
		//				}
		//				else if (method_152(Class49.smethod_15(((int)class36_0.method_4((uint)(uint_1 + i * 70584 + 48L)) + int_2).ToString())) <= num5)
		//				{
		//					int_20 = 0;
		//					flag3 = true;
		//					num2 = class10_0.int_49;
		//					int_19 = class10_0.int_49;
		//				}
		//			}
		//			else
		//			{
		//				int int_31 = (int)class36_0.method_4((uint)(uint_1 + i * 70584 + 40L));
		//				if (method_153(int_31) <= num3)
		//				{
		//					int_20 = 0;
		//					flag3 = true;
		//					num2 = class10_0.int_49;
		//					int_19 = class10_0.int_49;
		//				}
		//			}
		//		}
		//		if (int_20 == 0 && !bool_5)
		//		{
		//			if (!class10_0.bool_31)
		//			{
		//				if (!class10_0.bool_32)
		//				{
		//					if (int_30 < num2)
		//					{
		//						num2 = int_30;
		//						num = i;
		//					}
		//				}
		//				else
		//				{
		//					int num10 = method_152(Class49.smethod_15(((int)class36_0.method_4((uint)(uint_1 + i * 70584 + 48L)) + int_2).ToString()));
		//					if (num10 == num5)
		//					{
		//						if (int_30 < num2)
		//						{
		//							num5 = num10;
		//							num2 = int_30;
		//							num = i;
		//						}
		//					}
		//					else if (num10 < num5)
		//					{
		//						num5 = num10;
		//						num2 = int_30;
		//						num = i;
		//					}
		//				}
		//			}
		//			else
		//			{
		//				int int_32 = (int)class36_0.method_4((uint)(uint_1 + i * 70584 + 40L));
		//				int num11 = method_153(int_32);
		//				if (num11 == num3)
		//				{
		//					if (int_30 < num2)
		//					{
		//						num3 = num11;
		//						num2 = int_30;
		//						num = i;
		//					}
		//				}
		//				else if (num11 < num3)
		//				{
		//					num3 = num11;
		//					num2 = int_30;
		//					num = i;
		//				}
		//			}
		//		}
		//		else if (int_30 < num2)
		//		{
		//			num2 = int_30;
		//			num = i;
		//		}
		//		if (flag)
		//			break;
		//	}
		//}
		//if (num == 0)
		//{
		//	if (int_20 == 0)
		//		int_20 = 1;
		//	if (class10_0.bool_46)
		//		int_22 = 0;
		//	if (class10_0.bool_48)
		//	{
		//		if (class10_0.bool_58 && !bool_6)
		//			bool_6 = true;
		//		else if (!class10_0.bool_58 && bool_6)
		//		{
		//			bool_6 = false;
		//		}
		//	}
		//}
		return num;
	}

	public int method_163()
	{
		return int_22;
	}

	public bool method_164()
	{
		return bool_6;
	}

	public void method_165(int int_27)
	{
	}

	public int method_166(int int_27, int int_28, int int_29)
	{
		int num = 0;
		//int int_30 = class10_0.int_50;
		//bool flag = false;
		//bool flag2 = false;
		//int num2 = 0;
		//int_24 = 0;
		//int_23 = 0;
		//int_25 = 0;
		//int_26 = 0;
		//int num3;
		//if (int_20 == 1)
		//{
		//	num3 = class10_0.int_50;
		//	int_19 = class10_0.int_50;
		//}
		//else
		//{
		//	num3 = class10_0.int_49;
		//	int_19 = class10_0.int_49;
		//}
		//bool flag3 = false;
		//bool flag4 = false;
		//int num4 = class10_0.int_52 - 1;
		//int num5 = class10_0.int_53 - 1;
		//int num6 = 4;
		//if (int_27 == 0 && class10_0.bool_32)
		//{
		//	flag3 = true;
		//	int_20 = 1;
		//}
		//if (int_29 > 1)
		//	struct20_2 = method_118(int_29);
		//else
		//	struct20_2 = method_118(method_17());
		//for (int i = 1; i < 256; i++)
		//{
		//	if (i == method_17() || (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 20L)) < 1 || (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 4168L)) <= 0)
		//		continue;
		//	num2 = (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 36L));
		//	if (num2 > 1)
		//		continue;
		//	int num7 = (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 232L));
		//	if (num7 == 1 && class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 1)
		//		int_23++;
		//	else if (num7 == 2 && class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 1)
		//	{
		//		int_24++;
		//	}
		//	else if (num7 == 3 && class36_0.method_3((uint)(uint_1 + i * 70584 + 36L)) == 1)
		//	{
		//		int_25++;
		//	}
		//	else if (num7 == 5)
		//	{
		//		int_21 = i;
		//		int_26++;
		//	}
		//	if (num2 != 1)
		//	{
		//		if (class10_0.bool_49)
		//		{
		//			if (class36_0.method_3((uint)(uint_1 + i * 70584 + 70400L)) > 500000)
		//			{
		//				if (num7 == method_93(method_17()))
		//					continue;
		//				flag = true;
		//			}
		//		}
		//		else if (class10_0.bool_86 && method_129() == 959)
		//		{
		//			string text = class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true);
		//			if (text.Contains("Hoµng Kim") || text.Contains("Huy Hoµng"))
		//				bool_5 = true;
		//		}
		//	}
		//	if (int_27 == 1 && !flag && !bool_5)
		//	{
		//		if ((class10_0.bool_56 && class36_0.method_3((uint)(uint_1 + i * 70584 + 236L)) == 3 && method_104() != i) || class36_0.method_3((uint)(uint_1 + i * 70584 + 5476L)) == 0 || (class10_0.bool_80 && (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 32L)) < class10_0.int_80) || class36_0.method_3((uint)(uint_1 + i * 70584 + 5476L)) == 0)
		//			continue;
		//		int num8 = (int)class36_0.method_3((uint)(uint_1 + i * 70584 + 5536L));
		//		if (num7 == 0)
		//			continue;
		//		if (method_93(method_17()) != 4)
		//		{
		//			if (i != int_28 && class10_0.int_71 != 3 && num7 == method_93(method_17()) && method_129() != 998 && !class10_0.bool_69)
		//			{
		//				if (!class10_0.bool_44 || int_28 > 0 || !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)))
		//					continue;
		//			}
		//			else if (i != int_28 && class10_0.int_71 != 3 && num7 == method_93(method_17()) && method_129() != 998 && class10_0.bool_69)
		//			{
		//				string item = class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), 6, bool_0: true);
		//				if (class10_0.bool_44)
		//				{
		//					if ((int_28 > 0 && !class10_0.arrayList_2.Contains(item)) || (!class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && !class10_0.arrayList_2.Contains(item)))
		//						continue;
		//				}
		//				else if (!class10_0.arrayList_2.Contains(item))
		//				{
		//					continue;
		//				}
		//			}
		//			else if (i != int_28 && class10_0.int_71 != 3 && num7 != method_93(method_17()) && method_129() != 998 && class10_0.bool_69)
		//			{
		//				string item = class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), 6, bool_0: true);
		//				if (class10_0.bool_44)
		//				{
		//					if ((num8 == 0 && !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && !class10_0.arrayList_2.Contains(item)) || (!class10_0.arrayList_2.Contains(item) && !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && ((class10_0.bool_50 && method_96(method_17()) == 0 && num8 < 2) || (!class10_0.bool_59 && method_96(method_17()) != 2 && num8 < 1))))
		//						continue;
		//				}
		//				else if (!class10_0.arrayList_2.Contains(item) && ((class10_0.bool_50 && method_96(method_17()) == 0 && num8 < 2) || (!class10_0.bool_59 && method_96(method_17()) != 2 && num8 < 1)))
		//				{
		//					continue;
		//				}
		//			}
		//			else if (class10_0.int_71 == 3 || method_129() == 998)
		//			{
		//				string item = method_128();
		//				if (class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), item.Length, bool_0: true).Equals(item))
		//					continue;
		//			}
		//		}
		//		if (i != int_28 && class10_0.int_71 != 3 && !class10_0.bool_69 && ((method_96(method_17()) != 2 && num8 < 1 && (!class10_0.bool_44 || !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)))) || (class10_0.bool_50 && method_96(method_17()) == 0 && num8 < 2 && (!class10_0.bool_44 || !class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true))))))
		//			continue;
		//	}
		//	else if (num7 == method_93(method_17()) && !bool_5)
		//	{
		//		continue;
		//	}
		//	struct20_3 = method_118(i);
		//	int_30 = method_145(struct20_2.x, struct20_2.y, struct20_3.x, struct20_3.y);
		//	if (int_29 > 1)
		//	{
		//		if (class10_0.bool_48)
		//		{
		//			if (int_30 >= class10_0.int_72 + 400)
		//				continue;
		//		}
		//		else if (int_30 >= class10_0.int_72 + class10_0.int_51 - 10)
		//		{
		//			continue;
		//		}
		//	}
		//	if (int_30 >= int_19)
		//	{
		//		if (flag)
		//			flag = false;
		//		else if (bool_5)
		//		{
		//			bool_5 = false;
		//		}
		//		else if (flag2)
		//		{
		//			flag2 = false;
		//		}
		//		continue;
		//	}
		//	if (num2 != 1)
		//	{
		//		if ((class10_0.bool_68 && class10_0.arrayList_1.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 6020L), 6, bool_0: true))) || (class10_0.bool_70 && class10_0.arrayList_3.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true))))
		//			continue;
		//		if (class10_0.bool_48)
		//		{
		//			arrayList_3.Clear();
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 11652L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 11808L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 11964L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 12120L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 12276L)));
		//			arrayList_3.Add((int)class36_0.method_3((uint)(uint_1 + i * 70584 + 12432L)));
		//			if (method_154() && !bool_6)
		//				continue;
		//			if (bool_6 && !method_154())
		//				bool_6 = false;
		//		}
		//		if (class10_0.bool_45 || !class10_0.bool_57 || bool_5 || class10_0.method_4().method_104() <= 0 || class10_0.method_4().method_98(class10_0.method_4().method_104()) != i || class10_0.method_4().method_102() <= 4)
		//		{
		//			if (num7 == method_93(method_17()))
		//			{
		//				int num9 = class30_0.method_13(method_17(), i);
		//				if (class10_0.bool_44)
		//				{
		//					if (int_28 == 0)
		//					{
		//						string text2 = class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true);
		//						if (!dictionary_0.ContainsValue(text2) && method_73() == 0 && class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)) && num9 != 8)
		//						{
		//							if (dictionary_0.ContainsKey(method_53()))
		//								dictionary_0[method_53()] = text2;
		//							else
		//								dictionary_0.Add(method_53(), text2);
		//							class30_0.method_74("Revenge('" + text2 + "')");
		//							continue;
		//						}
		//						if (num9 != 8)
		//							continue;
		//					}
		//					else if (num9 != 8)
		//					{
		//						continue;
		//					}
		//				}
		//				else if (num9 != 8)
		//				{
		//					continue;
		//				}
		//			}
		//			else if (class10_0.bool_44 && class36_0.method_3((uint)(uint_1 + i * 70584 + 5536L)) == 0 && method_96(method_17()) != 2 && class10_0.arrayList_5.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true)))
		//			{
		//				class30_0.method_18(2);
		//				continue;
		//			}
		//			if (class10_0.bool_141)
		//			{
		//				if (!class10_0.arrayList_4.Contains(class36_0.method_2((uint)(uint_1 + i * 70584 + 4877L), 32, bool_0: true).Trim()))
		//				{
		//					if (flag2)
		//						continue;
		//				}
		//				else
		//				{
		//					flag2 = true;
		//					if (int_30 < num3 || !flag3 || !flag4)
		//					{
		//						num3 = int_30;
		//						num = i;
		//						flag3 = true;
		//						if (!flag4)
		//						{
		//							flag4 = true;
		//							num6 = method_160(class36_0.method_2((uint)(uint_1 + i * 70584 + 5328L), 32, bool_0: true).Trim(), 4);
		//						}
		//					}
		//				}
		//			}
		//			if (class10_0.int_71 == 2 && class10_0.bool_123)
		//			{
		//				int num10 = method_160(class36_0.method_2((uint)(uint_1 + i * 70584 + 5328L), 32, bool_0: true).Trim(), 4);
		//				if (num10 > num6 || num10 < 0)
		//					continue;
		//				num6 = num10;
		//			}
		//			if (int_30 < class10_0.int_49 && !flag3 && int_20 == 1 && !bool_5)
		//			{
		//				if (class10_0.bool_31)
		//				{
		//					int int_31 = (int)class36_0.method_4((uint)(uint_1 + i * 70584 + 40L));
		//					if (method_153(int_31) <= num4)
		//					{
		//						int_20 = 0;
		//						flag3 = true;
		//						num3 = class10_0.int_49;
		//						int_19 = class10_0.int_49;
		//					}
		//				}
		//				else if (class10_0.bool_32)
		//				{
		//					if (method_152(Class49.smethod_15(((int)class36_0.method_4((uint)(uint_1 + i * 70584 + 48L)) + int_2).ToString())) <= num5)
		//					{
		//						int_20 = 0;
		//						flag3 = true;
		//						num3 = class10_0.int_49;
		//						int_19 = class10_0.int_49;
		//					}
		//				}
		//				else
		//				{
		//					int_20 = 0;
		//					flag3 = true;
		//					num3 = class10_0.int_49;
		//					int_19 = class10_0.int_49;
		//				}
		//			}
		//			if (int_20 == 0 && !bool_5)
		//			{
		//				if (class10_0.bool_31)
		//				{
		//					int int_32 = (int)class36_0.method_4((uint)(uint_1 + i * 70584 + 40L));
		//					int num11 = method_153(int_32);
		//					if (num11 == num4)
		//					{
		//						if (int_30 < num3)
		//						{
		//							num4 = num11;
		//							num3 = int_30;
		//							num = i;
		//						}
		//					}
		//					else if (num11 < num4)
		//					{
		//						num4 = num11;
		//						num3 = int_30;
		//						num = i;
		//					}
		//				}
		//				else if (!class10_0.bool_32)
		//				{
		//					if (int_30 < num3)
		//					{
		//						num3 = int_30;
		//						num = i;
		//					}
		//				}
		//				else
		//				{
		//					int num12 = method_152(Class49.smethod_15(((int)class36_0.method_4((uint)(uint_1 + i * 70584 + 48L)) + int_2).ToString()));
		//					if (num12 == num5)
		//					{
		//						if (int_30 < num3)
		//						{
		//							num5 = num12;
		//							num3 = int_30;
		//							num = i;
		//						}
		//					}
		//					else if (num12 < num5)
		//					{
		//						num5 = num12;
		//						num3 = int_30;
		//						num = i;
		//					}
		//				}
		//			}
		//			else if (int_30 < num3)
		//			{
		//				num3 = int_30;
		//				num = i;
		//			}
		//			if (flag)
		//				break;
		//			continue;
		//		}
		//		num = i;
		//		break;
		//	}
		//	int_22 = 1;
		//	break;
		//}
		//if (num == 0)
		//{
		//	if (int_20 == 0)
		//		int_20 = 1;
		//	if (class10_0.bool_48)
		//	{
		//		if (class10_0.bool_58 && !bool_6)
		//			bool_6 = true;
		//		else if (!class10_0.bool_58 && bool_6)
		//		{
		//			bool_6 = false;
		//		}
		//	}
		//}
		return num;
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
