#pragma once

//使用するヘッダー
#include "GameL\SceneManager.h"

//使用するネームスペース
using namespace GameL;

#define RIGHT_ARM 1
#define LEFT_ARM 2

//------------------------------------------

//オブジェクト：第五ステージのボスの腕
class CObjStage5BossArms :public CObj
{
public:
	//ステージ5ボスcppからの描画の都合で、xとyの型をintからfloatに変更しています。(描画で細かい値を使いたいため)
	CObjStage5BossArms(float x, float y ,int type);      //コンストラクタ
	~CObjStage5BossArms() {};	//デストラクタ
	void Init();	        //イニシャライズ
	void Action();	        //アクション
	void Draw();	        //ドロー

	//アクセサーーーーー-------------------------------
	void SetPosX(float x) { m_px = x; }//ポジションXをセットする
	void SetPosY(float y) { m_py = y; }//ポジションYをセットする
	void SetVecX(float vx) { m_vx = vx; }//ベクトルXをセットする
	void SetVecY(float vy) { m_vy = vy; }//ベクトルYをセットする

	float GetPosX() { return m_px; }//ポジションXを渡す
	float GetPosY() { return m_py; }//ポジションYを渡す
	float GetVecX() { return m_vx; }//ベクトルXを渡す
	float GetVecY() { return m_vy; }//ベクトルYを渡す

	void ArmDownTimeInit() { m_armdown_time = 0; }//タイムの初期化
	bool GetHitDown() {	return m_hit_down;	}//下のヒットフラグを返す
	//-------------------------------------------------

	void DiffusionAttack(int limit_time);		//拡散弾を打つ攻撃
	void BlockDownAttackMove(float px);//ブロックを落とす攻撃の移動
	void MoveShotAttack();//移動しながら弾を撃つ攻撃
	void SetPosture(bool posture) { m_input_posture = posture; }//入力姿をセットする　true=開く　false=閉じる
	
	void ArmLowerAttack(float x);	//腕を下ろす攻撃
private:
	float m_px;	 // 第五ボスのアームX座標
	float m_py;	 // 第五ボスのアームY座標
	
	float m_vx;  // 第五ボスのアームX軸移動ベクトル
	float m_vy;  // 第五ボスのアームY軸移動ベクトル

	float m_arm_lower_marker_px;//腕を下ろす位置を示すかどうかとそのX位置　0.0f以下 ＝示さない　0.0fを超える＝示すかつX位置情報

	float m_initial_px;//初期X位置(ボスの横)
	float m_initial_py;//初期Y位置(ボスの横)

	int m_arms_type;	 //第五ボスアームのタイプ(1…ライトアーム  2…レフトアーム)
	int m_armdown_time;//腕を下ろすときの管理用タイム
	int m_arm_hp;    // 第五ボスのアームのＨＰ
	bool m_posture;//姿　true=開いている　false=閉じている
	bool m_input_posture;//入力された姿　true=開いている	false=閉じている

	bool m_ani_flag_claw;//爪の開閉アニメーションフラグ　true=アニメーションをする	false=アニメーションをしない

	bool m_hit_flag;//衝突時の多重判定を防ぐフラグ

	//ブロックとの当たり判定用
	bool m_hit_up;
	bool m_hit_down; 
	bool m_hit_left;
	bool m_hit_right;

	int m_ani_frame_claw;//描画フレーム(爪)
	int m_ani_time_claw;	//アニメーションフレーム動作感覚(爪)
	int m_ani_max_time_claw;//アニメーションフレーム動作感覚最大値(爪)

	void UpdateInitial();//初期位置を計算する
};