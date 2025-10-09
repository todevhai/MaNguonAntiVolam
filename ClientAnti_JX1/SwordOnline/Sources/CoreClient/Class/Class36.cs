using RGiesecke.DllExport;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

internal class Class36
{
	public static Class36 m_Class36;

	private Class35 class35_0;

	private static object object_0;

	public Class36(Process process_0)
	{
		m_Class36 = this;
		class35_0 = new Class35();
		class35_0.method_1(process_0);
		class35_0.method_2();
	}

	public void method_0()
	{
		class35_0.method_4();
	}

	public string method_1(uint uint_0, int int_0, bool bool_0)
	{
		byte[] array = new byte[int_0];
		int int_ = 0;
		array = class35_0.method_5((IntPtr)uint_0, (uint)int_0, out int_);
		if (bool_0)
			return Class19.smethod_3(array);
		return Class19.smethod_2(array);
	}

	public string method_2(uint uint_0, int int_0, bool bool_0)
	{
		byte[] array = new byte[int_0];
		int int_ = 0;
		array = class35_0.method_5((IntPtr)uint_0, (uint)int_0, out int_);
		if (bool_0)
			return Class19.smethod_4(array);
		return Class19.smethod_2(array);
	}

	[DllExport(CallingConvention = CallingConvention.StdCall)]
	public static uint Class36_GetInfoSkill(uint uint_0)
	{
		return m_Class36.method_3(uint_0);
	}

	public uint method_3(uint uint_0)
	{
		byte[] array = new byte[4];
		int int_ = 0;
		array = class35_0.method_5((IntPtr)uint_0, 4u, out int_);
		byte b = array[3];
		array[3] = array[0];
		array[0] = b;
		b = array[2];
		array[2] = array[1];
		array[1] = b;
		return Class19.smethod_8(Class19.smethod_2(array));
	}

	public uint method_4(uint uint_0)
	{
		byte[] array = new byte[2];
		int int_ = 0;
		array = class35_0.method_5((IntPtr)uint_0, 2u, out int_);
		byte b = array[1];
		array[1] = array[0];
		array[0] = b;
		return Class19.smethod_8(Class19.smethod_2(array));
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
