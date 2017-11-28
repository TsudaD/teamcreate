//使用するヘッダーファイル
#include "GameL\HitBoxManager.h"

#include "GameHead.h"
#include "ObjFallingBlock.h"
#include "Function.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
CObjFallingBlock::CObjFallingBlock(int x, int y)
{
	m_map_x = x;
	m_map_y = y;
	m_px = x * BLOCK_SIZE;
	m_py = y * BLOCK_SIZE;
}

//イニシャライズ
void CObjFallingBlock::Init()
{
	//当たり判定用HitBoxを作成                          
	Hits::SetHitBox(this, m_px, m_py, BLOCK_SIZE, BLOCK_SIZE, ELEMENT_GIMMICK, OBJ_FALLING_BLOCK, 1);


}

//アクション
void CObjFallingBlock::Action()
{

	//マップオブジェクトを持ってくる
	CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//HitBoxの位置を更新する
	HitBoxUpData(Hits::GetHitBox(this), m_px, m_py);

}

//ドロー
void CObjFallingBlock::Draw()
{
	//描画カラー
	float color[4] = { 1.0f,1.0f,1.0f, 1.0f };

	RECT_F src, dst;

	//マップオブジェクトを持ってくる
	CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//map_numにマップ情報の一個上の情報を送る
	int map_num = objmap->GetMap(m_map_x, m_map_y - 1);

	//切り取り位置
	src.m_top = 0.0f;
	src.m_left = 0.0f;
	src.m_right = 64.0f;
	src.m_bottom = 64.0f;

	//ブロックの一個上にブロックがある時描画変更
	if (map_num == MAP_FALLING_BLOCK)
	{
		src.m_top = 0.0f;
		src.m_left = 65.0f;
		src.m_right = 128.0f;
		src.m_bottom = 64.0f;
	}
	//描画位置
	dst.m_top = m_py - objmap->GetScrollY();
	dst.m_left = m_px - objmap->GetScrollX();
	dst.m_right = dst.m_left + BLOCK_SIZE;
	dst.m_bottom = dst.m_top + BLOCK_SIZE;

	//描画
	Draw::Draw(GRA_BLOCK, &src, &dst, color, 0.0f);
}

//主人公が当たったときの処理
void CObjFallingBlock::HeroHit()
{
	//主人公オブジェクトを持ってくる
	CObjHero* objhero = (CObjHero*)Objs::GetObj(OBJ_HERO);
	//自身のHitBoxをもってくる
	CHitBox*hit = Hits::GetHitBox(this);

	HIT_DATA** hit_data;										//衝突の情報を入れる構造体
	hit_data = hit->SearchObjNameHit(OBJ_HERO);					//衝突の情報をhit_dataに入れる

	for (int i = 0; i < hit->GetCount(); i++)
	{
		if (hit_data[i] != nullptr)								//ヒットデータがあれば
		{
			float r = hit_data[i]->r;							//あたっている角度を持ってくる

			//上側があたっていれば
			if (35.0f < r && r < 145.0f)
			{
				objhero->SetHitDown(true);						//主人公が乗っていたらm_hit_downにtrueを返す
																//乗せる処理
				objhero->SetPosY(m_py - BLOCK_SIZE);				//ブロックの上側に調節する

																//主人公の移動ベクトルが下向きなら
				if (objhero->GetVecY()>0.0f)
					objhero->SetVecY(0.0f);						//主人公のY方向の移動を0にする
			}
			//左側が当たっていれば
			else if (145.0f <= r && r <= 180.0f)
			{
				//左に反発する処理
				objhero->SetPosX(m_px - BLOCK_SIZE);	//主人公の位置をブロックの左にする
				objhero->SetVecX(-1.0f * objhero->GetVecX());	//主人公のX方向の移動量を反転する
			}
			//右側が当たっていれば
			else if (0.0f <= r && r <= 35.0f)
			{
				//右に反発する処理
				objhero->SetPosX(m_px + BLOCK_SIZE);	//主人公の位置をブロックの右にする
				objhero->SetVecX(-1.0f * objhero->GetVecX());	//主人公のX方向の移動量を反転する
			}

			//下側があたっていれば
			else if (180.0f < r && r < 360.0f)
			{
				//下に反発する処理
				objhero->SetPosY(m_py + BLOCK_SIZE);//主人公の位置をブロックの下にする
				objhero->SetVecY(-1.0f * objhero->GetVecY());	//主人公のY方向の移動量を反転する
			}
		}
	}
}