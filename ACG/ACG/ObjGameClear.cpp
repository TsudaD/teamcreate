//使用するヘッダーファイル
#include "GameL\DrawFont.h"
#include "GameL\WinInputs.h"
#include "GameL\SceneManager.h"
#include "GameL\UserData.h"
#include "GameL\Audio.h"

#include "ObjGameClear.h"
#include "GameHead.h"

//使用するネームスペース
using namespace GameL;

//イニシャライズ
void CObjGameClear::Init()
{
	
	Audio::Start(CLEAR);
	//文字のグラフィック作成
	Font::SetStrTex(L"CONGRATULATIONS!");
	Font::SetStrTex(L"TITLE:PUSH ENTER KEY");
}

//アクション
void CObjGameClear::Action()
{
	//エンターキーが押された時
	if (Input::GetVKey(VK_RETURN) == true)
	{
		//シーンタイトルに移動
		Scene::SetScene(new CSceneTitle());
		return;
	}
}

//ドロー
void CObjGameClear::Draw()
{
	float back_color [4] = { 1.0f,1.0f,1.0f,1.0f };//背景用カラー
	float text_color [4] = { 0.0f,0.0f,0.0f,1.0f };//文字用カラー(黒)

	RECT_F src;	//描画先切り取り位置
	RECT_F dst;	//描画先表示位置

	//切り取り位置設定
	src.m_top = 0.0f;
	src.m_left = 0.0f;
	src.m_right = 1024.0f;
	src.m_bottom = 1024.0f;

	//表示位置設定
	dst.m_top = 0.0f;
	dst.m_left = 0.0f;
	dst.m_right = 1024.0f;
	dst.m_bottom = 1024.0f;

	//背景描画
	Draw::Draw(GRA_GAME_CLEAR, &src, &dst, back_color, 0.0f);

	//文字描画                                    x                       y              textsize
	Font::StrDraw(L"CONGRATULATIONS!"    , WINDOW_SIZE_W - 844.0f, WINDOW_SIZE_H - 716.0f, FONT_SIZE_GC, text_color);//黒色
	Font::StrDraw(L"TITLE:PUSH ENTERKEY" , WINDOW_SIZE_W - 444.0f, WINDOW_SIZE_H - 58.0f, FONT_SIZE_GC - 41.0f, text_color);

}