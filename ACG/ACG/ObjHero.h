#pragma once

//使用するヘッダー
#include "GameL\SceneObjManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：主人公
class CObjHero:public CObj
{
public:
	CObjHero(); 	//コンストラクタ
	~CObjHero() {};	//デストラクタ
	void Init();	//イニシャライズ
	void Action();	//アクション
	void Draw();	//ドロー
	void Scroll();	//スクロール処理の関数

private:
	float m_px;	//X座標
	float m_py;	//Y座標
	float m_vx;	//X軸方向のベクトル
	float m_vy;	//Y軸方向のベクトル
	float m_r;//主人公の回転角度
	float m_mouse_angle;	//マウスの角度

	//blockとの衝突確認用
	bool m_hit_up;
	bool m_hit_down;
	bool m_hit_left;
	bool m_hit_right;

	//踏んでいるブロックの種類を確認用
	int m_block_type;
};