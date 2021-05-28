
/**
 * @file msgqueue.h
 * @brief ϵͳ��Ϣ����������� SYSTEM MESSAGE �ķ�װ��
 * @author tomszhou
 * @version 1.1
 * @date 2008-3-19
 */

#ifndef _TTSYS_MESSAGE_QUEUE_H_
#define _TTSYS_MESSAGE_QUEUE_H_


__BEGIN_DECLS

typedef unsigned int  uint32;
#define TTS_SUCCESS 0
#define TTS_EINVAL -1
#define TTS_TIMEOUT -2


/**
 * @defgroup TTSYS_MSGQUEUE ��Ϣ���
 * @brief ϵͳ��Ϣ����������� SYSTEM MESSAGE �ķ�װ��
 * @author tomszhou
 * @version 1.1
 * @date 2008-3-19
 * @ingroup TTSYS 
 * @{
 */


/**
 * TMessageQueue ��Ϣ���������
 */

typedef struct TMessageQueue  TMessageQueue;
/**
 * TMessageQueue ��Ϣ����ṹ����
 */

struct TMessageQueue
{
#define  MESSAGE_QUEUE_NAME_MAX_LEN  32               /**< ��Ϣ���Ƶ���󳤶�  */
  char    name[MESSAGE_QUEUE_NAME_MAX_LEN];           /**< ��Ϣ����     */
  uint32  key;                                        /**< ��Ϣ���м�ֵ */
  int     qid;                                        /**< ��Ϣ����ID   */
  int     flag;                                       /**< ��Ϣ������ʽ */
};

/**
 * TMessageQueueBuffer ��Ϣ����
 */

typedef struct TMessageQueueBuffer   TMessageQueueBuffer;

/**
 * TMessageQueueBuffer ��Ϣ�ṹ����
 */
struct TMessageQueueBuffer 
{
  long   mtype;        /**< message type, must be > 0 */
  char   mtext[1];     /**< message data */
};


/** ��������
 *  @param queue ������Ķ��ж���
 *  @param key  ����ID��
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueCreate(TMessageQueue *queue,uint32 key);

/** �������ƴ�������
 *  @param queue ������Ķ��ж���
 *  @param name  ��������
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueCreateEx(TMessageQueue *queue,const char *name);

/** ���ٶ���
 *  @param key  ����KEY��
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueDestory(uint32 key);

/** ���ٶ���
 *  @param qid  ����ID��
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueDestroyByID(uint32 qid);

/** �����������ٶ���
 *  @param name  ��������
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueDestoryEx(const char *name);

/** �򿪶���
 *  @param queue �򿪺�Ķ��ж���
 *  @param key  ����ID��
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueOpen(TMessageQueue *queue,uint32 key);

/** ����ID�򿪶���
 *  @param queue �򿪺�Ķ��ж���
 *  @param qid  ����ID��
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int  MessageQueueOpenByID(TMessageQueue *queue,uint32 qid);

/** �������ƴ򿪶���
 *  @param queue �򿪺�Ķ��ж���
 *  @param name  ��������
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueOpenEx(TMessageQueue *queue,const char *name);

/** ����з�������
 *  @param queue ���Ӧ�Ķ���
 *  @param buffer  ����
 *  @param size ���ݴ�С
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueSend(TMessageQueue *queue,TMessageQueueBuffer *buffer,uint32 size);

/** �Ӷ��н�������
 *  @param queue ���Ӧ�Ķ���
 *  @param buffer  ����
 *  @param size ���ݴ�С
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueRecv(TMessageQueue *queue,TMessageQueueBuffer *buffer,uint32 size);

/** �Ӷ��н�������һֱ����ʱʱ��
 *  @param queue ���Ӧ�Ķ���
 *  @param buffer  ����
 *  @param size ���ݴ�С
 *  @param times ��ʱʱ��
 *  @return �ɹ�����TTS_SUCCESS��ʧ�ܷ������Ӧ�Ĵ�����롣
 */
int MessageQueueRecvEx(TMessageQueue *queue,TMessageQueueBuffer *buffer,uint32 size,int times);


/** @} */

__END_DECLS

#endif



