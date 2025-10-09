using System.Drawing;

internal struct Struct25
{
	public int int_0;

	public int int_1;

	internal static object object_0;

	public Struct25(int int_2, int int_3)
	{
		int_0 = int_2;
		int_1 = int_3;
	}

	public static implicit operator Point(Struct25 p)
	{
		return new Point(p.int_0, p.int_1);
	}

	public static implicit operator Struct25(Point p)
	{
		return new Struct25(p.X, p.Y);
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
