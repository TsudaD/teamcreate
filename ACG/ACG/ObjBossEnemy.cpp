#include "GameL\DrawTexture.h"
#include "GameL\SceneManager.h"
#include "GameL\HitBoxManager.h"

#include "GameHead.h"
#include "ObjBossEnemy.h"

//�g�p����l�[���X�y�[�X
using namespace GameL;

//�R���X�g���N�^
CObjBossEnemy::CObjBossEnemy()
{

}

//�C�j�V�����C�Y
void CObjBossEnemy::Init()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_vx = 0.0f;
    m_vy = 0.0f;

    m_hp = 10;//�{�X�̂g�o(���ɂg�o��[ 10 ]�Ɛݒ�)

}

//�A�N�V����
void CObjBossEnemy::Action()
{

}

//�h���[
void CObjBossEnemy::Draw()
{

}