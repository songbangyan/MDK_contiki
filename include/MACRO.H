#ifndef __MACRO_H__
#define __MACRO_H__

/*���ļ����峣�õĺ�*/
#define UART_FRAME_MAX_LENGTH          (240)

//#define printf(...)
#define PRODUCT_SN0                   'G'
#define PRODUCT_SN1                   'B'
#define CHIP_SN0                      'G'
#define CHIP_SN1                      'B'

#define TEST_MODE                       0
#define MODE_SHORTADDR_SUPPORT          (0X80000000)//�����ݴ���ʱ�̵�ַģʽ֧��
#define MODE_LOCK_TO_ZERO_CHANNEL       (1<<30)//�����ŵ����������ŵ�
#define MODE_ROUTE_UPDATE               (1<<29)//·���Զ�����ά������
#define DEFAULT_SHORTADDR_MODE          (!MODE_SHORTADDR_SUPPORT)
#define DEFAULT_ZEROCHANNEL_MODE        (!MODE_LOCK_TO_ZERO_CHANNEL)
#define DEFAULT_ROUTEUPDATE_MODE        (MODE_ROUTE_UPDATE)

#define MAGIC_KEY                     (0X0951FEBC)
//�ɵ�������
#define REGET_PATH_VALUE               (125)//��������·���ķ�ֵ
#define HCLK                           (72000000)
#define SYSTICK_NUM                    (HCLK/8/1000)//1msһ���ж�
#define MAX_NEIGHBOUR_NODE_NUM        (80)//����ͨѶ�ڵ������ھӱ�����
#define MAX_NEIGHBOUR_NUM_ONE_TIME    (16)//���������ھӽڵ���������
#define TRACE_MAX_NODE_NUM             (MAX_RELAY_NUM+1)//���·�����
#define MAX_PATH_NUM                    (3)//����·�ɼ�¼����¼��·����Ŀ
#define MAX_RELAY_NUM                   (7)//·�ɼ�¼�б���·��������м���Ŀ
#define MT_RSSI_VAL                     (90)//ά������ʹ�õĳ�ǿ������ֵ
#define MT_FAIL_TO_UNCONFIG_NUM         (1)//����ά������������Ϣ��յ�ά��������

#define MAX_NUM_SINGLE_REPLY          (28)//����Ӧ��ӽڵ���Ϣ�������
#define VER_NUM                         (9)
#define MCU_ID_LEN                      (12)
#define COM_SUC_PAR                     (1)//ͨ�ųɹ��ۼ�Ʒ�ʲ���
#define COM_FAIL_PAR                    (3)//ͨ��ʧ���µ�Ʒ�ʲ���



#if !defined (V10) 
 #error "please define the hardware version!"
#endif
#define BOOTLOADER_VER                  (0x10)
#ifdef V10
#define HARDWARE_VER                    (0x0010)//
#define FLASH_SIZE                      (2*1024*1024Byte)
#define PROM_SIZE                     (2*1024Byte)
#define SRAM_SIZE                     (512*16bit)

#endif




//���ܺ�
#define MEMBER_OFFSET(struc, member) ((unsigned int)(&(((struc*)0)->member)))//����ṹ���Ա����ڽṹ�����ַ��ƫ��
#define MEMBER_SIZE(struc,member)     sizeof(((struc*)0)->member)//����ṹ���Ա�ռ�
#define STRUCT_SIZE(struc)             (sizeof(struc))//����ṹ��ߴ�

#define NODE1_SIZE                      STRUCT_SIZE(node_t)//�ڵ�洢�ռ��С
#define NODE_SIZE                      STRUCT_SIZE(subnode_t)//�ڵ�洢�ռ��С
#define NODE_NETADDR_SHIFT             MEMBER_OFFSET(subnode_t,net_addr)//�ڵ������ַ�洢ƫ��
#define NODE_NETADDR_SIZE              MEMBER_SIZE(subnode_t,net_addr)
#define NODE_FLG_SHIFT                 MEMBER_OFFSET(subnode_t,flg)//�ڵ������ַ�洢ƫ��
#define NODE_FLG_SIZE                  MEMBER_SIZE(subnode_t,flg)
#define NODE_ASNBNUM_SHIFT             MEMBER_OFFSET(subnode_t,as_nb_num)//�ڵ������ַ�洢ƫ��
#define NODE_ASNBNUM_SIZE              MEMBER_SIZE(subnode_t,as_nb_num)
#define NODE_CNT_SHIFT                 MEMBER_OFFSET(subnode_t,cnt)//�ڵ������ַ�洢ƫ��
#define NODE_CNT_SIZE                  MEMBER_SIZE(subnode_t,cnt)
#define NB_SIZE                        STRUCT_SIZE(neighbour_t)
#define NB_RSSI_SHIFT                  MEMBER_OFFSET(neighbour_t,rssi)
#define NB_RSSI_SIZE                   MEMBER_SIZE(neighbour_t,rssi)
#define NB_RATE_SHIFT                  MEMBER_OFFSET(neighbour_t,rate)
#define NB_RATE_SIZE                   MEMBER_SIZE(neighbour_t,rate)
#define PATH_SIZE                      STRUCT_SIZE(path_t)
#define PATH_RATE_SHIFT                MEMBER_OFFSET(path_t,succ_rate)
#define PATH_RATE_SIZE                 MEMBER_SIZE(path_t,succ_rate)
#define PATH_RELAY_SHIFT               MEMBER_OFFSET(path_t,relay[0])
#define PATH_RELAY_SIZE                (sizeof(unsigned short)*MAX_RELAY_NUM)

#define ROUTE_SIZE                     STRUCT_SIZE(route_table_t)//�����ھӱ�����
#define RT_NBNUM_SHIFT                 MEMBER_OFFSET(route_table_t,neighbour_num)
#define RT_NBNUM_SIZE                  MEMBER_SIZE(route_table_t,neighbour_num)
#define RT_LEV_SHIFT                   MEMBER_OFFSET(route_table_t,lev)
#define RT_LEV_SIZE                    MEMBER_SIZE(route_table_t,lev)
#define RT_CURPRE_SHIFT                   MEMBER_OFFSET(route_table_t,curpre)
#define RT_CURPRE_SIZE                    MEMBER_SIZE(route_table_t,curpre)
#define RT_MFB_SHIFT                   MEMBER_OFFSET(route_table_t,mfb)
#define RT_MFB_SIZE                    MEMBER_SIZE(route_table_t,mfb)
#define RT_CFN_SHIFT                   MEMBER_OFFSET(route_table_t,fail_cnt)
#define RT_CFN_SIZE                    MEMBER_SIZE(route_table_t,fail_cnt)
#define RT_NBS_SHIFT                   MEMBER_OFFSET(route_table_t,neighbours[0])
#define RT_NBS_SIZE                    (NB_SIZE*MAX_NEIGHBOUR_NODE_NUM)
#define RT_PATH_SHIFT                  MEMBER_OFFSET(route_table_t,path[0])
#define RT_PATH_SIZE                   (PATH_SIZE*MAX_PATH_NUM)
#define HOST_NODE_SIZE                 STRUCT_SIZE(host_t)//���ڵ㳤��
#define HOST_NETADR_SHIFT              MEMBER_OFFSET(host_t,net_addr)
#define HOST_NETADR_SIZE               MEMBER_SIZE(host_t,net_addr)
#define HOST_PANID_SHIFT               MEMBER_OFFSET(host_t,panid)
#define HOST_PANID_SIZE                MEMBER_SIZE(host_t,panid)
#define HOST_MXNUM_SHIFT               MEMBER_OFFSET(host_t,max_node_num)
#define HOST_MXNUM_SIZE                MEMBER_SIZE(host_t,max_node_num)
#define HOST_CURNUM_SHIFT              MEMBER_OFFSET(host_t,current_num)
#define HOST_CURNUM_SIZE               MEMBER_SIZE(host_t,current_num)
#define HOST_RTNUM_SHIFT               MEMBER_OFFSET(host_t,has_neighbour_node_num)
#define HOST_RTNUM_SIZE                MEMBER_SIZE(host_t,has_neighbour_node_num)
#define HOST_RENUMD_SHIFT              MEMBER_OFFSET(host_t,renumbered_seq)
#define HOST_RENUMD_SIZE               MEMBER_SIZE(host_t,renumbered_seq)
#define HOST_CHG_SHIFT                 MEMBER_OFFSET(host_t,channel_group)
#define HOST_CHG_SIZE                  MEMBER_SIZE(host_t,channel_group)
#define HOST_PORLEV_SHIFT              MEMBER_OFFSET(host_t,power_lev)
#define HOST_PORLEV_SIZE               MEMBER_SIZE(host_t,power_lev)
#define HOST_RSSITHR_SHIFT             MEMBER_OFFSET(host_t,rssi_thr)
#define HOST_RSSITHR_SIZE              MEMBER_SIZE(host_t,rssi_thr)
#define HOST_RSVR0_SHIFT               MEMBER_OFFSET(host_t,rsvr[0])
#define HOST_RSVR0_SIZE                MEMBER_SIZE(host_t,rsvr[0])
#define HOST_VER_SHIFT                 MEMBER_OFFSET(host_t,ver[0])
#define HOST_VER_SIZE                  (sizeof(unsigned char)*VER_NUM)
#define HOST_MCUID_SHIFT               MEMBER_OFFSET(host_t,mcu_id[0])
#define HOST_MCUID_SIZE                (sizeof(unsigned char)*MCU_ID_LEN)
#define HOST_LB1_SHIFT                 MEMBER_OFFSET(host_t,local_com_byte1)
#define HOST_LB1_SIZE                  MEMBER_SIZE(host_t,local_com_byte1)
#define HOST_LB2_SHIFT                 MEMBER_OFFSET(host_t,local_com_byte2)
#define HOST_LB2_SIZE                  MEMBER_SIZE(host_t,local_com_byte2)
#define HOST_LB3_SHIFT                 MEMBER_OFFSET(host_t,local_com_byte3)
#define HOST_LB3_SIZE                  MEMBER_SIZE(host_t,local_com_byte3)
#define HOST_LB4_SHIFT                 MEMBER_OFFSET(host_t,local_com_byte4)
#define HOST_LB4_SIZE                  MEMBER_SIZE(host_t,local_com_byte4)
#define HOST_MX13F1TIME_SHIFT         MEMBER_OFFSET(host_t,max_13f1_time)
#define HOST_MX13F1TIME_SIZE          MEMBER_SIZE(host_t,max_13f1_time)
#define HOST_MXBCTIEM_SHIFT         MEMBER_OFFSET(host_t,max_broadcast_time)
#define HOST_MXBCTIEM_SIZE          MEMBER_SIZE(host_t,max_broadcast_time)
#define HOST_MXFRAMELEN_SHIFT         MEMBER_OFFSET(host_t,max_frame_length)
#define HOST_MXFRAMELEN_SIZE          MEMBER_SIZE(host_t,max_frame_length)
#define HOST_MXTFFLEN_SHIFT         MEMBER_OFFSET(host_t,max_trans_file_frame_len)
#define HOST_MXTFFLEN_SIZE          MEMBER_SIZE(host_t,max_trans_file_frame_len)
#define HOST_UPDATETIME_SHIFT         MEMBER_OFFSET(host_t,update_wait_time)
#define HOST_UPDATETIME_SIZE          MEMBER_SIZE(host_t,update_wait_time)
#define HOST_RLSYEAR_SHIFT         MEMBER_OFFSET(host_t,protocal_releas_year)
#define HOST_RLSYEAR_SIZE          MEMBER_SIZE(host_t,protocal_releas_year)
#define HOST_RLSMON_SHIFT         MEMBER_OFFSET(host_t,protocal_releas_month)
#define HOST_RLSMON_SIZE          MEMBER_SIZE(host_t,protocal_releas_month)
#define HOST_RLSDAY_SHIFT         MEMBER_OFFSET(host_t,protocal_releas_date)
#define HOST_RLSDAY_SIZE          MEMBER_SIZE(host_t,protocal_releas_date)
#define HOST_RGTYEAR_SHIFT         MEMBER_OFFSET(host_t,protocal_regist_year)
#define HOST_RGTYEAR_SIZE          MEMBER_SIZE(host_t,protocal_regist_year)
#define HOST_RGTMON_SHIFT         MEMBER_OFFSET(host_t,protocal_regist_month)
#define HOST_RGTMON_SIZE          MEMBER_SIZE(host_t,protocal_regist_month)
#define HOST_RGTDAY_SHIFT         MEMBER_OFFSET(host_t,protocal_regist_date)
#define HOST_RGTDAY_SIZE          MEMBER_SIZE(host_t,protocal_regist_date)
#define HOST_COMRATE_SHIFT         MEMBER_OFFSET(host_t,com_rate)
#define HOST_COMRATE_SIZE          MEMBER_SIZE(host_t,com_rate)
#define HOST_DBGSWITCH_SHIFT         MEMBER_OFFSET(host_t,dbg_switch)
#define HOST_DBGSWITCH_SIZE          MEMBER_SIZE(host_t,dbg_switch)
#define HOST_FLASHSIZE_SHIFT         MEMBER_OFFSET(host_t,flash_size)
#define HOST_FLASHSIZE_SIZE          MEMBER_SIZE(host_t,flash_size)
#define HOST_HARDWAREVER_SHIFT         MEMBER_OFFSET(host_t,hardware_ver)
#define HOST_HARDWAREVER_SIZE          MEMBER_SIZE(host_t,hardware_ver)
#define HOST_NETSIZE_SHIFT         MEMBER_OFFSET(host_t,net_size)
#define HOST_NETSIZE_SIZE          MEMBER_SIZE(host_t,net_size)
#define HOST_SOFTWAREVER_SHIFT         MEMBER_OFFSET(host_t,software_ver)
#define HOST_SOFTWAREVER_SIZE          MEMBER_SIZE(host_t,software_ver)
#define HOST_ADDNODE_SHIFT         MEMBER_OFFSET(host_t,add_node_flg)
#define HOST_ADDNODE_SIZE          MEMBER_SIZE(host_t,add_node_flg)
#define HOST_FLG_SHIFT         MEMBER_OFFSET(host_t,flg)
#define HOST_FLG_SIZE          MEMBER_SIZE(host_t,flg)
#define HOST_WORKMODE_SHIFT         MEMBER_OFFSET(host_t,work_mode)
#define HOST_WORKMODE_SIZE          MEMBER_SIZE(host_t,work_mode)
//#define HOST_CNETSTIME_SHIFT         MEMBER_OFFSET(host_t,creat_net_start_time)
//#define HOST_CNETSTIME_SIZE          MEMBER_SIZE(host_t,creat_net_start_time)
//#define HOST_CNETFTIME_SHIFT         MEMBER_OFFSET(host_t,creat_net_finish_time)
//#define HOST_CNETFTIME_SIZE          MEMBER_SIZE(host_t,creat_net_finish_time)
//#define HOST_RELIVENET_SHIFT         MEMBER_OFFSET(host_t,relivenet_time)
//#define HOST_RELIVENET_SIZE          MEMBER_SIZE(host_t,relivenet_time)










/**************************1376.2��غ�**********************************/
//Ӧ�ù�����
#define AFN_00H                         (0)
#define AFN_01H                         (1)
#define AFN_02H                         (2)
#define AFN_03H                         (3)
#define AFN_04H                         (4)
#define AFN_05H                         (5)
#define AFN_06H                         (6)
#define AFN_10H                         (0X10)
#define AFN_11H                         (0X11)
#define AFN_12H                         (0X12)
#define AFN_13H                         (0X13)
#define AFN_14H                         (0X14)
#define AFN_15H                         (0X15)
#define AFN_F0H                         (0XF0)

//��������
#define DT_F1                           (1)
#define DT_F2                           (2)
#define DT_F3                           (3)
#define DT_F4                           (4)
#define DT_F5                           (5)
#define DT_F6                           (6)
#define DT_F7                           (7)
#define DT_F8                           (8)
#define DT_F9                           (9)
#define DT_F10                          (10)
#define DT_F11                          (11)
#define DT_F12                          (12)
#define DT_F13                          (13)
#define DT_F14                          (14)
#define DT_F15                          (15)
#define DT_F16                          (16)
#define DT_F100                         (100)
#define DT_F101                         (101)
#define DT_F102                         (102)
//ͨ�ŷ����߱�ʶ
#define PRM_1                           (1)//�ڵ�Ϊ����ͨ�ŷ�����
#define PRM_0                           (0)//�ڵ�Ϊ����ͨ��Ӧ����
//�¼��ϱ���ʶ
#define EVT_FLG_1                       (1)//�ӽڵ����¼��ϱ�
#define EVT_FLG_0                       (0)//�ӽڵ����ϱ��¼�
//���ݴ��䷽��
#define DIR_TO_HOST_NODE               (0)//�����������ڵ㷢��
#define DIR_FROM_HOST_NODE             (1)//���ڵ�����������
//ͨ�ŷ�ʽ
#define MODE_WIRELESS                  (10)//΢��������ͨ��
#define MODE_CENTRA_NPLC               (1)//����ʽ·��խ���ز�ͨ��
#define MODE_DISTRIBUTE_NPLC           (2)//�ֲ�ʽ·��խ���ز�ͨ��
#define MODE_BPLC                       (3)//�����ز�ͨ��
#define MODE_ENTHERNET                 (4)//��̫��ͨ��
#define MY_COM_MODE                     MODE_WIRELESS
//��Ϣ���ʶ
#define TARGET_HOST_NODE               (0)//ֱ�������ڵ�ͨ��
#define TARGET_SUB_NODE                (1)//��ӽڵ�ͨ��
#define ROUTE_FLG_ROUTE                (0)//ͨ��ģ�鹤����·��ģʽ
#define ROUTE_FLG_BYPASS               (1)//ͨ��ģ�鹤������·ģʽ
#define ROUTE_MODE                      (1)//ר���ڻظ����߷���·��ģʽ
#define CONFLICT_DETECT_FLG_SET       (1)//���г�ͻ���
#define CONFLICT_DETECT_FLG_RESET     (0)//�����г�ͻ���
#define PHASE_UNKNOWN                   (0)//��λ��ȷ��
#define PHASE_A                          (1)//��һ��
#define PHASE_B                          (2)
#define PHASE_C                          (3)
#define CHARACTER_UNKNOWN               (0)//����ͨ������
#define QUALITY_UNKNOWN                 (0)//���ź�Ʒ��
//#define BYPASS_MODE                     (1)
#define STD_ADDR_LENGTH                (6)

#define CMD_HAS_FINISH                 (1)
#define CMD_HAS_NOT_FINISH            (0)
#define CONFIGED                        (1)
#define NPLC                             (1)//խ�������ز�ͨ��
#define BPLC                             (2)//���������ز�ͨ��
#define MPWC                             (3)//΢��������ͨ��
#define HOST_ROUTE_MANAGE                (1)//���ڵ����·��
#define HOST_NO_ROUTE_MANAGE             (0)  

#define CONCENTRATOR_LEAD              (1)//��������������
#define HOST_NODE_LEAD                  (2)//���ڵ���������
#define BOTH_LEAD                       (3)//��������·��������֧��

#define SUBNODE_ADDR_NEED              (1)//��������Ҫ�·��ӽڵ���Ϣ�����ڵ�
#define SUBNODE_ADDR_NOT_NEED          (0)//����������Ҫ�·��ӽڵ���Ϣ�����ڵ�

#define B_CONFIRM_AFTER_EXCUTE         (0)//�㲥ͨ��ִ����Ϻ�ȷ��
#define B_CONFIRM_BEFOR_EXCUTE         (1)//�㲥ͨ��ִ��֮ǰ����ȷ��
#define BROADCAST_CHANNEL_NOT_NEED     (0)//�㲥�����Ҫָ���ŵ�
#define BROADCAST_CHANNEL_NEED          (1)//�㲥������Ҫ���������ŵ�����

#define TRANS_DELAY_ARG_NOT_SUPPORT   (0)//��֧���������ṩ���������ʱ

#define CONCENTRATOR_SWITCH             (2)//�����������л�
#define HOST_NODE_SWITCH                (1)//���ڵ㷢���л�
#define BOTH_SWITCH                       (3)//�����л���֧��

#define CHANNEL_NUM_NOT_MANAGE           (0)//����ͨ��ģ��֧�ֵ��ŵ���������Ҫ����

#define MAX_13F1_TIME                     (60)//��شӽڵ����ʱʱ��Ϊ60S
#define MAX_SINGLE_FILE_FRAME_LEN       (200)//�����ļ�����ְ���󳤶�
#define MAX_UPDATE_FINISH_TIME           (20)//���ڵ��������������������������õ�ʱ��
#define FIRST_SECTION                     (0)//�ļ������һ�α�ʶ
#define MIDDLE_SECTION                    (0)//�ļ������м�α�ʶ
#define LAST_SECTION            (1)//�ļ��������һ�α�ʶ
#define MESSAGE_MODE                      (0)//���ķ�ʽ��װ�ļ�
#define FILE_UPDATE                       (3)//���������ļ�
#define FILE_DELETE                       (0)//�����װ�ļ�








#define ERR_13762_TIMEOUT             (0)//ͨ�ų�ʱ
#define ERR_13762_INVALID_DATA        (1)//��Ч���ݵ�Ԫ
#define ERR_13762_LENGTH              (2)//���ȴ���
#define ERR_13762_VRIFY               (3)//У�����
#define ERR_13762_NO_INFOTYPE         (4)//��Ϣ�಻����
#define ERR_13762_FORMAT              (5)//��ʽ����
#define ERR_13762_METER_REPEAT        (6)//�����ظ�
#define ERR_13762_METER_NOT_EXSIT     (7)//���Ų�����
#define ERR_13762_APP_REPLY           (8)//Ӧ�ò���Ӧ��
#define ERR_13762_HOST_BUSY           (9)//���ڵ�æ
#define ERR_13762_CMD_NOT_SUPPORT     (10)//���ڵ㲻֧�ִ�����
#define ERR_13762_SUBNODE_NOT_REPLY   (11)//�ӽڵ㲻Ӧ��
#define ERR_13762_SUBNODE_HAS_NONET   (12)//�ӽڵ㲻������

#define ERR_13762_AUTOROUTE_AFN        (200)//�Զ�·�����յ���·��ģʽ�Ľ���ָ��
#define ERR_13762_TARGET               (199)//ͨ��ģ�鲻֧�ָ�����
#define ERR_13762_NUM_NOT_VALID        (198)//ѯ�ʴӽڵ���Ϣ����̫��
#define ERR_13762_START_NUM            (197)//ѯ�ʴӽڵ���Ϣ��ʼ��Ų��ϸ�
#define ERR_13762_NO_MEM               (196)//û�д洢�ռ�
#define ERR_13762_ARG_DATA             (195)//��������
#define ERR_13762_CRC                  (194)//�ļ�����������CRCУ�����





//�����ӿ���غ궨��
#define MODE_ADDR_STD                    (3)
#define MODE_ADDR_SHORT                  (2)

#define MAINTAIN_ST_CHOOSE_NODE         (0)//Ѱ��һ����ά���Ľڵ�
#define MAINTAIN_ST_TRACE_GET           (1)//Ѱ������·��
#define MAINTAIN_ST_SEND_REQ            (2)//����ά������
#define MAINTAIN_ST_HANDLE_REQ          (3)//����ά�����
#define NB_BIT_MAP(x)                   (1<<(x))


#define DBGINFO_DEFAULT                  (0)//�ر�����
#define DBGINFO_WIRELESSCOM              ((unsigned int)(0x80000000))//��ӡ�����շ�����
#define DBGINFO_ROUTE                    ((unsigned int)(1<<30))//��ӡ·�ɽӿڵ���
#define DBGINFO_ERRORNUM                 ((unsigned int)(1<<29))//��ӡ������
#define DBGINFO_USART2WIRELESS           ((unsigned int)(1<<28))//����ת����
#define DBGINFO_PRINTF                   ((unsigned int)(1<<27))//pintf�������֧��



#define MAINTAINANCE_WAIT_TIME                (0)//����ά��״̬�ȴ���������
#define MAINTAINANCE_START                    (1)//ʱ�䴥����������
#define MAINTAINANCE_PROCESS                  (2)//����ά��������
#define MAINTAINANCE_END                      (3)//����ά�����̽���

#define CREATNET_WAIT_TIME                (0)//����״̬�ȴ���������
#define CREATNET_START                    (1)//ʱ�䴥����������
#define CREATNET_PROCESS                  (2)//����������
#define CREATNET_END                      (3)//�������̽���


#define RECONFIG_WAIT_TIME                (0)//������״̬�ȴ���������
#define RECONFIG_START                    (1)//ʱ�䴥����������
#define RECONFIG_PROCESS                  (2)//�����ù�����
#define RECONFIG_END                      (3)//�����ó̽���

//host.work_status״̬λ
#define AFN_02_F1                        (0x01)
#define AFN_13_F1                        (0x02)
#define AFN_15_F1                        (0x03)
#define AFN_03_F1                        (0x04)
#define AFN_05_F3                        (0x05)
#define AFN_04_F2                        (0x06)
#define AFN_04_F3                        (0x07)
#define WORK_STATUS_IDLE                 (0x00)//����״̬
#define WORK_STATUS_CREATNET             (0x10)//����״̬
#define WORK_STATUS_MAINTAINANCE         (0X11)//·��ά��״̬
#define WORK_STATUS_RECONFIG             (0x12)//������״̬
#define WORK_STATUS_AUTORE               (0x13)//�ӽڵ�����ע��

#define READMETEREND              (1)
#define SEARCHMETEREND            (2)

#define NEIGHBOUR_RATE_THR               (REGET_PATH_VALUE)//�ı��豸�ȼ�������
#define CREAT_NET_WAIT_TIME               (2)//�������Ϻ������ĵȴ�ʱ�� ��

#if(DEFAULT_ZEROCHANNEL_MODE)
#define DEFAULT_CHANNEL_GROUP            (0)//Ĭ���ŵ�
#else
#define DEFAULT_CHANNEL_GROUP            (19)//Ĭ���ŵ�
#endif
#define DEFAULT_RSSI_THR                 (100)//Ĭ�ϳ�ǿ����
#define DEFAULT_POWER_LEVEL              (0)//��߹��ʣ��ȼ�Ϊ0�����Ϊ3
#define DEFAULT_NET_SIZE                 (256)//Ĭ�������ģ
#define MAX_NET_SIZE                     (512)//��������ģ


#define FLG_SET                       (1)
#define FLG_RESET                     (!FLG_SET)

#define NODE_NOCHANGE     (0)
#define NODE_DEC          (1)
#define NODE_ADD          (2)

#define UPGRADEFLG      0x153F587E

#define NODE_NUM                        (1025)
//eeprom��ַ����
//0~299---------------����������Э��ջʹ��
//300-499-------------376.2���ڵ�Host���ݽṹ�洢
//500-627-------------�ڵ�λͼ�б�
//628-1023-------------����
//1023-END-------------����¼���¼
#define EEPROM_SIZE                     (2048)
#define ARG_3762_STOR_START             (300)
#define UPGRADE_STOR_START              (1000)
#define SYSINFO_STOR_START              (1024)

#define HOST_NODE_ADDR                  ARG_3762_STOR_START 
#define UPGRADE_ADDR                    UPGRADE_STOR_START 
#define SYSINFO_ADDR                    SYSINFO_STOR_START 

//spiflash��ַ����
//0~20K(20480)---------------����������Э��ջʹ��
//300-499-------------376.2���ڵ�Host���ݽṹ�洢
//500-627-------------�ڵ�λͼ�б�
//628-END-------------����¼���¼��Ϣ
#define SPIFLASH_SIZE                   SPIFLASH_M25PE16_MEM
#define NODE_STOR_ADDR_START            (0x100)
#define NODE_INFO_STOR_ADDR_START       (((NODE_STOR_ADDR_START+NODE1_SIZE*NODE_NUM)&0xFFFFFF00)+0x100)
#define ROUTE_TABLE_STOR_START          (((NODE_NUM*NODE_SIZE+NODE_INFO_STOR_ADDR_START)&0xFFFFFF00)+0x200)

#define NODE_ADDR                       NODE_STOR_ADDR_START
#define NODE_INFO_ADDR                  NODE_INFO_STOR_ADDR_START
#define ROUTE_TABLE_START               ROUTE_TABLE_STOR_START
#define UPGRADESIZE                     0x80000
#define UPGRADEBASEADDR                 (SPIFLASH_SIZE-UPGRADESIZE)

//Inside flash
#define START_PAGE         8
#define END_PAGE           255
#define PAGE_SIZE          0x800
#define MAX_FILE_SIZE      (UPGRADESIZE-START_PAGE*PAGE_SIZE)

#define ERROR_IMPAOSSIBLE               (251)
#define ERROR_UART_SEND_LENGTH          (250)
#define ERROR_UART_SEND_BUSY            (249)
#define ERROR_UART_RECEIVE_VERIFY       (248)
#define ERROR_UART_FORMAT_C             (247)
#define ERROR_UART_MAC_LEN              (246)
#define ERROR_ADDR_CHECK_FAIL           (245)
#define ERROR_INDEX_NUM                 (244)//˳��ŷǷ�
#define ERROR_ALL_NOT_MINE              (243)//���ӵ��ھӱ�ȫ�������ڱ�����
#define ERROR_NUM_TOO_BIG               (242)//���������ھӱ�����̫��
#define ERROR_COM_NODE_NOT_IN_NET       (241)//ͨ�Žڵ㲻����
#define ERROR_COM_NODE_NO_NEIGHBOUR     (240)//ͨ�Žڵ�û���ھӽڵ�
#define ERROR_CAN_NOT_FIND_PATH         (239)//�Ҳ���·��
#define ERROR_MIX_PATH_GET              (238)//�̵�ַ·��ת��Ϊ���·������
#define ERROR_MIXPATH_TO_NETPATH        (237)//���·��ת��Ϊ����·��ʱ����
#define ERROR_NOT_DEFINE                (230)//δ�������
#define ERROR_CAN_NOT_FIND_COMNODE      (236)
#define ERROR_NET_ADDR_TO_SEQ           (235)//�����ַ���洢˳���ת����������
#define ERROR_NEIGHBOUR_NUM             (234)//�ھӽڵ�������������
#define ERROR_SEQ_TOO_BIG               (233)//���ɵ�λ�����̫��
#define ERROR_UPDATE_RATE_TYPE          (232)//�ھӱ��ɹ��ʸ���ʱ�ĸ������ʹ���
#define ERROR_INPUT_ARG                 (231)//�����������
#define ERROR_GET_FATHER_NODE           (229)//��·�ɱ�����·��ʱ��Ѱ�Ҹ��ڵ����
#define ERROR_PATH_TOO_LONG             (228)//����·��̫��
#define ERROR_CAN_NOT_FIND_NODE_INFO    (227)//�Ҳ����ڵ�����
#define ERROR_NOT_GET_MIXPATH           (226)//����·�����ɹ�
#define ERROR_NODE_ID_REPEAT            (225)//���������б������ظ�����
#define ERROR_NODE_NUM_TOO_BIG          (224)//����ڵ�����̫��
#define ERROR_ROUTE_LIST_REPEAT         (223)//·�ɱ������б������ظ�����
#define ERROR_ADD_PATH                  (222)//����·������ʧ��
#define ERROR_GET_MIXPATH_SHIFT         (221)//��ȡ·��ƫ�ƴ���
#define ERROR_NODE_NOT_MINE             (220)//����ѯ�ڵ㲻�������б���
#define ERROR_ADD_REPEAT_NEIGHBOURS     (219)//�ظ������ھӱ��ڵ�
#define ERROR_ADD_NEIGHBOUR_OVERFLOW    (218)//�����ھӱ������ھӱ����
#define ERROR_ROUTE_DEPTH               (217)//·�����̫��
#define ERROR_NET_ADDR_TO_POSITION      (216)//�����ַ���ڵ����ϴ洢λ��ת������
#define ERROR_GENERATE_PATH             (215)//����·������
#define ERROR_CRC_LENGTH                (214)//����CRCʱ����
#define ERROR_PATH_SEQ                  (213)//·���м�·����Ŵ���


#endif


