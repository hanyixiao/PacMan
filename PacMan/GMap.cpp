#include "stdafx.h"
#include "GMap.h"
int GMap::LD = 36; //墙的宽度
int GMap::PD = 3;  //豆子的直径
void GMap::initOp()
{
	peaMapData[E_ROW][E_ARRAY] = false;   //敌方的位置没有豆子
	peaMapData[P_ROW][P_ARRAY] = false;   //玩家的位置没有豆子
}
void GMap::DrawMap(HDC &memDC)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
			if (!mapData[i][j]) {
				RECT rect;
				rect.left = j * LD;
				rect.top = i * LD;
				rect.right = (j + 1)*LD;
				rect.bottom = (j + 1)*LD;
				FillRect(memDC,&rect, hBrush);
			}
		}
	}
	DeleteObject(hBrush);
}

void GMap::DrawPeas(HDC &hdc)
{
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j != MAPLENTH; j++) {
			if (!peaMapData[i][j]) {
				Ellipse(hdc, (LD / 2 - PD) + j * LD,
					(LD / 2 - PD) + i * LD,
					(LD / 2 + PD) + j * LD,
					(LD / 2 + PD) + i * LD);   //画圆模拟豆子
			}
		}
	}
	//游戏后门
	if (GetAsyncKeyState('B') & 0x8000) {
		MessageBox(NULL, TEXT("无意间你发现了游戏后门"),TEXT(" "), MB_OK);
		for (int i = 0; i != MAPLENTH; i++) {
			for (int j = 0; j != MAPLENTH; j++) {
				peaMapData[i][j] = false;
			}
		}
   }
}
GMap::~GMap() {

}
