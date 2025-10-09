using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

internal class Class9
{
	internal struct Struct14
	{
		public string string_0;

		public uint uint_0;
	}

	public static int int_0;

	public static int int_1;

	public static int int_2;

	public static int int_3;

	public static int int_4;

	public static int int_5;

	public static int int_6;

	public static int int_7;

	public static int int_8;

	public static int int_9;

	public static int int_10;

	internal static object object_0;

	[DllImport("user32.dll")]
	public static extern int MapVirtualKey(uint uint_0, uint uint_1);
	[DllImport("user32.dll", CharSet = CharSet.Unicode)]
	public static extern short VkKeyScan(int int_11);
	[DllImport("user32.dll", EntryPoint = "PostMessageA")]
	public static extern bool PostMessageA_1(IntPtr intptr_0, int int_11, IntPtr intptr_1, IntPtr intptr_2);
	public static void smethod_34(uint uint_0, int int_11)
	{
		int num = ((short)MapVirtualKey((uint)VkKeyScan(int_11), 0u) << 16) | 1;
		int num2 = num | -1073741824;
		PostMessageA_1((IntPtr)uint_0, int_7, (IntPtr)int_11, (IntPtr)num);
		PostMessageA_1((IntPtr)uint_0, int_8, (IntPtr)int_11, (IntPtr)num2);
	}
}
