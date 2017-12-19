#include "GameL\DrawTexture.h"
#include "GameL\HitBoxManager.h"

#include "GameL\Audio.h"
#include "GameHead.h"
#include "Function.h"

#include "ObjStage5BossArms.h"

//使用するネームスペース
using namespace GameL;

//コンストラクタ
//ステージ5ボスcppからの描画の都合で、xとyの型をintからfloatに変更しています。(描画で細かい値を使いたいため)
CObjStage5BossArms::CObjStage5BossArms(float x, float y, int type)
{
	m_px = x;
	m_py = y;
	m_arms_type = type;//type = 1…ライトアーム  2…レフトアーム
}

//イニシャライズ
void CObjStage5BossArms::Init()
{
	m_vx = 0.0f;
	m_vy = 0.0f;

	m_arm_hp = 10; //第5ボスアームのＨＰ(仮にＨＰを[10]と設定、左右のアーム共通)

	m_ani_flag_claw = false;//爪の開閉アニメーションをしない

	m_ani_frame_claw = 0;	//描画アニメーション(爪)
	m_ani_max_time_claw = 5;//アニメーションフレーム動作間隔最大値(爪)
	m_ani_time_claw = 0;		//アニメーションフレーム動作間隔(爪)

	m_arm_lower_marker_px = 0.0f;	//腕を下ろす位置を示すかどうかとそのX位置
	m_armdown_time=0;//腕を下ろすときの管理用タイム

	m_wall_hit_flag = false;

	m_block_hit_flag = false;
	m_initpos_flag = false;

	//腕の接続部分の電流を表示する/しないかを判断するための変数----------------------
	//接続部分の描画はObjStage5Boss.cppにて

	m_left_arm_move = false; //false…初期位置から動いていない　true…動いている(レフトアーム用)
	m_right_arm_move = false;//false…初期位置から動いていない　true…動いている(ライトアーム用)

	//-------------------------------------------------------------------------------

	//typeの値が1のときライトアームの当たり判定表示
	if (m_arms_type == RIGHT_ARM)
	{
		//当たり判定用HitBoxを作成(HITBOXのサイズ調整用に補正値を加えています)
		//																横サイズ補正値						   縦サイズ補正値
		Hits::SetHitBox(this, m_px, m_py, STAGE5_BOSS_ARMS_WIDTH_SIZE, STAGE5_BOSS_ARMS_HEIGHT_SIZE, ELEMENT_ENEMY, OBJ_STAGE5_BOSS_ARMS, 1);
		
	}
	//typeの値が1のときレフトアームの当たり判定表示
	else if (m_arms_type == LEFT_ARM)
	{
		//当たり判定用HitBoxを作成(HITBOXのサイズ調整用に補正値を加えています)
		//																横サイズ補正値						   縦サイズ補正値
		Hits::SetHitBox(this, m_px , m_py, STAGE5_BOSS_ARMS_WIDTH_SIZE, STAGE5_BOSS_ARMS_HEIGHT_SIZE, ELEMENT_ENEMY, OBJ_STAGE5_BOSS_ARMS, 2);
	}
}

//アクション
void CObjStage5BossArms::Action()
{
	//初期位置を更新する
	UpdateInitial();

	//HitBox更新用ポインター取得
	CHitBox* hit = Hits::GetHitBox(this);

	//アームタイプが1のとき、ライトアーム用の当たり判定表示
	if (m_arms_type == RIGHT_ARM)
	{
		//当たり判定更新
		HitBoxUpData(Hits::GetHitBox(this), m_px, m_py);
	}
	//アームタイプが2のとき、レフトアーム用の当たり判定表示
	else if (m_arms_type == LEFT_ARM)
	{
		//当たり判定更新
		HitBoxUpData(Hits::GetHitBox(this), m_px, m_py);
	}
	
	//移動
	m_px += m_vx;
	m_py += m_vy;

	//落ちるブロックと当たれば
	if (hit->CheckObjNameHit(OBJ_FALLING_BLOCK) != nullptr)
	{
		m_py -= m_vy;
		m_vx = 0.0f;
		m_vy = 0.0f;

		m_block_hit_flag = true;
	}

	
	//ボスオブジェクトの取得
	CObjStage5Boss* objboss = (CObjStage5Boss*)Objs::GetObj(OBJ_STAGE5_BOSS);

	//ステージ５のボスが存在していたら
	if (objboss != nullptr)
	{
		if (objboss->GetAttackMode() == 3)//攻撃パターン３なら
		{
			MoveShotAttack();
		}
	}

	//弾丸とあたったらＨＰを1減らす
	if (hit->CheckObjNameHit(OBJ_BULLET) != nullptr)
	{
		m_arm_hp -= 1;
	}

	//ＨＰが0になったらオブジェクトを初期位置に戻すためのフラグをオンにする
	if (m_arm_hp == 0)
	{
		m_initpos_flag = true;
	}

	//初期位置フラグがオンなら
	if (m_initpos_flag == true)
	{
		m_left_arm_move = false; //レフトアームが「初期位置から動いていない」判定を出す
		m_right_arm_move = false;//ライトアームが「初期位置から動いていない」判定を出す
		
		//初期位置に戻す
		m_px = m_initial_px;
		m_py = m_initial_py;
		//HPを戻す
		m_arm_hp = 10;
		m_initpos_flag = false;
		m_block_hit_flag = false;
		return;
	}
	
	//爪のアニメーション処理-----------------------------------
	//爪のアニメーションをする
	if (m_ani_flag_claw == true)
	{
		//アニメーション動作間隔を進める
		m_ani_time_claw++;
		//アニメーション動作間隔最大値以上なら
		if (m_ani_time_claw >= m_ani_max_time_claw)
		{
			//アニメーション動作間隔を0にする
			m_ani_time_claw = 0;
			m_ani_frame_claw++;
			
		}
		if (m_ani_frame_claw == 5)
		{
			m_ani_frame_claw = 0;
			m_ani_flag_claw = false;
		}
	}
	//---------------------------------------------------------
}

//ドロー
void CObjStage5BossArms::Draw()
{
	//描画カラー
	float color[4] = { 1.0f,1.0f,1.0f, 1.0f };
	RECT_F src, dst;

	//マップオブジェクトを持ってくる
	CObjMap* objmap = (CObjMap*)Objs::GetObj(OBJ_MAP);

	//爪の開閉アニメーションをするときの配列
	//要素数はm_ani_frame_claw
	int ani_claw[5] = { 1,2,0 };

	//右腕(ライトアーム)---------------------------------------
	//アームタイプが1のときはライトアームを描画
	if (m_arms_type == RIGHT_ARM)
	{
		//切り取り位置
		src.m_top = STAGE5_BOSS_ARMS_HEIGHT_SIZE;
		
		if(m_ani_flag_claw == true)//爪の開閉アニメーションをするなら
			src.m_left = STAGE5_BOSS_ARMS_WIDTH_SIZE*ani_claw[m_ani_frame_claw];
		else
			src.m_left = STAGE5_BOSS_ARMS_WIDTH_SIZE*1;

		src.m_right = src.m_left + STAGE5_BOSS_ARMS_WIDTH_SIZE;
		src.m_bottom = src.m_top + STAGE5_BOSS_ARMS_HEIGHT_SIZE;

		//描画位置
		dst.m_top = m_py - objmap->GetScrollY();
		dst.m_left = m_px - objmap->GetScrollX();
		dst.m_right = dst.m_left + STAGE5_BOSS_ARMS_WIDTH_SIZE;
		dst.m_bottom = dst.m_top + STAGE5_BOSS_ARMS_HEIGHT_SIZE;
		//描画
		Draw::Draw(GRA_STAGE5_BOSS_ARMS_ALL, &src, &dst, color, 0.0f);

	}

	//左腕(ライトアーム)---------------------------------------
	//アームタイプが2のときはレフトアームを描画
	if (m_arms_type == LEFT_ARM)
	{
		//切り取り位置
		src.m_top = 0.0f;
		if (m_ani_flag_claw == true)//爪の開閉アニメーションをするなら
			src.m_left = STAGE5_BOSS_ARMS_WIDTH_SIZE*ani_claw[m_ani_frame_claw];
		else
			src.m_left = STAGE5_BOSS_ARMS_WIDTH_SIZE * 1;
		src.m_right = src.m_left + STAGE5_BOSS_ARMS_WIDTH_SIZE;
		src.m_bottom = src.m_top + STAGE5_BOSS_ARMS_HEIGHT_SIZE;

		//描画位置
		dst.m_top = m_py - objmap->GetScrollY();
		dst.m_left =   m_px - objmap->GetScrollX();
		dst.m_right = dst.m_left + STAGE5_BOSS_ARMS_WIDTH_SIZE;
		dst.m_bottom = dst.m_top + STAGE5_BOSS_ARMS_HEIGHT_SIZE;
		//描画
		Draw::Draw(GRA_STAGE5_BOSS_ARMS_ALL, &src, &dst, color, 0.0f);
	}

	//腕を下ろす位置を示すなら示す
	if (m_arm_lower_marker_px > 0.0f)
	{
		//カラー情報
		float marker_color[4] = { 1.0f,0.0f,0.0f,0.2f };

		//腕を下ろすX位置をマップ番号に当てはめたときのX位置
		int map_x = (int)(m_arm_lower_marker_px / BLOCK_SIZE);

		//マーカーを四角形で出すのでその高さ
		float marker_h= WINDOW_SIZE_H;

		//X位置が最大値未満なら
		if (map_x < MAP_X_MAX)
		{
			//Y位置を0〜最大値まで調べる
			for (int map_y = 0; map_y < MAP_Y_MAX; map_y++)
			{
				//ブロックがあるか調べてあるならその位置までを高さとする
				if (objmap->GetMap(map_x, map_y) == MAP_BLOCK)
				{
					marker_h = map_y * BLOCK_SIZE-m_py;
					break;
				}
			}
		}
		//マーカー(四角形)を表示する
		Draw::DrawHitBox(m_arm_lower_marker_px - objmap->GetScrollX(), m_py - objmap->GetScrollY(), marker_h, STAGE5_BOSS_ARMS_WIDTH_SIZE, marker_color);
	}
}

//移動しながら弾を撃つ攻撃
void CObjStage5BossArms::MoveShotAttack()
{
	//HitBox更新用ポインター取得
	CHitBox* hit = Hits::GetHitBox(this);

	//ラストウォールと当たっていれば
	if (hit->CheckObjNameHit(OBJ_LAST_WALL) != nullptr)
	{
		//衝突ふらぐがオフなら
		if (m_wall_hit_flag == false)
		{
			//ボスのオブジェクトを持ってくる
			CObjStage5Boss* objbossbase = (CObjStage5Boss*)Objs::GetObj(OBJ_STAGE5_BOSS);
			if(objbossbase != nullptr)	//ステージ５のボスが存在していたら
				objbossbase->LastWallHit();//ラストウォールと当たった時の処理をする

			m_wall_hit_flag = true;//ふらぐをtrueに
		}
	}
	else
	{
		m_wall_hit_flag = false;//ふらぐをfalseに
	}
}

//拡散弾を打つ攻撃
//引数：爆発までの時間
void CObjStage5BossArms::DiffusionAttack(int limit_time)
{
	//拡散弾の源を作成
	CObjDiffusionSource* p = new CObjDiffusionSource(m_px + STAGE5_BOSS_ARMS_WIDTH_SIZE / 2.0f, m_py + STAGE5_BOSS_ARMS_HEIGHT_SIZE - 10.0f, limit_time);
	Objs::InsertObj(p, OBJ_DIFFUSION_SOURCE, 10);
}

//ブロックを落とす攻撃
//引数1:移動してほしい地点のX値
void CObjStage5BossArms::BlockDownAttackMove(float px)
{
	//HitBoxポインター取得
	CHitBox* hit = Hits::GetHitBox(this);

	if (m_arms_type == RIGHT_ARM)//右腕なら
	{
		//目的地までたどりついてなく　かつ　ラストウォールと当たっていなければ
		if ((px - m_px) > 0.0f && (hit->CheckObjNameHit(OBJ_LAST_WALL) == nullptr))
		{
			m_vx = 1.0f;
			m_right_arm_move = true;//ライトアームが「動いている」判定を出す
		}
		else
		{
			m_vx = 0.0f;//xの移動を0にする
			ArmLowerAttack(m_px);//腕を下ろす処理をする
			
		}
	}
	else //左腕なら
	{
		//目的地までたどりついてなく　かつ　ラストウォールと当たっていなけれ
		if ((px - m_px) < 0.0f && (hit->CheckObjNameHit(OBJ_LAST_WALL) == nullptr))
		{
			m_vx = -1.0f;
			m_left_arm_move = true;
		}
		else
		{
			m_vx = 0.0f;//xの移動を0にする
			ArmLowerAttack(m_px);//腕を下ろす処理をする
			
		}
	}
}

//腕を下ろす攻撃
//引数1	float x:腕を下ろすX位置
void CObjStage5BossArms::ArmLowerAttack(float x)
{
	m_armdown_time++;

	//ボスオブジェクトを持ってくる
	CObjStage5Boss* objboss = (CObjStage5Boss*)Objs::GetObj(OBJ_STAGE5_BOSS);

	//ステージ５のボスが存在していたら
	if (objboss != nullptr)
	{
		if (objboss->GetAttackMode() == 1)//攻撃パターン１なら
		{
			//攻撃が始まる瞬間に腕を下ろすX位置を決める
			if (m_armdown_time == 1)
			{
				m_arm_lower_marker_px = x;
			}
			//120フレームの間に主人公のX位置と同じになるようにベクトルXを調整
			if (m_armdown_time < 120)
			{
				m_vx = (m_arm_lower_marker_px - m_px) / (120 - m_armdown_time);
			}
			//120以上なら腕を下ろす攻撃をするのでX移動量を0.0fにする
			else
			{
				m_vx = 0.0f;
			}
		}
	}

	//時間が120になったら腕を下ろす攻撃をする
	if (m_armdown_time >= 120)
	{
		//腕を下ろす位置を示さない
		m_arm_lower_marker_px = 0.0f;

		//ライトアームまたはレフトアームがＸ軸初期位置から動かず、Ｙ軸初期位置の直下に
		//腕を下ろした場合に「腕が動いている」判定を出す処理---------------------------
		if (m_right_arm_move == false)	//ライトアームが「初期位置から動いていない」判定が出ているとき
		{
			m_right_arm_move = true;	//「ライトアームが動いている」判定を出す
		}
		if(m_left_arm_move == false)	//ライトアームが「初期位置から動いていない」判定が出ているとき
		{
			m_left_arm_move = true;		//「レフトアームが動いている」判定を出す
		}
		//---------------------------------------------------------------------------------

		//ブロックと当たるまで
		if (m_block_hit_flag == false)
		{
			//腕を下ろす
			m_vy = 10.0f;
			
			
		}
	}

	
}

//初期位置を計算する
//初期位置をボス(胴体)の位置を元に求める
void CObjStage5BossArms::UpdateInitial()
{
	//ボス(胴体)の情報を取得
	CObjStage5Boss* objstage5_boss = (CObjStage5Boss*)Objs::GetObj(OBJ_STAGE5_BOSS);
	
	//ステージ５のボスが存在していたら
	if (objstage5_boss != nullptr)
	{
		//ライトアームの時
		if (m_arms_type == RIGHT_ARM)
		{	//										↓ボスの胴体に密着する位置
			m_initial_px = (objstage5_boss->GetPosX() + STAGE5_BOSS_BODY_SIZE) + 60.0f;
			
		}
		//レフトアームの時
		else
		{
			//										↓ボスの胴体に密着する位置
			m_initial_px = (objstage5_boss->GetPosX() - STAGE5_BOSS_ARMS_WIDTH_SIZE) - 60.0f;
		
		}
		m_initial_py = objstage5_boss->GetPosY() - 100.0f;
	}
}