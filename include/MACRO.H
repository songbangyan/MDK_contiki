#ifndef __MACRO_H__
#define __MACRO_H__

/*本文件定义常用的宏*/
#define UART_FRAME_MAX_LENGTH          (240)

//#define printf(...)
#define PRODUCT_SN0                   'G'
#define PRODUCT_SN1                   'B'
#define CHIP_SN0                      'G'
#define CHIP_SN1                      'B'

#define TEST_MODE                       0
#define MODE_SHORTADDR_SUPPORT          (0X80000000)//在数据传输时短地址模式支持
#define MODE_LOCK_TO_ZERO_CHANNEL       (1<<30)//工作信道锁定到零信道
#define MODE_ROUTE_UPDATE               (1<<29)//路由自动更新维护启动
#define DEFAULT_SHORTADDR_MODE          (!MODE_SHORTADDR_SUPPORT)
#define DEFAULT_ZEROCHANNEL_MODE        (!MODE_LOCK_TO_ZERO_CHANNEL)
#define DEFAULT_ROUTEUPDATE_MODE        (MODE_ROUTE_UPDATE)

#define MAGIC_KEY                     (0X0951FEBC)
//可调整参数
#define REGET_PATH_VALUE               (125)//重新生成路径的阀值
#define HCLK                           (72000000)
#define SYSTICK_NUM                    (HCLK/8/1000)//1ms一次中断
#define MAX_NEIGHBOUR_NODE_NUM        (80)//单个通讯节点的最大邻居表数量
#define MAX_NEIGHBOUR_NUM_ONE_TIME    (16)//单次添加邻居节点的最大数量
#define TRACE_MAX_NODE_NUM             (MAX_RELAY_NUM+1)//最大路由深度
#define MAX_PATH_NUM                    (3)//单条路由记录最多记录的路径数目
#define MAX_RELAY_NUM                   (7)//路由记录中备份路径的最大中继数目
#define MT_RSSI_VAL                     (90)//维护过程使用的场强评估阀值
#define MT_FAIL_TO_UNCONFIG_NUM         (1)//连续维护导致配置信息清空的维护过程数

#define MAX_NUM_SINGLE_REPLY          (28)//单次应答从节点信息最大数量
#define VER_NUM                         (9)
#define MCU_ID_LEN                      (12)
#define COM_SUC_PAR                     (1)//通信成功累加品质参数
#define COM_FAIL_PAR                    (3)//通信失败下跌品质参数



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




//功能宏
#define MEMBER_OFFSET(struc, member) ((unsigned int)(&(((struc*)0)->member)))//计算结构体成员相对于结构体基地址的偏移
#define MEMBER_SIZE(struc,member)     sizeof(((struc*)0)->member)//计算结构体成员空间
#define STRUCT_SIZE(struc)             (sizeof(struc))//计算结构体尺寸

#define NODE1_SIZE                      STRUCT_SIZE(node_t)//节点存储空间大小
#define NODE_SIZE                      STRUCT_SIZE(subnode_t)//节点存储空间大小
#define NODE_NETADDR_SHIFT             MEMBER_OFFSET(subnode_t,net_addr)//节点网络地址存储偏移
#define NODE_NETADDR_SIZE              MEMBER_SIZE(subnode_t,net_addr)
#define NODE_FLG_SHIFT                 MEMBER_OFFSET(subnode_t,flg)//节点网络地址存储偏移
#define NODE_FLG_SIZE                  MEMBER_SIZE(subnode_t,flg)
#define NODE_ASNBNUM_SHIFT             MEMBER_OFFSET(subnode_t,as_nb_num)//节点网络地址存储偏移
#define NODE_ASNBNUM_SIZE              MEMBER_SIZE(subnode_t,as_nb_num)
#define NODE_CNT_SHIFT                 MEMBER_OFFSET(subnode_t,cnt)//节点网络地址存储偏移
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

#define ROUTE_SIZE                     STRUCT_SIZE(route_table_t)//单条邻居表长度
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
#define HOST_NODE_SIZE                 STRUCT_SIZE(host_t)//主节点长度
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










/**************************1376.2相关宏**********************************/
//应用功能码
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

//数据类型
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
//通信发起者标识
#define PRM_1                           (1)//节点为数据通信发起者
#define PRM_0                           (0)//节点为数据通信应答者
//事件上报标识
#define EVT_FLG_1                       (1)//从节点有事件上报
#define EVT_FLG_0                       (0)//从节点无上报事件
//数据传输方向
#define DIR_TO_HOST_NODE               (0)//集中器向主节点发送
#define DIR_FROM_HOST_NODE             (1)//主节点向集中器发送
//通信方式
#define MODE_WIRELESS                  (10)//微功率无线通信
#define MODE_CENTRA_NPLC               (1)//集中式路由窄带载波通信
#define MODE_DISTRIBUTE_NPLC           (2)//分布式路由窄带载波通信
#define MODE_BPLC                       (3)//宽带载波通信
#define MODE_ENTHERNET                 (4)//以太网通信
#define MY_COM_MODE                     MODE_WIRELESS
//信息域标识
#define TARGET_HOST_NODE               (0)//直接与主节点通信
#define TARGET_SUB_NODE                (1)//与从节点通信
#define ROUTE_FLG_ROUTE                (0)//通信模块工作于路由模式
#define ROUTE_FLG_BYPASS               (1)//通信模块工作于旁路模式
#define ROUTE_MODE                      (1)//专用于回复无线访问路由模式
#define CONFLICT_DETECT_FLG_SET       (1)//进行冲突检测
#define CONFLICT_DETECT_FLG_RESET     (0)//不进行冲突检测
#define PHASE_UNKNOWN                   (0)//相位不确定
#define PHASE_A                          (1)//第一相
#define PHASE_B                          (2)
#define PHASE_C                          (3)
#define CHARACTER_UNKNOWN               (0)//保留通道特征
#define QUALITY_UNKNOWN                 (0)//无信号品质
//#define BYPASS_MODE                     (1)
#define STD_ADDR_LENGTH                (6)

#define CMD_HAS_FINISH                 (1)
#define CMD_HAS_NOT_FINISH            (0)
#define CONFIGED                        (1)
#define NPLC                             (1)//窄带电力载波通信
#define BPLC                             (2)//宽带电力载波通信
#define MPWC                             (3)//微功率无线通信
#define HOST_ROUTE_MANAGE                (1)//主节点管理路由
#define HOST_NO_ROUTE_MANAGE             (0)  

#define CONCENTRATOR_LEAD              (1)//集中器主导抄表
#define HOST_NODE_LEAD                  (2)//主节点主导抄表
#define BOTH_LEAD                       (3)//集中器和路由主导均支持

#define SUBNODE_ADDR_NEED              (1)//集中器需要下发从节点信息到主节点
#define SUBNODE_ADDR_NOT_NEED          (0)//集中器不需要下发从节点信息到主节点

#define B_CONFIRM_AFTER_EXCUTE         (0)//广播通信执行完毕后确认
#define B_CONFIRM_BEFOR_EXCUTE         (1)//广播通信执行之前返回确认
#define BROADCAST_CHANNEL_NOT_NEED     (0)//广播命令不需要指定信道
#define BROADCAST_CHANNEL_NEED          (1)//广播命令需要按照命令信道发送

#define TRANS_DELAY_ARG_NOT_SUPPORT   (0)//不支持向集中器提供传输参数延时

#define CONCENTRATOR_SWITCH             (2)//集中器发起切换
#define HOST_NODE_SWITCH                (1)//主节点发起切换
#define BOTH_SWITCH                       (3)//两种切换都支持

#define CHANNEL_NUM_NOT_MANAGE           (0)//本地通信模块支持的信道数量不需要管理

#define MAX_13F1_TIME                     (60)//监控从节点最大超时时间为60S
#define MAX_SINGLE_FILE_FRAME_LEN       (200)//单次文件传输分包最大长度
#define MAX_UPDATE_FINISH_TIME           (20)//主节点接收完升级程序后到正常启动所用的时间
#define FIRST_SECTION                     (0)//文件传输第一段标识
#define MIDDLE_SECTION                    (0)//文件传输中间段标识
#define LAST_SECTION            (1)//文件传输最后一段标识
#define MESSAGE_MODE                      (0)//报文方式下装文件
#define FILE_UPDATE                       (3)//本地升级文件
#define FILE_DELETE                       (0)//清除下装文件








#define ERR_13762_TIMEOUT             (0)//通信超时
#define ERR_13762_INVALID_DATA        (1)//无效数据单元
#define ERR_13762_LENGTH              (2)//长度错误
#define ERR_13762_VRIFY               (3)//校验错误
#define ERR_13762_NO_INFOTYPE         (4)//信息类不存在
#define ERR_13762_FORMAT              (5)//格式错误
#define ERR_13762_METER_REPEAT        (6)//表号重复
#define ERR_13762_METER_NOT_EXSIT     (7)//表号不存在
#define ERR_13762_APP_REPLY           (8)//应用层无应答
#define ERR_13762_HOST_BUSY           (9)//主节点忙
#define ERR_13762_CMD_NOT_SUPPORT     (10)//主节点不支持此命令
#define ERR_13762_SUBNODE_NOT_REPLY   (11)//从节点不应答
#define ERR_13762_SUBNODE_HAS_NONET   (12)//从节点不在网内

#define ERR_13762_AUTOROUTE_AFN        (200)//自动路由下收到非路由模式的交互指令
#define ERR_13762_TARGET               (199)//通信模块不支持该命令
#define ERR_13762_NUM_NOT_VALID        (198)//询问从节点信息数量太大
#define ERR_13762_START_NUM            (197)//询问从节点信息起始序号不合格
#define ERR_13762_NO_MEM               (196)//没有存储空间
#define ERR_13762_ARG_DATA             (195)//参数错误
#define ERR_13762_CRC                  (194)//文件升级过程中CRC校验出错





//网络层接口相关宏定义
#define MODE_ADDR_STD                    (3)
#define MODE_ADDR_SHORT                  (2)

#define MAINTAIN_ST_CHOOSE_NODE         (0)//寻找一个待维护的节点
#define MAINTAIN_ST_TRACE_GET           (1)//寻找网络路径
#define MAINTAIN_ST_SEND_REQ            (2)//发送维护请求
#define MAINTAIN_ST_HANDLE_REQ          (3)//处理维护结果
#define NB_BIT_MAP(x)                   (1<<(x))


#define DBGINFO_DEFAULT                  (0)//关闭所有
#define DBGINFO_WIRELESSCOM              ((unsigned int)(0x80000000))//打印无线收发数据
#define DBGINFO_ROUTE                    ((unsigned int)(1<<30))//打印路由接口调用
#define DBGINFO_ERRORNUM                 ((unsigned int)(1<<29))//打印错误码
#define DBGINFO_USART2WIRELESS           ((unsigned int)(1<<28))//串口转无线
#define DBGINFO_PRINTF                   ((unsigned int)(1<<27))//pintf函数输出支持



#define MAINTAINANCE_WAIT_TIME                (0)//网络维护状态等待条件满足
#define MAINTAINANCE_START                    (1)//时间触发条件满足
#define MAINTAINANCE_PROCESS                  (2)//网络维护过程中
#define MAINTAINANCE_END                      (3)//网络维护过程结束

#define CREATNET_WAIT_TIME                (0)//组网状态等待条件满足
#define CREATNET_START                    (1)//时间触发条件满足
#define CREATNET_PROCESS                  (2)//组网过程中
#define CREATNET_END                      (3)//组网过程结束


#define RECONFIG_WAIT_TIME                (0)//重配置状态等待条件满足
#define RECONFIG_START                    (1)//时间触发条件满足
#define RECONFIG_PROCESS                  (2)//重配置过程中
#define RECONFIG_END                      (3)//重配置程结束

//host.work_status状态位
#define AFN_02_F1                        (0x01)
#define AFN_13_F1                        (0x02)
#define AFN_15_F1                        (0x03)
#define AFN_03_F1                        (0x04)
#define AFN_05_F3                        (0x05)
#define AFN_04_F2                        (0x06)
#define AFN_04_F3                        (0x07)
#define WORK_STATUS_IDLE                 (0x00)//空闲状态
#define WORK_STATUS_CREATNET             (0x10)//组网状态
#define WORK_STATUS_MAINTAINANCE         (0X11)//路由维护状态
#define WORK_STATUS_RECONFIG             (0x12)//重配置状态
#define WORK_STATUS_AUTORE               (0x13)//从节点主动注册

#define READMETEREND              (1)
#define SEARCHMETEREND            (2)

#define NEIGHBOUR_RATE_THR               (REGET_PATH_VALUE)//改变设备等级的门限
#define CREAT_NET_WAIT_TIME               (2)//添加资料后到组网的等待时间 秒

#if(DEFAULT_ZEROCHANNEL_MODE)
#define DEFAULT_CHANNEL_GROUP            (0)//默认信道
#else
#define DEFAULT_CHANNEL_GROUP            (19)//默认信道
#endif
#define DEFAULT_RSSI_THR                 (100)//默认场强门限
#define DEFAULT_POWER_LEVEL              (0)//最高功率，等级为0，最低为3
#define DEFAULT_NET_SIZE                 (256)//默认网络规模
#define MAX_NET_SIZE                     (512)//最大网络规模


#define FLG_SET                       (1)
#define FLG_RESET                     (!FLG_SET)

#define NODE_NOCHANGE     (0)
#define NODE_DEC          (1)
#define NODE_ADD          (2)

#define UPGRADEFLG      0x153F587E

#define NODE_NUM                        (1025)
//eeprom地址分配
//0~299---------------保留给无线协议栈使用
//300-499-------------376.2主节点Host数据结构存储
//500-627-------------节点位图列表
//628-1023-------------保留
//1023-END-------------相关事件记录
#define EEPROM_SIZE                     (2048)
#define ARG_3762_STOR_START             (300)
#define UPGRADE_STOR_START              (1000)
#define SYSINFO_STOR_START              (1024)

#define HOST_NODE_ADDR                  ARG_3762_STOR_START 
#define UPGRADE_ADDR                    UPGRADE_STOR_START 
#define SYSINFO_ADDR                    SYSINFO_STOR_START 

//spiflash地址分配
//0~20K(20480)---------------保留给无线协议栈使用
//300-499-------------376.2主节点Host数据结构存储
//500-627-------------节点位图列表
//628-END-------------相关事件记录信息
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
#define ERROR_INDEX_NUM                 (244)//顺序号非法
#define ERROR_ALL_NOT_MINE              (243)//添加的邻居表全部不属于本网络
#define ERROR_NUM_TOO_BIG               (242)//单次添加邻居表数量太大
#define ERROR_COM_NODE_NOT_IN_NET       (241)//通信节点不在网
#define ERROR_COM_NODE_NO_NEIGHBOUR     (240)//通信节点没有邻居节点
#define ERROR_CAN_NOT_FIND_PATH         (239)//找不到路径
#define ERROR_MIX_PATH_GET              (238)//短地址路径转换为混合路径出错
#define ERROR_MIXPATH_TO_NETPATH        (237)//混合路径转换为网络路径时出错
#define ERROR_NOT_DEFINE                (230)//未定义错误
#define ERROR_CAN_NOT_FIND_COMNODE      (236)
#define ERROR_NET_ADDR_TO_SEQ           (235)//网络地址到存储顺序号转换发生错误
#define ERROR_NEIGHBOUR_NUM             (234)//邻居节点数量发生错误
#define ERROR_SEQ_TOO_BIG               (233)//生成的位置序号太大
#define ERROR_UPDATE_RATE_TYPE          (232)//邻居表成功率更新时的更新类型错误
#define ERROR_INPUT_ARG                 (231)//输入参数错误
#define ERROR_GET_FATHER_NODE           (229)//从路由表生成路径时，寻找父节点错误
#define ERROR_PATH_TOO_LONG             (228)//生成路径太长
#define ERROR_CAN_NOT_FIND_NODE_INFO    (227)//找不到节点资料
#define ERROR_NOT_GET_MIXPATH           (226)//生成路径不成功
#define ERROR_NODE_ID_REPEAT            (225)//添加资料列表存在重复现象
#define ERROR_NODE_NUM_TOO_BIG          (224)//输入节点数量太大
#define ERROR_ROUTE_LIST_REPEAT         (223)//路由表资料列表存在重复现象
#define ERROR_ADD_PATH                  (222)//添加路径过程失败
#define ERROR_GET_MIXPATH_SHIFT         (221)//获取路径偏移错误
#define ERROR_NODE_NOT_MINE             (220)//所查询节点不在资料列表内
#define ERROR_ADD_REPEAT_NEIGHBOURS     (219)//重复添加邻居表节点
#define ERROR_ADD_NEIGHBOUR_OVERFLOW    (218)//添加邻居表导致邻居表溢出
#define ERROR_ROUTE_DEPTH               (217)//路由深度太深
#define ERROR_NET_ADDR_TO_POSITION      (216)//网络地址到节点资料存储位置转换错误
#define ERROR_GENERATE_PATH             (215)//生成路径出错
#define ERROR_CRC_LENGTH                (214)//计算CRC时出错
#define ERROR_PATH_SEQ                  (213)//路由中继路径序号错误


#endif



