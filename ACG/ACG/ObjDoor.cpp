#include "GameL\DrawTexture.h"
#include "GameL\SceneManager.h"
#include "GameL\HitBoxManager.h"

#include "GameHead.h"
#include "ObjDoor.h"
#include "Function.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
CObjDoor::CObjDoor(int x, int y)
{
	m_px = x * DOOR_SIZE_X;
	m_py = y * DOOR_SIZE_Y;
}

//イニシャライズ
void CObjDoor::Init()
{
}

//アクション
void CObjDoor::Action()
{
}

//ドロー
void CObjDoor::Draw()
{
	//描画カラー
	float color[4] = { 1.0f,1.0f,1.0f, 1.0f };

	RECT_F src, dst;

	//マップオブジェクトを持ってくる
	CObjMap* obj_m = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//切り取り位置
	src.m_top = 0.0f;
	src.m_left = 0.0f;
	src.m_right = 63.0f;
	src.m_bottom = 127.0f;

	//描画位置
	dst.m_top = 0.0f + m_py - obj_m->GetScrollY();
	dst.m_left = 0.0f + m_px - obj_m->GetScrollX();
	dst.m_right = dst.m_left + DOOR_SIZE_X;
	dst.m_bottom = dst.m_top + DOOR_SIZE_Y;

	//描画
	Draw::Draw(6, &src, &dst, color, 0);
}