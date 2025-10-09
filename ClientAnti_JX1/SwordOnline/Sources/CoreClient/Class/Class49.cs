using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace CoreClient
{
    class Class49
    {

		public static IntPtr smethod_23(Process process_0)
		{
			IntPtr intPtr = IntPtr.Zero;
			int int_;
			do
			{
				int_ = 0;
				intPtr = Class51.FindWindowEx(IntPtr.Zero, intPtr, "Sword3 Class", null);
				Class51.GetWindowThreadProcessId(intPtr, out int_);
			}
			while (int_ != process_0.Id && !intPtr.Equals(IntPtr.Zero));
			return intPtr;
		}
		public static int smethod_31()
		{
			return (int)Math.Truncate(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1)).TotalSeconds);
		}
		public static string smethod_26(int int_0)
		{
			string text = "";
			switch (int_0)
			{
				default:
					return "N/A";
				case 43:
				case 44:
				case 45:
				case 46:
				case 47:
				case 48:
				case 69:
				case 79:
					return "TVB";
				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
				case 24:
				case 65:
				case 75:
					return "CL";
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 63:
				case 73:
					return "VĐ";
				case 55:
				case 56:
				case 57:
				case 58:
				case 59:
				case 60:
				case 71:
				case 81:
					return "TN";
				case 25:
				case 26:
				case 27:
				case 28:
				case 29:
				case 30:
				case 66:
				case 76:
					return "ĐM";
				case 37:
				case 38:
				case 39:
				case 40:
				case 41:
				case 42:
				case 68:
				case 78:
					return "CB";
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 62:
				case 72:
					return "TL";
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
				case 54:
				case 70:
				case 80:
					return "NĐ";
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 64:
				case 74:
					return "NM";
				case 31:
				case 32:
				case 33:
				case 34:
				case 35:
				case 36:
				case 67:
				case 77:
					return "TY";
			}
		}
		public static int smethod_20(int int_0)
		{
			int result = 0;
			switch (int_0)
			{
				case 73:
					result = 73;
					break;
				case 70:
					result = 70;
					break;
				case 72:
					result = 72;
					break;
				case 64:
					result = 64;
					break;
				case 67:
					result = 67;
					break;
				case 356:
					result = 356;
					break;
				case 390:
					result = 390;
					break;
				case 175:
					result = 175;
					break;
				case 90:
					result = 90;
					break;
				case 393:
					result = 393;
					break;
				case 174:
					result = 174;
					break;
				case 392:
					result = 392;
					break;
				case 394:
					result = 394;
					break;
				case 143:
					result = 143;
					break;
				case 364:
					result = 364;
					break;
				case 140:
					result = 140;
					break;
				case 137:
					result = 137;
					break;
				case 136:
					result = 136;
					break;
				case 391:
					result = 391;
					break;
			}
			return result;
		}
	}
}
