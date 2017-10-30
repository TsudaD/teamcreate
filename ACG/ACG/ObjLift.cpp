#include "GameL\DrawTexture.h"
#include "GameL\SceneManager.h"
#include "GameL\HitBoxManager.h"
#include "GameL\WinInputs.h"

#include "GameHead.h"
#include "ObjLift.h"
#include "Function.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
CObjLift::CObjLift(int x, int y)
{
	m_px = (float)x * BLOCK_SIZE;
	m_py = (float)y * BLOCK_SIZE;
}

//イニシャライズ
void CObjLift::Init()
{
	m_vx = 0.0f;
	m_vy = 0.0f;

	//ステージ１のときのリフトの動きの初期位置を右にする
	m_right_max_x = m_px;
	//ステージ１のときのリフトの動きの最大左X位置を計算
	m_lift_max_x = m_px - 640;

	//当たり判定
	Hits::SetHitBox(this, m_px, m_py, LIFT_SIZE_WIDTH, LIFT_SIZE_HEIGHT, ELEMENT_GIMMICK, OBJ_LIFT, 1);

}

//アクション
void CObjLift::Action()
{
	//自身のHitBoxをもってくる
	CHitBox*hit = Hits::GetHitBox(this);

	//主人公が当たっていれば
	if (hit->CheckObjNameHit(OBJ_HERO) != nullptr)
	{
		HeroRide();//主人公を乗せる処理をする
	}

	//ロープオブジェクトを持ってくる
	CObjRopeSwitch* objrope_switch = (CObjRopeSwitch*)Objs::GetObj(OBJ_ROPE_SWITCH);
	
	if (objrope_switch->GetRopeFlag() == true)//ロープとロープスイッチがあたっているとき
	{
		if (Input::GetVKey('A'))
			m_vx = -1.0f;
		else
			m_vx = 0.0f;
	}
	else//ロープとロープスイッチがあたっていないとき
	{
		m_vx = 1.0f;
	}

	//左に行き過ぎないように
	if (m_px + m_vx <= m_lift_max_x)
	{
		m_vx = 0.0f;
	}
	//右に行き過ぎないように
	if (m_px + m_vx >= m_right_max_x)
	{
		m_vx = 0.0f;
	}
	//移動
	m_px += m_vx;
	m_py += m_vy;

	//HitBoxの位置を更新する
	HitBoxUpData(Hits::GetHitBox(this), m_px, m_py);
}

//ドロー
void CObjLift::Draw()
{	
}

//主人公を乗せる処理をする
void CObjLift::HeroRide()
{
	//自身のHitBoxをもってくる
	CHitBox*hit = Hits::GetHitBox(this);

	HIT_DATA** hit_data;	//衝突の情報を入れる構造体
	hit_data = hit->SearchObjNameHit(OBJ_HERO);//衝突の情報をhit_dataに入れる

	for (int i = 0; i < hit->GetCount(); i++)
	{
		if (hit_data[i] != nullptr)
		{
			float r = hit_data[i]->r;//あたっている角度を持ってくる

			//上側があたっていればで
			if (45.0f <= r && r <= 135.0f)
			{
				//主人公オブジェクトを持ってくる
				CObjHero* objhero = (CObjHero*)Objs::GetObj(OBJ_HERO);
				float h_px = objhero->GetPosX();//主人公の位置Xを持ってくる

				objhero->SetHitDown(true);//リフトの上に主人公が乗っていたらm_hit_downにtrueを返す

				//リフトに乗せる処理
				objhero->SetVecY(0.0f);//主人公のY方向の移動を０にする
				objhero->SetPosX(h_px + m_vx);//主人公の位置をもともと主人公が居た位置＋リフトの移動量にする
				objhero->SetPosY(m_py - HERO_SIZE_HEIGHT+1.0f);//主人公のポジションをリフトの上にする
			}
		}
	}
}