#pragma once

//include
#include "include.h"
#include "crypt_rsa.h"
//#include "crypt_hash.h"
//define
#define QUEUE_LENGTH 0x20

#define NODE_HEAVY 0
#define NODE_LIGHT 1

#define STEP_CONNECT 0
#define STEP_TANGLE 1

#define KEY_LEN 4 //��Կ���ֽ���
#define KEY_E 4 //��Կe�ֽ���
#define KEY_MASK 0 //ָ��ä���ֽ���

#define TIMER_CONNECT 1 //��������ʱ��(�ؽڵ��������)
//typedef
//struct
struct index_t
{
	uint32 number;//������������Ŀ
	uint32 *index;//�����������б�
	uint8 *key;//������������Կ
	uint8 *node;//0-�ؽڵ�,1-��ڵ�
};
struct key_t
{
	uint8 e[KEY_E];//��Կ
	uint8 n[KEY_LEN];//ģ��
};
struct list_t
{
	uint32 dag_index;//��������
	uint32 device_index;//�豸����
	key_t key;//��Կ
	uint8 node;//0-�ؽڵ�,1-��ڵ�
};
struct route_t
{
	uint8 flag;//0-δ����,1-����
	uint32 device_index;//���豸����(������Ψһ�����ַ)
	//uint32 hops;//��Ծ���
	//uint32 *path;//·��·��
	key_t key;//��Կ
	uint8 node;//0-�ؽڵ�,1-��ڵ�
	route_t *next;
};
struct queue_t
{
	volatile uint8 step;
	uint8 *data;
	queue_t *next;
};
struct mainchain_t
{
	queue_t *queue;//��Ϣ����
	uint32 dag_number;//������Ŀ
	uint32 list_number;//�ڵ���Ŀ
	list_t *list;//�ڵ������б�
};
struct device_t
{
	uint32 x;
	uint32 y;
	uint8 node;//0-�ؽڵ�(�����˱�),1-��ڵ�(���˱�)
	uint32 device_index;//�豸����(������Ψһ�����ַ)
	route_t *route;//�����豸·������
	queue_t *queue;//��Ϣ����
	rsa_t rsa;//��ǰ�豸�Ĺ�˽Կ��
	//uint8 line;//0-����,1-����
	//volatile uint8 step;
	//uint8 status;//0-��Ϊfree,1-��Ϊmaster,2-��Ϊslave
	
/*
	//index_t *index;//�豸��������
	//dag
	volatile uint32 dag_index;//��������.0-������,����-������
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
void route_insert(device_t *device,route_t *route);
void route_delete(device_t *device);
void queue_insert(device_t *device,queue_t *queue);
void queue_insert(mainchain_t *mainchain,queue_t *queue);
void queue_delete(device_t *device);
void list_delete(mainchain_t *mainchain);
void key_generate(device_t *device);
