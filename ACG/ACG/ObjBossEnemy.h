#pragma once

//使用するヘッダー
#include "GameL\SceneManager.h"

//使用するネームスペース
using namespace GameL;

//オブジェクト：ボス
class CObjBossEnemy :public CObj
{
public:
    CObjBossEnemy(int x,int y);        //コンストラクタ
    ~CObjBossEnemy() {};	//デストラクタ
    void Init();	        //イニシャライズ
    void Action();	        //アクション
    void Draw();	        //ドロー
private:
    float m_px;	 // ボスX座標
    float m_py;	 // ボスY座標
    float m_vx; // ボスX軸方向のベクトル
    float m_vy; // ボスY軸方向のベクトル

    int m_hp;   // ボスのＨＰ

};
