using System;
using System.Globalization;
using System.Text;

internal class Class19
{
	private static object object_0;

	public Class19()
	{
	}

	public static int smethod_0(string string_0)
	{
		int num = 0;
		for (int i = 0; i < string_0.Length; i++)
		{
			if (smethod_10(string_0[i]))
				num++;
		}
		if (num % 2 != 0)
			num--;
		return num / 2;
	}

	public static byte[] smethod_1(string string_0, out int int_0)
	{
		int_0 = 0;
		string text = "";
		for (int i = 0; i < string_0.Length; i++)
		{
			char char_ = string_0[i];
			if (!smethod_10(char_))
				int_0++;
			else
				text += char_;
		}
		if (text.Length % 2 != 0)
		{
			int_0++;
			text = text.Substring(0, text.Length - 1);
		}
		byte[] array = new byte[text.Length / 2];
		int num = 0;
		for (int j = 0; j < array.Length; j++)
		{
			string string_ = new string(new char[2]
			{
				text[num],
				text[num + 1]
			});
			array[j] = smethod_11(string_);
			num += 2;
		}
		return array;
	}

	public static string smethod_2(object object_1)
	{
		string text = "";
		for (int i = 0; i < ((Array)object_1).Length; i++)
		{
			text += ((byte[])object_1)[i].ToString("X2");
		}
		return text;
	}

	public static string smethod_3(object object_1)
	{
		StringBuilder stringBuilder = new StringBuilder();
		for (int i = 0; i < ((Array)object_1).Length; i++)
		{
			if (Convert.ToChar(((byte[])object_1)[i]).GetHashCode() != 0)
				stringBuilder.Append(Convert.ToChar(((byte[])object_1)[i]));
		}
		return stringBuilder.ToString();
	}

	public static string smethod_4(object object_1)
	{
		string text = "";
		for (int i = 0; i < ((Array)object_1).Length && Convert.ToChar(((byte[])object_1)[i]).GetHashCode() != 0; i++)
		{
			text += Convert.ToChar(((byte[])object_1)[i]);
		}
		return text;
	}

	public static byte smethod_5(char char_0)
	{
		Encoding encoding = Encoding.GetEncoding(1252);
		string text = char_0.ToString();
		return encoding.GetBytes((text == null) ? "" : text)[0];
	}

	public static byte[] smethod_6(string string_0)
	{
		byte[] array = new byte[32];
		for (int i = 0; i < string_0.Length; i++)
		{
			array[i] = Convert.ToByte(Convert.ToChar(string_0.Substring(i, 1)));
		}
		return array;
	}

	public static uint smethod_7(object object_1)
	{
		return Convert.ToUInt32(smethod_2(object_1), 16);
	}

	public static uint smethod_8(string string_0)
	{
		return Convert.ToUInt32(string_0, 16);
	}

	public static bool smethod_9(string string_0)
	{
		bool result = true;
		for (int i = 0; i < string_0.Length; i++)
		{
			if (!smethod_10(string_0[i]))
			{
				result = false;
				break;
			}
		}
		return result;
	}

	public static bool smethod_10(char char_0)
	{
		int num = Convert.ToInt32('A');
		int num2 = Convert.ToInt32('0');
		char_0 = char.ToUpper(char_0);
		int num3 = Convert.ToInt32(char_0);
		if (num3 >= num && num3 < num + 6)
			return true;
		if (num3 >= num2 && num3 < num2 + 10)
			return true;
		return false;
	}

	private static byte smethod_11(string string_0)
	{
		if (string_0.Length > 2 || string_0.Length <= 0)
			throw new ArgumentException("hex must be 1 or 2 characters in length");
		return byte.Parse(string_0, NumberStyles.HexNumber);
	}

	internal static bool smethod_12()
	{
		return object_0 == null;
	}
}
