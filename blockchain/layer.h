#pragma once

//include
#include "include.h"
#include "crypt_hash.h"
//define
#define DEVICE_LENGTH 100 //Ԥ���豸�����
#define QUEUE_LENGTH 100 //��Ϣ���г���
#define TRANSACTION_LENGTH 100 //transaction����(������)
#define TANGLE_LENGTH 1000 //tangle����(������)

#define QUEUE_LEN 80 //��Ϣ������Ϣ����
#define KEY_LEN 4 //��Կ���ֽ���

#define INFO_TX 0
#define INFO_RX 1
#define INFO_KEY 2
#define INFO_TRANSACTION 3
#define INFO_TANGLE 4
#define INFO_VALID_TRANSACTION 5
#define INFO_VALID_LEDGER 6

#define TASK_NONE 0
#define TASK_DEVICE_INITIAL 1
#define TASK_DEVICE_CONNECT 2
#define TASK_DEVICE_MERGE 3
#define TASK_DEVICE_OPTIMIZE 4
#define TASK_DEVICE_INDEXDAG 5
#define TASK_DEVICE_WALK 6
#define TASK_DAG_INITIAL 7
#define TASK_DAG_TANGLE 8
//typedef
//struct
struct route_t
{
	uint8 flag;//0-δ����,1-����
	uint32 device_index;//���豸����(������Ψһ�����ַ)
	uint32 hops;//��Ծ���
	uint32 *path;//·��·��
	route_t *next;
};
struct queue_t
{
	volatile uint32 device_index;//�豸����(������Ψһ�����ַ)
	volatile uint32 info;//��Ϣ����.0-INFO_TX,1-INFO_RX,2-INFO_TRANSACTION,3-INFO_TANGLE,4-INFO_VALID_TRANSACTION,5-INFO_VALID_LEDGER
	volatile uint8 buffer[QUEUE_LEN];//������Ϣ
};
struct key_t
{
	uint32 device_index;//�豸����(������Ψһ�����ַ)
	uint8 buffer[65*4];//��Կrsa=1(e)+64(n)=65 word
};
struct transaction_t
{
	volatile uint32 device_index;//�豸����(������Ψһ�����ַ)
	hash_t index;//��������
	uint32 weight_self;//����Ȩ��
	uint32 weight_accu;//�ۻ�Ȩ��
	uint32 height;//�߶�(��������)
	uint32 depth;//���(����Զtip)
	uint32 integral;//���׻���
	uint32 nonce;//��ʱ�����(֤���Ǵӵ�ǰdevice����,��ֹŮ�׹������������׹���)
	uint8 type;//��������.0-��ͨ��Ϣ,1-�м���Ϣ
	uint8 plain[KEY_LEN];//������֤
	uint8 cipher[KEY_LEN];//������֤
	uint32 pow[2];//���������õ���ǰ��trunk/branch��powֵ
	//hash_t address;//��ַ
	//hash_t trunk;//������
	//hash_t branch;//�ӽ���
	//hash_t bundle;//��
	//hash_t tag;//��ǩ
	volatile uint8 flag;//����״̬.0-none,1-solid,2-tangle,3-milestone
	hash_t trunk;//�����׽ڵ�->��������
	hash_t branch;//�ӽ��׽ڵ�->��������
};
struct device_t
{
	//device
	uint32 x;
	uint32 y;
	uint8 node;//0-�ؽڵ�(����ȫ�˱�),1-��ڵ�(���˱�)
	uint8 line;//0-����,1-����
	uint32 device_index;//�豸����(������Ψһ�����ַ)
	uint8 status;//0-��Ϊfree,1-��Ϊmaster,2-��Ϊslave
	route_t *route;//�����豸·������
	queue_t queue[QUEUE_LENGTH];//�豸����Ϣ����(����/����)
	uint32 queue_index;//queue��������
	//dag
	volatile uint32 dag_index;//��������.0-������,����-������
	transaction_t tangle[TANGLE_LENGTH];//��tangle����
	volatile uint32 tangle_index;//��tangle��������
	transaction_t transaction[TRANSACTION_LENGTH];//transaction����
	volatile uint32 transaction_index;//transaction��������
	key_t key[DEVICE_LENGTH];//key��Կ����
	volatile uint32 key_index;//key��������
	uint8 pair[300*4];//��Կ��rsa=1(e)+64(n)+64(d)+32(p)+32(q)+32(dp)+32(dq)+32(qp)=289 word
	//application
	uint32 account_id;//�˻�id
	uint32 account_money;//�˻��ֽ�
};
//function
