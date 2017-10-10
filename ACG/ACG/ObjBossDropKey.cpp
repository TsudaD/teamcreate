//�g�p����w�b�_�[�t�@�C���@
#include "GameL\HitBoxManager.h"
#include "GameHead.h"
#include "ObjBossDropKey.h"
#include "Function.h"

//�g�p����l�[���X�y�[�X
using namespace GameL;

//�R���X�g���N�^
CObjBossDropKey::CObjBossDropKey(int x, int y)
{
	m_px = x * BOSS_DORP_KEY_SIZE;
	m_py = y * BOSS_DORP_KEY_SIZE;
}

//�C�j�V�����C�Y
void CObjBossDropKey::Init()
{
	//�����蔻��
	Hits::SetHitBox(this, m_px, m_py, BOSS_DORP_KEY_SIZE, BOSS_DORP_KEY_SIZE, ELEMETN_ITEM, OBJ_BOSS_DROP_KEY, 1);
}

//�A�N�V����
void CObjBossDropKey::Action()
{
	//HitBox�̈ʒu���X�V����
	HitBoxUpData(Hits::GetHitBox(this), m_px, m_py);
}

//�h���[
void CObjBossDropKey::Draw()
{

}