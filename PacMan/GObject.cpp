#include "stdafx.h"
#include "GObject.h"

//GObject��Ա�Ķ���
GMap *GObject::pStage = NULL;
int GObject::GetRow()
{
	return m_nRow;
}
int GObject::GetArray()
{
	return m_nArray;
}

int GObject::PtTransform(int k)                //����ת������
{
	return (k - (pStage->LD) / 2) / pStage->LD;

}
//�ж������Ƿ񵽴��߼�������λ��
bool GObject::Achive()
{
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;//����x���������
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD; //����y������
	bool l = (n == 0 && k == 0);
	return l;
}
//�����߼���֮���������
void GObject::AchiveCtrl()
{
	if (Achive()) {
		m_nArray = PtTransform(m_ptCenter.x);    //������
		m_nRow = PtTransform(m_ptCenter.y);      //������
	}
}
void GObject::DrawBlank(HDC &hdc)
{
	//������Դ������������ָ����д���
	HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255));
	std::shared_ptr<HBRUSH> phbr(&hbr, [](auto hbr) {   //��������ָ�봦���Զ��ͷ���Դ
		DeleteObject(*hbr);                    //�뿪DrawBlank����ʱ���Զ������ͷ���Դ
	});
	RECT rect;
	rect.top = m_nY - RD;
	rect.left = m_nX - RD;
	rect.right = m_nX + RD;
	rect.bottom = m_nY + RD;
	FillRect(hdc, &rect, *phbr); //���ƾ���
}
//�������ĵ�λ��
void GObject::SetPosition(int Row, int Array)
{
	m_nRow = Row;
	m_nArray = Array;
	this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
	this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
}
//������ײ���
bool GObject::Collision()
{
	bool b = false;

	//�����к��е����ݣ����Ǵ��죬��ִ��PacMAN��д��AchiveCTRL����������������
	AchiveCtrl();
	if (m_nArray<0 || m_nRow<0 || m_nArray>MAPLENTH - 1
		|| m_nRow>MAPLENTH - 1) {
		b = true;
	}
	else if (Achive()) {
		switch (m_cmd) {
		case LEFT:
			//�ж���һ�������Ƿ���ͨ��
			if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray - 1]) {
				b = true;          //ײǽ��
			}
			break;
		case RIGHT:
			//�ж���һ�������Ƿ���ͨ��
			if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray + 1]) {
				b = true;          //ײǽ��
			}
			break;
		case UP:
			//�ж���һ�������Ƿ���ͨ��
			if (m_nArray > 0 && !pStage->mapData[m_nRow - 1][m_nArray]) {
				b = true;          //ײǽ��
			}
			break;
		case DOWN:
			//�ж���һ�������Ƿ���ͨ��
			if (m_nArray > 0 && !pStage->mapData[m_nRow + 1][m_nArray]) {
				b = true;          //ײǽ��
			}
			break;
		}
		if (!b) {
			m_dir = m_cmd;
		}
	}
	//������ʵ�ķ���λ��
	m_nX = m_ptCenter.x;
	m_nY = m_ptCenter.y;
	int MAX = pStage->LD*MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	switch (m_dir) {
		case LEFT:
			//�ж���һ�������Ƿ��ܹ�ͨ��
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
//PacMan �ĳ�Ա��������
void PacMan::AchiveCtrl()
{
	GObject::AchiveCtrl();
	if (Achive()) {
		if (m_nRow >= 0 && m_nRow < MAPLENTH&&
			m_nArray >= 0 && m_nArray < MAPLENTH) {  //��ֹ����Խ��
			if (pStage->peaMapData[m_nRow][m_nArray]) {
				pStage->peaMapData[m_nRow][m_nArray] = false;
			}
		}
	}
}

void PacMan::action()
{
	Collision();                             //������ײ���
}

void PacMan::SetTwCommand(TWARDS command) {
	m_cmd = command;                          //�����ƶ�����
}

bool PacMan::IsOver()
{
	return m_dir == OVER;                    //�ж���Ϸ�Ƿ����
}

bool PacMan::IsWin()
{
	for (int i = 0; i <= MAPLENTH; i++) {
		for (int j = 0; j <= MAPLENTH; j++) {
			if (pStage->peaMapData[i][j] == true) {
				return false;                //��������һ������ û��ʤ��
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
		//��Ϸ����ʲôҲ���ø���
	}
	else if (m_nFrame % 2 == 0) {   //����֡ͼ����ڶ�֡ͼ����ͬ������
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
		//�������߲���
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
			x1, y1, x2, y2);
		//��ֱ��
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	else if (m_nFrame % 3 == 0) {
		Ellipse(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE); //����Բ��
	}
	else {                                 //����ȫ�ſ�����״
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
		//��������
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
			x1, y1, x2, y2);
		//��ֱ��
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	m_nFrame++;//������һ֡
}
//�о���ҵ�ʵ��
//Enermy��Ա����
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
		m_ptCenter.x - DISTANCE, m_ptCenter.y);//��Բ�ε�ͷ��
	int const LEGLENTH = (DISTANCE) / LEGCOUNT;
	//����֡������������͡��Ȳ���
	if (m_nFrame % 2 == 0) {
		//���ε�����
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
