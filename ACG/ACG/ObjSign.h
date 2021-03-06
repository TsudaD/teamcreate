#pragma once
//使用するヘッダー
#include "GameL\SceneObjManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：看板
class CObjSign :public CObj
{
public:
	CObjSign(int x, int y); // コンストラクタ
	~CObjSign() {};			// デストラクタ
	void Init();			// イニシャライズ
	void Action();			// アクション
	void Draw();			// ドロー

private:
	float m_px;		//位置X
	float m_py;		//位置Y
	bool m_strdrow;	//文字を表示させるかのフラグ

	//マップ上の位置（要素数）
	int m_map_x;//X
	int m_map_y;//Y
};
