#pragma once

//使用するヘッダー
#include "GameL\SceneManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト:マップ
class CObjMap :public CObj
{
public:
	CObjMap(int map[MAP_Y_MAX][MAP_X_MAX]); 	//コンストラクタ
	~CObjMap() {};	//デストラクタ
	void Init();	//イニシャライズ
	void Action();	//アクション
	void Draw();	//ドロー

	//スクロール関連--------------------------------------------------
	float GetScrollX() { return m_scroll_x; }		//X方向のスクロール値を返す関数
	float GetScrollY() { return m_scroll_y; }		//Y方向のスクロール値を返す関数
	void SetScrollX(float x) { m_scroll_x += x; }	//X方向のスクロール値をセットする関数
	void SetScrollY(float y) { m_scroll_y += y; }	//X方向のスクロール値をセットする関数
	
	//マップをもとにしたオブジェクトの生成関連----------------------------------
	void CreateObj(int x, int y);//オブジェクトの生成を行う
	void ScrollCreateObj(int scroll_block_num_x, int scroll_block_num_y);//スクロール量をもとにオブジェクトの生成を行う
	
private:
	float m_scroll_x;//X軸のスクロール値
	float m_scroll_y;//Y軸のスクロール値

	int m_map[MAP_Y_MAX][MAP_X_MAX];//マップ用配列	←マップができたらこの配列に入れる
};