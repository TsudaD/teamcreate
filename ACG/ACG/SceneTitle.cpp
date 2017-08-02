//STLデバック機能をOFFにする
#define _SECURE_SCL (0)
#define _HAS_ITERATOR_DEBUGGING (0)

//GameLで使用するヘッダー
#include "GameL\SceneObjManager.h"
#include "GameL\DrawFont.h"
#include "GameL\UserData.h"
#include "GameL\Audio.h"


//使用するネームスペース
using namespace GameL;

//使用ヘッダー
#include "SceneTitle.h"
#include "GameHead.h"

//初期化メソッド
void InitScene()
{
}

//実行中メソッド
void Scene()
{
	//オブジェクトを作成する
	CObjTitle* ObjTitle = new CObjTitle();
	Objs::InsertObj(ObjTitle, OBJ_TITLE, 10);
}

//画像データ読み込み関数
void InageDataLoading()
{
}

//音楽データ読み込み関数
void AudioDataLoading()
{
}

