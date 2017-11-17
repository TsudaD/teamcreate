#pragma once

//�g�p����w�b�_�[
#include "GameL\SceneObjManager.h"

//�g�p����l�[���X�y�[�X
using namespace GameL;

//�N���X�@��]�u���b�N
class CObjRollBlock : public CObj
{
public:
	CObjRollBlock(int x,int y, int pattan) ;		//�R���X�g���N�^
	~CObjRollBlock() {};	//�f�X�g���N�^

	void Init();	//�C�j�V�����C�Y
	void Action();	//�A�N�V����
	void Draw();	//�h���[
private:
	//�I�u�W�F�N�g�̈ʒu
	float m_px;		
	float m_py;		
	
	//��]�̒��S�ʒu
	float m_center_x;
	float m_center_y;

	//��]��̃|�W�V�����ƒ��S�ʒu�̍���
	float m_difference_x;
	float m_difference_y;

	float m_r;		//��]�p
	int m_count;	//��]�̐���p�J�E���^�[
	int m_pattan;	//�^�C�v(1:��莞�Ԃŉ�],2�F���[�v�ň���������)
	bool m_roll_flag;//��]���邩�ǂ����̃t���O
};