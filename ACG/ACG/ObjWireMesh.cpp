#include "GameL\DrawTexture.h"
#include "GameL\HitBoxManager.h"
#include "GameHead.h"
#include "ObjWireMesh.h"
#include "Function.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
CObjWireMesh::CObjWireMesh(int x, int y)
{
	m_px = (float)x * BLOCK_SIZE;
	m_py = (float)y * BLOCK_SIZE;

	m_map_x = x;
	m_map_y = y;
}

//イニシャライズ
void CObjWireMesh::Init()
{
	//当たり判定用HitBoxを作成                          
	Hits::SetHitBox(this, m_px, m_py, WIRE_MESH_SIZE_WIDTH, WIRE_MESH_SIZE_HEIGHT, ELEMENT_GIMMICK, OBJ_WIRE_MESH, 1);
}

//アクション
void CObjWireMesh::Action()
{
	//HitBoxの位置の変更
	CHitBox* hit = Hits::GetHitBox(this);

	HIT_DATA** hit_data;	//衝突の情報を入れる構造体
	hit_data = hit->SearchObjNameHit(OBJ_HERO);//衝突の情報をhit_dataに入れる

	for (int i = 0; i < hit->GetCount(); i++)
	{
		if (hit_data[i] != nullptr)//ヒットデータがあれば
		{
			float r = hit_data[i]->r;//あたっている角度を持ってくる

			//主人公オブジェクトを持ってくる
			CObjHero* objhero = (CObjHero*)Objs::GetObj(OBJ_HERO);

			//上側があたっていれば
			if (5.5f < r && r < 174.5f)
			{
				objhero->SetHitDown(true);//主人公が乗っていたらm_hit_downにtrueを返す

				//乗せる処理
				objhero->SetPosY(m_py - 126.0f);//ブロックの上側に調節する
				//主人公の移動ベクトルが下向きなら
				if (objhero->GetVecY() > 0.0f)
					objhero->SetVecY(0.0f);//主人公のY方向の移動を0にする
			}
		}
	}
	//HitBoxの位置を更新する
	HitBoxUpData(Hits::GetHitBox(this), m_px, m_py);
}

//ドロー
void CObjWireMesh::Draw()
{
	//描画カラー
	float color[4] = { 1.0f,1.0f,1.0f, 1.0f };

	RECT_F src, dst;

	//マップオブジェクトを持ってくる
	CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//切り取り位置
	src.m_top = 0.0f;
	src.m_left = 0.0f;
	src.m_right = src.m_left + 640.0f;
	src.m_bottom = src.m_top + 64.0f;

	//描画位置
	dst.m_top =  m_py - objmap->GetScrollY() ;
	dst.m_left = m_px - objmap->GetScrollX();
	dst.m_right = dst.m_left + WIRE_MESH_SIZE_WIDTH;
	dst.m_bottom = dst.m_top + WIRE_MESH_SIZE_HEIGHT;

	//描画
	Draw::Draw(GRA_WIRE_MASH, &src, &dst, color, 0.0f);
	//-------------------------------------------------------
}