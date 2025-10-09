using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;


internal class Class18
{
	public static string string_0;

	public static string string_1;

	public static string string_2;

	public static bool bool_0;

	public static string string_3;

	public static string string_4;

	public static string string_5;

	public static string string_6;

	public static string string_7;

	public static string string_8;

	public static string string_9;

	public static string string_10;

	public static string string_11;

	public static string string_12;

	public static string string_13;

	public static string string_14;

	public static string string_15;

	public static string string_16;

	public static string string_17;

	public static string string_18;

	private static string[] string_19;

	public static object object_0;

	public static string string_20;

	public static string string_21;

	public static string string_22;

	public static string string_23;

	public static string string_24;

	public static string string_25;

	public static string string_26;

	public static byte[] byte_0;

	public static string string_27;

	internal static object object_1;
	public static string smethod_4(string string_28)
	{
		try
		{
			MD5 mD = MD5.Create();
			FileStream inputStream = File.OpenRead(string_28);
			return BitConverter.ToString(mD.ComputeHash(inputStream)).Replace("-", string.Empty).ToLower();
		}
		catch
		{
		}
		return "";
	}
}
