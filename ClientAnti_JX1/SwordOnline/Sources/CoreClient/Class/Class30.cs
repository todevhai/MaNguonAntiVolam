using System;
using System.Text;
using System.Threading;

internal class Class30
{
	private enum Enum8
	{

	}

	private Class29 class29_0;

	public uint uint_0;

	public uint uint_1;

	public uint uint_2;

	public uint uint_3;

	public uint uint_4;

	public uint uint_5;

	public uint uint_6;

	public uint uint_7;

	private Class12 class12_0;

	private Class36 class36_0;

	private bool bool_0;

	private static object object_0;

	public Class30(Class29 class29_1, Class36 class36_1)
	{
		uint_0 = Class51.RegisterWindowMessageA("WM_KID_HOOK");
		uint_1 = Class51.RegisterWindowMessageA("WM_KID_WRITE");
		uint_2 = Class51.RegisterWindowMessageA("WM_SET_STATUS_NPC");
		uint_3 = Class51.RegisterWindowMessageA("WM_SET_STATUS");
		uint_4 = Class51.RegisterWindowMessageA("WM_SET_MY_STATUS");
		uint_5 = Class51.RegisterWindowMessageA("WM_SET_FLAG");
		uint_6 = Class51.RegisterWindowMessageA("WM_CHANGE_NAME");
		uint_7 = Class51.RegisterWindowMessageA("WM_SET_INFO_TK");
		class12_0 = new Class12();
		bool_0 = true;

		class29_0 = class29_1;
		class36_0 = class36_1;
	}

	public int method_0()
	{
		return class29_0.method_43();
	}

	public int method_1()
	{
		return class29_0.method_44();
	}

	public void method_2(bool bool_1)
	{
		bool_0 = bool_1;
	}

	public int method_3(int int_0)
	{
		return class29_0.method_39(int_0);
	}

	public void method_4()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 104u, 0u);
	}

	public void method_5()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 105u, 0u);
	}

	public void method_6(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 107u, (uint)int_0);
	}

	public void method_7(string string_0, int int_0)
	{
		method_27(string_0);
		int length = string_0.Length;
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)length);
		Class51.PostMessage(class29_0.method_14(), uint_1, 103u, (uint)int_0);
	}

	public void method_8(int int_0, int int_1)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_1, 106u, (uint)int_1);
	}

	public void method_9()
	{
		bool flag = false;
		uint num = class36_0.method_3(6907584u);
		uint num2 = class36_0.method_3(num + 256);
		uint num3 = num2;
		int num4 = default(int);
		while (true)
		{
			class36_0.method_3(num3);
			if (class36_0.method_3(num2) == 0)
				break;
			method_6(0);
			num3 += 4;
			num4++;
			Thread.Sleep(50);
			if (flag)
				return;
		}
		flag = true;
		method_7("§· xãa hÕt <color=yellow>" + num4 + " <color=red>lêi mêi", 1);
	}

	public void method_10(int int_0, int int_1, int int_2, int int_3, int int_4, int int_5)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 108u, (uint)int_4);
		Class51.PostMessage(class29_0.method_14(), uint_1, 109u, (uint)int_5);
		Class51.PostMessage(class29_0.method_14(), uint_1, 110u, (uint)int_3);
		Class51.PostMessage(class29_0.method_14(), uint_1, 111u, (uint)int_0);
	}

	public void method_11(int int_0, int int_1, int int_2, int int_3)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 108u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 109u, (uint)int_3);
		Class51.PostMessage(class29_0.method_14(), uint_1, 110u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 114u, (uint)int_0);
	}

	public void method_12(int int_0, int int_1)
	{
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_31() + 5912, 5u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_31() + 5916, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_31() + 5920, uint.MaxValue);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_31() + 5924, (uint)int_1);
	}

	public int method_13(int int_0, int int_1)
	{
		Class51.SendMessage(class29_0.method_14(), uint_1, 9u, (uint)int_0);
		Class51.SendMessage(class29_0.method_14(), uint_1, 113u, (uint)int_1);
		return class29_0.method_23();
	}

	public string method_14(int int_0, int int_1)
	{
		Class51.SendMessage(class29_0.method_14(), uint_1, 108u, (uint)int_1);
		Class51.SendMessage(class29_0.method_14(), uint_1, 115u, (uint)int_0);
		return class29_0.method_25();
	}

	public void method_15(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 49u, (uint)int_0);
	}

	public Struct20 method_16(int int_0)
	{
		return class29_0.method_118(int_0);
	}

	public Struct20 method_17()
	{
		return class29_0.method_118(class29_0.method_17());
	}

	public void method_18(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 21u, (uint)int_0);
	}

	public void method_19()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 39u, 0u);
	}

	public void method_20(string string_0)
	{
		method_26(string_0);
		Class51.PostMessage(class29_0.method_14(), uint_1, 40u, 0u);
	}

	public void method_21(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 27u, (uint)int_0);
	}

	public void method_22()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 28u, 0u);
	}

	public void method_23(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 29u, (uint)int_0);
	}

	public void method_24(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 45u, (uint)int_0);
	}

	public void method_25(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 46u, (uint)int_0);
	}

	public void method_26(string string_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 20u, 0u);
		for (int i = 0; i < string_0.Length; i++)
		{
			int num = 65536 * i + smethod_0(Convert.ToChar(string_0.Substring(i, 1)));
			Class51.PostMessage(class29_0.method_14(), uint_1, 11u, (uint)num);
		}
	}

	public void method_27(string string_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 123u, 0u);
		for (int i = 0; i < string_0.Length; i++)
		{
			int num = 65536 * i + smethod_0(Convert.ToChar(string_0.Substring(i, 1)));
			Class51.PostMessage(class29_0.method_14(), uint_1, 124u, (uint)num);
		}
	}

	public void method_28(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 5u, (uint)int_0);
	}

	public void method_29()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 35u, 0u);
	}

	public void method_30()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 30u, 0u);
	}

	public void method_31()
	{
		if (class29_0.method_139() != 0)
		{
			method_74("Open([[PlayerNpc]])");
			method_74("Open([[MissleSame]])");
			method_74("Open([[VisiblePlayer]])");
		}
	}

	public void method_32(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 94u, (uint)int_0);
	}

	public void method_33(int int_0, int int_1, int int_2, int int_3)
	{
		if (int_1 < 0 || int_1 > 360)
			int_1 = 0;
		double num = (double)int_1 * 3.14 / 180.0;
		int int_4 = (int)((double)int_2 + (double)int_0 * Math.Cos(num));
		int int_5 = (int)((double)int_3 - (double)int_0 * Math.Sin(num));
		method_68(3, int_4, int_5, class29_0.method_106());
	}

	public bool method_34(int int_0, int int_1)
	{
		int num = 0;
		int num2 = 0;
		if (int_0 > 10000 && int_1 > 10000)
		{
			int_0 /= 256;
			int_1 /= 512;
		}
		if (class29_0.method_145(class29_0.method_118(class29_0.method_17()).x, class29_0.method_118(class29_0.method_17()).y, int_0 * 256, int_1 * 512) > 300)
		{
			num = int_0 * 8;
			num2 = int_1 * 16;
			if (class29_0.method_102() != 3)
				method_32(126, num, num2);
			return false;
		}
		return true;
	}

	public void method_35(int int_0, int int_1)
	{
		int num = 0;
		int num2 = 0;
		if (int_0 > 10000 && int_1 > 10000)
		{
			int_0 /= 256;
			int_1 /= 512;
		}
		num = int_0 * 8;
		num2 = int_1 * 16;
		method_32(126, num, num2);
	}

	public void method_36(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 100u, (uint)int_0);
	}

	public void method_37(int int_0, int int_1, int int_2)
	{
		int millisecondsTimeout = 10;
		int num = 100;
		int num2 = class29_0.method_38();
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 100u, (uint)int_0);
		while (true)
		{
			num--;
			if (class29_0.method_38() == num2 && num > 0)
			{
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			break;
		}
	}

	public void method_38()
	{
		int millisecondsTimeout = 10;
		int num = 100;
		int num2 = class29_0.method_38();
		method_84(401, 365, 1);
		while (true)
		{
			num--;
			if (class29_0.method_38() == num2 && num > 0)
			{
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			break;
		}
	}

	public void method_39(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 78u, (uint)int_0);
	}

	public void method_40(int int_0, string string_0)
	{
		//Class9.smethod_34((uint)(int)class29_0.method_14(), 8);
		//Thread.Sleep(200);
		//for (int i = 0; i < string_0.Length; i++)
		//{
		//	int int_ = Class49.smethod_34(string_0.Substring(i, 1));
		//	Class9.smethod_34((uint)(int)class29_0.method_14(), int_);
		//	Thread.Sleep(300);
		//}
		//Thread.Sleep(1000);
		//Class9.smethod_34((uint)(int)class29_0.method_14(), 13);
	}

	public void method_41(int int_0)
	{
		Thread.Sleep(100);
		method_32(87, int_0, 0);
		Thread.Sleep(500);
		Class9.smethod_34((uint)(int)class29_0.method_14(), 27);
	}

	public void method_42(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5912, 5u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5916, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5920, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5924, (uint)int_2);
	}

	public void method_43(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 32, (uint)int_0);
	}

	public void method_44(uint uint_8)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 68u, uint_8);
	}

	public void method_45(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 51u, (uint)int_0);
	}

	public void method_46(int int_0)
	{
		uint num = class29_0.method_15() + 5036;
		Class51.PostMessage(class29_0.method_14(), uint_0, num, (uint)int_0);
	}

	public void method_47(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 93u, (uint)int_0);
	}

	public void method_48(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 57u, (uint)int_0);
	}

	public void method_49(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 56u, (uint)int_0);
	}

	public void method_50(string string_0, int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 20u, 0u);
		for (int i = 0; i < string_0.Length; i++)
		{
			int num = 65536 * i + smethod_0(Convert.ToChar(string_0.Substring(i, 1)));
			Class51.PostMessage(class29_0.method_14(), uint_1, 11u, (uint)num);
		}
		Class51.PostMessage(class29_0.method_14(), uint_1, 8u, (uint)int_0);
	}

	public void method_51()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 8u, 8u);
	}

	public void method_52(int int_0)
	{
		switch (int_0)
		{
			case 1:
				Class51.PostMessage(class29_0.method_14(), uint_1, 24u, 0u);
				break;
			case 2:
				Class51.PostMessage(class29_0.method_14(), uint_1, 25u, 0u);
				break;
			case 3:
				Class51.PostMessage(class29_0.method_14(), uint_1, 26u, 0u);
				break;
		}
	}

	public void method_53(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 7u, 0u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5912, 5u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5916, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5920, 0u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5924, 0u);
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, 255u);
		Class51.PostMessage(class29_0.method_14(), uint_1, 52u, (uint)int_0);
	}

	public void method_54(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 7u, 0u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5912, 5u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5916, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5920, 0u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5924, 0u);
	}

	public void method_55(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, 255u);
		Class51.PostMessage(class29_0.method_14(), uint_1, 52u, (uint)int_0);
	}

	public void method_56(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 112u, (uint)int_2);
	}

	public void method_57(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 99u, (uint)int_0);
	}

	public void method_58(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 102u, (uint)int_0);
	}

	public void method_59(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 19u, (uint)int_0);
	}

	public void method_60(uint uint_8, uint uint_9, int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_8, uint_9, (uint)int_0);
	}

	public static byte smethod_0(char char_0)
	{
		Encoding encoding = Encoding.GetEncoding(1252);
		string text = char_0.ToString();
		return encoding.GetBytes((text == null) ? "" : text)[0];
	}

	public void method_61(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 47u, (uint)int_0);
	}

	public void method_62(int int_0, int int_1)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 47u, (uint)int_0);
		if (int_1 == 1 || int_1 == 4)
			method_123("Sửa kỹ", bool_1: true);
	}

	public void method_63()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 16u, 0u);
	}

	public void method_64(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 34u, (uint)int_0);
	}

	public void method_65()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 32u, 0u);
	}

	public void method_66(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 31u, (uint)int_0);
	}

	public void method_67(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 33u, (uint)int_0);
	}

	public void method_68(int int_0, int int_1, int int_2, int int_3)
	{
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5912, (uint)(int_3 + 2));
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5916, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5920, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 53u, (uint)int_3);
	}

	public void method_69(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 71u, (uint)int_0);
	}

	public void method_70(int int_0)
	{
		uint num = class29_0.method_15() + 5020;
		Class51.PostMessage(class29_0.method_14(), uint_0, num, (uint)int_0);
	}

	public void method_71(int int_0)
	{
		uint num = class29_0.method_15() + 5020;
		Class51.PostMessage(class29_0.method_14(), uint_0, num, (uint)int_0);
	}

	public void method_72(int int_0, int int_1, int int_2)
	{
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5912, 5u);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5916, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5920, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_0, class29_0.method_15() + 5924, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_1, 10u, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_1, 52u, (uint)int_0);
	}

	public void method_73()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 17u, 0u);
	}

	public void method_74(string string_0)
	{
		method_26(string_0);
		Class51.PostMessage(class29_0.method_14(), uint_1, 14u, 0u);
	}

	public void method_75(int int_0, int int_1)
	{
		uint num = class29_0.method_15() + 264;
		uint num2 = class29_0.method_15() + 4440;
		Class51.PostMessage(class29_0.method_14(), uint_0, num, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_0, num2, (uint)int_0);
	}

	public void method_76(int int_0, int int_1, int int_2)
	{
		uint num = class29_0.method_15() + 5020;
		uint num2 = class29_0.method_15() + 264;
		uint num3 = class29_0.method_15() + 4440;
		Class51.PostMessage(class29_0.method_14(), uint_0, num2, (uint)int_2);
		Class51.PostMessage(class29_0.method_14(), uint_0, num3, (uint)int_1);
		Class51.PostMessage(class29_0.method_14(), uint_0, num, (uint)int_0);
	}

	public void method_77(int int_0)
	{
		if (class29_0.method_140() <= 0 && class29_0.method_141() <= 0 && class29_0.method_52() == 0)
		{
			int num = 0;
			int num2 = 0;
			bool flag = false;
			num = 400 + class29_0.method_118(int_0).x - class29_0.method_118(class29_0.method_17()).x;
			num2 = 300 + (class29_0.method_118(int_0).y - class29_0.method_118(class29_0.method_17()).y) / 2;
			if (class29_0.method_138() > 0)
				flag = true;
			if (!method_82(num, num2 - 2))
			{
				if (method_82(num, num2 - 38))
					method_84(num, num2 - 38, 1);
			}
			else
				method_84(num, num2 - 2, 1);
			if (flag)
				method_74("Focus([[commandline]])");
		}
		else
		{
			method_60(256u, 27u, 1835009);
			method_60(257u, 27u, 1835009);
		}
	}

	public Struct20 method_78()
	{
		int int_ = class29_0.method_135() - 400 + class29_0.method_118(class29_0.method_17()).x;
		int int_2 = (class29_0.method_136() - 300) * 2 + class29_0.method_118(class29_0.method_17()).y;
		return new Struct20(int_, int_2);
	}

	public Struct20 method_79(int int_0, int int_1)
	{
		int int_2 = int_0 + 400 - class29_0.method_118(class29_0.method_17()).x;
		int int_3 = (int_1 - class29_0.method_118(class29_0.method_17()).y) / 2 + 300;
		return new Struct20(int_2, int_3);
	}

	public bool method_80(int int_0)
	{
		int num = 0;
		int num2 = 0;
		num = 400 + class29_0.method_118(int_0).x - class29_0.method_118(class29_0.method_17()).x;
		num2 = 300 + (class29_0.method_118(int_0).y - class29_0.method_118(class29_0.method_17()).y) / 2;
		if (method_82(num, num2 - 2))
			return true;
		if (!method_82(num, num2 - 40))
			return false;
		return true;
	}

	public bool method_81(int int_0, int int_1, int int_2, int int_3)
	{
		int num = 0;
		int num2 = 0;
		num = 400 + int_2 - int_0;
		num2 = 300 + (int_3 - int_1) / 2;
		if (method_82(num, num2 - 2))
			return true;
		if (method_82(num, num2 - 40))
			return true;
		return false;
	}

	public bool method_82(int int_0, int int_1)
	{
		if (int_0 > 19 && int_0 < 120 && int_1 > 549 && int_1 < 580)
			return false;
		if (int_0 > 515 && int_0 < 612 && int_1 > 572 && int_1 < 594)
			return false;
		if (int_0 > 0 && int_0 < 672 && int_1 > 0 && int_1 < 16)
			return false;
		if (int_0 > 253 && int_0 < 565 && int_1 > 541 && int_1 < 565)
			return false;
		if (int_0 > 713 && int_0 < 785 && int_1 > 545 && int_1 < 584)
			return false;
		if (int_0 > 670 && int_0 < 800 && int_1 > 127 && int_1 < 142)
			return false;
		if (int_0 > 626 && int_0 < 693 && int_1 > 543 && int_1 < 588)
			return false;
		if (int_0 > 781 && int_0 < 800 && int_1 > 171 && int_1 < 500)
			return false;
		if (int_0 > 0 && int_0 < 360 && int_1 > 480 && int_1 < 493)
			return false;
		return true;
	}

	public void method_83()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 37u, 0u);
	}

	public void method_84(int int_0, int int_1, int int_2)
	{
		IntPtr intptr_ = smethod_1(int_0, int_1);
		method_85(intptr_, int_2);
	}

	private static IntPtr smethod_1(int int_0, int int_1)
	{
		return (IntPtr)((int_1 << 16) | (int_0 & 0xFFFF));
	}

	public void method_85(IntPtr intptr_0, int int_0)
	{
		if (int_0 == 1)
		{
			Class51.SendMessage_1(class29_0.method_14(), 512u, 0u, intptr_0);
			Class51.PostMessage_1(class29_0.method_14(), 513u, 1u, intptr_0);
			Class51.PostMessage_1(class29_0.method_14(), 514u, 0u, intptr_0);
		}
		else
		{
			Class51.PostMessage_1(class29_0.method_14(), 512u, 0u, intptr_0);
			Class51.PostMessage_1(class29_0.method_14(), 516u, 2u, intptr_0);
			Class51.PostMessage_1(class29_0.method_14(), 517u, 0u, intptr_0);
		}
	}

	public void method_86(int int_0, int int_1)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 9u, (uint)int_0);
		Class51.PostMessage(class29_0.method_14(), uint_1, 62u, (uint)int_1);
	}

	public void method_87(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 44u, (uint)int_0);
	}

	public void method_88(int int_0)
	{
		switch (int_0)
		{
			case 0:
				method_87(16);
				break;
			case 1:
				method_87(14);
				break;
			case 2:
				method_87(15);
				break;
			case 3:
				method_87(3);
				break;
			case 4:
				method_87(7);
				break;
			case 5:
				method_87(13);
				break;
			case 6:
				method_87(0);
				break;
			case 7:
				method_87(1);
				break;
			case 8:
				method_87(44);
				break;
		}
	}

	public void method_89(int int_0, int int_1)
	{
		int num = int_0 * 16;
		int num2 = int_1 * 16;
		Class51.PostMessage(class29_0.method_14(), uint_0, 6809096u, (uint)num);
		Class51.PostMessage(class29_0.method_14(), uint_0, 6809100u, (uint)num2);
	}

	public void method_90(int int_0, int int_1)
	{
		switch (int_1)
		{
			case 1:
				switch (int_0)
				{
					case 1:
						method_87(14);
						break;
					case 2:
						method_87(15);
						break;
					case 3:
						method_87(3);
						break;
					case 4:
						method_87(7);
						break;
					case 5:
						method_87(13);
						break;
					case 6:
						method_87(13);
						break;
					case 0:
						method_87(16);
						break;
				}
				break;
			case 2:
				switch (int_0)
				{
					case 1:
						method_87(5);
						break;
					case 2:
						method_87(10);
						break;
					case 3:
						method_87(4);
						break;
					case 4:
						method_87(9);
						break;
					case 5:
						method_87(19);
						break;
					case 6:
						method_87(20);
						break;
					case 0:
						method_87(15);
						break;
				}
				break;
			case 3:
				switch (int_0)
				{
					case 0:
						method_87(14);
						break;
					case 1:
						method_87(4);
						break;
					case 2:
						method_87(9);
						break;
					case 3:
						method_87(3);
						break;
					case 4:
						method_87(8);
						break;
					case 5:
						method_87(18);
						break;
					case 6:
						method_87(19);
						break;
				}
				break;
			case 4:
				switch (int_0)
				{
					case 1:
						method_87(4);
						break;
					case 2:
						method_87(9);
						break;
					case 3:
						method_87(3);
						break;
					case 4:
						method_87(8);
						break;
					case 5:
						method_87(18);
						break;
					case 6:
						method_87(19);
						break;
					case 0:
						method_87(14);
						break;
				}
				break;
			case 5:
				switch (int_0)
				{
					case 1:
						method_87(4);
						break;
					case 2:
						method_87(9);
						break;
					case 3:
						method_87(3);
						break;
					case 4:
						method_87(8);
						break;
					case 5:
						method_87(18);
						break;
					case 6:
						method_87(19);
						break;
					case 0:
						method_87(14);
						break;
				}
				break;
		}
	}

	public void method_91(int int_0)
	{
		if (class36_0.method_3(6879284u) == 0)
		{
			if (class36_0.method_3(6911384u) != 0)
				method_96(int_0);
		}
		else
			method_93(int_0);
	}

	public void method_92(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 122u, (uint)int_0);
	}

	public void method_93(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 74u, (uint)int_0);
	}

	public void method_94(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 122u, (uint)int_0);
	}

	public void method_95(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 83u, (uint)int_0);
	}

	public void method_96(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 75u, (uint)int_0);
	}

	public void method_97()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 60u, 0u);
	}

	public void method_98()
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 76u, 0u);
	}

	public void method_99(int int_0)
	{
		Class51.PostMessage(class29_0.method_14(), uint_1, 61u, (uint)int_0);
	}

	public string method_100()
	{
		uint num = 0u;
		uint num2 = 0u;
		string result = "";
		int int_ = 256;
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num3 = 0u;
			num3 = class36_0.method_3(6927704u);
			if (num == num3)
				num2 = num3 + 2644;
			else if (num3 != 0)
			{
				num2 = num + 960;
			}
			int_ = (int)class36_0.method_3(num3 + 2632);
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
				num2 = num + 4784;
		}
		if (num2 != 0)
			result = class12_0.method_2(class36_0.method_2(num2, int_, bool_0: true), 2, 5);
		return result;
	}

	public int method_101(string string_0)
	{
		uint num = 0u;
		uint num2 = 0u;
		int num3 = 0;
		int result = -1;
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num4 = 0u;
			num4 = class36_0.method_3(6927704u);
			if (num != num4)
				goto IL_0111;
			num3 = (int)class36_0.method_3(num + 1008);
			num2 = class36_0.method_3(num + 1004);
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
			{
				num3 = (int)class36_0.method_3(num + 2072);
				num2 = class36_0.method_3(num + 2068);
			}
		}
		if (num2 != 0)
		{
			for (int i = 0; i < num3; i++)
			{
				uint num5 = 0u;
				num5 = class36_0.method_3((uint)(num2 + 4 * i));
				if (num5 != 0)
				{
					int num6 = 0;
					num6 = (int)class36_0.method_3(num5 + 24);
					if (class12_0.method_2(class36_0.method_2(num5 + 28, num6 + 1, bool_0: true), 2, 5).Contains(string_0))
						result = i;
				}
			}
		}
		goto IL_0111;
	IL_0111:
		return result;
	}

	public int method_102(string string_0)
	{
		uint num = 0u;
		uint num2 = 0u;
		int num3 = 0;
		int result = -1;
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num4 = 0u;
			num4 = class36_0.method_3(6927704u);
			if (num != num4)
				goto IL_0104;
			num3 = (int)class36_0.method_3(num + 1008);
			num2 = class36_0.method_3(num + 1004);
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
			{
				num3 = (int)class36_0.method_3(num + 2072);
				num2 = class36_0.method_3(num + 2068);
			}
		}
		if (num2 != 0)
		{
			for (int i = 0; i < num3; i++)
			{
				uint num5 = 0u;
				num5 = class36_0.method_3((uint)(num2 + 4 * i));
				if (num5 != 0)
				{
					int num6 = 0;
					num6 = (int)class36_0.method_3(num5 + 24);
					if (class36_0.method_2(num5 + 28, num6 + 1, bool_0: true).Contains(string_0))
						result = i;
				}
			}
		}
		goto IL_0104;
	IL_0104:
		return result;
	}

	public int method_103()
	{
		uint num = 0u;
		int result = 0;
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num2 = 0u;
			num2 = class36_0.method_3(6927704u);
			if (num != num2)
				num = 0u;
			else
			{
				result = (int)class36_0.method_3(num + 1008);
				class36_0.method_3(num + 1004);
			}
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
			{
				result = (int)class36_0.method_3(num + 2072);
				class36_0.method_3(num + 2068);
			}
		}
		return result;
	}

	public bool method_104()
	{
		if (class36_0.method_3(6879284u) == 0 && class36_0.method_3(6911384u) == 0)
			return false;
		return true;
	}

	public int method_105()
	{
		return (int)class36_0.method_3(6879284u);
	}

	public int method_106()
	{
		return (int)class36_0.method_3(6927704u);
	}

	public int method_107()
	{
		return (int)class36_0.method_3(6911384u);
	}

	public bool method_108()
	{
		if (class36_0.method_3(6927704u) == 0 && class36_0.method_3(6879284u) == 0)
			return false;
		return true;
	}

	public bool method_109()
	{
		if (class36_0.method_3(6911384u) != 0)
			return true;
		return false;
	}

	public void method_110()
	{
		if (class36_0.method_3(6879284u) == 0)
		{
			if (class36_0.method_3(6911384u) != 0)
				method_98();
		}
		else
			method_97();
	}

	public string method_111(int int_0)
	{
		uint num = 0u;
		uint num2 = 0u;
		string result = "";
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num3 = 0u;
			num3 = class36_0.method_3(6927704u);
			if (num != num3)
				goto IL_0111;
			int num4 = 0;
			num4 = (int)class36_0.method_3(num + 1008);
			if (int_0 >= 0 && int_0 < num4)
				num2 = class36_0.method_3(num + 1004);
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
			{
				int num5 = 0;
				num5 = (int)class36_0.method_3(num + 2072);
				if (int_0 >= 0 && int_0 < num5)
					num2 = class36_0.method_3(num + 2068);
			}
		}
		if (num2 != 0)
		{
			uint num6 = 0u;
			num6 = class36_0.method_3((uint)(num2 + 4 * int_0));
			if (num6 != 0)
			{
				int num7 = 0;
				num7 = (int)class36_0.method_3(num6 + 24);
				result = class12_0.method_2(class36_0.method_2(num6 + 28, num7 + 1, bool_0: true), 2, 5);
			}
		}
		goto IL_0111;
	IL_0111:
		return result;
	}

	public string method_112(int int_0)
	{
		uint num = 0u;
		uint num2 = 0u;
		string result = "";
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num3 = 0u;
			num3 = class36_0.method_3(6927704u);
			if (num != num3)
				goto IL_0102;
			int num4 = 0;
			num4 = (int)class36_0.method_3(num + 1008);
			if (int_0 >= 0 && int_0 < num4)
				num2 = class36_0.method_3(num + 1004);
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
			{
				int num5 = 0;
				num5 = (int)class36_0.method_3(num + 2072);
				if (int_0 >= 0 && int_0 < num5)
					num2 = class36_0.method_3(num + 2068);
			}
		}
		if (num2 != 0)
		{
			uint num6 = 0u;
			num6 = class36_0.method_3((uint)(num2 + 4 * int_0));
			if (num6 != 0)
			{
				int num7 = 0;
				num7 = (int)class36_0.method_3(num6 + 24);
				result = class36_0.method_2(num6 + 28, num7 + 1, bool_0: true);
			}
		}
		goto IL_0102;
	IL_0102:
		return result;
	}

	public bool method_113(string string_0)
	{
		int millisecondsTimeout = 10;
		int num = 100;
		bool result = false;
		while (true)
		{
			num--;
			if (!method_100().Contains(string_0))
			{
				if (num <= 0)
					break;
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			result = true;
			break;
		}
		return result;
	}

	public void method_114()
	{
		int millisecondsTimeout = 10;
		int num = 500;
		bool flag = false;
		while (true)
		{
			num--;
			if (class29_0.method_132() <= 0 || flag)
			{
				if (num > 0)
				{
					Thread.Sleep(millisecondsTimeout);
					continue;
				}
				break;
			}
			method_60(256u, 13u, 1835009);
			method_60(258u, 13u, 1835009);
			flag = true;
			break;
		}
	}

	public void method_115(string string_0, bool bool_1)
	{
		int millisecondsTimeout = 10;
		int num = 300;
		int num2 = -1;
		while (true)
		{
			num2 = method_101(string_0);
			num--;
			if (num2 != -1 || num <= 0)
				break;
			Thread.Sleep(millisecondsTimeout);
		}
		if (num2 == -1)
			return;
		method_92(num2);
		if (!bool_1)
			return;
		while (method_104())
		{
			if (method_104() && method_111(num2).Contains(string_0))
				method_92(num2);
			Thread.Sleep(millisecondsTimeout);
		}
	}

	public bool method_116()
	{
		bool result = false;
		int millisecondsTimeout = 10;
		int num = 300;
		while (true)
		{
			num--;
			if (!method_104())
			{
				if (num <= 0)
					break;
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			result = true;
			break;
		}
		return result;
	}

	public bool method_117(string string_0, bool bool_1)
	{
		int millisecondsTimeout = 10;
		int num = 600;
		int num2 = -1;
		bool result = false;
		while (true)
		{
			num2 = method_101(string_0);
			num--;
			if (num2 != -1 || num <= 0)
				break;
			Thread.Sleep(millisecondsTimeout);
		}
		if (num2 != -1)
		{
			method_92(num2);
			if (bool_1)
			{
				num = 600;
				while (true)
				{
					num2 = method_101(string_0);
					num--;
					if (num2 != -1)
					{
						if (num <= 0)
							break;
						Thread.Sleep(millisecondsTimeout);
						continue;
					}
					result = true;
					break;
				}
			}
		}
		return result;
	}

	public bool method_118(int int_0, bool bool_1)
	{
		int millisecondsTimeout = 10;
		int num = 300;
		int num2 = -1;
		bool result = false;
		while (true)
		{
			num--;
			if (!method_104())
			{
				if (num <= 0)
					break;
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			num2 = 0;
			break;
		}
		if (num2 != -1)
		{
			method_92(int_0);
			if (bool_1)
			{
				while (method_104())
				{
					Thread.Sleep(millisecondsTimeout);
				}
				result = true;
			}
		}
		return result;
	}

	public string method_119()
	{
		uint num = 0u;
		uint num2 = 0u;
		string text = "";
		int int_ = 384;
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num3 = 0u;
			num3 = class36_0.method_3(6927704u);
			if (num == num3)
				num2 = num3 + 2644;
			else if (num3 != 0)
			{
				num2 = num + 960;
			}
			int_ = (int)class36_0.method_3(num3 + 2632);
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
				num2 = num + 4784;
		}
		if (num2 != 0)
			text = class36_0.method_1(num2, int_, bool_0: true).Replace("\u0003", "");
		return text.Replace("\u0002", "").Replace("ÿÿ", "");
	}

	public string method_120()
	{
		uint num = 0u;
		uint num2 = 0u;
		string text = "";
		int int_ = 256;
		num = class36_0.method_3(6879284u);
		if (num == 0)
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
				num2 = num + 4784;
		}
		else
		{
			uint num3 = 0u;
			num3 = class36_0.method_3(6927704u);
			if (num == num3)
				num2 = num3 + 2644;
			else if (num3 != 0)
			{
				num2 = num + 960;
			}
			int_ = (int)class36_0.method_3(num3 + 2632);
		}
		if (num2 != 0)
			text = class36_0.method_1(num2, int_, bool_0: true).Replace("\u0003", "");
		return class12_0.method_2(text.Replace("\u0002", "").Replace("ÿ", ""), 2, 5);
	}

	public string method_121()
	{
		uint num = 0u;
		uint num2 = 0u;
		string text = "";
		int int_ = 256;
		num = class36_0.method_3(6879284u);
		if (num != 0)
		{
			uint num3 = 0u;
			num3 = class36_0.method_3(6927704u);
			if (num == num3)
				num2 = num3 + 2644;
			else if (num3 != 0)
			{
				num2 = num + 960;
			}
			int_ = (int)class36_0.method_3(num3 + 2632);
		}
		else
		{
			num = class36_0.method_3(6911384u);
			if (num != 0)
				num2 = num + 4784;
		}
		if (num2 != 0)
			text = class36_0.method_1(num2, int_, bool_0: true).Replace("\u0003", "");
		return text.Replace("\u0002", "").Replace("ÿ", "");
	}

	public string method_122()
	{
		string result = "";
		int millisecondsTimeout = 10;
		int num = 600;
		int num2 = -1;
		while (true)
		{
			num--;
			if (!method_104())
			{
				if (num <= 0)
					break;
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			num2 = 1;
			break;
		}
		if (num2 != -1)
			result = method_119();
		return result;
	}

	public void method_123(string string_0, bool bool_1)
	{
		int millisecondsTimeout = 10;
		int num = 300;
		int num2 = -1;
		while (true)
		{
			num2 = method_101(string_0);
			num--;
			if (num2 != -1 || num <= 0)
				break;
			Thread.Sleep(millisecondsTimeout);
		}
		if (num2 == -1)
			return;
		method_92(num2);
		if (!bool_1)
			return;
		num = 500;
		while (true)
		{
			num--;
			if (method_104() && num > 0)
			{
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			break;
		}
	}

	public void method_124(string string_0, bool bool_1)
	{
		int millisecondsTimeout = 10;
		int num = 300;
		int num2 = -1;
		while (true)
		{
			num2 = method_102(string_0);
			num--;
			if (num2 != -1 || num <= 0)
				break;
			Thread.Sleep(millisecondsTimeout);
		}
		if (num2 == -1)
			return;
		method_92(num2);
		if (!bool_1)
			return;
		num = 500;
		while (true)
		{
			num--;
			if (method_104() && num > 0)
			{
				Thread.Sleep(millisecondsTimeout);
				continue;
			}
			break;
		}
	}

	internal static bool smethod_2()
	{
		return object_0 == null;
	}
}
