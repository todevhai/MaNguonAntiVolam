using CoreClient;
using RGiesecke.DllExport;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows.Forms;

internal struct StatusSkill
{
	public int idskill;

	public int timeout;

	public int totaltime;

	public string skillname;

	public string skillshow;

}

internal class TrangThai : Form
{
	private Class29 class29_0;

	public IntPtr intptr_0;

	private Class10 class10_0;

	public static Timer timer_0;

	public static Timer timer_1;

	public Class36 class36_0;

	private Class12 class12_0;

	private ArrayList arrayList_0;

	private string string_0;

	private Class51.Struct34 struct34_0;

	public int int_0;

	private Container container_0;

	//private ListViewEX listView1;

	//private ListView listView1;
	Graphics draw;
	private List<StatusSkill> ListStatusSkill;
	private List<int> ListSkill;
	private Process process_0;
    private IContainer components;
    private ImageList imageList_0;
    private ListViewEx listView1;
	Font newfont;
	FontFamily fontFamily;
	internal static object object_0;
	[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
	private static extern Int32 SendMessage(IntPtr hwnd, Int32 wMsg, Int32 wParam, Int32 lParam);

	const int LVM_FIRST = 0x1000;
	const int LVM_SETICONSPACING = LVM_FIRST + 53;
	//protected override void OnPaintBackground(PaintEventArgs e) { /* Ignore */ }
	public void SetControlSpacing(Control control, Int16 x, Int16 y)
	{
		SendMessage(control.Handle, LVM_SETICONSPACING, 0, x * 65536 + y);
		control.Refresh();
	}
	//protected override void OnPaintBackground(PaintEventArgs e)
	//{
	//	e.Graphics.FillRectangle(Brushes.White, e.ClipRectangle);
	//}
	public TrangThai()
	{
		//this.FormClosing += new FormClosingEventHandler(TrangThai_FormClosing);
		//class10_0 = class10_1;
		timer_0 = new Timer();
		timer_1 = new Timer();
        SetStyle(ControlStyles.SupportsTransparentBackColor, true);
		//SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.AllPaintingInWmPaint, true);
		//this.BackColor = Color.Transparent;
		//this.BackColor = Color.White;
		//this.TransparencyKey = Color.White;
		PrivateFontCollection collection = new PrivateFontCollection();
		collection.AddFontFile(@".\font\VNARIALN.TTF");
		fontFamily = new FontFamily(".VnArial Narrow", collection);
		//newfont = new Font(fontFamily, 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)), false);
		newfont = new Font(fontFamily, 8.25F, System.Drawing.GraphicsUnit.Point);

		InitializeComponent();

		this.Font = newfont;
		Panel P = new Panel();
        P.BackColor = listView1.BackColor;
		P.Location = this.Location;
        P.Size = this.Size;
        P.Padding = new System.Windows.Forms.Padding(0, 0, 0, 0);
        P.BorderStyle = listView1.BorderStyle;
        this.Controls.Add(P);
        //P.SuspendLayout();
        process_0 = Process.GetCurrentProcess();
		class12_0 = new Class12();
		arrayList_0 = new ArrayList();
		string_0 = "N/A";
		//class29_0 = class29_1;
		//class10_0 = class10_1;
		class29_0 = new Class29(process_0, process_0.Id);
		class36_0 = new Class36(process_0);
		_ = Screen.GetWorkingArea(default(Rectangle)).Width;
		this.imageList_0.Images.SetKeyName(0, "15");
		this.imageList_0.Images.SetKeyName(1, "202");
		this.imageList_0.Images.SetKeyName(2, "157");
		this.imageList_0.Images.SetKeyName(3, "159");
		this.imageList_0.Images.SetKeyName(4, "273");
		this.imageList_0.Images.SetKeyName(5, "211");
		this.imageList_0.Images.SetKeyName(6, "206");
		this.imageList_0.Images.SetKeyName(7, "207");
		this.imageList_0.Images.SetKeyName(8, "208");
		this.imageList_0.Images.SetKeyName(9, "333");
		this.imageList_0.Images.SetKeyName(10, "93");
        //Class51.Struct34 @struct = default(Class51.Struct34);
        //Class51.GetWindowRect(intptr_0, out @struct);
        //base.Location = new Point(805 + @struct.int_0, @struct.int_1);
        //base.Location = new Point(50 + @struct.int_0, 125 + @struct.int_1);

        //ImageList newStateImageList = new ImageList();

        //foreach (Image image in listView1.StateImageList.Images)
        //{
        //	Bitmap bitmap = new Bitmap(listView1.StateImageList.ImageSize.Width,
        //	listView1.StateImageList.ImageSize.Height, PixelFormat.Format24bppRgb);
        //	using (Graphics g = Graphics.FromImage(bitmap))
        //	{
        //		g.Clear(listView1.BackColor);
        //		g.DrawImage(image, 0, 0);
        //	}
        //	newStateImageList.Images.Add(bitmap);
        //}

        //listView1.BorderStyle = BorderStyle.None;
        listView1.Parent = P;
        //listView1.Dock = DockStyle.Fill;
        //listView1.Size = new Size(600, 55);
        //      listView1.TileSize = new Size(40, 51);
        //listView1.View = View.Tile;
        //listView1.OwnerDraw = true;
        //listView1.SmallImageList = newStateImageList;
        //listView1.LargeImageList = newStateImageList;
        //listView1.BorderStyle = BorderStyle.None;
        //listView1.Parent = this;
        //listView1.Dock = DockStyle.Fill;
        //listView1.Location = new Point(50 + @struct.int_0, 125 + @struct.int_1);
        //      listView1.Size = new System.Drawing.Size(600, 55);
        //listView1.Enabled = true;
        listView1.Font = newfont;
		//listView1.View = View.LargeIcon;
		listView1.DrawItem += new DrawListViewItemEventHandler(listView1_DrawItem);
        //listView1.SuspendLayout();
        //listView1.DrawSubItem += new DrawListViewSubItemEventHandler(listView1_DrawSubItem);
        //listView1.BackColor = Color.Transparent;
        //listView1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
        //listView1.Show();
        //Class51.SetParent(P.Handle, this.Handle);
        //Class51.SetForegroundWindow(listView1.Handle);
        //listView1.Top = 999;
        //listView1.BringToFront();
        //listView1.BringToFront();
        //listView1.BringToFront();
        //SetControlSpacing(listView1, 10, 10);
        ListStatusSkill = new List<StatusSkill>();
        ListSkill = new List<int>();

		//Class51.SetParent(this.Handle, Process.GetCurrentProcess().MainWindowHandle);
		//Class51.SetParent(listView1.Handle, this.Handle);
		//Class51.BringWindowToTop(this.Handle);
		//Class51.BringWindowToTop(listView1.Handle);
		//.GetProperty("DoubleBuffered", System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic)
		//.SetValue(listView1, true, null);
		//ObjectListView.CheckForIllegalCrossThreadCalls = false;
		//timer_1.Tick += CheckSkillState;
		//timer_1.Interval = 990;
		//timer_1.Start();

		//listView1.DoubleBuffering(true);
	}
	//public void DoubleBuffered(this Control control, bool enable)
	//{
	//	var doubleBufferPropertyInfo = control.GetType().GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic);
	//	doubleBufferPropertyInfo.SetValue(control, enable, null);
	//}
	public void method_0()
	{
		//class29_0 = class29_1;
		//class10_0 = class10_1;
		//class29_0 = new Class29(process_0, process_0.Id);
		//class36_0 = new Class36(process_0);
		listView1.Items.Clear();
		if (!timer_0.Enabled)
			timer_0.Start();
	}

	private void CheckSkillState(object sender, EventArgs e)
	{
		try
		{
			string string_2;
			//Graphics draw = listView1.CreateGraphics();
			//if (int_0 == 0 || class29_0.method_50() == 0 || class29_0.method_129() < 1 || class29_0.method_55(class29_0.method_17()) == 0)
			//{
			//	if (listView1.Items.Count > 0)
			//		listView1.Items.Clear();
			//	return;
			//}

			if (!process_0.HasExited)
			{

				uint num = class36_0.method_3(class29_0.method_15() + 256);

				//var result = from s in ListStatusSkill
				//			 where s.int_0 == num2
				//			 select s;

				//var Weapon = ListStatusSkill.FirstOrDefault(w => w.int_0 == num2);
				
				//ListStatusSkill.Add(new StatusSkill() { int_0 = num2, int_1 = num3, string_0 = text });

				int count = listView1.Items.Count;

				if (count > 0)
				{
					//if (!listView1.Items[listView1.SelectedIndices[0]].Bounds.IntersectsWith(listView1.ClientRectangle))
					//	listView1.TopItem.Focused = true;
					//else
					//	listView1.Items[listView1.SelectedIndices[0]].Focused = true;

					for (int j = 0; j < count; j++)
					{
						listView1.BeginUpdate();
						ListViewItem listViewItem2 = listView1.Items[j];
						//MessageBox.Show(count.ToString() + " - " + listViewItem2.SubItems[1].Text);

						//File.WriteAllText("L:\\test1.txt", class29_0.method_41((int)listViewItem2.Tag).ToString() + " - " + listViewItem2.SubItems[0].Text + " - " + listViewItem2.Tag.ToString() + " - " + listViewItem2.SubItems[1].Text);
						if (class29_0.method_41((int)listViewItem2.Tag) <= 0 && listViewItem2.SubItems[1].Text != "N/A")
						{
							//ListSkill.RemoveAt((int)listViewItem2.Tag);
							//listView1.Items.RemoveAt(j);
							method_1((int)listViewItem2.Tag);
							//listView1.EndUpdate();
							//listView1.Invalidate();
							continue; 
						}

						float num4 = class29_0.method_41((int)listViewItem2.Tag) * 60 / 1000;

						if (num4 / 3600f > 10f)
							continue;
						if (num4 < 1f || num4 == 1)
							string_2 = "N/A";
						else if (num4 > 60f && num4 < 3600f)
						{
							string_2 = (num4 / 60f).ToString("F0") + " m";
						}
						else if (num4 >= 3600f)
						{
							string_2 = (num4 / 3600f).ToString("F0") + " h";
						}
						else
						{
							string_2 = num4 + " s";
						}
						//MessageBox.Show(listViewItem2.SubItems[1].Text);
						listViewItem2.SubItems[1].Text = string_2;
						//listView1.EndUpdate();
						//listView1.Invalidate();

						//ImageList iList = listView1.LargeImageList;
						//Size iSize = iList.ImageSize;
						//int fSize2 = 3;

						//Rectangle R0 = new Rectangle(Point.Empty, iSize);
						//Rectangle R1 = new Rectangle(new Point(listView1.Bounds.X, listView1.Bounds.Y),
						//				new Size(iSize.Width - fSize2, iSize.Height - fSize2));

						//draw.DrawImage(iList.Images[int.Parse(listViewItem2.SubItems[1].Text)], R1, R0, GraphicsUnit.Pixel);
						//draw.DrawString(string_2, Font, Brushes.LimeGreen,
						//				  2f, listView1.Bounds.Y + iSize.Height - fSize2);
						//draw.Save();


						//if ((int)listViewItem2.Tag == num2)
						//{
						//	flag = true;
						//	if (num3 <= 0)
						//	{
						//		listView1.Items.RemoveAt(j);
						//		//listView1.Update();
						//		//listView1.Invalidate();
						//	}
						//	else
						//	{
						//		//listView1.Items[j].SubItems[1].Text = string_0;
						//		//listView1.Update();
						//		//listView1.Invalidate();
						//	}

						//}
						//else if (class29_0.method_41((int)listViewItem2.Tag) <= 0)
						//{
						//	listView1.Items.RemoveAt(j);
						//	//listView1.Update();
						//	//listView1.Invalidate();
						//}
						listView1.EndUpdate();
					}

					//num = class36_0.method_3(num + 8);
				}
				else
				{
					if (listView1.Items.Count > 0)
						listView1.Items.Clear();
					ListSkill.Clear();
					return;
				}

			}
			else
			{
				timer_1.Stop();
				Close();
			}
		}
		catch (Exception ex)
		{
			MessageBox.Show(ex.StackTrace);
		}

	}

	public void method_1(int int_1)
	{
		try
        {
			for (int i = 0; i < listView1.Items.Count; i++)
			{
				if ((int)listView1.Items[i].Tag == int_1)
				{
					listView1.Items.RemoveAt(i);
					//ListSkill.Remove(int_1);
				}
				//else
    //            {
				//	if (class29_0.method_41((int)listView1.Items[i].Tag) <= 0)
				//	{
				//		listView1.Items.RemoveAt(i);
				//		ListSkill.Remove(int_1);
				//	}
				//}
			}
		}
		catch { }
	}

	//private void TrangThai_FormClosing(object sender, FormClosingEventArgs e)
 //   {
		
	//}
	private void method_2(object sender, EventArgs e)
	{
		
		try
        {
			int mapid = class29_0.method_129();
			int playerindex = class29_0.method_55(class29_0.method_17());
			//MessageBox.Show(class29_0.method_129().ToString() + " - " + class29_0.method_98(class29_0.method_17()).ToString());
			//if (int_0 == 0 || class29_0.method_50() == 0 || class29_0.method_129() < 1 || class29_0.method_55(class29_0.method_17()) == 0)
			if (mapid < 1 || playerindex == 0)
			{
				listView1.Visible = false;
				if (listView1.Items.Count > 0)
					listView1.Items.Clear();
				ListSkill.Clear();
				listView1.Clear();
				Hide();
				return;
			}

			if (!process_0.HasExited)
			{

				if (!base.Visible)
				{
					listView1.Visible = true;
					Show(); 
				}
				else
				{
					struct34_0 = default(Class51.Struct34);
					Class51.GetWindowRect(process_0.MainWindowHandle, out struct34_0);
					//base.Location = new Point(805 + struct34_0.int_0, struct34_0.int_1);
					base.Location = new Point(25 + struct34_0.int_0, 60 + struct34_0.int_1);
				}

                //struct34_0 = default(Class51.Struct34);
				//Class51.GetWindowRect(process_0.MainWindowHandle, out struct34_0);
				//base.Location = new Point(805 + struct34_0.int_0, struct34_0.int_1);
				//base.Location = new Point(25 + struct34_0.int_0, 65 + struct34_0.int_1);
				uint num = class36_0.method_3(class29_0.method_15() + 0x100);
				//ImageList iList = listView1.LargeImageList;
				//Size iSize = iList.ImageSize;
				//int fSize2 = 7;

				//Rectangle R0 = new Rectangle(Point.Empty, iSize);
				//Rectangle R1 = new Rectangle(new Point(e.Bounds.X, e.Bounds.Y),
				//				new Size(iSize.Width - fSize2, iSize.Height - fSize2));

				//e.Graphics.DrawImage(iList.Images[e.Item.ImageKey], R1, R0, GraphicsUnit.Pixel);
				//e.Graphics.DrawString(class29_0.method_41(int.Parse(e.Item.Text)).ToString(), Font, Brushes.LimeGreen,
				//				  1f, e.Bounds.Y + iSize.Height - fSize2);
				
				for (int i = 0; i < 10; i++)
				{
					//if (Class51.GetForegroundWindow() != process_0.MainWindowHandle && Class51.GetForegroundWindow() != base.Handle)
					//{
					//	listView1.Visible = false;
					//	Hide();
					//	return;
					//}
					int num2 = 0; // ID Skill
					int num3 = 0; // Time SKill
					num2 = (int)class36_0.method_3(num + 16);
					num3 = (int)class36_0.method_3(num + 24);
					//File.WriteAllText("L:\\test.txt", class29_0.method_129().ToString() + " - " + class29_0.method_98(class29_0.method_17()).ToString() + " - " + num2 + " - "+ num3);
					//File.WriteAllText("L:\\test.txt", Class49.smethod_20(num2).ToString());
					//File.WriteAllText("L:\\test.txt", num3.ToString() + " - " + num2.ToString());
					if (mapid < 1 || playerindex == 0)
					{
						listView1.Visible = false;
						listView1.Clear();
						Hide();
						return; 
					}

					if (num3 < 0)
                    {
						method_1(num2);
					}

                    if (num2 > 0)
					{

						//var result = from s in ListStatusSkill
						//             where s.idskill == num2
						//             select s;

						//var Weapon = ListStatusSkill.FirstOrDefault(w => w.idskill == num2);
						//Weapon.timeout = num3;
						//if (num3 > 0 && !listView1.Items.ContainsKey(num2.ToString()))
						//File.WriteAllText("L:\\test3.txt", File.ReadAllText("L:\\test.txt") + "\n"+ Weapon.ToString() + " - " + Weapon.totaltime.ToString() + " - " + Weapon.idskill.ToString() + " - " + listView1.Items.ContainsKey(num2.ToString()));
						//if (num3 > 0 && !listView1.Items.ContainsKey(num2.ToString()))
						if (num3 > 0 && !listView1.Items.ContainsKey(num2.ToString()))
						{
							if ((num3 * 60 / 1000) <= 1f && listView1.Items.Count <= 1)
								method_1(num2);

							string text = class12_0.method_2(class29_0.method_77(num2), 2, 5);
							//if (num4 < 1f)
							float num4 = num3 * 60 / 1000;

							if (num4 / 3600f > 10f)
								continue;

							if (num4 <= 1f)
							{
								string_0 = "N/A";
							}
							else if (num4 > 60f && num4 < 3600f)
							{
								string_0 = (num4 / 60f).ToString("F0") + " m";
							}
							else if (num4 >= 3600f)
							{
								string_0 = (num4 / 3600f).ToString("F0") + " h";
							}
							else
							{
								string_0 = num4 + " s";
							}

							ListViewItem listViewItem = new ListViewItem(new string[2]
							{
							num2.ToString(),
							string_0
							});
							listViewItem.Text = text;
							listViewItem.Name = num2.ToString();
                            listViewItem.BackColor = Color.White;
                            //listViewItem.BackColor = Color.Transparent;
                            listViewItem.Tag = num2;
                            listViewItem.ImageKey = num2.ToString();
							listViewItem.UseItemStyleForSubItems = true;
							if (num4 > 1f)
								listViewItem.ToolTipText = "aura";
							//StatusSkill infoskill;
							//infoskill.idskill = num2;
							//infoskill.timeout = num3;
							//infoskill.totaltime = num3;
							//infoskill.skillshow = string_0;
							//infoskill.skillname = text;

							if (num2 >= 661 && num2 <= 667)
								continue;
							if (num2 >= 256 && num2 <= 265)
							{
								listViewItem.ForeColor = Color.White;
								//if (class10_0.int_116 != 2)
								//{
								//	method_1(num2);
								//	continue;
								//}
							}
							else if (num2 >= 438 && num2 <= 700)
							{
								listViewItem.ForeColor = Color.Lime;
							}
							else
							{

								listViewItem.ForeColor = Color.Gold;

								if (Class49.smethod_20(num2) > 0)
								{
									listViewItem.ForeColor = Color.Cyan;

									//if (class10_0.int_116 == 0)
									//{
									//	method_1(num2);
									//	continue;
									//}
								}
                                //else
                                //{
                                //    method_1(num2);
                                //    continue;
                                //}
                            }

							//ListStatusSkill.Add(new StatusSkill() { int_0 = num2, int_1 = num3, string_0 = text });
							//ListStatusSkill.Add(infoskill);
							listView1.Items.Add(listViewItem);
							//listView1.EndUpdate();
							//listView1.Invalidate();
							//File.WriteAllText("L:\\test.txt", num2.ToString() + " - " + ListSkill.Contains(num2).ToString());
							//if (!ListSkill.Contains(num2))
							//                     {
							//	ListSkill.Add(num2);
							//	//File.WriteAllText("L:\\test2.txt", num2.ToString() + " - " + ListSkill.Contains(num2).ToString());
							//	//ImageList iList = listView1.LargeImageList;
							//	//Size iSize = iList.Images[int.Parse(num2.ToString())].Size;
							//	//int fSize2 = 3;
							//	//Graphics draw = listView1.CreateGraphics();
							//	//Rectangle R0 = new Rectangle(Point.Empty, iSize);
							//	//Rectangle R1 = new Rectangle(new Point(listView1.Bounds.X, listView1.Bounds.Y),
							//	//				new Size(iSize.Width - fSize2, iSize.Height - fSize2));

							//	//draw.DrawImage(iList.Images[num2.ToString()], R1, R0, GraphicsUnit.Pixel);
							//	//draw.DrawString(string_0, Font, Brushes.LimeGreen,
							//	//				  2f, listView1.Bounds.Y + iSize.Height - fSize2);
							//	//draw.Save();
							//}
						}

                        int count = listView1.Items.Count;

						if (count > 0)
						{
							//if (!listView1.Items[listView1.SelectedIndices[0]].Bounds.IntersectsWith(listView1.ClientRectangle))
							//	listView1.TopItem.Focused = true;
							//else
							//	listView1.Items[listView1.SelectedIndices[0]].Focused = true;
							//listView1.BeginUpdate();
							
							for (int j = 0; j < count; j++)
							{
								if (mapid < 1 || playerindex == 0)
								{
									Hide();
									listView1.Visible = false;
									listView1.Clear();
									return;
								}
								//if (Class51.GetForegroundWindow() != process_0.MainWindowHandle && Class51.GetForegroundWindow() != base.Handle)
								//{
								//	listView1.Visible = false;
								//	Hide();
								//	return;
								//}
								ListViewItem listViewItem2 = listView1.Items[j];
								//MessageBox.Show(count.ToString() + " - " + listViewItem2.SubItems[1].Text);
								if(count == 1 && num2 != (int)listViewItem2.Tag)
                                {
									listView1.Items.RemoveAt(j);
									continue;
								}
								if ((int)listViewItem2.Tag == num2)
								{
									float num4 = class29_0.method_41((int)listViewItem2.Tag) * 60 / 1000;

									string string_2;

									if (num4 / 3600f > 10f)
										continue;
									if (num4 <= 1f)
									{
										//string_2 = "N/A";

										string_2 = "N/A";

									}
									else if (num4 > 60f && num4 < 3600f)
									{
										string_2 = (num4 / 60f).ToString("F0") + " m";
									}
									else if (num4 >= 3600f)
									{
										string_2 = (num4 / 3600f).ToString("F0") + " h";
									}
									else
									{
										string_2 = num4 + " s";
									}

									listView1.Items[j].SubItems[1].Text = string_2;

									//File.WriteAllText("L:\\test1.txt", found.timeout.ToString() + " - " + found.skillshow.ToString() + " - " + found.skillname.ToString() + " - " + found.totaltime.ToString() + " - " + listViewItem2.Tag.ToString() + " - " + listViewItem2.SubItems[1].Text);

									//if (found.timeout < 16)
									//                           {

									//	method_1((int)listViewItem2.Tag);
									//	ListStatusSkill.Remove(found);
									//	continue;
									//}
									//else
									//                           {
									//	found.timeout = class29_0.method_41((int)listViewItem2.Tag);
									//	found.skillshow = string_2;

									//}

									if (class29_0.method_41((int)listViewItem2.Tag) < 0)
									{
										listView1.Items.RemoveAt(j);
										continue;
									}
									//listView1.Update();
									//listView1.Invalidate();
								}
								else if(class29_0.method_41((int)listViewItem2.Tag) < 0)
								{
									listView1.Items.RemoveAt(j);
									continue;
                                }
							}
							//num = class36_0.method_3(num + 8);
						}
						else
						{
							if (listView1.Items.Count > 0)
								listView1.Items.Clear();
							ListSkill.Clear();
							return;
						}
					}
					else if (num3 <= 0)
					{
						method_1(num2);
						continue;
					}
					num = class36_0.method_3(num + 8);
                }

			}
			else
			{
				timer_0.Stop();
				Close();
			}
		}
		catch //(Exception ex)
        {
			//try
			//{
			//	for (int i = 0; i < listView1.Items.Count; i++)
			//	{
			//		listView1.Items.RemoveAt(i);
			//	}
			//}
			//catch { }
			//MessageBox.Show(ex.StackTrace);
		}
	}

	private void listView1_DrawItem(object sender, DrawListViewItemEventArgs e)
	{
		//e.Graphics.Clear(Color.White);
		e.DrawBackground();
        //e.DrawDefault = true;
        ImageList iList = listView1.LargeImageList;
        //ImageList iList = listView1.SmallImageList;
        //Size iSize = iList.Images[e.Item.ImageKey].Size;
        Size iSize = iList.ImageSize;
        int fSize2 = 7;

        Rectangle R0 = new Rectangle(Point.Empty, iSize);
        Rectangle R1 = new Rectangle(new Point(e.Bounds.X, e.Bounds.Y),
                        new Size(iSize.Width, iSize.Height));

		//var flags = TextFormatFlags.HorizontalCenter;
		//e.DrawFocusRectangle();
		//e.Graphics.DrawImage(iList.Images[e.Item.ImageKey], R1, R0, GraphicsUnit.Pixel);
		//Bitmap bmpOK = Bitmap.FromFile("//spr//png//skill//" + e.Item.ImageKey + ".png");
		//e.DrawText();
		//FullImage.LoadImage(Image.FromFile(@".\spr\png\skill\" + e.Item.ImageKey + ".png"));
		//System.Drawing.Image image = iList.Images[e.Item.ImageKey];
		//Bitmap bitmap = new Bitmap(37, 37, PixelFormat.Format24bppRgb);
		//bitmap.SetResolution(image.HorizontalResolution, image.VerticalResolution);

		//e.Graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
		//e.Graphics.CompositingQuality = CompositingQuality.HighQuality;
		//e.Graphics.SmoothingMode = SmoothingMode.HighQuality;
		//IntPtr screenDc = Process.GetCurrentProcess().MainWindowHandle;
		Image image = iList.Images[e.Item.ImageKey];
		Bitmap bitmap = new Bitmap(image);
        
        //IntPtr hBitmap = IntPtr.Zero;
        //IntPtr hOldBitmap = IntPtr.Zero;
        //hBitmap = bitmap.GetHbitmap(Color.FromArgb(0));
        //hOldBitmap = SelectObject(memDc, hBitmap);
        e.Graphics.DrawImage(bitmap, R1, R0, GraphicsUnit.Pixel);
        //ReleaseDC(IntPtr.Zero, screenDc);
        //if (hBitmap != IntPtr.Zero)
        //{
        //	SelectObject(memDc, hOldBitmap);
        //	DeleteObject(hBitmap);
        //}
        //DeleteDC(memDc);

        //e.Graphics.DrawImage(image, R1, R0, GraphicsUnit.Pixel);
        //FontFamily newfontFamily = new FontFamily(".VnArial");
        //Font newfont = new System.Drawing.Font(newfontFamily, 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

        //this.Font.FontFamily
        //var gp = new GraphicsPath();
        //Rectangle R2 = new Rectangle(new Point(e.Bounds.X + (iSize.Width / 2) - (fSize2 + 3), e.Bounds.Y + iSize.Height),
        //                      new Size(e.Bounds.Width + 1, e.Bounds.Height));
        //gp.AddString(e.Item.Text, newfontFamily, 0, 12F, R2, StringFormat.GenericDefault);
        //gp.IsOutlineVisible(e.Bounds.X + (iSize.Width / 2) - (fSize2 + 3), e.Bounds.Y + iSize.Height, System.Drawing.Pens.Black);
        //gp.Widen(System.Drawing.Pens.Black);
        //e.Graphics.DrawPath(System.Drawing.Pens.LimeGreen, gp);

        //Class51.OutputText(12, e.Item.Text, -1, 10, 20, Color.Red.ToArgb(), 0, 0, 0);

        //var gp2 = new GraphicsPath();
        //gp2.AddString(e.Item.Text, newfont.FontFamily, 0, 10.25F, R2, StringFormat.GenericTypographic);
        //e.Graphics.DrawPath(System.Drawing.Pens.Lime, gp2);

        //System.Drawing.SolidBrush brush = new SolidBrush(System.Drawing.Pens.Gold);

        //IntPtr screenDc = e.Graphics.GetHdc();
        
        //IntPtr memDc = CreateCompatibleDC(screenDc);
        //if (Class51.ExtTextOut(screenDc, e.Bounds.X + (iSize.Width / 2) - (fSize2 + 3), e.Bounds.Y + iSize.Height, 0, -1, e.Item.SubItems[1].Text, e.Item.SubItems[1].Text.Length, -1))
        //      {
        //	ReleaseDC(IntPtr.Zero, screenDc);
        //}

        e.Graphics.TextRenderingHint = TextRenderingHint.SingleBitPerPixelGridFit;
		e.Graphics.DrawString(e.Item.SubItems[1].Text, newfont, Brushes.Lime,
		e.Bounds.X + (iSize.Width / 2) - (fSize2 + 3), e.Bounds.Y + iSize.Height);
        //e.Graphics.ReleaseHdc(screenDc);
        //if (hBitmap != IntPtr.Zero)
        //{
        //	SelectObject(memDc, hOldBitmap);
        //	DeleteObject(hBitmap);
        //}
        //DeleteDC(memDc);

        //if (e.State == ListViewItemStates.Selected)
        //{
        //e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixelGridFit;
        //e.Graphics.DrawString(e.Item.SubItems[1].Text, newfont, Brushes.Lime,
        //e.Bounds.X + (iSize.Width / 2) - (fSize2 + 3), e.Bounds.Y + iSize.Height);
        //}


        e.Graphics.Dispose();
		//e.Graphics.DrawRectangle(System.Drawing.Pens.Black, e.Bounds.X + (iSize.Width / 2) - (fSize2 + 3), e.Bounds.Y + iSize.Height, iSize.Width, iSize.Height);
		//e.DrawText(flags);
		//e.DrawText();
		//e.Graphics.Clear(listView1.BackColor);
		//e.Graphics.Flush();
		//e.DrawDefault = true;
		//e.Graphics.
	}
    private void listView1_DrawSubItem(object sender, DrawListViewSubItemEventArgs e)
    {
        const int TEXT_OFFSET = 1;
        //e.DrawBackground();
        e.DrawText();
        TextFormatFlags flags = TextFormatFlags.HorizontalCenter;
        flags = TextFormatFlags.HorizontalCenter;
        ListView listView = (ListView)sender;
        ImageList iList = listView.LargeImageList;
        Size iSize = iList.ImageSize;
        int fSize2 = 7;
		e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixelGridFit;
		e.Graphics.DrawString(e.Item.SubItems[1].Text, newfont, Brushes.Lime,
								e.Bounds.X + (iSize.Width / 2) - (fSize2 + 3), e.Bounds.Y + iSize.Height);
		e.Graphics.Dispose();
	}
    private void TrangThai_Load(object sender, EventArgs e)
	{
		//panel1.Location = listView1.Location;
		int_0 = 1;
		timer_0.Tick += method_2;
		timer_0.Interval = 300;
		timer_0.Start();
	}

	private void listView1_SelectedIndexChanged(object sender, EventArgs e)
	{
	}

	private void listView1_Click(object sender, EventArgs e)
	{
		listView1.SelectedItems.Clear();
		if (listView1.SelectedItems.Count > 0)
			listView1.SelectedItems[0].Selected = false;
	}

	private void listView1_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
	{
		if (e.IsSelected)
		{
			e.Item.Selected = false;
			e.Item.Focused = false;
		}
	}

	private void TrangThai_FormClosing(object sender, FormClosingEventArgs e)
	{
		int_0 = 0;
		timer_0.Stop();
		listView1.Items.Clear();
		Close();
		Dispose();
		base.Parent = null;
		e.Cancel = true;
	}

	private void button1_Click(object sender, EventArgs e)
	{
		int_0 = 0;
		class10_0.method_26(bool_148: false);
		Hide();
	}

	protected override void Dispose(bool disposing)
	{
		if (disposing && container_0 != null)
			((IDisposable)container_0).Dispose();
		base.Dispose(disposing);
	}

	private void InitializeComponent()
	{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TrangThai));
            this.imageList_0 = new System.Windows.Forms.ImageList(this.components);
            this.listView1 = new TrangThai.ListViewEx();
            this.SuspendLayout();
            // 
            // imageList_0
            // 
            this.imageList_0.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList_0.ImageStream")));
            this.imageList_0.TransparentColor = System.Drawing.Color.White;
            this.imageList_0.Images.SetKeyName(0, "15.png");
            this.imageList_0.Images.SetKeyName(1, "16.png");
            this.imageList_0.Images.SetKeyName(2, "157.png");
            this.imageList_0.Images.SetKeyName(3, "211.png");
            this.imageList_0.Images.SetKeyName(4, "273.png");
            this.imageList_0.Images.SetKeyName(5, "211.png");
            this.imageList_0.Images.SetKeyName(6, "206.png");
            this.imageList_0.Images.SetKeyName(7, "207.png");
            this.imageList_0.Images.SetKeyName(8, "208.png");
            this.imageList_0.Images.SetKeyName(9, "332.png");
            this.imageList_0.Images.SetKeyName(10, "95.png");
            // 
            // listView1
            // 
            this.listView1.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
            this.listView1.BackColor = System.Drawing.Color.White;
            this.listView1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.listView1.CausesValidation = false;
            this.listView1.Font = new System.Drawing.Font(".VnArial Narrow", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listView1.ForeColor = System.Drawing.Color.Lime;
            this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.listView1.HideSelection = false;
            this.listView1.LargeImageList = this.imageList_0;
            this.listView1.Location = new System.Drawing.Point(0, 0);
            this.listView1.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.listView1.MultiSelect = false;
            this.listView1.Name = "listView1";
            this.listView1.OwnerDraw = true;
            this.listView1.Scrollable = false;
            this.listView1.ShowGroups = false;
            this.listView1.Size = new System.Drawing.Size(605, 53);
            this.listView1.TabIndex = 0;
            this.listView1.TileSize = new System.Drawing.Size(41, 42);
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Tile;
            // 
            // TrangThai
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(605, 53);
            this.Controls.Add(this.listView1);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font(".VnArial Narrow", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)), true);
            this.ForeColor = System.Drawing.Color.LimeGreen;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.Name = "TrangThai";
            this.Opacity = 0.95D;
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Trạng thái hiện tại";
            this.TransparencyKey = System.Drawing.Color.Transparent;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TrangThai_FormClosing);
            this.Load += new System.EventHandler(this.TrangThai_Load);
            this.ResumeLayout(false);

	}

	static TrangThai()
	{
		//timer_0 = new Timer();
	}

	internal static bool smethod_0()
	{
		return object_0 == null;
	}
	public class ListViewEx : ListView
	{
		protected override CreateParams CreateParams
		{
			get
			{
				CreateParams cp = base.CreateParams;
				cp.ExStyle |= 0x02000000;  // Turn on WS_EX_COMPOSITED
                return cp;
			}
		}
	}

	[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	static extern IntPtr CreateCompatibleDC(IntPtr hDC);

	[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	static extern IntPtr GetDC(IntPtr hWnd);

	[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

	[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	[return: MarshalAs(UnmanagedType.Bool)]
	static extern bool DeleteDC(IntPtr hdc);

	[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	static extern IntPtr SelectObject(IntPtr hDC, IntPtr hObject);

	[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	[return: MarshalAs(UnmanagedType.Bool)]
	static extern bool DeleteObject(IntPtr hObject);

	//[DllImport("engine.dll", EntryPoint = "DrawBitmap", CharSet = CharSet.Auto, SetLastError = true)]
	//public static extern void DrawBitmap(void This, int a2, int a3, int a4, int a5, void* a6, void* a7);

}
//public class ListViewEX : ListView
//{
//	public ListViewEX()
//	{
//		SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
//	}
//}


