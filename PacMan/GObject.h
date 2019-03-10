#pragma once
#include "stdafx.h"
#include <time.h>
#include "GMap.h"
#define PLAYERSPEED 6
#define ENERMYSPEED 4
#define LEGCOUNT    5
#define DISTANCE    10
#define BLUE_ALERT  8
#define D_OFFSET    3
#define RD  (DISTANCE+D_OFFSET)
enum TWARDS {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	OVER,
};

class GObject
{
public:
	GObject(int Row, int Array) {
		m_nFrame = 1;
		pStage = NULL;
		this->m_nRow = Row;
		this->m_nArray = Array;
		//中心位置
		this->m_ptCenter.x = m_nRow * pStage->LD + pStage->LD / 2;
		this->m_ptCenter.y = m_nArray * pStage->LD + pStage->LD / 2;
		this->m_nX = m_ptCenter.x;
		this->m_nY = m_ptCenter.y;
	}
	~GObject();

	void SetPosition(int Row, int Array);
	void DrawBlank(HDC &hdc);
	void virtual Draw(HDC &hdc) = 0;
	void virtual action() = 0;
	int GetRow();
	int GetArray();
	static GMap *pStage;
protected:
	int m_nX;
	int m_nY;
	TWARDS m_cmd;
	POINT m_ptCenter;
	int m_nRow;
	int m_nArray;
	int m_nSpeed;
	TWARDS m_dir;
	int m_nFrame;
	bool Achive();
	bool Collision();
	int PtTransform(int k);
	virtual void AchiveCtrl();   
};
class PacMan :public GObject
{
protected:
	virtual void AchiveCtrl();
public:
	POINT GetPos();
	bool IsOver();
	bool IsWin();
	void SetTwCommand(TWARDS command);
	void Draw(HDC &hdc);
	PacMan(int x, int y) :GObject(x, y) {
		this->m_nSpeed = PLAYERSPEED;
		m_cmd = m_dir = LEFT;
	}
	void action();
	void SetOver();
};
class Enermy :public GObject
{
protected:
	void Catch();
	void virtual MakeDecision(bool b) = 0;
	COLORREF color;
public:
	static std::shared_ptr<PacMan>player;
	void virtual Draw(HDC &hdc);
	Enermy(int x, int y) :GObject(x, y) {
		this->m_nSpeed = ENERMYSPEED;
		m_dir = LEFT;
		m_cmd = UP;   
	}
	void virtual action();
};
//三种敌人
class RedOne :public Enermy  //随即移动S
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC &hdc);
	RedOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(255, 0, 0);
	}

};
class BlueOne :public Enermy  //随即移动S
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC &hdc);
	BlueOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(0, 0, 255);
	}

};
class YellowOne :public Enermy  //随即移动S
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC &hdc);
	YellowOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(200, 200, 100);
	}

};
