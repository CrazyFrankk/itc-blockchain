#pragma once

//include
#include "layer.h"
#include "crypt_rsa.h"
#include "crypt_sha256.h"
//define
#define TRANSACTION_TYPE_NORMAL 0 //��ͨ����
#define TRANSACTION_TYPE_VALUE 1 //�м۽���(������ǩ��)

#define TRANSACTION_NONE 0 //δ������֤transaction
#define TRANSACTION_TIP 1 //������tangle��Ϊtip
#define TRANSACTION_TANGLE 2 //������tangle������������ָ��
#define TRANSACTION_SOLID 3 //�ѽ�����֤transaction
#define TRANSACTION_MILESTONE 4 //���˱���֤milestone

#define RET_QUEUE_EMPTY 1
#define RET_QUEUE_FULL 2
#define RET_TRANSACTION_EMPTY 3
#define RET_TRANSACTION_FULL 4
#define RET_TRANSACTION_NONE 5
#define RET_TANGLE_FULL 6
#define RET_TANGLE_NONE 7

#define MASK_LEN 0 //ָ��ä���ֽ���
#define HASH_LEN 32 //��ϣ�ֽ���
#define COMPARE_LEN 4 //�Ƚ�λ��
#define CACHE_LEN 10 //cache�ڵ����
//typedef
//struct
//function
uint8 process_dag(device_t *device);