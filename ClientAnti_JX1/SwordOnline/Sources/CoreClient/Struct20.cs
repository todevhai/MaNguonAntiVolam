internal struct Struct20
{
	private int int_0;

	private int int_1;

	private string string_0;

	private static object object_0;

	public int x
	{
		get
		{
			return int_0;
		}
		set
		{
			int_0 = value;
		}
	}

	public int y
	{
		get
		{
			return int_1;
		}
		set
		{
			int_1 = value;
		}
	}

	public Struct20(int int_2, int int_3)
	{
		int_0 = int_2;
		int_1 = int_3;
		string_0 = "";
	}

	public Struct20(int int_2, int int_3, string string_1)
	{
		int_0 = int_2;
		int_1 = int_3;
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

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
}
