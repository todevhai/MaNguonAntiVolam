using System;
using System.Runtime.InteropServices;
using System.Text;

internal class Class20
{
	private string string_0;

	private int int_0;

	internal static object object_0;

	[DllImport("kernel32", SetLastError = true)]
	private static extern int WritePrivateProfileString(string string_1, string string_2, string string_3, string string_4);

	[DllImport("kernel32", SetLastError = true)]
	private static extern int WritePrivateProfileStruct(string string_1, string string_2, string string_3, int int_1, string string_4);

	[DllImport("kernel32", SetLastError = true)]
	private static extern int GetPrivateProfileString(string string_1, string string_2, string string_3, byte[] byte_0, int int_1, string string_4);

	[DllImport("kernel32", SetLastError = true)]
	private static extern int GetPrivateProfileStruct(string string_1, string string_2, byte[] byte_0, int int_1, string string_3);

	public Class20(string string_1)
	{
		int_0 = 320;

		string_0 = string_1;
	}

	public string method_0()
	{
		return string_0;
	}

	public void method_1(string string_1)
	{
		string_0 = string_1;
	}

	public int method_2()
	{
		return int_0;
	}

	public void method_3(int int_1)
	{
		if (int_1 <= 32767)
		{
			if (int_1 < 1)
				int_0 = 1;
			else
				int_0 = int_1;
		}
		else
			int_0 = 32767;
	}

	public string method_4(string string_1, string string_2, string string_3)
	{
		return method_11(string_1, string_2, string_3);
	}

	public string method_5(string string_1, string string_2)
	{
		return method_11(string_1, string_2, "");
	}

	public void method_6(string string_1, string string_2, string string_3)
	{
		WritePrivateProfileString(string_1, string_2, string_3, string_0);
	}

	public void method_7(string string_1, string string_2)
	{
		WritePrivateProfileString(string_1, string_2, null, string_0);
	}

	public void method_8(string string_1, ref Array array_0)
	{
		array_0 = method_11(string_1, null, null).Split(default(char));
	}

	public void method_9(ref Array array_0)
	{
		array_0 = method_11(null, null, null).Split(default(char));
	}

	public void method_10(string string_1)
	{
		WritePrivateProfileString(string_1, null, null, string_0);
	}

	private string method_11(string string_1, string string_2, string string_3)
	{
		byte[] array = new byte[int_0];
		int privateProfileString = GetPrivateProfileString(string_1, string_2, string_3, array, int_0, string_0);
		return Encoding.GetEncoding(1252).GetString(array, 0, privateProfileString).TrimEnd(default(char));
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
