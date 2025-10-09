using System;
using System.Runtime.InteropServices;

internal class Class34
{
	[Flags]
	internal enum Enum10
	{

	}

	private static object object_0;

	[DllImport("KERNEL32.DLL")]
	public static extern IntPtr OpenProcess(uint uint_0, int int_0, uint uint_1);

	[DllImport("KERNEL32.DLL")]
	public static extern int CloseHandle(IntPtr intptr_0);

	[DllImport("KERNEL32.DLL")]
	public static extern int ReadProcessMemory(IntPtr intptr_0, IntPtr intptr_1, [In][Out] byte[] byte_0, uint uint_0, out IntPtr intptr_2);

	[DllImport("KERNEL32.DLL")]
	public static extern int WriteProcessMemory(IntPtr intptr_0, IntPtr intptr_1, [In][Out] byte[] byte_0, uint uint_0, out IntPtr intptr_2);

	[DllImport("KERNEL32.DLL", ExactSpelling = true, SetLastError = true)]
	public static extern IntPtr VirtualAllocEx(IntPtr intptr_0, IntPtr intptr_1, uint uint_0, uint uint_1, uint uint_2);

	[DllImport("KERNEL32.DLL", SetLastError = true)]
	public static extern bool VirtualProtectEx(IntPtr intptr_0, IntPtr intptr_1, uint uint_0, uint uint_1, out uint uint_2);

	[DllImport("KERNEL32.DLL", SetLastError = true)]
	public static extern bool VirtualProtect(IntPtr intptr_0, uint uint_0, uint uint_1, out uint uint_2);

	public Class34()
	{
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
