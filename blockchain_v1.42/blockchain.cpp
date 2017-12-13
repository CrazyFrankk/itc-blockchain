#include "blockchain.h"

////global var
//device
uint32 g_devicenum;//�豸����
uint32 g_devicerange;//�豸���귶Χ
uint32 g_devicestep;//�豸����ֵ
//dag
//common
CRITICAL_SECTION g_cs;
device_t *g_device,g_mainchain;//�豸����
volatile uint32 g_index;//��ʱ����ͳ�ƽ��׺����(�Ժ����hash_t����,������1��ʼ)
volatile uint8 g_task;//���ݸ��̵߳Ĺ��̱��
volatile uint8 *g_init;//ÿ���̵߳ĳ�ʼ������.0-δ��ʼ��,1-�ѳ�ʼ��
volatile uint8 *g_flag;//ÿ���̵߳ĵ�ǰ������
CString g_string[9]={"none","device_initial","device_connect","device_merge","device_optimize","device_indexdag","device_walk","dag_initial","dag_tangle"};

//�����߳�
uint32 WINAPI thread_mainchain(PVOID pParam)
{
	device_t *device;

	device=(device_t *)pParam;
	while(1)
	{
		process_mainchain(device);
		EnterCriticalSection(&g_cs);
		//printf("initial thread_mainchain pass\r\n");
		LeaveCriticalSection(&g_cs);
	}

	return 0;
}

//�豸�߳�
uint32 WINAPI thread_device(PVOID pParam)
{
	device_t *device;
	uint8 flag;

	device=(device_t *)pParam;
	while(1)
	{
		if (g_task==g_flag[device->device_index])
		{
			flag=g_task<=TASK_DEVICE_WALK ? process_device(device) : process_dag(device);
			EnterCriticalSection(&g_cs);
			//printf("%s thread_device%ld pass\r\n",g_string[g_task],device->device_index);
			g_flag[device->device_index]=TASK_NONE;
			LeaveCriticalSection(&g_cs);
		}
	}

	return 0;
}

void main(int argc,char* argv[])
{
	uint32 interval;//Timerˢ�¼��(��)
	uint32 account_num;//�˻�����
	uint32 money;//��ʼ���ʽ�
	//
	uint32 i;
	int8 buf[1000];
	FILE *file;
	MSG msg;
	HANDLE thread_handle;
	uint32 thread_id;

	//read initial.ini
	file=fopen("initial.ini","r");
	fgets(buf,1000,file);//[system]
	fgets(buf,1000,file);
	buf[strlen(buf)-1]=0;
	interval=atol(&buf[9]);//interval
	fgets(buf,1000,file);//[network]
	fgets(buf,1000,file);
	buf[strlen(buf)-1]=0;
	g_devicenum=atol(&buf[11]);//g_devicenum
	fgets(buf,1000,file);
	buf[strlen(buf)-1]=0;
	g_devicerange=atol(&buf[13]);//g_devicerange
	fgets(buf,1000,file);
	buf[strlen(buf)-1]=0;
	g_devicestep=atol(&buf[12]);//g_devicestep
	fgets(buf,1000,file);//[blockchain]
	fgets(buf,1000,file);
	buf[strlen(buf)-1]=0;
	account_num=atol(&buf[12]);//account_num
	fgets(buf,1000,file);
	buf[strlen(buf)-1]=0;
	money=atol(&buf[6]);//money
	fclose(file);
	//0.initial device/timer/thread_device/thread_mainchain/cs
	InitializeCriticalSection(&g_cs);
	srand((unsigned)time(NULL));
	g_index=0;
	g_task=TASK_DEVICE_INITIAL;
	g_init=new uint8[g_devicenum];
	memset((void *)g_init,0,g_devicenum*sizeof(uint8));
	g_flag=new uint8[g_devicenum];
	memset((void *)g_flag,TASK_DEVICE_INITIAL,g_devicenum*sizeof(uint8));
	g_device=new device_t[g_devicenum];
	for (i=0;i<g_devicenum;i++)
	{
		g_device[i].x=rand()%g_devicerange;
		g_device[i].y=rand()%g_devicerange;
		g_device[i].node=DEVICE_NODE_HEAVY;//rand()%2 ? DEVICE_NODE_LIGHT : DEVICE_NODE_HEAVY;
		g_device[i].line=DEVICE_LINE_ON;
		g_device[i].device_index=i;
		g_device[i].status=DEVICE_STATUS_FREE;
		g_device[i].route=NULL;
		memset((void *)g_device[i].queue,0,QUEUE_LENGTH*sizeof(queue_t));//INFO_TX
		g_device[i].queue_index=0;
		g_device[i].dag_index=0;
		g_device[i].tangle_index=0;
		g_device[i].transaction_index=0;
		g_device[i].key_index=0;
		g_device[i].account_id=rand()%account_num;
		g_device[i].account_money=money;
		//lpThreadAttributes:ָ��security attributes
		//dwStackSize:ջ��С
		//lpStartAddress:�̺߳�����������ʽ����uint32 WINAPI xxx(PVOID pParam)
		//lpParameter:������
		//dwCreationFlags:�߳�״̬(suspend,running)
		//lpThreadId:�����߳�id
		thread_handle=CreateThread(NULL,0,thread_device,(PVOID)&g_device[i],0,&thread_id);
		if (!thread_handle)
		{
			printf("initial thread_device failed\r\n");
			return;
		}
	}
	thread_handle=CreateThread(NULL,0,thread_mainchain,(PVOID)&g_mainchain,0,&thread_id);
	if (!thread_handle)
	{
		printf("initial thread_mainchain failed\r\n");
		return;
	}
	SetTimer(NULL,1,interval*1000,NULL);
	//msg loop
	while(1)
	{
		//get message from thread's message queue
		GetMessage(&msg,NULL,0,0);
		//
		switch(g_task)
		{
		case TASK_DEVICE_INITIAL://0.initial device
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			g_task=TASK_DEVICE_CONNECT;
			memset((void *)g_flag,TASK_DEVICE_CONNECT,g_devicenum*sizeof(uint8));
			break;
		case TASK_DEVICE_CONNECT://1.connect device
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			//print_status();
			//print_route();
			g_task=TASK_DEVICE_MERGE;
			memset((void *)g_init,0,g_devicenum*sizeof(uint8));
			memset((void *)g_flag,TASK_DEVICE_MERGE,g_devicenum*sizeof(uint8));
			for (i=0;i<g_devicenum;i++)
				memset((void *)g_device[i].queue,0,QUEUE_LENGTH*sizeof(queue_t));
			break;
		case TASK_DEVICE_MERGE://2.merge route
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			//print_status();
			//print_route();
			//while(1);
			g_task=TASK_DEVICE_OPTIMIZE;
			memset((void *)g_init,0,g_devicenum*sizeof(uint8));
			memset((void *)g_flag,TASK_DEVICE_OPTIMIZE,g_devicenum*sizeof(uint8));
			for (i=0;i<g_devicenum;i++)
				memset((void *)g_device[i].queue,0,QUEUE_LENGTH*sizeof(queue_t));
			break;
		case TASK_DEVICE_OPTIMIZE://3.optimize route
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			g_task=TASK_DEVICE_INDEXDAG;
			memset((void *)g_flag,TASK_DEVICE_INDEXDAG,g_devicenum*sizeof(uint8));
			break;
		case TASK_DEVICE_INDEXDAG://4.index dag
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			//print_status();
			//print_route();
			//while(1);
			g_task=TASK_DAG_INITIAL;
			memset((void *)g_init,0,g_devicenum*sizeof(uint8));
			memset((void *)g_flag,TASK_DAG_INITIAL,g_devicenum*sizeof(uint8));
			for (i=0;i<g_devicenum;i++)
				memset((void *)g_device[i].queue,0,QUEUE_LENGTH*sizeof(queue_t));
			break;
		case TASK_DAG_INITIAL://5.dag initial
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			//while(1);
			g_task=TASK_DAG_TANGLE;
			memset((void *)g_init,0,g_devicenum*sizeof(uint8));
			memset((void *)g_flag,TASK_DAG_TANGLE,g_devicenum*sizeof(uint8));
			break;
		case TASK_DAG_TANGLE://6.dag tangle
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			while(1);
			g_task=TASK_DEVICE_WALK;
			memset((void *)g_init,0,g_devicenum*sizeof(uint8));
			memset((void *)g_flag,TASK_DEVICE_WALK,g_devicenum*sizeof(uint8));
			for (i=0;i<g_devicenum;i++)
				memset((void *)g_device[i].queue,0,QUEUE_LENGTH*sizeof(queue_t));
			break;
		case TASK_DEVICE_WALK://7.device walk & reconstruct
			while(1)
			{
				for (i=0;i<g_devicenum;i++)
					if (g_flag[i]!=TASK_NONE)
						break;
				if (i==g_devicenum)
					break;
			}
			g_task=TASK_DEVICE_INITIAL;
			memset((void *)g_init,0,g_devicenum*sizeof(uint8));
			memset((void *)g_flag,TASK_DEVICE_INITIAL,g_devicenum*sizeof(uint8));
			break;
		}
	}
	//release
	for (i=0;i<g_devicenum;i++)
		device_release(&g_device[i]);
	delete[] (uint8 *)g_init;
	delete[] (uint8 *)g_flag;
	delete[] g_device;
	DeleteCriticalSection(&g_cs);
}