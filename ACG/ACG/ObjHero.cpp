#include "GameL\DrawTexture.h"
#include "GameL\WinInputs.h"
#include "GameL\HitBoxManager.h"

#include "GameHead.h"
#include "ObjHero.h"
#include"Function.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
CObjHero::CObjHero(int x, int y)
{
	m_px = x * HERO_SIZE_X;
	m_py = y * HERO_SIZE_Y;
}

//イニシャライズ
void CObjHero::Init()
{
	m_px = 0.0f;
	m_py = 0.0f;
	m_vx = 0.0f;
	m_vy = 0.0f;
	m_posture = 0.0f; //右向き0.0f 左向き1.0f
	m_r = 0.0f;

	m_bullet_control = false;  //弾丸発射制御用
	m_rope_control = false;	//ロープ発射制御用
	m_jump_control = false;	//ジャンプ制御
	m_w_jump_control = false; //2段ジャンプ制御
	m_landingflag = false;

	m_ani_time = 0;
	m_ani_frame = 1;  //静止フレームを初期にする
	m_ani_max_time = 6; //アニメーション間隔幅

	//ブロックとの衝突した状態(場所)確認用
	m_hit_up	= false;
	m_hit_left  = false;
	m_hit_right = false;
	m_hit_down  = false;

	//当たり判定
	Hits::SetHitBox(this, m_px, m_py, HERO_SIZE_X, HERO_SIZE_Y, ELEMENT_PLAYER, OBJ_HERO, 1);
}

//アクション
void CObjHero::Action()
{
	//自身のHitBoxをもってくる
	CHitBox*hit = Hits::GetHitBox(this);

	//落下にリスタート----------------------------------
	//m_pyが1000以下ならリスタートする
	if (m_py > 1000.0f)
	{
		//場外に出たらリスタート
		Scene::SetScene(new CSceneMain());
	}
	
	LandingCheck();//着地フラグの更新

	if (m_landingflag == true)//着地していれば
		m_vy = 0.0f;//移動ベクトルを初期化

	//移動ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//Aキーがおされたとき：右移動
	if (Input::GetVKey('D') == true)
	{
		m_vx += 0.5f;
		m_ani_frame_stop = 0;
		m_posture = 0.0f;//主人公の向き
		m_ani_time += 1;
	}
	//Dキーがおされたとき：左移動
	else if (Input::GetVKey('A') == true)
	{
		m_vx -= 0.5f;
		m_ani_frame_stop = 0;
		m_posture = 1.0f;//主人公の向き
		m_ani_time += 1;
	}
	else
	{
		m_ani_frame_stop = 1; //キー入力が無い時は1を入れる
		m_ani_frame = 1; //キー入力が無い場合は静止フレームにする
		m_ani_time = 0;
	}

	//アニメーションの感覚管理
	if (m_ani_time > m_ani_max_time)
	{
		m_ani_frame += 1;
		m_ani_time = 0;
	}

	//最後までアニメーションが進むと最初に戻る
	if (m_ani_frame == 3)
	{
		m_ani_frame = 0;
	}
	
	CObjBlock* obj_b = (CObjBlock*)Objs::GetObj(OBJ_BLOCK);


	//ジャンプ--------------------------------------------------------------------
	//スペースキーを押されたとき：二段ジャンプ防止フラグオン
	if (Input::GetVKey(VK_SPACE) == true)
	{
		m_w_jump_control = true;
	}
	else
	{
		m_w_jump_control = false;
	}

	//着地フラグがオン かつ　二段ジャンプ防止フラグがオンのとき：ジャンプ
	if (m_landingflag == true && m_w_jump_control == true)
	{
		if (m_jump_control == true)
		{
			m_vy = -20.0f;
			m_jump_control = false;
		}
	}
	else
		m_jump_control = true; //スペース押してなければジャンプでるフラグにする。*/

	//ジャンプ終了-------------------------------------------------------------------------------------


	//↓キーがおされたとき：下に下がる（デバッグ）
	if (Input::GetVKey(VK_DOWN) == true)
	{
		m_vy = 20.0f;
	}

	//はしご-------------------------------------------------
	////はしごと接触しているかどうかを調べる
	if (hit->CheckObjNameHit(OBJ_LADDERS) != nullptr)
	{
		//Wキーがおされたとき 上るとき
		if (Input::GetVKey('W') == true)
		{
			m_vy = -2.0f;
		}

		//Sキーがおされたとき　下るとき
		if (Input::GetVKey('S') == true)
		{
			m_vy = 2.0f;
		}
	}
	//はしご終了---------------------------------------------
	

	//摩擦
	m_vx += -(m_vx * 0.098);

	if (m_landingflag == false)
	{
		//自由落下運動
		m_vy += 9.8 / (16.0f);  //ブロックに着地できるようになったらはずしてください
	}

	//Scroll();	//スクロール処理をおこなう
	//ブロックとの当たり判定
	obj_b->BlockHit(&m_px, &m_py, HERO_SIZE_X, HERO_SIZE_Y, true,
		&m_hit_up, &m_hit_down, &m_hit_left, &m_hit_right, &m_vx, &m_vy);

	//移動ベクトルをポジションに加算
	m_px += m_vx;
	m_py += m_vy;

	////移動ベクトルを初期化
	//m_vx = 0.0f;
	//m_vy = 0.0f;

	//移動終わり-----------------------------------------

	//発砲---------------------------------------------------
	//左クリックを押したら
	if (Input::GetMouButtonL() == true)
	{
		if (m_bullet_control == true)
		{
			if (m_posture == 0)//主人公が右を向いているとき右側から発射
			{
				//弾丸作成
				CObjBullet* Objbullet = new CObjBullet(m_px + 64.0f, m_py + 50.0f);
				Objs::InsertObj(Objbullet, OBJ_BULLET, 10);
				m_bullet_control = false; //弾丸を出ないフラグにする。
			}
			else//主人公が左を向いているとき右側から発射
			{
				//弾丸作成
				CObjBullet* Objbullet = new CObjBullet(m_px - 16.0f, m_py + 50.0f);
				Objs::InsertObj(Objbullet, OBJ_BULLET, 10);
				m_bullet_control = false; //弾丸を出ないフラグにする。
			}
		}
	}
	else
		m_bullet_control = true; //左クリックしてなければ弾丸をでるフラグにする。

	//発砲終了-----------------------------------------------


	//ロープ射出---------------------------------------------
	//右クリックを押したら
	if (Input::GetMouButtonR() == true)
	{
		if (m_rope_control == true)
		{
			//ロープ作成
			CObjRope* Objrope = new CObjRope(m_px, m_py);
			Objs::InsertObj(Objrope, OBJ_ROPE, 10);
			m_rope_control = false;
		}
	}
	else
		m_rope_control = true; //右クリックを押していなければロープが出るフラグを立てる。
	//射出終了------------------------------------------------

	//水オブジェクトと衝突していれば
	if (hit->CheckObjNameHit(OBJ_WATER) != nullptr)
	{
		this->SetStatus(false);		//自身を削除
		Hits::DeleteHitBox(this);	//ヒットボックスを削除

		//メインへ移行
		Scene::SetScene(new CSceneMain());
		return;
	}

	//敵オブジェクトと衝突していれば
	if (hit->CheckObjNameHit(OBJ_ENEMY) != nullptr) //仮です。敵が多いようならElementに変えます
	{
		this->SetStatus(false);		//自身を削除
		Hits::DeleteHitBox(this);	//ヒットボックスを削除

		//メインへ移行
		Scene::SetScene(new CSceneMain());
		return;
	}

	//木オブジェクトと衝突してれば
	if (hit->CheckObjNameHit(OBJ_WOOD) != nullptr)
	{
		
	}

	////水オブジェクトと衝突していれば
	//if (hit->CheckObjNameHit(OBJ_WATER) != nullptr)
	//{
	//	this->SetStatus(false);		//自身を削除
	//	Hits::DeleteHitBox(this);	//ヒットボックスを削除

	//								//メインへ移行
	//	Scene::SetScene(new CSceneMain());
	//}

	//HitBoxの位置を更新する
	HitBoxUpData(Hits::GetHitBox(this), m_px, m_py);

}

//スクロール処理の関数
void CObjHero::Scroll()
{
	//マップオブジェクトを持ってくる
	CObjMap* obj_m = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//主人公が左または右のスクロールラインを超えそうなら
	if ((m_px + m_vx) < SCROLL_LINE_LEFT || (m_px + m_vx) > SCROLL_LINE_RIGHT)
	{
		obj_m->SetScrollX(m_vx);	//移動量をスクロールにセット
	}

	//主人公が上または下のスクロールラインを超えそうなら
	if ((m_py + m_vy) < SCROLL_LINE_UP || (m_py + m_vy) > SCROLL_LINE_DOWN) 
	{
		obj_m->SetScrollY(m_vy);	//移動量をスクロールにセット
	}

}

//ドロー
void CObjHero::Draw()
{
	//画像の切り取り配列
	int AniData[3] =
	{
		0  , 1 , 2
	};

	//描画カラー
	float color[4] = { 1.0f,1.0f,1.0f, 1.0f };

	RECT_F src, dst;

	//マップオブジェクトを持ってくる
	CObjMap* obj_m = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//切り取り位置
	//止まっている時
	if (m_ani_frame_stop == 1)  //仮
	{
		src.m_top = 0.0f;
		src.m_left = 0.0f;
		src.m_right = 64.0f;
		src.m_bottom = 128.0f;
	}
	else//動いているとき
	{
		src.m_top = 128.0f;
		src.m_left = 0.0f + AniData[m_ani_frame] * 64;
		src.m_right = 64.0f + AniData[m_ani_frame] * 64;
		src.m_bottom = 256.0f;
	}

	//描画位置
	dst.m_top		= 0.0f + m_py - obj_m->GetScrollY();
	dst.m_left		= (HERO_SIZE_X * m_posture) + m_px - obj_m->GetScrollX();
	dst.m_right	    = (HERO_SIZE_X - HERO_SIZE_X * m_posture) + m_px - obj_m->GetScrollX();
	dst.m_bottom	= dst.m_top  + HERO_SIZE_Y;

	//描画
	Draw::Draw(3, &src, &dst, color, m_r);

	////画面全体を暗くするです。
	//Draw::SetFill(true);
	////画面全体をこの色にする
	////staticなのは消すかもしれないから
	//static float col[4] = { 0.0f };
	//col[0] -= 0.01f;
	//col[1] -= 0.01f;
	//col[2] -= 0.01f;
	//Draw::SetColor(col);
}

//着地できてるかどうかを調べる関数
void CObjHero::LandingCheck()
{
	bool c1, c2;//チェック結果を保存するための変数:チェック項目を増やすたびに数を増やす必要がある
	
	c1=HitUpCheck(OBJ_BLOCK);//ブロックとの着地チェック
	c2=HitUpCheck(OBJ_LIFT); //リフトとの着地チェック

	//チェック項目のどれか一つでもtrueなら
	if (c1 == true || c2 == true)
		m_landingflag = true;//着地フラグをオンにする
	else
		m_landingflag = false;//着地フラグをオフにする

}

//指定したオブジェクトの上側に当たっているかしらべる
//引数　調べたいオブジェクトネーム
//戻り値　着地していれば:true　していなければ:false
bool CObjHero::HitUpCheck(int obj_name)
{
	//自身のHitBoxをもってくる
	CHitBox*hit = Hits::GetHitBox(this);
	
	//引数で持ってきたオブジェクトとあたっているか調べる
	if (hit->CheckObjNameHit(obj_name) != nullptr)
	{
		HIT_DATA** hit_data;	//衝突の情報を入れる構造体
		hit_data = hit->SearchObjNameHit(obj_name);//衝突の情報をhit_dataに入れる

		//あたっている数分調べる
		for (int i = 0; i < hit->GetCount(); i++)
		{
			//データがあれば
			if (hit_data[i] != nullptr)
			{
				float r = hit_data[i]->r;//あたっている角度をもってくる

				 //Heroの下側があたっていれば
				if (225.0f < r && r < 315.0f)
				{
					return true;//着地している
				}
			}
		}
	}
	return false;//着地していない
}

