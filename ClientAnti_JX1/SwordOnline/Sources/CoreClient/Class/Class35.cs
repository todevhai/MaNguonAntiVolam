using System;
using System.Diagnostics;

internal class Class35
{
	private Process process_0;

	private IntPtr intptr_0;

	internal static object object_0;

	public Class35()
	{
		intptr_0 = IntPtr.Zero;
	}

	public Process method_0()
	{
		return process_0;
	}

	public void method_1(Process process_1)
	{
		process_0 = process_1;
	}

	public void method_2()
	{
		intptr_0 = Class34.OpenProcess(16u, 1, (uint)process_0.Id);
	}

	public void method_3()
	{
		intptr_0 = Class34.OpenProcess(2035711u, 0, (uint)process_0.Id);
	}

	public void method_4()
	{
		if (Class34.CloseHandle(intptr_0) == 0)
			throw new Exception("CloseHandle failed");
	}

	public byte[] method_5(IntPtr intptr_1, uint uint_0, out int int_0)
	{
		byte[] array = new byte[uint_0];
		Class34.ReadProcessMemory(intptr_0, intptr_1, array, uint_0, out var intptr_2);
		int_0 = intptr_2.ToInt32();
		return array;
	}

	public void method_6(IntPtr intptr_1, byte[] byte_0)
	{
		Class34.VirtualProtectEx(intptr_0, intptr_1, 64u, 4u, out var _);
		Class34.VirtualAllocEx(intptr_0, intptr_1, 64u, 4096u, 4u);
		Class34.WriteProcessMemory(intptr_0, intptr_1, byte_0, 32u, out var _);
	}

	public void method_7(IntPtr intptr_1, uint uint_0, uint uint_1, uint uint_2)
	{
		Class34.VirtualAllocEx(intptr_0, intptr_1, uint_0, uint_1, uint_2);
	}

	public void method_8(IntPtr intptr_1, uint uint_0, uint uint_1)
	{
		Class34.VirtualProtectEx(intptr_0, intptr_1, uint_0, uint_1, out var _);
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
