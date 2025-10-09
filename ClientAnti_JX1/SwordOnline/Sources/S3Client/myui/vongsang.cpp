#include "KWin32.h"
#include "../../Represent/iRepresent/iRepresentShell.h"

#include "vongsang.h"


extern iRepresentShell*	g_pRepresentShell;



#define  minframeitemback 30
#define  maxframeitemback 120
#define  toframeitemback  4

#define  framedelayitemback 16
#define	 framegoitemback 4





void _settypeitemcolor(int type,unsigned char &a,unsigned char &r,unsigned char &g,unsigned char &b,unsigned char ki=0)
{
	unsigned char zcol=255;

	
	zcol-=ki;

	switch(type)
	{
	case enomal:a=r=g=b=zcol;

		break;
	case ehoangkim:a=r=g=zcol;b=0;//vang
	//	printf("asasa= %d\t%d\t%d\n",r,g,b);
		break;
	case ebachkim:a=r=g=zcol;b=255;//sang bac,chua tim ra mau
		break;
	case ehuyenkim:a=r=b=zcol;g=0;//tim
		break;
	case ethan:a=r=g=zcol;b=0;//chua dugn den
		break;


	}
}



void _draw_vongsang(KRUPoint *zdata,int zsizeline,int &zpos,unsigned char &zdir,int x,int y,int xe,int ye,int typecol)
{
	int i,temp,temp1;
	int tempcol=255/zsizeline;


	for(i=0;i<zsizeline;i++)
	{
		_settypeitemcolor(typecol,zdata[i].Color.Color_b.a,zdata[i].Color.Color_b.r,
			zdata[i].Color.Color_b.g,zdata[i].Color.Color_b.b,
			i*tempcol);

	}
	switch(zdir)
	{
	case  0:
		for(i=0;i<zsizeline;i++)
		{	temp=zpos-i;
			if(temp>=x)//dir 0
			{
				zdata[i].oPosition.nX=temp;
				zdata[i].oPosition.nY=y;	
			}
			else 
			{
				temp1=y+x-temp;//pos end
				if(temp1<=ye)//chua dat den height cua dir 3
				{zdata[i].oPosition.nX=x;
				zdata[i].oPosition.nY=temp1;
				}
				else//truong hop cuoi,khong qua phan nua rect
				{
					zdata[i].oPosition.nX=x+temp1-ye;
					zdata[i].oPosition.nY=ye;
						
				}

			}
		}
		break;
	
	//===========
	case  1:
		for(i=0;i<zsizeline;i++)
		{	temp=zpos-i;
		if(temp>=y)//dir 1
		{
			zdata[i].oPosition.nY=temp;
			zdata[i].oPosition.nX=xe;	
		}
		else 
		{
			temp1=xe-(y-temp);//pos end
			if(temp1>=x)//chua dat den height cua dir 3
			{zdata[i].oPosition.nX=temp1;
			zdata[i].oPosition.nY=y;
			}
			else//truong hop cuoi,khong qua phan nua rect
			{
				zdata[i].oPosition.nY=y+x-temp1;
				zdata[i].oPosition.nX=x;

			}

		}
		}
		break;
//===========
	case  2:
		for(i=0;i<zsizeline;i++)
		{	temp=zpos+i;
		if(temp<=xe)//dir 2
		{
			zdata[i].oPosition.nX=temp;
			zdata[i].oPosition.nY=ye;	
		}
		else 
		{
			temp1=ye-(temp-xe);//pos end
			if(temp1>=y)
			{zdata[i].oPosition.nX=xe;
			zdata[i].oPosition.nY=temp1;
			}
			else//truong hop cuoi,khong qua phan nua rect
			{
				zdata[i].oPosition.nX=xe-(y-temp1);
				zdata[i].oPosition.nY=y;

			}

		}
		}
		break;
		//===========
	case  3:
		for(i=0;i<zsizeline;i++)
		{	temp=zpos+i;
		if(temp<=ye)//dir 2
		{
			zdata[i].oPosition.nX=x;
			zdata[i].oPosition.nY=temp;	
		}
		else 
		{
			temp1=x+(temp-ye);//pos end
			if(temp1<=xe)
			{zdata[i].oPosition.nX=temp1;
			zdata[i].oPosition.nY=ye;
			}
			else//truong hop cuoi,khong qua phan nua rect
			{
				zdata[i].oPosition.nY=ye-(temp1-xe);
				zdata[i].oPosition.nX=xe;

			}

		}
		}
		break;

	}

	g_pRepresentShell->DrawPrimitives(zsizeline,zdata,RU_T_POINT,true);
}

static int dllmaxsizeline[16]={
	26,2*26,3*26,4*26,5*26,6*26,7*26,8*26,
	9*26,10*26,11*26,12*26,13*26,14*26,15*26,16*26
};

void _swapposvs(int &zpos,unsigned char &zdir,int x,int y,int width,int height)
{
	if(zdir==0)
	{
		zdir=2;
		zpos=2*x+width-zpos;
	}
	else if(zdir==1)
	{
		zdir=3;
		zpos=2*y+height-zpos;
	}
	else if(zdir==2)
	{
		zdir=0;
		zpos=2*x+width-zpos;
	}
	else if(zdir==3)
	{
		zdir=1;
		zpos=2*y+height-zpos;
	}

}

void draw_vongsang(int &zpos,unsigned char &zdir,unsigned char &ztimedelay,int x,int y,int width,int height,int zsizeline,int typecol)
{
ztimedelay+=framegoitemback;

if(ztimedelay>=framedelayitemback)
{
ztimedelay-=framedelayitemback;
if(zdir<=1)
zpos++;
else
zpos--;
}

//KRUShadow tempshadow;
//_settypeitemcolor(typecol,tempshadow.Color.Color_b.r,tempshadow.Color.Color_b.g,
//				  tempshadow.Color.Color_b.b);
//tempshadow.Color.Color_b.a=100;
//tempshadow.Color.Color_dw=0x0a654915;

// tempshadow.oPosition.nX=x+100;
// tempshadow.oPosition.nY=y;
// tempshadow.oEndPos.nX=x+200;	//doc left
// tempshadow.oEndPos.nY=y+height+1;

// tempshadow.oPosition.nX=x-1;
// tempshadow.oPosition.nY=y-1;
// tempshadow.oEndPos.nX=x+1;	//doc left
// tempshadow.oEndPos.nY=y+height+1;
//g_pRepresent->DrawPrimitives(1,&tempshadow,RU_T_SHADOW,true);
/*
tempshadow.oPosition.nX=x+width-1;
tempshadow.oPosition.nY=y-1;	//doc right
tempshadow.oEndPos.nX=x+width+1;
tempshadow.oEndPos.nY=y+height+1;
g_pRepresent->DrawPrimitives(1,&tempshadow,RU_T_SHADOW,true);

tempshadow.oPosition.nX=x-1;
tempshadow.oPosition.nY=y-1;	//ngang top
tempshadow.oEndPos.nX=x+1;
tempshadow.oEndPos.nY=y+1;
g_pRepresent->DrawPrimitives(1,&tempshadow,RU_T_SHADOW,true);

*/

	if(zdir==0)
	{	if(zpos<x)zpos=x;
		else if(zpos>=x+width)
		{	zpos=y+1;zdir=1;
		}
	}
	else if(zdir==1)
	{	if(zpos<y)zpos=y;
		else if(zpos>=y+height)
		{	zpos=x+width-1;zdir=2;
		}
	}
	else if(zdir==2)
	{	if(zpos>x+width)zpos=x+width;
		else if(zpos<=x)
		{	zpos=y+height-1;zdir=3;
		}
	}
	else if(zdir==3)
	{	if(zpos>y+height)zpos=y+height;
		else if(zpos<=y)
		{	zpos=x+1;zdir=0;
		}
	}



	int tempsize=width+height;
	if(zsizeline>tempsize||zsizeline<0)
		return;

	else if(zsizeline<=dllmaxsizeline[0])
	{
		KRUPoint temp[26];
		_draw_vongsang(temp,dllmaxsizeline[0],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[0],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[1])
	{
		KRUPoint temp[2*26];
		_draw_vongsang(temp,dllmaxsizeline[1],zpos,zdir, x,y,x+width,y+height,typecol);

		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[1],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);


	}
	else if(zsizeline<=dllmaxsizeline[2])
	{
		KRUPoint temp[3*26];
		_draw_vongsang(temp,dllmaxsizeline[2],zpos,zdir, x,y,x+width,y+height,typecol);

		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[2],zpos,zdir,x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);


	}
	else if(zsizeline<=dllmaxsizeline[3])
	{
		KRUPoint temp[4*26];
	//	char temp1[30];	

		_draw_vongsang(temp,dllmaxsizeline[3],zpos,zdir, x,y,x+width,y+height,typecol);
	//	sprintf(temp1,"pos=%d va dir=%d",zpos,zdir);	
		//g_pRepresent->OutputText(12,temp1,-1,x,y,0xffffffff);

		_swapposvs(zpos,zdir,x,y,width,height);
		//sprintf(temp1,"pos=%d va dir=%d",zpos,zdir);	
		//g_pRepresent->OutputText(12,temp1,-1,x,y+20,0xffffffff);


		_draw_vongsang(temp,dllmaxsizeline[3],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		//sprintf(temp1,"pos=%d va dir=%d",zpos,zdir);	
		//g_pRepresent->OutputText(12,temp1,-1,x,y+40,0xffffffff);


	}
	else if(zsizeline<=dllmaxsizeline[4])
	{
		KRUPoint temp[5*26];
		_draw_vongsang(temp,dllmaxsizeline[4],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[4],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);


	}
	else if(zsizeline<=dllmaxsizeline[5])
	{
		KRUPoint temp[6*26];
		_draw_vongsang(temp,dllmaxsizeline[5],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[5],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);


	}
	else if(zsizeline<=dllmaxsizeline[6])
	{
		KRUPoint temp[7*26];
		_draw_vongsang(temp,dllmaxsizeline[6],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[6],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);


	}
	else if(zsizeline<=dllmaxsizeline[7])
	{
		KRUPoint temp[8*26];
		_draw_vongsang(temp,dllmaxsizeline[7],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[7],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[8])
	{
		KRUPoint temp[9*26];
		_draw_vongsang(temp,dllmaxsizeline[8],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[8],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[9])
	{
		KRUPoint temp[10*26];
		_draw_vongsang(temp,dllmaxsizeline[9],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[9],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[10])
	{
		KRUPoint temp[11*26];
		_draw_vongsang(temp,dllmaxsizeline[10],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[10],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);


	}
	else if(zsizeline<=dllmaxsizeline[11])
	{
		KRUPoint temp[12*26];
		_draw_vongsang(temp,dllmaxsizeline[11],zpos,zdir, x,y,x+width,y+height,typecol);
	
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[11],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[12])
	{
		KRUPoint temp[13*26];
		_draw_vongsang(temp,dllmaxsizeline[12],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[12],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[13])
	{
		KRUPoint temp[14*26];
		_draw_vongsang(temp,dllmaxsizeline[13],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[13],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[14])
	{
		KRUPoint temp[15*26];
		_draw_vongsang(temp,dllmaxsizeline[14],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[14],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}
	else if(zsizeline<=dllmaxsizeline[15])
	{
		KRUPoint temp[16*26];
		_draw_vongsang(temp,dllmaxsizeline[15],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);
		_draw_vongsang(temp,dllmaxsizeline[15],zpos,zdir, x,y,x+width,y+height,typecol);
		_swapposvs(zpos,zdir,x,y,width,height);

	}

}


