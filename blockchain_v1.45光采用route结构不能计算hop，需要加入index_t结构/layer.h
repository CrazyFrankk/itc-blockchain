#pragma once

//include
#include "include.h"
//#include "crypt_hash.h"
//define
#define BUFFER_LENGTH 0x100

#define NODE_HEAVY 0
#define NODE_LIGHT 1

#define STATUS_FREE 0
#define STATUS_MASTER 1
#define STATUS_SLAVE 2

#define TIMER_CONNECT 1 //��������ʱ��(�ؽڵ��������)
//enum
typedef enum step
{
	STEP_INITIAL,STEP_CONNECT,STEP_MERGE,STEP_OPTIMIZE,STEP_INDEXDAG,STEP_TANGLE
};
//typedef
//struct
struct msg_t
{
	uint8 type;

};
struct route_t
{
	uint8 flag;//0-δ����,1-����
	uint32 device_index;//���豸����(������Ψһ�����ַ)
	uint32 hops;//��Ծ���
	uint32 *path;//·��·��
	route_t *next;
};
struct index_t
{
	uint32 *index;//�豸�����б�
	uint32 number;//�豸������Ŀ
	index_t *next;
};
struct device_t
{
	//device
	uint32 x;
	uint32 y;
	uint8 node;//0-����/�ؽڵ�(����ȫ���˱��������˱�),1-��ڵ�(���˱�)
	//uint8 line;//0-����,1-����
	uint32 device_index;//�豸����(������Ψһ�����ַ)
	volatile uint8 step;
	uint8 buffer[BUFFER_LENGTH];//���ݽ��ջ�����




	uint8 status;//0-��Ϊfree,1-��Ϊmaster,2-��Ϊslave
	route_t *route;//�����豸·������

	//index_t *index;//�豸��������
	//dag
	volatile uint32 dag_index;//��������.0-������,����-������


	/*

	uint8 status;//0-��Ϊfree,1-��Ϊmaster,2-��Ϊslave
	route_t *route;//�����豸·������
	//queue_t queue[QUEUE_LENGTH];//�豸����Ϣ����(����/����)
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
	*/
};
//function
