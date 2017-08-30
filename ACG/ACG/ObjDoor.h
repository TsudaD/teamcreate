#pragma once
//使用するヘッダー
#include "GameL\DrawTexture.h"
#include "GameL\SceneObjManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：リフト
class CObjDoor :public CObj
{
public:
	CObjDoor(int x, int y); // コンストラクタ
	~CObjDoor() {};	// デストラクタ
	void Init();	// イニシャライズ
	void Action();	// アクション
	void Draw();	// ドロー

private:
	float m_px;	// リフトX座標
	float m_py; // リフトY座標
};
