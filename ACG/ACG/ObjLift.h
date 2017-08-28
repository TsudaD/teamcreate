#pragma once
//使用するヘッダー
#include "GameL\DrawTexture.h"
#include "GameL\SceneObjManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：リフト
class CObjLift :public CObj
{
public:
	CObjLift(float x, float y); // コンストラクタ
	~CObjLift() {};	// デストラクタ
	void Init();	// イニシャライズ
	void Action();	// アクション
	void Draw();	// ドロー

private:
	float m_lift_x;	// リフトX座標
	float m_lift_y; // リフトY座標
};
