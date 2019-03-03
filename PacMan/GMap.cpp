#include "stdafx.h"
#include "GMap.h"
int GMap::LD = 36; //ǽ�Ŀ��
int GMap::PD = 3;  //���ӵ�ֱ��
void GMap::initOp()
{
	peaMapData[E_ROW][E_ARRAY] = false;   //�з���λ��û�ж���
	peaMapData[P_ROW][P_ARRAY] = false;   //��ҵ�λ��û�ж���
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
					(LD / 2 + PD) + i * LD);   //��Բģ�ⶹ��
			}
		}
	}
	//��Ϸ����
	if (GetAsyncKeyState('B') & 0x8000) {
		MessageBox(NULL, TEXT("������㷢������Ϸ����"),TEXT(" "), MB_OK);
		for (int i = 0; i != MAPLENTH; i++) {
			for (int j = 0; j != MAPLENTH; j++) {
				peaMapData[i][j] = false;
			}
		}
   }
}
GMap::~GMap() {

}
