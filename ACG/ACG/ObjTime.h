#pragma once
//使用するヘッダー
#include "GameL\DrawTexture.h"
#include "GameL\SceneObjManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：タイム
class CObjTime :public CObj
{
public:
	CObjTime() {}; // コンストラクタ
	~CObjTime() {};			// デストラクタ
	void Init();			// イニシャライズ
	void Action();			// アクション
	void Draw();			// ドロー

private:
	float m_px;		//位置X
	float m_py;		//位置Y
	int m_time;		//時間(フレーム数)
};
