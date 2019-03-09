#include "stdafx.h"
#include "GObject.h"

//GObject成员的定义
GMap *GObject::pStage = NULL;
int GObject::GetRow()
{
	return m_nRow;
}
int GObject::GetArray()
{
	return m_nArray;
}

int GObject::PtTransform(int k)                //坐标转换函数
{
	return (k - (pStage->LD) / 2) / pStage->LD;

}
//判断物体是否到达逻辑点坐标位置
bool GObject::Achive()
{
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;//计算x坐标的余数
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD; //计算y的余数
	bool l = (n == 0 && k == 0);
	return l;
}
//到达逻辑点之后更新数据
void GObject::AchiveCtrl()
{
	if (Achive()) {
		m_nArray = PtTransform(m_ptCenter.x);    //更新行
		m_nRow = PtTransform(m_ptCenter.y);      //更新列
	}
}
void GObject::DrawBlank(HDC &hdc)
{
	//申请资源，并交给智能指针进行处理
	HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255));
	std::shared_ptr<HBRUSH> phbr(&hbr, [](auto hbr) {   //交给智能指针处理，自动释放资源
		DeleteObject(*hbr);                    //离开DrawBlank函数时，自动调用释放资源
	});
	RECT rect;
	rect.top = m_nY - RD;
	rect.left = m_nX - RD;
	rect.right = m_nX + RD;
	rect.bottom = m_nY + RD;
	FillRect(hdc, &rect, *phbr); //绘制矩形
}
//设置中心点位置
void GObject::SetPosition(int Row, int Array)
{
	m_nRow = Row;
	m_nArray = Array;
	this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
	this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
}
//设置碰撞检测
bool GObject::Collision()
{
	bool b = false;

	//更新行和列的数据，若是大嘴，会执行PacMAN重写的AchiveCTRL（）函数消除豆子
	AchiveCtrl();
	if (m_nArray<0 || m_nRow<0 || m_nArray>MAPLENTH - 1
		|| m_nRow>MAPLENTH - 1) {
		b = true;
	}
	else if (Achive()) {
		switch (m_cmd) {
		case LEFT:
			//判断下一个格子是否能通过
			if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray - 1]) {
				b = true;          //撞墙了
			}
			break;
		case RIGHT:
			//判断下一个格子是否能通过
			if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray + 1]) {
				b = true;          //撞墙了
			}
			break;
		case UP:
			//判断下一个格子是否能通过
			if (m_nArray > 0 && !pStage->mapData[m_nRow - 1][m_nArray]) {
				b = true;          //撞墙了
			}
			break;
		case DOWN:
			//判断下一个格子是否能通过
			if (m_nArray > 0 && !pStage->mapData[m_nRow + 1][m_nArray]) {
				b = true;          //撞墙了
			}
			break;
		}
		if (!b) {
			m_dir = m_cmd;
		}
	}
	//依照真实的方向位移
	m_nX = m_ptCenter.x;
	m_nY = m_ptCenter.y;
	int MAX = pStage->LD*MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	switch (m_dir) {
		case LEFT:
			//判断下一个格子是否能够通过
			if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray - 1]) {
				b = true;
				break;
			}
			m_ptCenter.x -= m_nSpeed;
			if (m_ptCenter.x < MIN) {
				m_ptCenter.x = MAX;
			}
			break;
		case RIGHT:
			if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray + 1]) {
				b = true;
				break;
			}
			m_ptCenter.x += m_nSpeed;
			if (m_ptCenter.x > MAX) {
				m_ptCenter.x = MIN;
			}
		case UP:
			if (m_nArray > 0 && !pStage->mapData[m_nRow-1][m_nArray]) {
				b = true;
				break;
			}
			m_ptCenter.y -= m_nSpeed;
			if (m_ptCenter.y < MIN) {
				m_ptCenter.y = MAX;
			}
			break;
		case DOWN:
			if (m_nArray > 0 && !pStage->mapData[m_nRow+1][m_nArray]) {
				b = true;
				break;
			}
			m_ptCenter.y += m_nSpeed;
			if (m_ptCenter.y > MAX) {
				m_ptCenter.y = MIN;
			}
			break;
	}
	return b;
}
//PacMan 的成员函数定义
void PacMan::AchiveCtrl()
{
	GObject::AchiveCtrl();
	if (Achive()) {
		if (m_nRow >= 0 && m_nRow < MAPLENTH&&
			m_nArray >= 0 && m_nArray < MAPLENTH) {  //防止数组越界
			if (pStage->peaMapData[m_nRow][m_nArray]) {
				pStage->peaMapData[m_nRow][m_nArray] = false;
			}
		}
	}
}

void PacMan::action()
{
	Collision();                             //进行碰撞检测
}

void PacMan::SetTwCommand(TWARDS command) {
	m_cmd = command;                          //设置移动方向
}

bool PacMan::IsOver()
{
	return m_dir == OVER;                    //判断游戏是否结束
}

bool PacMan::IsWin()
{
	for (int i = 0; i <= MAPLENTH; i++) {
		for (int j = 0; j <= MAPLENTH; j++) {
			if (pStage->peaMapData[i][j] == true) {
				return false;                //存在任意一个豆子 没有胜利
			}
		}
	}
	return true;
}

POINT PacMan::GetPos()
{
	return m_ptCenter;
}
void PacMan::SetOver()
{
	m_dir = OVER;
}
void PacMan::Draw(HDC &memDC)
{
	if (m_dir == OVER) {
		//游戏结束什么也不用肝了
	}
	else if (m_nFrame % 2 == 0) {   //第四帧图像与第二帧图像相同：张嘴
		int x1 = 0, int x2 = 0, y1 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + D_OFFSET;
		int offsetY = DISTANCE / 2 + D_OFFSET;
		switch (m_dir) {
		case UP:
			x1 = m_ptCenter.x - offsetX;
			x2 = m_ptCenter.y + offsetY;
			y2 = y1 = m_ptCenter.y - offsetY;
			break;
		case DOWN:
			x1 = m_ptCenter.x + offsetX;
			x2 = m_ptCenter.x - offsetX;
			y2 = y1 = m_ptCenter.y + offsetY;
			break;
		case LEFT:
			x2 = x1 = m_ptCenter.x - offsetX;
			y1 = m_ptCenter.y + offsetY;
			y2 = m_ptCenter.y - offsetY;
			break;
		case RIGHT:
			x2 = x1 = m_ptCenter.x + offsetX;
			y1 = m_ptCenter.y - offsetY;
			y2 = m_ptCenter.y + offsetY;
			break;
		}
		//画出弧线部分
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
			x1, y1, x2, y2);
		//画直线
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	else if (m_nFrame % 3 == 0) {
		Ellipse(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE); //绘制圆形
	}
	else {                                 //嘴完全张开的形状
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		switch (m_dir) {
		case UP:
			x1 = m_ptCenter.x - DISTANCE;
			x2 = m_ptCenter.x + DISTANCE;
			y1 = y2 = m_ptCenter.y;
			break;
		case DOWN:
			x1 = m_ptCenter.x + DISTANCE;
			x2 = m_ptCenter.x - DISTANCE;
			y1 = y2 = m_ptCenter.y;
			break;
		case LEFT:
			x1 = x2 = m_ptCenter.x;
			y1 = m_ptCenter.y + DISTANCE;
			y2 = m_ptCenter.y - DISTANCE;
			break;
		case RIGHT:
			x1 = x2 = m_ptCenter.x;
			y1 = m_ptCenter.y - DISTANCE;
			y2 = m_ptCenter.y + DISTANCE;
			break;
		}
		//画出弧线
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
			x1, y1, x2, y2);
		//画直线
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	m_nFrame++;//绘制下一帧
}
//敌军玩家的实现
//Enermy成员定义
std::shared_ptr<PacMan>Enermy::player = nullptr;

void Enermy::Catch()
{
	int Dx = m_ptCenter.x - player->GetPos().x;
	int Dy = m_ptCenter.y - player->GetPos().y;
	if((-RD < Dx&&Dx < RD) && (-RD < Dy&&Dy < RD)){
		player->SetOver();
    }
}
void Enermy::Draw(HDC &hdc)
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldPen =(HPEN)SelectObject(hdc, pen);
	Arc(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
		m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
		m_ptCenter.x + DISTANCE, m_ptCenter.y,
		m_ptCenter.x - DISTANCE, m_ptCenter.y);//半圆形的头？
	int const LEGLENTH = (DISTANCE) / LEGCOUNT;
	//根据帧数来绘制身体和“腿部”
	if (m_nFrame % 2 == 0) {
		//矩形的身体
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE,
			m_ptCenter.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);
		for (int i = 0; i < LEGCOUNT; i++) {
			Arc(hdc,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH
			);

		}
	}

}
GObject::~GObject()
{
}
