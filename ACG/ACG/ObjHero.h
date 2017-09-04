#pragma once

//使用するヘッダー
#include "GameL\SceneObjManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：主人公
class CObjHero:public CObj
{
public:
	CObjHero(int x, int y); 	//コンストラクタ
	~CObjHero() {};	//デストラクタ
	void Init();	//イニシャライズ
	void Action();	//アクション
	void Draw();	//ドロー
	void Scroll();	//スクロール処理の関数
	void BlockHit();//ブロックとの当たり判定を処理する関数

	//アクセサ------------------------------------------
	void SetVecX(float x) { m_vx = x; };	//ベクトルXをセットする
	void SetVecY(float y) { m_vy = y; };	//ベクトルYをセットする
	void SetPosX(float x) { m_px = x; };	//ポジションXをセットする
	void SetPosY(float y) { m_py = y; };	//ポジションYをセットする
	float GetPosX() { return m_px; }//ポジションXを返す
	float GetPosY() { return m_py; }//ポジションXを返す

	bool bu;//ブロックの上にのっている時の情報を持ってくる奴

private:
	float m_px;	//X座標
	float m_py;	//Y座標
	float m_vx;	//X軸方向のベクトル
	float m_vy;	//Y軸方向のベクトル
	float m_posture;//姿勢
	float m_r;//主人公の回転角度

	bool  m_f;  //弾丸発射制御用
	bool  m_rf; //ロープ発射制御用
	bool  m_jf;  //ジャンプ制御

	//アニメーション関係
	int m_ani_frame_stop; //止まった時の描画を調整
	int m_ani_time; //アニメーションフレーム動作感覚
	int m_ani_frame;//描画フレーム
	float m_ani_max_time;   //アニメーション動作間隔最大値

	//blockとの衝突確認用
	bool m_hit_up;
	bool m_hit_down;
	bool m_hit_left;
	bool m_hit_right;

	////踏んでいるブロックの種類を確認用
	//int m_block_type;
};