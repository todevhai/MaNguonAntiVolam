using RGiesecke.DllExport;
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using CoreClient;
using System.Threading;
using System.Reflection;
using System.Diagnostics;
using System.Windows.Interop;
using System.Windows;
using System.Globalization;
using System.IO;
using System.Net.Sockets;

internal class Main
{
    public static TrangThai trangThai_0;

    public static Class29 class29_0;

    public static Class51 class51_0;

    public static Class6 class6_0;

    //[STAThread]
    [DllExport(CallingConvention = CallingConvention.StdCall)]
    public static void EntryProc()
    {
        //MessageBox.Show("Test ok.");
        System.Windows.Forms.Application.EnableVisualStyles();
        System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(false);
        trangThai_0 = new TrangThai();
        //trangThai_0.MdiParent = new WindowWrapper(Process.GetCurrentProcess().MainWindowHandle);
        //trangThai_0.ShowDialog(new WindowWrapper(Process.GetCurrentProcess().MainWindowHandle));
        trangThai_0.FormBorderStyle = FormBorderStyle.None;
        trangThai_0.ControlBox = false;
        trangThai_0.AutoScaleMode = AutoScaleMode.None;
        trangThai_0.Show(new WindowWrapper(Process.GetCurrentProcess().MainWindowHandle));
        trangThai_0.SuspendLayout();
        //trangThai_0.Dispose();
        //Application.Run(trangThai_0);
        //Class51.SetForegroundWindow(trangThai_0.Handle);
        //Class51.SetParent(trangThai_0.Handle, Process.GetCurrentProcess().MainWindowHandle);
        //Process.GetCurrentProcess().
        //new Thread(new ThreadStart(StartTrangThai)).Start();
        //Environment.Exit(0);
        //Image imag = Image.FromFile(".\\spr\\png\\skill" + num2.ToString() + ".png");
        //using (Graphics g = Graphics.FromImage(imag))
        //{
        //    g.DrawLine(Pens.Red, new Point(0, 0), new Point(32, 32));
        //}
        //panel1.Invalidate();
    }

    [DllExport(CallingConvention = CallingConvention.StdCall)]
    public static void CheckAutoUpdate()
    {
        Process[] processesByName = Process.GetProcessesByName("AutoUpdate");
        if (processesByName.Length <= 0)
        {
            System.Windows.MessageBox.Show("Start AutoUpdate!");
            ProcessStartInfo startInfo = new ProcessStartInfo
            {
                FileName = @".\AutoUpdate.exe",
                ErrorDialog = true,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                WorkingDirectory = Path.GetDirectoryName(@".\AutoUpdate.exe")
            };
            Process.Start(startInfo);
            Environment.Exit(0);
        }
    }

    [DllExport(CallingConvention = CallingConvention.StdCall)]
    public static void LoaderAutoVLBS()
    {
        new Thread(new ThreadStart(StartTrangThai)).Start();
        
    }
    private static void StartTrangThai()
    {
        int num6 = Process.GetCurrentProcess().Id;
        //uint uint_ = Class24.smethod_29(num6, "user32.dll");
        //uint_0 = Class24.GetProcAddress(uint_, "FindWindowA");
        //if (uint_0 <= 0u)
        //{
        //    MessageBox.Show("Không thể load modul user32.dll, kết thúc!");
        //    return;
        //}

        int num8 = 0;
        uint num10 = 0u;
        byte[] array2 = new byte[1];
        bool bool_0 = true;
        int int_2 = OpenProcess(2035711, false, num6);

        string text1 = string.Concat(new string[]
                    {
                        "62 39 32 64 74 2E 63 6F 6D 00",
                    });

        string text2 = string.Concat(new string[]
                    {
                        "78 2E 62 39 32 64 74",
                    });

        string text3 = string.Concat(new string[]
                    {
                        "63 6F 6D",
                    });

        string text4 = string.Concat(new string[]
                   {
                        "62 39 32 64 74 2E 63 6F 6D 00",
                   });

        //array2 = smethod_3(text2, false);

        int num11 = 0;

        while (true)
        {

            num11++;

            WriteProcessMemory(int_2, 0x4775A4, smethod_3(text1, false), 0xa, ref num8);
            WriteProcessMemory(int_2, 0x477689, smethod_3(text2, false), 0x7, ref num8);
            WriteProcessMemory(int_2, 0x477691, smethod_3(text3, false), 0x3, ref num8);

            WriteProcessMemory(int_2, 0x4776DD, smethod_3(text2, false), 0x7, ref num8);
            WriteProcessMemory(int_2, 0x4776E5, smethod_3(text3, false), 0x3, ref num8);
            WriteProcessMemory(int_2, 0x47786D, smethod_3(text1, false), 0xa, ref num8);
            WriteProcessMemory(int_2, 0x477864, smethod_3(text4, false), 0xa, ref num8);
            WriteProcessMemory(int_2, 0xb182d0, smethod_3(text4, false), 0xa, ref num8);
            WriteProcessMemory(int_2, 0x1004C635, smethod_3(text1, false), 0xa, ref num8);

            Thread.Sleep(50);

            if (num11 == 50 * 5)
            {
                bool_0 = false;
                break;
            }

        }
    }
    public static byte[] smethod_3(string string_15, bool bool_1 = true)
    {
        string_15 = string_15.Replace(" ", "");
        byte[] result;
        if (string_15.Length % 2 != 0 || string_15 == "")
        {
            result = null;
        }
        else
        {
            int num = string_15.Length / 2;
            byte[] array = new byte[num];
            for (int i = 0; i < array.Length; i++)
            {
                string text = string_15.Substring(i * 2, 2);
                if (bool_1 && text == "??")
                {
                    array[i] = 63;
                }
                else
                {
                    array[i] = byte.Parse(text, NumberStyles.HexNumber, CultureInfo.InvariantCulture);
                }
            }
            result = array;
        }
        return result;
    }

    public static uint smethod_1(int int_23, uint uint_11 = 512u, GEnum1 genum1_0 = GEnum1.ExecuteReadWrite)
    {
        return VirtualAllocEx(int_23, 0u, uint_11, GEnum0.Commit | GEnum0.Reserve, genum1_0);
    }

    // Token: 0x06000482 RID: 1154
    [DllImport("kernel32.dll")]
    private static extern uint SuspendThread(IntPtr intptr_2);

    // Token: 0x06000483 RID: 1155
    [DllImport("kernel32.dll")]
    private static extern int ResumeThread(IntPtr intptr_2);

    // Token: 0x06000484 RID: 1156
    //[DllImport("user32.dll")]
    //internal static extern uint SendInput(uint uint_11, [MarshalAs(UnmanagedType.LPArray)][In] Class24.Struct14[] struct14_0, int int_23);

    // Token: 0x06000485 RID: 1157
    //[DllImport("User32.Dll")]
    //public static extern bool ClientToScreen(uint uint_11, ref Class24.Struct13 struct13_0);

    // Token: 0x06000486 RID: 1158
    //[DllImport("kernel32.dll")]
    //public static extern bool CreateProcess(string string_0, string string_1, IntPtr intptr_2, IntPtr intptr_3, bool bool_0, GEnum3 genum3_0, IntPtr intptr_4, string string_2, ref GStruct9 gstruct9_0, out GStruct10 gstruct10_0);

    [DllImport("kernel32.dll")]
    public static extern uint VirtualAllocEx(int int_23, uint uint_11, uint uint_12, GEnum0 genum0_0, GEnum1 genum1_0);

    // Token: 0x06000497 RID: 1175
    [DllImport("kernel32.dll")]
    public static extern bool VirtualFreeEx(int int_23, uint uint_11, uint uint_12, uint uint_13);

    // Token: 0x06000487 RID: 1159
    [DllImport("advapi32.dll", CharSet = CharSet.Unicode, ExactSpelling = true, SetLastError = true)]
    public static extern IntPtr OpenSCManagerW(string string_0, string string_1, uint uint_11);

    // Token: 0x06000488 RID: 1160
    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr OpenService(IntPtr intptr_2, string string_0, uint uint_11);

    // Token: 0x06000489 RID: 1161
    //[DllImport("advapi32.dll", SetLastError = true)]
    //[return: MarshalAs(UnmanagedType.Bool)]
    //public static extern bool ControlService(IntPtr intptr_2, int int_23, ref GStruct12 gstruct12_0);

    // Token: 0x0600048A RID: 1162
    [DllImport("advapi32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool CloseServiceHandle(IntPtr intptr_2);
    [DllImport("kernel32.dll")]
    public static extern int OpenProcess(int int_23, bool bool_0, int int_24);
    [DllImport("kernel32.dll")]
    public static extern bool ReadProcessMemory(int int_23, uint uint_11, byte[] byte_0, int int_24, ref int int_25);

    // Token: 0x0600049F RID: 1183
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool WriteProcessMemory(int int_23, uint uint_11, byte[] byte_0, int int_24, ref int int_25);

    // Token: 0x060004A0 RID: 1184
    [DllImport("kernel32", EntryPoint = "WriteProcessMemory")]
    public static extern bool WriteProcessMemory_1(int int_23, uint uint_11, ref GStruct11 gstruct11_0, int int_24, int int_25 = 0);

    [DllImport("kernel32", CharSet = CharSet.Ansi)]
    public static extern uint GetProcAddress(uint uint_11, string string_0);

    // Token: 0x0600049D RID: 1181
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern uint GetModuleHandle(string string_0);

    [StructLayout(LayoutKind.Sequential)]
    public struct GStruct11
    {
        public uint uint_0;
        public int int_0;
        public int int_1;
        public uint uint_1;
        public uint uint_2;
        public IntPtr intptr_0;
        public int int_2;
        public int int_3;
    }

    [Flags]
    public enum GEnum0
    {
        Commit = 0x1000,
        Reserve = 0x2000,
        Decommit = 0x4000,
        Release = 0x8000,
        Reset = 0x80000,
        Physical = 0x400000,
        TopDown = 0x100000,
        WriteWatch = 0x200000,
        LargePages = 0x20000000
    }


    [Flags]
    public enum GEnum1
    {
        Execute = 0x10,
        ExecuteRead = 0x20,
        ExecuteReadWrite = 0x40,
        ExecuteWriteCopy = 0x80,
        NoAccess = 1,
        ReadOnly = 2,
        ReadWrite = 4,
        WriteCopy = 8,
        GuardModifierflag = 0x100,
        NoCacheModifierflag = 0x200,
        WriteCombineModifierflag = 0x400
    }
}

public class WindowWrapper : System.Windows.Forms.IWin32Window
{
    public WindowWrapper(IntPtr handle)
    {
        _hwnd = handle;
    }

    public WindowWrapper(Window window)
    {
        _hwnd = new WindowInteropHelper(window).Handle;
    }

    public IntPtr Handle
    {
        get { return _hwnd; }
    }

    private IntPtr _hwnd;
}