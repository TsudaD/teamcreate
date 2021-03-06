#pragma once

//使用するヘッダー
#include "GameL\SceneObjManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：LastWall(ボス戦の柱）
class CObjLastWall :public CObj
{
	public:
		CObjLastWall(int x, int y); 	//コンストラクタ
		~CObjLastWall() {};			//デストラクタ
		void Init();				//イニシャライズ
		void Action();				//アクション
		void Draw();				//ドロー
		
		//アクセサ
		float GetPosX() { return m_px; }
	private:
		float m_px;	//X座標
		float m_py;	//Y座標

		//マップ上の位置（要素数）
		int m_map_x;//X
		int m_map_y;//Y
		int m_wall_gauge;//壁の高さ管理
		int m_wall_down_gauge;//壁をおろすときの管理用
		bool m_wall_unlock_flag;//ボタンがおされたかのフラグ
		bool m_hero_hit_flag;   //壁の左側にheroが当たったときのフラグ
		bool m_wall_down_flag;  //壁が下に下がるフラグ
};