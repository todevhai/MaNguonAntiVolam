#ifndef VONGSANG_H_
#define VONGSANG_H_
#pragma once


#include "../../Represent/iRepresent/KRepresentUnit.h"
enum enumtypeitem
{
	enomal=0,
	ehoangkim,
	ebachkim,
	ehuyenkim,
	ethan,
	enummer
};
void draw_vongsang(int &zpos,unsigned char &zdir,unsigned char &ztimedelay,int x,int y,int width,int height,int zsizeline=52,int typecol=ehoangkim);


#endif
