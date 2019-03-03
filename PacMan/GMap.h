#pragma once

#include <list>

#define MAPLENTH 19            //the logic map size
#define P_ROW    10            //my position
#define P_ARRAY  9            
#define E_ROW	 8            //enemy position
#define E_ARRAY  9 
using std::list;
//抽象类 GMAP
class GMap
{
protected:
	static int LD;
	static int PD;
	bool mapData[MAPLENTH][MAPLENTH];
	bool peaMapData[MAPLENTH][MAPLENTH];
	void initOp();             //敌我双方的位置没有豆子出现
	COLORREF color;
public:
	void DrawMap(HDC &hdc);
	void DrawPeas(HDC &hdc);
	virtual ~GMap();
	GMap()
	{

	}
	friend class GObject;       //允许物体类直接使用直线的起点和终点的信息做碰撞检测
	friend class PacMan;        //允许大嘴访问豆子地图
};
//第一关
class Stage_1 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH]; //地图数据
public :
	Stage_1();
};
//第二关
class Stage_2 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_2();
};

//第三关的地图
class Stage_3 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_3();
};
