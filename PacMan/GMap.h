#pragma once

#include <list>

#define MAPLENTH 19            //the logic map size
#define P_ROW    10            //my position
#define P_ARRAY  9            
#define E_ROW	 8            //enemy position
#define E_ARRAY  9 
using std::list;
//������ GMAP
class GMap
{
protected:
	static int LD;
	static int PD;
	bool mapData[MAPLENTH][MAPLENTH];
	bool peaMapData[MAPLENTH][MAPLENTH];
	void initOp();             //����˫����λ��û�ж��ӳ���
	COLORREF color;
public:
	void DrawMap(HDC &hdc);
	void DrawPeas(HDC &hdc);
	virtual ~GMap();
	GMap()
	{

	}
	friend class GObject;       //����������ֱ��ʹ��ֱ�ߵ������յ����Ϣ����ײ���
	friend class PacMan;        //���������ʶ��ӵ�ͼ
};
//��һ��
class Stage_1 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH]; //��ͼ����
public :
	Stage_1();
};
//�ڶ���
class Stage_2 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_2();
};

//�����صĵ�ͼ
class Stage_3 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_3();
};
