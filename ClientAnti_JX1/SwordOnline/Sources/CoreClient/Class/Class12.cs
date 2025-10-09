using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

internal class Class12
{
	private string[,] string_0;

	internal static object object_0;

	public Class12()
	{
		method_0();
	}

	private void method_0()
	{
		string_0 = new string[135, 6];
		string_0[0, 0] = "NCR";
		string_0[0, 1] = "UTF";
		string_0[0, 2] = "TCV";
		string_0[0, 3] = "VNI";
		string_0[0, 4] = "UTH";
		string_0[0, 5] = "UNI";
		method_9();
		method_10();
		method_11();
		method_12();
		method_13();
		method_15();
	}
	private int method_1(string string_1)
	{
		switch (string_1)
		{
			default:
				return 2;
			case "NCR":
				return 7;
			case "UNI":
			case "UTH":
				return 1;
			case "UTF":
				return 3;
		}
	}
	public string method_2(string string_1, int int_0, int int_1)
	{
		if (string_1.Trim() == "")
			return "";
		if (int_0 == int_1)
			return string_1;
		string text = "";
		string text2 = "";
		if (int_0 == -1)
		{
			int num = method_3(method_8(string_1));
			if (num <= -1)
				return string_1;
			int_0 = num;
		}
		if (int_1 == -1)
			int_1 = 0;
		int num2 = 0;
		int num3 = method_1(string_0[0, int_0]);
		int num4 = ((num3 <= 1) ? 1 : (num3 - 1));
		string text3 = "";
		string_1 += "       ";
		while (num2 < string_1.Length - 7)
		{
			for (int num5 = num3; num5 >= num4; num5--)
			{
				text = string_1.Substring(num2, num5);
				text2 = "";
				for (int i = 1; i < 135; i++)
				{
					if (text == string_0[i, int_0])
					{
						text2 = string_0[i, int_1];
						num2 += num5;
						break;
					}
				}
				if (text2 != "" || num5 == 5)
					break;
			}
			if (text2 != "")
			{
				text3 += text2;
				continue;
			}
			text3 += text.Substring(0, 1);
			num2++;
		}
		string_1 = text3.TrimEnd();
		return string_1;
	}
	private int method_3(string string_1)
	{
		for (int i = 0; i < 6; i++)
		{
			if (string_0[0, i] == string_1)
				return i;
		}
		return -1;
	}
	private bool method_7(string string_1, bool bool_0)
	{
		if (string_1.Length > 2)
			return false;
		string[] array = new string[37]
		{
			"í",
			"ì",
			"ó",
			"ò",
			"ô",
			"ñ",
			"î",
			"Ê",
			"È",
			"É",
			"á",
			"à",
			"â",
			"è",
			"é",
			"ê",
			"ù",
			"ý",
			"ú",
			"ö",
			"Í",
			"Ì",
			"Ó",
			"Ò",
			"Ô",
			"Ñ",
			"Î",
			"Õ",
			"Ý",
			"Ã",
			"oà",
			"oá",
			"oã",
			"uû",
			"OÁ",
			"OÀ",
			"OÃ"
		};
		string[] array2 = new string[44]
		{
			"ă",
			"â",
			"ê",
			"ô",
			"ơ",
			"ư",
			"đ",
			"í",
			"ì",
			"ó",
			"ò",
			"ô",
			"ñ",
			"î",
			"Ê",
			"È",
			"É",
			"á",
			"à",
			"â",
			"è",
			"é",
			"ê",
			"ù",
			"ý",
			"ú",
			"ö",
			"Í",
			"Ì",
			"Ó",
			"Ò",
			"Ô",
			"Ñ",
			"Î",
			"Õ",
			"Ý",
			"Ã",
			"oà",
			"oá",
			"oã",
			"uû",
			"OÁ",
			"OÀ",
			"OÃ"
		};
		string[] array3 = (bool_0 ? array2 : array);
		for (int i = 0; i < array3.Length; i++)
		{
			if (string.Compare(string_1, array3[i], ignoreCase: true) == 0)
				return true;
		}
		return false;
	}
	private string method_8(string string_1)
	{
		if (!(string_1.Trim() == ""))
		{
			int num = -1;
			string text = "";
			string_1 += "       ";
			for (int i = 0; i < string_1.Length - 7; i++)
			{
				for (int num2 = 7; num2 > 0; num2--)
				{
					text = string_1.Substring(i, num2);
					for (int j = 0; j < 6; j++)
					{
						if (num2 == 4)
							break;
						if (num2 == 5)
							break;
						if (num2 >= 6 && j != 0)
							break;
						if ((num2 == 3 && j != 1) || ((j == 3 || j == 2 || j == 4) && num2 > 2))
							continue;
						for (int k = 1; k < 135; k++)
						{
							if (text == string_0[k, j])
							{
								if (method_7(text, j == 4 || j == 5))
								{
									num = j;
									break;
								}
								return string_0[0, j];
							}
						}
					}
				}
			}
			if (num >= 0)
				return string_0[0, num];
			return "";
		}
		return "";
	}
	private void method_9()
	{
		string_0[1, 5] = "á";
		string_0[2, 5] = "à";
		string_0[3, 5] = "ả";
		string_0[4, 5] = "ã";
		string_0[5, 5] = "ạ";
		string_0[6, 5] = "ă";
		string_0[7, 5] = "ắ";
		string_0[8, 5] = "ằ";
		string_0[9, 5] = "ẳ";
		string_0[10, 5] = "ẵ";
		string_0[11, 5] = "ặ";
		string_0[12, 5] = "â";
		string_0[13, 5] = "ấ";
		string_0[14, 5] = "ầ";
		string_0[15, 5] = "ẩ";
		string_0[16, 5] = "ẫ";
		string_0[17, 5] = "ậ";
		string_0[18, 5] = "é";
		string_0[19, 5] = "è";
		string_0[20, 5] = "ẻ";
		string_0[21, 5] = "ẽ";
		string_0[22, 5] = "ẹ";
		string_0[23, 5] = "ê";
		string_0[24, 5] = "ế";
		string_0[25, 5] = "ề";
		string_0[26, 5] = "ể";
		string_0[27, 5] = "ễ";
		string_0[28, 5] = "ệ";
		string_0[29, 5] = "í";
		string_0[30, 5] = "ì";
		string_0[31, 5] = "ỉ";
		string_0[32, 5] = "ĩ";
		string_0[33, 5] = "ị";
		string_0[34, 5] = "ó";
		string_0[35, 5] = "ò";
		string_0[36, 5] = "ỏ";
		string_0[37, 5] = "õ";
		string_0[38, 5] = "ọ";
		string_0[39, 5] = "ô";
		string_0[40, 5] = "ố";
		string_0[41, 5] = "ồ";
		string_0[42, 5] = "ổ";
		string_0[43, 5] = "ỗ";
		string_0[44, 5] = "ộ";
		string_0[45, 5] = "ơ";
		string_0[46, 5] = "ớ";
		string_0[47, 5] = "ờ";
		string_0[48, 5] = "ở";
		string_0[49, 5] = "ỡ";
		string_0[50, 5] = "ợ";
		string_0[51, 5] = "ú";
		string_0[52, 5] = "ù";
		string_0[53, 5] = "ủ";
		string_0[54, 5] = "ũ";
		string_0[55, 5] = "ụ";
		string_0[56, 5] = "ư";
		string_0[57, 5] = "ứ";
		string_0[58, 5] = "ừ";
		string_0[59, 5] = "ử";
		string_0[60, 5] = "ữ";
		string_0[61, 5] = "ự";
		string_0[62, 5] = "ý";
		string_0[63, 5] = "ỳ";
		string_0[64, 5] = "ỷ";
		string_0[65, 5] = "ỹ";
		string_0[66, 5] = "ỵ";
		string_0[67, 5] = "đ";
		string_0[68, 5] = "Á";
		string_0[69, 5] = "À";
		string_0[70, 5] = "Ả";
		string_0[71, 5] = "Ã";
		string_0[72, 5] = "Ạ";
		string_0[73, 5] = "Ă";
		string_0[74, 5] = "Ắ";
		string_0[75, 5] = "Ằ";
		string_0[76, 5] = "Ẳ";
		string_0[77, 5] = "Ẵ";
		string_0[78, 5] = "Ặ";
		string_0[79, 5] = "Â";
		string_0[80, 5] = "Ấ";
		string_0[81, 5] = "Ầ";
		string_0[82, 5] = "Ẩ";
		string_0[83, 5] = "Ẫ";
		string_0[84, 5] = "Ậ";
		string_0[85, 5] = "É";
		string_0[86, 5] = "È";
		string_0[87, 5] = "Ẻ";
		string_0[88, 5] = "Ẽ";
		string_0[89, 5] = "Ẹ";
		string_0[90, 5] = "Ê";
		string_0[91, 5] = "Ế";
		string_0[92, 5] = "Ề";
		string_0[93, 5] = "Ể";
		string_0[94, 5] = "Ễ";
		string_0[95, 5] = "Ệ";
		string_0[96, 5] = "Í";
		string_0[97, 5] = "Ì";
		string_0[98, 5] = "Ỉ";
		string_0[99, 5] = "Ĩ";
		string_0[100, 5] = "Ị";
		string_0[101, 5] = "Ó";
		string_0[102, 5] = "Ò";
		string_0[103, 5] = "Ỏ";
		string_0[104, 5] = "Õ";
		string_0[105, 5] = "Ọ";
		string_0[106, 5] = "Ô";
		string_0[107, 5] = "Ố";
		string_0[108, 5] = "Ồ";
		string_0[109, 5] = "Ổ";
		string_0[110, 5] = "Ỗ";
		string_0[111, 5] = "Ộ";
		string_0[112, 5] = "Ơ";
		string_0[113, 5] = "Ớ";
		string_0[114, 5] = "Ờ";
		string_0[115, 5] = "Ở";
		string_0[116, 5] = "Ỡ";
		string_0[117, 5] = "Ợ";
		string_0[118, 5] = "Ú";
		string_0[119, 5] = "Ù";
		string_0[120, 5] = "Ủ";
		string_0[121, 5] = "Ũ";
		string_0[122, 5] = "Ụ";
		string_0[123, 5] = "Ư";
		string_0[124, 5] = "Ứ";
		string_0[125, 5] = "Ừ";
		string_0[126, 5] = "Ử";
		string_0[127, 5] = "Ữ";
		string_0[128, 5] = "Ự";
		string_0[129, 5] = "Ý";
		string_0[130, 5] = "Ỳ";
		string_0[131, 5] = "Ỷ";
		string_0[132, 5] = "Ỹ";
		string_0[133, 5] = "Ỵ";
		string_0[134, 5] = "Đ";
	}

	private void method_10()
	{
		string_0[1, 3] = "aù";
		string_0[2, 3] = "aø";
		string_0[3, 3] = "aû";
		string_0[4, 3] = "aõ";
		string_0[5, 3] = "aï";
		string_0[6, 3] = "aê";
		string_0[7, 3] = "aé";
		string_0[8, 3] = "aè";
		string_0[9, 3] = "aú";
		string_0[10, 3] = "aü";
		string_0[11, 3] = "aë";
		string_0[12, 3] = "aâ";
		string_0[13, 3] = "aá";
		string_0[14, 3] = "aà";
		string_0[15, 3] = "aå";
		string_0[16, 3] = "aã";
		string_0[17, 3] = "aä";
		string_0[18, 3] = "eù";
		string_0[19, 3] = "eø";
		string_0[20, 3] = "eû";
		string_0[21, 3] = "eõ";
		string_0[22, 3] = "eï";
		string_0[23, 3] = "eâ";
		string_0[24, 3] = "eá";
		string_0[25, 3] = "eà";
		string_0[26, 3] = "eå";
		string_0[27, 3] = "eã";
		string_0[28, 3] = "eä";
		string_0[29, 3] = "í";
		string_0[30, 3] = "ì";
		string_0[31, 3] = "æ";
		string_0[32, 3] = "ó";
		string_0[33, 3] = "ò";
		string_0[34, 3] = "où";
		string_0[35, 3] = "oø";
		string_0[36, 3] = "oû";
		string_0[37, 3] = "oõ";
		string_0[38, 3] = "oï";
		string_0[39, 3] = "oâ";
		string_0[40, 3] = "oá";
		string_0[41, 3] = "oà";
		string_0[42, 3] = "oå";
		string_0[43, 3] = "oã";
		string_0[44, 3] = "oä";
		string_0[45, 3] = "ô";
		string_0[46, 3] = "ôù";
		string_0[47, 3] = "ôø";
		string_0[48, 3] = "ôû";
		string_0[49, 3] = "ôõ";
		string_0[50, 3] = "ôï";
		string_0[51, 3] = "uù";
		string_0[52, 3] = "uø";
		string_0[53, 3] = "uû";
		string_0[54, 3] = "uõ";
		string_0[55, 3] = "uï";
		string_0[56, 3] = "ö";
		string_0[57, 3] = "öù";
		string_0[58, 3] = "öø";
		string_0[59, 3] = "öû";
		string_0[60, 3] = "öõ";
		string_0[61, 3] = "öï";
		string_0[62, 3] = "yù";
		string_0[63, 3] = "yø";
		string_0[64, 3] = "yû";
		string_0[65, 3] = "yõ";
		string_0[66, 3] = "î";
		string_0[67, 3] = "ñ";
		string_0[68, 3] = "AÙ";
		string_0[69, 3] = "AØ";
		string_0[70, 3] = "AÛ";
		string_0[71, 3] = "AÕ";
		string_0[72, 3] = "AÏ";
		string_0[73, 3] = "AÊ";
		string_0[74, 3] = "AÉ";
		string_0[75, 3] = "AÈ";
		string_0[76, 3] = "AÚ";
		string_0[77, 3] = "AÜ";
		string_0[78, 3] = "AË";
		string_0[79, 3] = "AÂ";
		string_0[80, 3] = "AÁ";
		string_0[81, 3] = "AÀ";
		string_0[82, 3] = "AÅ";
		string_0[83, 3] = "AÃ";
		string_0[84, 3] = "AÄ";
		string_0[85, 3] = "EÙ";
		string_0[86, 3] = "EØ";
		string_0[87, 3] = "EÛ";
		string_0[88, 3] = "EÕ";
		string_0[89, 3] = "EÏ";
		string_0[90, 3] = "EÂ";
		string_0[91, 3] = "EÁ";
		string_0[92, 3] = "EÀ";
		string_0[93, 3] = "EÅ";
		string_0[94, 3] = "EÃ";
		string_0[95, 3] = "EÄ";
		string_0[96, 3] = "Í";
		string_0[97, 3] = "Ì";
		string_0[98, 3] = "Æ";
		string_0[99, 3] = "Ó";
		string_0[100, 3] = "Ò";
		string_0[101, 3] = "OÙ";
		string_0[102, 3] = "OØ";
		string_0[103, 3] = "OÛ";
		string_0[104, 3] = "OÕ";
		string_0[105, 3] = "OÏ";
		string_0[106, 3] = "OÂ";
		string_0[107, 3] = "OÁ";
		string_0[108, 3] = "OÀ";
		string_0[109, 3] = "OÅ";
		string_0[110, 3] = "OÃ";
		string_0[111, 3] = "OÄ";
		string_0[112, 3] = "Ô";
		string_0[113, 3] = "ÔÙ";
		string_0[114, 3] = "ÔØ";
		string_0[115, 3] = "ÔÛ";
		string_0[116, 3] = "ÔÕ";
		string_0[117, 3] = "ÔÏ";
		string_0[118, 3] = "UÙ";
		string_0[119, 3] = "UØ";
		string_0[120, 3] = "UÛ";
		string_0[121, 3] = "UÕ";
		string_0[122, 3] = "UÏ";
		string_0[123, 3] = "Ö";
		string_0[124, 3] = "ÖÙ";
		string_0[125, 3] = "ÖØ";
		string_0[126, 3] = "ÖÛ";
		string_0[127, 3] = "ÖÕ";
		string_0[128, 3] = "ÖÏ";
		string_0[129, 3] = "YÙ";
		string_0[130, 3] = "YØ";
		string_0[131, 3] = "YÛ";
		string_0[132, 3] = "YÕ";
		string_0[133, 3] = "Î";
		string_0[134, 3] = "Ñ";
	}

	private void method_11()
	{
		string_0[1, 2] = "\u00b8";
		string_0[2, 2] = "µ";
		string_0[3, 2] = "¶";
		string_0[4, 2] = "·";
		string_0[5, 2] = "¹";
		string_0[6, 2] = "\u00a8";
		string_0[7, 2] = "¾";
		string_0[8, 2] = "»";
		string_0[9, 2] = "¼";
		string_0[10, 2] = "½";
		string_0[11, 2] = "Æ";
		string_0[12, 2] = "©";
		string_0[13, 2] = "Ê";
		string_0[14, 2] = "Ç";
		string_0[15, 2] = "È";
		string_0[16, 2] = "É";
		string_0[17, 2] = "Ë";
		string_0[18, 2] = "Ð";
		string_0[19, 2] = "Ì";
		string_0[20, 2] = "Î";
		string_0[21, 2] = "Ï";
		string_0[22, 2] = "Ñ";
		string_0[23, 2] = "ª";
		string_0[24, 2] = "Õ";
		string_0[25, 2] = "Ò";
		string_0[26, 2] = "Ó";
		string_0[27, 2] = "Ô";
		string_0[28, 2] = "Ö";
		string_0[29, 2] = "Ý";
		string_0[30, 2] = "×";
		string_0[31, 2] = "Ø";
		string_0[32, 2] = "Ü";
		string_0[33, 2] = "Þ";
		string_0[34, 2] = "ã";
		string_0[35, 2] = "ß";
		string_0[36, 2] = "á";
		string_0[37, 2] = "â";
		string_0[38, 2] = "ä";
		string_0[39, 2] = "«";
		string_0[40, 2] = "è";
		string_0[41, 2] = "å";
		string_0[42, 2] = "æ";
		string_0[43, 2] = "ç";
		string_0[44, 2] = "é";
		string_0[45, 2] = "¬";
		string_0[46, 2] = "í";
		string_0[47, 2] = "ê";
		string_0[48, 2] = "ë";
		string_0[49, 2] = "ì";
		string_0[50, 2] = "î";
		string_0[51, 2] = "ó";
		string_0[52, 2] = "ï";
		string_0[53, 2] = "ñ";
		string_0[54, 2] = "ò";
		string_0[55, 2] = "ô";
		string_0[56, 2] = "­";
		string_0[57, 2] = "ø";
		string_0[58, 2] = "õ";
		string_0[59, 2] = "ö";
		string_0[60, 2] = "÷";
		string_0[61, 2] = "ù";
		string_0[62, 2] = "ý";
		string_0[63, 2] = "ú";
		string_0[64, 2] = "û";
		string_0[65, 2] = "ü";
		string_0[66, 2] = "þ";
		string_0[67, 2] = "®";
		string_0[68, 2] = "\u00b8";
		string_0[69, 2] = "µ";
		string_0[70, 2] = "¶";
		string_0[71, 2] = "·";
		string_0[72, 2] = "¹";
		string_0[73, 2] = "¡";
		string_0[74, 2] = "¾";
		string_0[75, 2] = "»";
		string_0[76, 2] = "¼";
		string_0[77, 2] = "½";
		string_0[78, 2] = "Æ";
		string_0[79, 2] = "¢";
		string_0[80, 2] = "Ê";
		string_0[81, 2] = "Ç";
		string_0[82, 2] = "È";
		string_0[83, 2] = "É";
		string_0[84, 2] = "Ë";
		string_0[85, 2] = "Ð";
		string_0[86, 2] = "Ì";
		string_0[87, 2] = "Î";
		string_0[88, 2] = "Ï";
		string_0[89, 2] = "Ñ";
		string_0[90, 2] = "£";
		string_0[91, 2] = "Õ";
		string_0[92, 2] = "Ò";
		string_0[93, 2] = "Ó";
		string_0[94, 2] = "Ô";
		string_0[95, 2] = "Ö";
		string_0[96, 2] = "Ý";
		string_0[97, 2] = "×";
		string_0[98, 2] = "Ø";
		string_0[99, 2] = "Ü";
		string_0[100, 2] = "Þ";
		string_0[101, 2] = "ã";
		string_0[102, 2] = "ß";
		string_0[103, 2] = "á";
		string_0[104, 2] = "â";
		string_0[105, 2] = "ä";
		string_0[106, 2] = "¤";
		string_0[107, 2] = "è";
		string_0[108, 2] = "å";
		string_0[109, 2] = "æ";
		string_0[110, 2] = "ç";
		string_0[111, 2] = "é";
		string_0[112, 2] = "¥";
		string_0[113, 2] = "í";
		string_0[114, 2] = "ê";
		string_0[115, 2] = "ë";
		string_0[116, 2] = "ì";
		string_0[117, 2] = "î";
		string_0[118, 2] = "ó";
		string_0[119, 2] = "ï";
		string_0[120, 2] = "ñ";
		string_0[121, 2] = "ò";
		string_0[122, 2] = "ô";
		string_0[123, 2] = "¦";
		string_0[124, 2] = "ø";
		string_0[125, 2] = "õ";
		string_0[126, 2] = "ö";
		string_0[127, 2] = "÷";
		string_0[128, 2] = "ù";
		string_0[129, 2] = "ý";
		string_0[130, 2] = "ú";
		string_0[131, 2] = "û";
		string_0[132, 2] = "ü";
		string_0[133, 2] = "þ";
		string_0[134, 2] = "§";
	}

	private void method_12()
	{
		string_0[1, 4] = "a\u0301";
		string_0[2, 4] = "a\u0300";
		string_0[3, 4] = "a\u0309";
		string_0[4, 4] = "a\u0303";
		string_0[5, 4] = "a\u0323";
		string_0[6, 4] = "ă";
		string_0[7, 4] = "ă\u0301";
		string_0[8, 4] = "ă\u0300";
		string_0[9, 4] = "ă\u0309";
		string_0[10, 4] = "ă\u0303";
		string_0[11, 4] = "ă\u0323";
		string_0[12, 4] = "â";
		string_0[13, 4] = "â\u0301";
		string_0[14, 4] = "â\u0300";
		string_0[15, 4] = "â\u0309";
		string_0[16, 4] = "â\u0303";
		string_0[17, 4] = "â\u0323";
		string_0[18, 4] = "e\u0301";
		string_0[19, 4] = "e\u0300";
		string_0[20, 4] = "e\u0309";
		string_0[21, 4] = "e\u0303";
		string_0[22, 4] = "e\u0323";
		string_0[23, 4] = "ê";
		string_0[24, 4] = "ê\u0301";
		string_0[25, 4] = "ê\u0300";
		string_0[26, 4] = "ê\u0309";
		string_0[27, 4] = "ê\u0303";
		string_0[28, 4] = "ê\u0323";
		string_0[29, 4] = "i\u0301";
		string_0[30, 4] = "i\u0300";
		string_0[31, 4] = "i\u0309";
		string_0[32, 4] = "i\u0303";
		string_0[33, 4] = "i\u0323";
		string_0[34, 4] = "o\u0301";
		string_0[35, 4] = "o\u0300";
		string_0[36, 4] = "o\u0309";
		string_0[37, 4] = "o\u0303";
		string_0[38, 4] = "o\u0323";
		string_0[39, 4] = "ô";
		string_0[40, 4] = "ô\u0301";
		string_0[41, 4] = "ô\u0300";
		string_0[42, 4] = "ô\u0309";
		string_0[43, 4] = "ô\u0303";
		string_0[44, 4] = "ô\u0323";
		string_0[45, 4] = "ơ";
		string_0[46, 4] = "ơ\u0301";
		string_0[47, 4] = "ơ\u0300";
		string_0[48, 4] = "ơ\u0309";
		string_0[49, 4] = "ơ\u0303";
		string_0[50, 4] = "ơ\u0323";
		string_0[51, 4] = "u\u0301";
		string_0[52, 4] = "u\u0300";
		string_0[53, 4] = "u\u0309";
		string_0[54, 4] = "u\u0303";
		string_0[55, 4] = "u\u0323";
		string_0[56, 4] = "ư";
		string_0[57, 4] = "ư\u0301";
		string_0[58, 4] = "ư\u0300";
		string_0[59, 4] = "ư\u0309";
		string_0[60, 4] = "ư\u0303";
		string_0[61, 4] = "ư\u0323";
		string_0[62, 4] = "y\u0301";
		string_0[63, 4] = "y\u0300";
		string_0[64, 4] = "y\u0309";
		string_0[65, 4] = "y\u0303";
		string_0[66, 4] = "y\u0323";
		string_0[67, 4] = "đ";
		string_0[68, 4] = "A\u0301";
		string_0[69, 4] = "A\u0300";
		string_0[70, 4] = "A\u0309";
		string_0[71, 4] = "A\u0303";
		string_0[72, 4] = "A\u0323";
		string_0[73, 4] = "Ă";
		string_0[74, 4] = "Ă\u0301";
		string_0[75, 4] = "Ă\u0300";
		string_0[76, 4] = "Ă\u0309";
		string_0[77, 4] = "Ă\u0303";
		string_0[78, 4] = "Ă\u0323";
		string_0[79, 4] = "Â";
		string_0[80, 4] = "Â\u0301";
		string_0[81, 4] = "Â\u0300";
		string_0[82, 4] = "Â\u0309";
		string_0[83, 4] = "Â\u0303";
		string_0[84, 4] = "Â\u0323";
		string_0[85, 4] = "E\u0301";
		string_0[86, 4] = "E\u0300";
		string_0[87, 4] = "E\u0309";
		string_0[88, 4] = "E\u0303";
		string_0[89, 4] = "E\u0323";
		string_0[90, 4] = "Ê";
		string_0[91, 4] = "Ê\u0301";
		string_0[92, 4] = "Ê\u0300";
		string_0[93, 4] = "Ê\u0309";
		string_0[94, 4] = "Ê\u0303";
		string_0[95, 4] = "Ê\u0323";
		string_0[96, 4] = "I\u0301";
		string_0[97, 4] = "I\u0300";
		string_0[98, 4] = "I\u0309";
		string_0[99, 4] = "I\u0303";
		string_0[100, 4] = "I\u0323";
		string_0[101, 4] = "O\u0301";
		string_0[102, 4] = "O\u0300";
		string_0[103, 4] = "O\u0309";
		string_0[104, 4] = "O\u0303";
		string_0[105, 4] = "O\u0323";
		string_0[106, 4] = "Ô";
		string_0[107, 4] = "Ô\u0301";
		string_0[108, 4] = "Ô\u0300";
		string_0[109, 4] = "Ô\u0309";
		string_0[110, 4] = "Ô\u0303";
		string_0[111, 4] = "Ô\u0323";
		string_0[112, 4] = "Ơ";
		string_0[113, 4] = "Ơ\u0301";
		string_0[114, 4] = "Ơ\u0300";
		string_0[115, 4] = "Ơ\u0309";
		string_0[116, 4] = "Ơ\u0303";
		string_0[117, 4] = "Ơ\u0323";
		string_0[118, 4] = "U\u0301";
		string_0[119, 4] = "U\u0300";
		string_0[120, 4] = "U\u0309";
		string_0[121, 4] = "U\u0303";
		string_0[122, 4] = "U\u0323";
		string_0[123, 4] = "Ư";
		string_0[124, 4] = "Ư\u0301";
		string_0[125, 4] = "Ư\u0300";
		string_0[126, 4] = "Ư\u0309";
		string_0[127, 4] = "Ư\u0303";
		string_0[128, 4] = "Ư\u0323";
		string_0[129, 4] = "Y\u0301";
		string_0[130, 4] = "Y\u0300";
		string_0[131, 4] = "Y\u0309";
		string_0[132, 4] = "Y\u0303";
		string_0[133, 4] = "Y\u0323";
		string_0[134, 4] = "Đ";
	}

	private void method_13()
	{
		string_0[1, 1] = "Ã¡";
		string_0[2, 1] = "Ã\u00a0";
		string_0[3, 1] = "áº£";
		string_0[4, 1] = "Ã£";
		string_0[5, 1] = "áº¡";
		string_0[6, 1] = "Äƒ";
		string_0[7, 1] = "áº\u00af";
		string_0[8, 1] = "áº±";
		string_0[9, 1] = "áº³";
		string_0[10, 1] = "áºµ";
		string_0[11, 1] = "áº·";
		string_0[12, 1] = "Ã¢";
		string_0[13, 1] = "áº¥";
		string_0[14, 1] = "áº§";
		string_0[15, 1] = "áº©";
		string_0[16, 1] = "áº«";
		string_0[17, 1] = "áº­";
		string_0[18, 1] = "Ã©";
		string_0[19, 1] = "Ã\u00a8";
		string_0[20, 1] = "áº»";
		string_0[21, 1] = "áº½";
		string_0[22, 1] = "áº¹";
		string_0[23, 1] = "Ãª";
		string_0[24, 1] = "áº¿";
		string_0[25, 1] = "á»\u0081";
		string_0[26, 1] = "á»ƒ";
		string_0[27, 1] = "á»…";
		string_0[28, 1] = "á»‡";
		string_0[29, 1] = "Ã­";
		string_0[30, 1] = "Ã¬";
		string_0[31, 1] = "á»‰";
		string_0[32, 1] = "Ä©";
		string_0[33, 1] = "á»‹";
		string_0[34, 1] = "Ã³";
		string_0[35, 1] = "Ã²";
		string_0[36, 1] = "á»\u008f";
		string_0[37, 1] = "Ãµ";
		string_0[38, 1] = "á»\u008d";
		string_0[39, 1] = "Ã\u00b4";
		string_0[40, 1] = "á»‘";
		string_0[41, 1] = "á»“";
		string_0[42, 1] = "á»•";
		string_0[43, 1] = "á»—";
		string_0[44, 1] = "á»™";
		string_0[45, 1] = "Æ¡";
		string_0[46, 1] = "á»›";
		string_0[47, 1] = "á»\u009d";
		string_0[48, 1] = "á»Ÿ";
		string_0[49, 1] = "á»¡";
		string_0[50, 1] = "á»£";
		string_0[51, 1] = "Ãº";
		string_0[52, 1] = "Ã¹";
		string_0[53, 1] = "á»§";
		string_0[54, 1] = "Å©";
		string_0[55, 1] = "á»¥";
		string_0[56, 1] = "Æ°";
		string_0[57, 1] = "á»©";
		string_0[58, 1] = "á»«";
		string_0[59, 1] = "á»­";
		string_0[60, 1] = "á»\u00af";
		string_0[61, 1] = "á»±";
		string_0[62, 1] = "Ã½";
		string_0[63, 1] = "á»³";
		string_0[64, 1] = "\u009dá»·".Substring(1);
		string_0[65, 1] = "á»¹";
		string_0[66, 1] = "á»µ";
		string_0[67, 1] = "Ä‘";
		string_0[68, 1] = "Ã\u0081";
		string_0[69, 1] = "Ã€";
		string_0[70, 1] = "áº¢";
		string_0[71, 1] = "Ãƒ";
		string_0[72, 1] = "áº ";
		string_0[73, 1] = "Ä‚";
		string_0[74, 1] = "áº®";
		string_0[75, 1] = "áº°";
		string_0[76, 1] = "áº²";
		string_0[77, 1] = "áº\u00b4";
		string_0[78, 1] = "áº¶";
		string_0[79, 1] = "Ã‚";
		string_0[80, 1] = "áº¤";
		string_0[81, 1] = "áº¦";
		string_0[82, 1] = "áº\u00a8";
		string_0[83, 1] = "áºª";
		string_0[84, 1] = "áº¬";
		string_0[85, 1] = "Ã‰";
		string_0[86, 1] = "Ãˆ";
		string_0[87, 1] = "áºº";
		string_0[88, 1] = "áº¼";
		string_0[89, 1] = "áº\u00b8";
		string_0[90, 1] = "ÃŠ";
		string_0[91, 1] = "áº¾";
		string_0[92, 1] = "á»€";
		string_0[93, 1] = "á»‚";
		string_0[94, 1] = "á»„";
		string_0[95, 1] = "á»†";
		string_0[96, 1] = "Ã\u008d";
		string_0[97, 1] = "ÃŒ";
		string_0[98, 1] = "á»ˆ";
		string_0[99, 1] = "Ä\u00a8";
		string_0[100, 1] = "á»Š";
		string_0[101, 1] = "Ã“";
		string_0[102, 1] = "Ã’";
		string_0[103, 1] = "á»Ž";
		string_0[104, 1] = "Ã•";
		string_0[105, 1] = "á»Œ";
		string_0[106, 1] = "Ã”";
		string_0[107, 1] = "á»\u0090";
		string_0[108, 1] = "á»’";
		string_0[109, 1] = "á»”";
		string_0[110, 1] = "á»–";
		string_0[111, 1] = "á»\u02dc";
		string_0[112, 1] = "Æ ";
		string_0[113, 1] = "á»š";
		string_0[114, 1] = "á»œ";
		string_0[115, 1] = "á»ž";
		string_0[116, 1] = "á» ";
		string_0[117, 1] = "á»¢";
		string_0[118, 1] = "Ãš";
		string_0[119, 1] = "Ã™";
		string_0[120, 1] = "á»¦";
		string_0[121, 1] = "Å\u00a8";
		string_0[122, 1] = "á»¤";
		string_0[123, 1] = "Æ\u00af";
		string_0[124, 1] = "á»\u00a8";
		string_0[125, 1] = "á»ª";
		string_0[126, 1] = "á»¬";
		string_0[127, 1] = "á»®";
		string_0[128, 1] = "á»°";
		string_0[129, 1] = "Ã\u009d";
		string_0[130, 1] = "á»²";
		string_0[131, 1] = "á»¶";
		string_0[132, 1] = "á»\u00b8";
		string_0[133, 1] = "á»\u00b4";
		string_0[134, 1] = "Ä\u0090";
	}

	private void method_14()
	{
		string_0[1, 6] = "a'";
		string_0[2, 6] = "a`";
		string_0[3, 6] = "a?";
		string_0[4, 6] = "a~";
		string_0[5, 6] = "a.";
		string_0[6, 6] = "a(";
		string_0[7, 6] = "a('";
		string_0[8, 6] = "a(`";
		string_0[9, 6] = "a(?";
		string_0[10, 6] = "a(~";
		string_0[11, 6] = "a(.";
		string_0[12, 6] = "a^";
		string_0[13, 6] = "a^'";
		string_0[14, 6] = "a^`";
		string_0[15, 6] = "a^?";
		string_0[16, 6] = "a^~";
		string_0[17, 6] = "a^.";
		string_0[18, 6] = "e'";
		string_0[19, 6] = "e`";
		string_0[20, 6] = "e?";
		string_0[21, 6] = "e~";
		string_0[22, 6] = "e.";
		string_0[23, 6] = "e^";
		string_0[24, 6] = "e^'";
		string_0[25, 6] = "e^`";
		string_0[26, 6] = "e^?";
		string_0[27, 6] = "e^~";
		string_0[28, 6] = "e^.";
		string_0[29, 6] = "i'";
		string_0[30, 6] = "i`";
		string_0[31, 6] = "i?";
		string_0[32, 6] = "i~";
		string_0[33, 6] = "i.";
		string_0[34, 6] = "o'";
		string_0[35, 6] = "o`";
		string_0[36, 6] = "o?";
		string_0[37, 6] = "o~";
		string_0[38, 6] = "o.";
		string_0[39, 6] = "o^";
		string_0[40, 6] = "o^'";
		string_0[41, 6] = "o^`";
		string_0[42, 6] = "o^?";
		string_0[43, 6] = "o^~";
		string_0[44, 6] = "o^.";
		string_0[45, 6] = "o+";
		string_0[46, 6] = "o+'";
		string_0[47, 6] = "o+`";
		string_0[48, 6] = "o+?";
		string_0[49, 6] = "o+~";
		string_0[50, 6] = "o+.";
		string_0[51, 6] = "u'";
		string_0[52, 6] = "u`";
		string_0[53, 6] = "u?";
		string_0[54, 6] = "u~";
		string_0[55, 6] = "u.";
		string_0[56, 6] = "u+";
		string_0[57, 6] = "u+'";
		string_0[58, 6] = "u+`";
		string_0[59, 6] = "u+?";
		string_0[60, 6] = "u+~";
		string_0[61, 6] = "u+.";
		string_0[62, 6] = "y'";
		string_0[63, 6] = "y`";
		string_0[64, 6] = "y?";
		string_0[65, 6] = "y~";
		string_0[66, 6] = "y.";
		string_0[67, 6] = "dd";
		string_0[68, 6] = "A'";
		string_0[69, 6] = "A`";
		string_0[70, 6] = "A?";
		string_0[71, 6] = "A~";
		string_0[72, 6] = "A.";
		string_0[73, 6] = "A(";
		string_0[74, 6] = "A('";
		string_0[75, 6] = "A(`";
		string_0[76, 6] = "A(?";
		string_0[77, 6] = "A(~";
		string_0[78, 6] = "A(.";
		string_0[79, 6] = "A^";
		string_0[80, 6] = "A^'";
		string_0[81, 6] = "A^`";
		string_0[82, 6] = "A^?";
		string_0[83, 6] = "A^~";
		string_0[84, 6] = "A^.";
		string_0[85, 6] = "E'";
		string_0[86, 6] = "E`";
		string_0[87, 6] = "E?";
		string_0[88, 6] = "E~";
		string_0[89, 6] = "E.";
		string_0[90, 6] = "E^";
		string_0[91, 6] = "E^'";
		string_0[92, 6] = "E^`";
		string_0[93, 6] = "E^?";
		string_0[94, 6] = "E^~";
		string_0[95, 6] = "E^.";
		string_0[96, 6] = "I'";
		string_0[97, 6] = "I`";
		string_0[98, 6] = "I?";
		string_0[99, 6] = "I~";
		string_0[100, 6] = "I.";
		string_0[101, 6] = "O'";
		string_0[102, 6] = "O`";
		string_0[103, 6] = "O?";
		string_0[104, 6] = "O~";
		string_0[105, 6] = "O.";
		string_0[106, 6] = "O^";
		string_0[107, 6] = "O^'";
		string_0[108, 6] = "O^`";
		string_0[109, 6] = "O^?";
		string_0[110, 6] = "O^~";
		string_0[111, 6] = "O^.";
		string_0[112, 6] = "O+";
		string_0[113, 6] = "O+'";
		string_0[114, 6] = "O+`";
		string_0[115, 6] = "O+?";
		string_0[116, 6] = "O+~";
		string_0[117, 6] = "O+.";
		string_0[118, 6] = "U'";
		string_0[119, 6] = "U`";
		string_0[120, 6] = "U?";
		string_0[121, 6] = "U~";
		string_0[122, 6] = "U.";
		string_0[123, 6] = "U+";
		string_0[124, 6] = "U+'";
		string_0[125, 6] = "U+`";
		string_0[126, 6] = "U+?";
		string_0[127, 6] = "U+~";
		string_0[128, 6] = "U+.";
		string_0[129, 6] = "Y'";
		string_0[130, 6] = "Y`";
		string_0[131, 6] = "Y?";
		string_0[132, 6] = "Y~";
		string_0[133, 6] = "Y.";
		string_0[134, 6] = "DD";
	}

	private void method_15()
	{
		string_0[1, 0] = "&#225;";
		string_0[2, 0] = "&#224;";
		string_0[3, 0] = "&#7843;";
		string_0[4, 0] = "&#227;";
		string_0[5, 0] = "&#7841;";
		string_0[6, 0] = "&#259;";
		string_0[7, 0] = "&#7855;";
		string_0[8, 0] = "&#7857;";
		string_0[9, 0] = "&#7859;";
		string_0[10, 0] = "&#7861;";
		string_0[11, 0] = "&#7863;";
		string_0[12, 0] = "&#226;";
		string_0[13, 0] = "&#7845;";
		string_0[14, 0] = "&#7847;";
		string_0[15, 0] = "&#7849;";
		string_0[16, 0] = "&#7851;";
		string_0[17, 0] = "&#7853;";
		string_0[18, 0] = "&#233;";
		string_0[19, 0] = "&#232;";
		string_0[20, 0] = "&#7867;";
		string_0[21, 0] = "&#7869;";
		string_0[22, 0] = "&#7865;";
		string_0[23, 0] = "&#234;";
		string_0[24, 0] = "&#7871;";
		string_0[25, 0] = "&#7873;";
		string_0[26, 0] = "&#7875;";
		string_0[27, 0] = "&#7877;";
		string_0[28, 0] = "&#7879;";
		string_0[29, 0] = "&#237;";
		string_0[30, 0] = "&#236;";
		string_0[31, 0] = "&#7881;";
		string_0[32, 0] = "&#297;";
		string_0[33, 0] = "&#7883;";
		string_0[34, 0] = "&#243;";
		string_0[35, 0] = "&#242;";
		string_0[36, 0] = "&#7887;";
		string_0[37, 0] = "&#245;";
		string_0[38, 0] = "&#7885;";
		string_0[39, 0] = "&#244;";
		string_0[40, 0] = "&#7889;";
		string_0[41, 0] = "&#7891;";
		string_0[42, 0] = "&#7893;";
		string_0[43, 0] = "&#7895;";
		string_0[44, 0] = "&#7897;";
		string_0[45, 0] = "&#417;";
		string_0[46, 0] = "&#7899;";
		string_0[47, 0] = "&#7901;";
		string_0[48, 0] = "&#7903;";
		string_0[49, 0] = "&#7905;";
		string_0[50, 0] = "&#7907;";
		string_0[51, 0] = "&#250;";
		string_0[52, 0] = "&#249;";
		string_0[53, 0] = "&#7911;";
		string_0[54, 0] = "&#361;";
		string_0[55, 0] = "&#7909;";
		string_0[56, 0] = "&#432;";
		string_0[57, 0] = "&#7913;";
		string_0[58, 0] = "&#7915;";
		string_0[59, 0] = "&#7917;";
		string_0[60, 0] = "&#7919;";
		string_0[61, 0] = "&#7921;";
		string_0[62, 0] = "&#253;";
		string_0[63, 0] = "&#7923;";
		string_0[64, 0] = "&#7927;";
		string_0[65, 0] = "&#7929;";
		string_0[66, 0] = "&#7925;";
		string_0[67, 0] = "&#273;";
		string_0[68, 0] = "&#193;";
		string_0[69, 0] = "&#192;";
		string_0[70, 0] = "&#7842;";
		string_0[71, 0] = "&#195;";
		string_0[72, 0] = "&#7840;";
		string_0[73, 0] = "&#258;";
		string_0[74, 0] = "&#7854;";
		string_0[75, 0] = "&#7856;";
		string_0[76, 0] = "&#7858;";
		string_0[77, 0] = "&#7860;";
		string_0[78, 0] = "&#7862;";
		string_0[79, 0] = "&#194;";
		string_0[80, 0] = "&#7844;";
		string_0[81, 0] = "&#7846;";
		string_0[82, 0] = "&#7848;";
		string_0[83, 0] = "&#7850;";
		string_0[84, 0] = "&#7852;";
		string_0[85, 0] = "&#201;";
		string_0[86, 0] = "&#200;";
		string_0[87, 0] = "&#7866;";
		string_0[88, 0] = "&#7868;";
		string_0[89, 0] = "&#7864;";
		string_0[90, 0] = "&#202;";
		string_0[91, 0] = "&#7870;";
		string_0[92, 0] = "&#7872;";
		string_0[93, 0] = "&#7874;";
		string_0[94, 0] = "&#7876;";
		string_0[95, 0] = "&#7878;";
		string_0[96, 0] = "&#205;";
		string_0[97, 0] = "&#204;";
		string_0[98, 0] = "&#7880;";
		string_0[99, 0] = "&#296;";
		string_0[100, 0] = "&#7882;";
		string_0[101, 0] = "&#211;";
		string_0[102, 0] = "&#210;";
		string_0[103, 0] = "&#7886;";
		string_0[104, 0] = "&#213;";
		string_0[105, 0] = "&#7884;";
		string_0[106, 0] = "&#212;";
		string_0[107, 0] = "&#7888;";
		string_0[108, 0] = "&#7890;";
		string_0[109, 0] = "&#7892;";
		string_0[110, 0] = "&#7894;";
		string_0[111, 0] = "&#7896;";
		string_0[112, 0] = "&#416;";
		string_0[113, 0] = "&#7898;";
		string_0[114, 0] = "&#7900;";
		string_0[115, 0] = "&#7902;";
		string_0[116, 0] = "&#7904;";
		string_0[117, 0] = "&#7906;";
		string_0[118, 0] = "&#218;";
		string_0[119, 0] = "&#217;";
		string_0[120, 0] = "&#7910;";
		string_0[121, 0] = "&#360;";
		string_0[122, 0] = "&#7908;";
		string_0[123, 0] = "&#431;";
		string_0[124, 0] = "&#7912;";
		string_0[125, 0] = "&#7914;";
		string_0[126, 0] = "&#7916;";
		string_0[127, 0] = "&#7918;";
		string_0[128, 0] = "&#7920;";
		string_0[129, 0] = "&#221;";
		string_0[130, 0] = "&#7922;";
		string_0[131, 0] = "&#7926;";
		string_0[132, 0] = "&#7928;";
		string_0[133, 0] = "&#7924;";
		string_0[134, 0] = "&#272;";
	}
}
