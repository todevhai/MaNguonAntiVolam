using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;

internal class Class51
{
	internal delegate bool Delegate2(IntPtr hWnd, int lParam);

	internal struct Struct33
	{
		[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 60)]
		public string string_0;

		[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 60)]
		public string string_1;

		public int int_0;
	}

	internal delegate void Delegate3(IntPtr hWnd, uint nMsg, int nIDEvent, int dwTime);

	internal struct Struct34
	{
		public int int_0;

		public int int_1;

		public int int_2;

		public int int_3;
	}

	internal enum Enum28 : uint
	{

	}
	internal struct KOutputTextParam
	{
		int nX;
		int nY;
		int nZ;
		short nSkipLine;
		short nNumLine;
		int Color;
		int BorderColor;
		short nVertAlign;  //0:¾ÓÖÐ/1:ÉÏ¶ÔÆë/2:ÏÂ¶ÔÆë
		int bPicPackInSingleLine;

		//¼ÓÒ»¸ö¹¹Ôìº¯Êý,Ö÷ÒªÊÇnVertAlignµÄ³õÊ¼»¯[wxb 2003-7-10]
		//KOutputTextParam()
		//{
		//	nX = nY = nZ = 0;
		//	nSkipLine = nNumLine = 0;
		//	Color = 0;
		//	nVertAlign = 0; //È±Ê¡Îª¾ÓÖÐ
		//	bPicPackInSingleLine = false;
		//}
	}

	private static object object_0;

	[DllImport("core.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
	public static extern int SetHook(IntPtr intptr_0);

	[DllImport("core.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
	public static extern int UnHook(IntPtr intptr_0);

	[DllImport("user32.dll", SetLastError = true)]
	public static extern IntPtr SetWindowText(IntPtr intptr_0, string string_0);

	[DllImport("User32", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
	public static extern uint RegisterWindowMessageA(string string_0);

	[DllImport("user32.dll", CharSet = CharSet.Auto)]
	public static extern int MessageBox(IntPtr intptr_0, string string_0, string string_1, int int_0);

	[DllImport("user32.dll", SetLastError = true)]
	public static extern IntPtr FindWindow(string string_0, string string_1);

	[DllImport("user32.dll")]
	public static extern IntPtr FindWindowEx(IntPtr intptr_0, IntPtr intptr_1, string string_0, string string_1);

	[DllImport("user32.dll", SetLastError = true)]
	public static extern int GetWindowThreadProcessId(IntPtr intptr_0, out int int_0);

	[DllImport("user32.dll")]
	public static extern bool EnumWindows(Delegate2 delegate2_0, int int_0);

	[DllImport("user32.dll")]
	public static extern IntPtr GetShellWindow();

	[DllImport("user32.dll", CharSet = CharSet.Auto, EntryPoint = "GetWindowThreadProcessId", SetLastError = true)]
	public static extern int GetWindowThreadProcessId_1(IntPtr intptr_0, out uint uint_0);

	[DllImport("KidHookPr.DLL")]
	public static extern int GetLicInfo(ref Struct33 struct33_0);

	[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	public static extern int GetClassName(IntPtr intptr_0, StringBuilder stringBuilder_0, int int_0);

	[DllImport("user32.dll")]
	public static extern IntPtr PostMessage(IntPtr intptr_0, uint uint_0, uint uint_1, uint uint_2);

	[DllImport("user32.dll")]
	public static extern short GetAsyncKeyState(int int_0);

	[DllImport("user32.dll")]
	public static extern short GetKeyState(int int_0);

	[DllImport("user32.dll")]
	public static extern IntPtr SetWindowPos(IntPtr intptr_0, int int_0, int int_1, int int_2, int int_3, int int_4, uint uint_0);

	[DllImport("user32.dll")]
	public static extern IntPtr GetLastActivePopup(IntPtr intptr_0);

	[DllImport("user32.dll")]
	public static extern bool ScreenToClient(IntPtr intptr_0, ref Struct25 struct25_0);

	[DllImport("user32.dll")]
	public static extern bool ClientToScreen(IntPtr intptr_0, ref Struct25 struct25_0);

	[DllImport("user32.dll")]
	public static extern bool ShowWindowAsync(IntPtr intptr_0, int int_0);

	[DllImport("user32.dll")]
	public static extern bool SetCursorPos(int int_0, int int_1);

	[DllImport("user32.dll")]
	public static extern bool GetCursorPos(out System.Drawing.Point point_0);

	[DllImport("user32.dll")]
	public static extern bool IsWindow(IntPtr intptr_0);

	[DllImport("user32.dll")]
	public static extern IntPtr SendMessage(IntPtr intptr_0, uint uint_0, uint uint_1, uint uint_2);

	[DllImport("user32.dll", EntryPoint = "SendMessage")]
	public static extern IntPtr SendMessage_1(IntPtr intptr_0, uint uint_0, uint uint_1, IntPtr intptr_1);

	[DllImport("user32.dll", EntryPoint = "PostMessage")]
	public static extern IntPtr PostMessage_1(IntPtr intptr_0, uint uint_0, uint uint_1, IntPtr intptr_1);

	[DllImport("KERNEL32.DLL", ExactSpelling = true, SetLastError = true)]
	public static extern IntPtr VirtualAllocEx(IntPtr intptr_0, IntPtr intptr_1, uint uint_0, uint uint_1, uint uint_2);

	[DllImport("User32")]
	public static extern int SetTimer(IntPtr intptr_0, int int_0, int int_1, Delegate3 delegate3_0);

	[DllImport("User32", SetLastError = true)]
	[return: MarshalAs(UnmanagedType.Bool)]
	public static extern bool RegisterHotKey(IntPtr intptr_0, int int_0, uint uint_0, uint uint_1);

	[DllImport("User32", SetLastError = true)]
	public static extern int UnregisterHotKey(IntPtr intptr_0, int int_0);

	[DllImport("kernel32", SetLastError = true)]
	public static extern short GlobalAddAtom(string string_0);

	[DllImport("kernel32", SetLastError = true)]
	public static extern short GlobalDeleteAtom(short short_0);

	[DllImport("User32")]
	public static extern int KillTimer(IntPtr intptr_0, int int_0);

	[DllImport("user32.dll")]
	public static extern IntPtr SetParent(IntPtr intptr_0, IntPtr intptr_1);
	[DllImport("user32.dll")]
	public static extern IntPtr BringWindowToTop(IntPtr intptr_0);

	[DllImport("user32.dll")]
	public static extern IntPtr SetActiveWindow(IntPtr intptr_0);

	[DllImport("user32.dll")]
	public static extern IntPtr GetActiveWindow();

	[DllImport("user32.dll")]
	public static extern bool GetClientRect(IntPtr intptr_0, ref Rectangle rectangle_0);

	[DllImport("user32.dll")]
	[return: MarshalAs(UnmanagedType.Bool)]
	public static extern bool SetForegroundWindow(IntPtr intptr_0);

	[DllImport("user32.dll")]
	public static extern IntPtr GetForegroundWindow();

	[DllImport("user32.dll", SetLastError = true)]
	public static extern void SwitchToThisWindow(IntPtr intptr_0, bool bool_0);

	[DllImport("user32.dll")]
	[return: MarshalAs(UnmanagedType.Bool)]
	public static extern bool IsWindowVisible(IntPtr intptr_0);

	[DllImport("user32.dll")]
	[return: MarshalAs(UnmanagedType.Bool)]
	public static extern bool IsIconic(IntPtr intptr_0);

	[DllImport("user32.dll")]
	public static extern bool ShowWindow(IntPtr intptr_0, Enum28 enum28_0);

	[DllImport("user32.dll")]
	public static extern int GetWindowRect(IntPtr intptr_0, out Struct34 struct34_0);

	[DllImport("gdi32.dll", CharSet = CharSet.Unicode, EntryPoint = "ExtTextOutW")]
	public static extern bool ExtTextOut(IntPtr hdc, int X, int Y, uint fuOptions, int clip, string lpString, int len, int spacings);

	public Class51()
	{
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
