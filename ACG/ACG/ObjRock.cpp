//使用するヘッダーファイル　
#include "GameL\DrawTexture.h"
#include "GameL\SceneManager.h"
#include "GameL\HitBoxManager.h"
#include "GameL\Audio.h"

#include "GameHead.h"
#include "ObjRock.h"
#include "Function.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
CObjRock::CObjRock(int x, int y)
{
	m_px = x * BLOCK_SIZE;
	m_py = y * BLOCK_SIZE;
}

//イニシャライズ
void CObjRock::Init()
{
	m_ani_time = 0;
	m_ani_frame = 1;  //静止フレームを初期にする
	m_ani_max_time = 5; //アニメーション間隔幅
	m_ani_start_flag = false;//アニメフラグOFF

	//当たり判定用HitBoxを作成                          
	Hits::SetHitBox(this, m_px, m_py, ROCK_SIZE_WIDTH, ROCK_SIZE_HEIGHT, ELEMENT_GIMMICK, OBJ_ROCK, 1);
}

//アクション
void CObjRock::Action()
{
	//ブロック情報を持ってくる
	CObjBlock* objblock = (CObjBlock*)Objs::GetObj(OBJ_BLOCK);

	//HitBoxの位置の変更
	CHitBox* hit = Hits::GetHitBox(this);
	
	//弾と接触しているかどうかを調べる
	if (hit->CheckObjNameHit(OBJ_BULLET) != nullptr)
	{
		Audio::Start(ROCK);//岩の音楽スタート
		m_ani_start_flag = true;//アニメフラグON
	}
	//アニメフラグONだと
	if (m_ani_start_flag == true)
	{
		m_ani_time+=1;
		//アニメーションの感覚管理
		if (m_ani_time > m_ani_max_time)
		{
			m_ani_frame += 1;
			m_ani_time = 0;
		}

		//最後までアニメーションが進むと存在を消す
		if (m_ani_frame == 4)
		{
			Hits::DeleteHitBox(this);	//岩が所有するHitBoxに削除する
			this->SetStatus(false);		//自身に削除命令を出す
			return;
		}
	}

	HIT_DATA** hit_data;	//衝突の情報を入れる構造体
	hit_data = hit->SearchObjNameHit(OBJ_HERO);//衝突の情報をhit_dataに入れる

	//主人公オブジェクトを持ってくる
	CObjHero* objhero = (CObjHero*)Objs::GetObj(OBJ_HERO);

	for (int i = 0; i < hit->GetCount(); i++)
	{
		//データがあれば
		if (hit_data[i] != nullptr)
		{
			float r = hit_data[i]->r;//あたっている角度をもってくる

			//岩の右側が衝突している場合
			if (0.0f < r && r < 65.0f|| 315.0f < r && r < 360.0f)
			{
				//岩にめりこまないようにする処理
				if (objhero->GetPosture() == 0.0f)//右向き
				{
					objhero->SetPosX(m_px + ROCK_SIZE_WIDTH);//主人公の位置を岩の右側までずらす
				}
				else//左向き
				{
					objhero->SetPosX(m_px + 155.0f);//主人公の位置を岩の右側までずらす
				}
				
			}
			
			//岩の左側が衝突している場合
			else if (115.0f < r && r < 225.0f)
			{
				//岩にめりこまないようにする処理
				if (objhero->GetPosture() == 0.0f)//右向き
				{
					objhero->SetPosX(m_px - HERO_SIZE_WIDTH+15.0f);//主人公を岩の左に行くようにする
				}
				else//左向き
				{
					//めり込み防止のため
					objhero->SetPosX(m_px - HERO_SIZE_WIDTH);//主人公を岩の左に行くようにする
				}
				
			}

			//岩の下側が衝突している場合
			else if (225.0f < r && r < 315.0f)
			{
				objhero->SetVecY(0.0f);//主人公のY方向の移動を０にする
				objhero->SetPosY(m_py + ROCK_SIZE_HEIGHT);//主人公の位置を岩の下側までずらす
			}
		}
	}
	//HitBoxの位置を更新する
	HitBoxUpData(Hits::GetHitBox(this), m_px, m_py- ROCK_SIZE_WIDTH);
}

//ドロー
void CObjRock::Draw()
{
	//画像の切り取り配列
	int AniData[4] =
	{
		0, 1, 2, 3
	};

	//描画カラー
	float color[4] = { 1.0f,1.0f,1.0f, 1.0f };

	RECT_F src, dst;

	//マップオブジェクトを持ってくる
	CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//切り取り位置
	src.m_top = 1.0f;
	src.m_left = (float)AniData[m_ani_frame] * 128.0f - 128.0f;
	src.m_right = src.m_left + 128.0f;
	src.m_bottom = src.m_top + 198.0f;
	
	//描画位置
	dst.m_top = m_py - objmap->GetScrollY()- ROCK_SIZE_WIDTH;
	dst.m_left = m_px - objmap->GetScrollX();
	dst.m_right = dst.m_left + ROCK_SIZE_WIDTH;
	dst.m_bottom = dst.m_top + ROCK_SIZE_HEIGHT;

	//描画
	Draw::Draw(GRA_ROCK, &src, &dst, color, 0.0f);
}