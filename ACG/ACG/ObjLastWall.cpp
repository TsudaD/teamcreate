#include "GameL\DrawTexture.h"
#include "GameL\HitBoxManager.h"
#include "GameL\Audio.h"
#include "GameL\UserData.h"


#include "GameHead.h"
#include "ObjLastWall.h"
#include "Function.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
CObjLastWall::CObjLastWall(int x, int y)
{
	m_px = (float)x * 64.0f;
	m_py = (float)y * 64.0f;
}

//イニシャライズ
void CObjLastWall::Init()
{
	//当たり判定																
	Hits::SetHitBox(this, m_px, m_py, 32, 512, ELEMENT_GIMMICK, OBJ_LAST_WALL, 1);
	m_wall_gauge = 0;
	m_wall_gauge2 = 0;
	m_look_unlock_flag = false;
	a = false;
	m_hero_hit_flag = false;
	switch (((UserData*)Save::GetData())->stagenum)
	{
		//ステージ１
	case 1:
		m_wall_type = 1;
		break;
		//ステージ２
	case 2:
		m_wall_type = 2;
		break;
		//ステージ３
	case 3:
		m_wall_type = 3;
		break;
		//ステージ５
	case 5:
		m_wall_type = 5;
		break;
	default:

		break;
	}
}

//アクション
void CObjLastWall::Action()
{
	
	//HitBoxの位置の変更
	//引数で持ってきたオブジェクトとあたっているか調べる
	CHitBox* hit = Hits::GetHitBox(this);

	//主人公オブジェクトを持ってくる
	CObjHero* objhero = (CObjHero*)Objs::GetObj(OBJ_HERO);
	//敵オブジェクと持ってくる
	CObjEnemy*objenemy = (CObjEnemy*)Objs::GetObj(OBJ_ENEMY);

	if (hit->CheckObjNameHit(OBJ_HERO) != nullptr)
	{
		HIT_DATA** hit_data;		//主人公の衝突の情報を入れる構造体
		hit_data = hit->SearchObjNameHit(OBJ_HERO);//主人公の衝突の情報をhit_dataに入れる
		for (int i=0;i < hit->GetCount();i++)
		{
			if (hit_data[i] != nullptr)
			{
				float r=0.0f;
				
				r=hit_data[i]->r;
				if (0 < r && r < 85 || 275 < r && r < 360)
				{
					objhero->SetVecX(0.0f);//主人公のX方向の移動を０にする
					objhero->SetPosX(m_px + 62.0f);//主人公の位置をLastWallの右側までずらす
				}

				//LastWallの上側が衝突している場合
				else if (85 < r && r < 94)
				{

					objhero->SetVecY(0.0f);//主人公のY方向の移動を０にする
					objhero->SetPosY(m_py - 60.0f);//主人公の位置をLastWallの上側までずらす
				}

				//LastWallの左側が衝突している場合
				else if (94 < r && r < 266)
				{
					objhero->SetVecX(0.0f);//主人公のX方向の移動を０にする
										   //objhero->SetPosX(m_px - HERO_SIZE_WIDTH+28.0f);//主人公の位置をLastWallの左側までずらす
					m_hero_hit_flag = true;


				}

				//LastWallの下側が衝突している場合
				else if (266 < r && r < 275)
				{
					objhero->SetVecY(0.0f);//主人公のY方向の移動を０にする
					objhero->SetPosY(m_py + 512.0f);//主人公の位置をLastWallの下側までずらす
					
				}

			}
		}
	}
	else if (hit->CheckObjNameHit(OBJ_ENEMY) != nullptr)
	{
		HIT_DATA** hit_data_enemy;	//敵の衝突の情報を入れる構造体
		hit_data_enemy = hit->SearchObjNameHit(OBJ_ENEMY);//敵の衝突の情報をhit_dataに入れる

		for (int i = 0; i < hit->GetCount(); i++)
		{
			if (hit_data_enemy[i] != nullptr)
			{
				float r = 0.0f;
				r = hit_data_enemy[i]->r;


				//LastWallの右側が衝突している場合
				if (0 < r && r < 85 || 275 < r && r < 360)
				{
					objenemy->SetVX(2.0f);//主人公のX方向の移動を０にする
					objenemy->SetPosture(1.0f);
				}
				else if (94 < r && r < 266)
				{
					objenemy->SetVX(-2.0f);//主人公のX方向の移動を０にする
					objenemy->SetPosture(0.0f);//主人公の位置をLastWallの左側までずらす
				}

			}
		}
	}
	
	

	

	
	

	
	
	//// ボタンオブジェクトを持ってくる
	//CObjButton* objbutton = (CObjButton*)Objs::GetObj(OBJ_BUTTON);
	
	//マップオブジェクトを持ってくる
	CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);
	int map_num = objmap->GetMap(m_map_x, m_map_y);

	//ボスの情報を呼ぶの
	CObjBoss*objboss = (CObjBoss*)Objs::GetObj(OBJ_BOSS);
	


		////ボタンを押してたら扉を開くフラグオン
		//if (objbutton->GetTrickFlag() == true)
		//{
		//	m_look_unlock_flag = true;
		//}

		// m_wall_gaugeが512を越えたら処理ストップ
		if (m_wall_gauge >= 512)
		{
			a = true;
			Audio::Stop(WALL);//音楽ストップ
			m_wall_gauge = 0;
		}
		else
		{
			//heroが左側に振れてたら押されていたら
			if (m_hero_hit_flag == true && a == false&&objenemy==nullptr)
			{
				m_wall_gauge += 3; // 3ずつ増やしていく
				Audio::Start(WALL);//開門の音楽スタート
			}
		}

		if (a == false)
		{
			// hitboxが小さくなる
			HitBoxUpData(hit, m_px, m_py + m_wall_gauge, 32, 512 - m_wall_gauge);
		}

		if (m_wall_gauge2 >= 512)
		{
			Audio::Stop(WALL);//音楽ストップ
		}
		else
		{
			if (a == true)
			{
				m_wall_gauge2 += 8;
				// hitboxが小さくなる
				Audio::Start(WALL);//開門の音楽スタート
				HitBoxUpData(hit, m_px, m_py + m_wall_gauge, 32, 0 + m_wall_gauge2);

			}
		}

/*
	//当たり判定-----------------------------------------------
	for (int i = 0; i < hit->GetCount(); i++)
	{
		//データがあれば
		if (hit_data[i] != nullptr)
		{
			float r = hit_data[i]->r;//あたっている角度をもってくる

			//LastWallの右側が衝突している場合
			if (0 < r && r < 85 || 275 < r && r < 360)
			{
				objhero->SetVecX(0.0f);//主人公のX方向の移動を０にする
				objhero->SetPosX(m_px + 62.0f);//主人公の位置をLastWallの右側までずらす
			}

			//LastWallの上側が衝突している場合
			else if (85 < r && r < 94)
			{

				objhero->SetVecY(0.0f);//主人公のY方向の移動を０にする
				objhero->SetPosY(m_py-60.0f);//主人公の位置をLastWallの上側までずらす
			}

			//LastWallの左側が衝突している場合
			else if (94 < r && r < 266)
			{
				objhero->SetVecX(0.0f);//主人公のX方向の移動を０にする
				//objhero->SetPosX(m_px - HERO_SIZE_WIDTH+28.0f);//主人公の位置をLastWallの左側までずらす
				m_hero_hit_flag = true;

				
			}

			//LastWallの下側が衝突している場合
			else if (266 < r && r < 275)
			{
				objhero->SetVecY(0.0f);//主人公のY方向の移動を０にする
				objhero->SetPosY(m_py + 512.0f);//主人公の位置をLastWallの下側までずらす
			}
		}
		
		
		
	}
	*/
	//HitBoxの位置を更新する
	HitBoxUpData(Hits::GetHitBox(this), m_px+29, m_py+65);
}
//指定したオブジェクトの上側に当たっているかしらべる
//引数　調べたいオブジェクトネーム
//戻り値　着地していれば:true　していなければ:false
//bool CObjHero::HitUpCheck(int obj_name)
//{
//	//自身のHitBoxをもってくる
//	CHitBox*hit = Hits::GetHitBox(this);
//
//	//引数で持ってきたオブジェクトとあたっているか調べる
//	if (hit->CheckObjNameHit(obj_name) != nullptr)
//	{
//		HIT_DATA** hit_data;	//衝突の情報を入れる構造体
//		hit_data = hit->SearchObjNameHit(obj_name);//衝突の情報をhit_dataに入れる
//
//												   //あたっている数分調べる
//		for (int i = 0; i < hit->GetCount(); i++)
//		{
//			//データがあれば
//			if (hit_data[i] != nullptr)
//			{
//				//衝突した相手の位置、幅、高さ情報を取得
//				HIT_BOX* hit = Hits::GetHitBox(hit_data[i]->o)->GetHitBox();
//				//マップオブジェクトを持ってくる
//				CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);
//
//				//HitTestOfAB関数を使うと変更されてしまうので仮変数作成
//				float hero_x = m_px;
//				float hero_y = m_py;
//				float hero_vx = m_vx;
//				float hero_vy = m_vy;
//				//衝突した方向を取得
//				int collision = HitTestOfAB(hit->x + objmap->GetScrollX(), hit->y + objmap->GetScrollY(), hit->w, hit->h,
//					&hero_x, &hero_y, HERO_SIZE_WIDTH, HERO_SIZE_HEIGHT,
//					&hero_vx, &hero_vy
//				);
//				//主人公から見て下に衝突したら着地している
//				if (collision == 2)
//					return true;
//			}
//		}
//	}
//	return false;//着地していない
//}


//ドロー
void CObjLastWall::Draw()
{
	//描画カラー
	float color[4] = { 1.0f,1.0f,1.0f, 1.0f };

	RECT_F src, dst;

	//マップオブジェクトを持ってくる
	CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);
	switch (m_wall_type)
	{
	case 1:
	case 2:
		//切り取り位置
		src.m_top = 0.0f;
		src.m_left = 0.0f;
		src.m_right = 64.0f;
		src.m_bottom = 256.0f;

		//描画位置
		dst.m_top = m_py - objmap->GetScrollY() - 190;
		dst.m_left = m_px - objmap->GetScrollX();
		dst.m_right = dst.m_left + 96.0f;
		dst.m_bottom = dst.m_top + 256.0f;

		//描画(上の部分)
		Draw::Draw(GRA_LAST_WALL, &src, &dst, color, 0.0f);

		//切り取り位置
		src.m_top = 0.0 + m_wall_gauge;
		src.m_left = 0.0f;
		src.m_right = 32.0f;
		src.m_bottom = 512.0f;

		//描画位置
		dst.m_top = m_py - objmap->GetScrollY() + 65;
		dst.m_left = m_px - objmap->GetScrollX() + 30;
		dst.m_right = dst.m_left + 32;
		dst.m_bottom = dst.m_top + 512-m_wall_gauge;

		//描画(下の部分)
		Draw::Draw(GRA_OPEN_WALL, &src, &dst, color, 0.0f);
		break;

	case 3:
		//切り取り位置
		src.m_top = 0.0f;
		src.m_left = 0.0f;
		src.m_right = 64.0f;
		src.m_bottom = 256.0f;

		//描画位置
		dst.m_top = m_py - objmap->GetScrollY() - 190;
		dst.m_left = m_px - objmap->GetScrollX();
		dst.m_right = dst.m_left + 96.0f;
		dst.m_bottom = dst.m_top + 256.0f;

		//描画(上の部分)
		Draw::Draw(GRA_LAST_WALL3, &src, &dst, color, 0.0f);

		//切り取り位置
		src.m_top = 0.0 + m_wall_gauge;
		src.m_left = 0.0f;
		src.m_right = 32.0f;
		src.m_bottom = 512.0f;
		//描画位置
		dst.m_top = m_py - objmap->GetScrollY() + 65;
		dst.m_left = m_px - objmap->GetScrollX() + 30;
		dst.m_right = dst.m_left + 32;
		dst.m_bottom = dst.m_top + 512;

		//描画(下の部分)
		Draw::Draw(GRA_OPEN_WALL3, &src, &dst, color, 0.0f);
		break;
	

	case 5:
		break;
	}
	

	//-----------------------------------------------------
	//if (a == false)
	//{
	//	//切り取り位置
	//	src.m_top = 0.0 + m_wall_gauge;
	//	src.m_left = 0.0f;
	//	src.m_right = 32.0f;
	//	src.m_bottom = 512.0f;
	//
	//	//描画位置
	//	dst.m_top = m_py - objmap->GetScrollY() + 65;
	//	dst.m_left = m_px - objmap->GetScrollX() + 30;
	//	dst.m_right = dst.m_left + 32;
	//	dst.m_bottom = dst.m_top + 512 - m_wall_gauge;
	//}
	//else
	//{
	//	//切り取り位置
	//	src.m_top = 0.0;
	//	src.m_left = 0.0f;
	//	src.m_right = 32.0f;
	//	src.m_bottom = 0.0f+m_wall_gauge2;
	//	//描画位置
	//	dst.m_top = m_py - objmap->GetScrollY() + 65;
	//	dst.m_left = m_px - objmap->GetScrollX() + 30;
	//	dst.m_right = dst.m_left + 32;
	//	dst.m_bottom = dst.m_top +  m_wall_gauge2;
	//}
	////描画(下の部分)
	//Draw::Draw(GRA_OPEN_WALL, &src, &dst, color, 0.0f);
}
