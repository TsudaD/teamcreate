#pragma once
//使用するヘッダーファイル
#include "GameL\DrawFont.h"
#include "GameL\WinInputs.h"
#include "GameL\SceneManager.h"
#include "GameL\UserData.h"
#include "GameL\Audio.h"

#include "GameHead.h"
#include "ObjGameOver.h"

//使用するネームスペース
using namespace GameL;

//イニシャライズ
void CObjGameOver::Init()
{
	m_key_flag = false;
	m_audio_se = 0;//SEの音楽の管理
	//文字のグラフィック作成
	Font::SetStrTex(L"GAMEOVER（仮）");	//スタート
}

//アクション
void CObjGameOver::Action()
{
	
}

//ドロー
void CObjGameOver::Draw()
{
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };

	//RECT_F src;	//描画先切り取り位置
	//RECT_F dst;	//描画先表示位置
	//メニュー描画
	Font::StrDraw(L"☆ G ☆ A ☆ M ☆ E ☆ O ☆ V ☆ E ☆ R ☆", 0, WINDOW_SIZE_H / 2.0f, 50.0f, color);

	//切り取り位置設定
	//src.m_top = 0.0f;
	//src.m_left = 0.0f;
	//src.m_right = 512.0f;
	//src.m_bottom = 512.0f;

	////表示位置設定
	//dst.m_top = 0.0f;
	//dst.m_left = 0.0f;
	//dst.m_right = 800.0f;
	//dst.m_bottom = 600.0f;

	////描画
	//Draw::Draw(7, &src, &dst, c, 0.0f);
}