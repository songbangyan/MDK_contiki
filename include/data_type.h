#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__
#include "macro.h"
 
typedef struct  
{
  unsigned char *pdst;
  unsigned char *ptrace;
  unsigned char *pdata;
  unsigned char len;
  unsigned char relay_num;
  unsigned char trace_flg;//1表示指定路径
}wireless_trans_t;

typedef volatile struct 
{
  unsigned int tick_num;         //tick计数器
}systick_t;


typedef struct
{
  unsigned short year;
  unsigned char mon;
  unsigned char day;
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
  unsigned char week;
  unsigned short ms;
}Time_Def;

#pragma pack(push,1)
typedef  struct
{
  unsigned char file_id;
  unsigned char file_property;
  unsigned char file_cmd;
  unsigned short file_sec_num;
  unsigned int section_seq;
  unsigned short len;
  unsigned char pdata[]; 
}transfile_t;
#pragma pack(pop)


typedef struct 
{
  unsigned char trans_mode:6;
  unsigned char PRM:1;
  unsigned char DIR:1;
}contrl_byte_t;

typedef struct 
{
  unsigned char route_flg:1;
  unsigned char sub_flg:1;
  unsigned char target_flg:1;
  unsigned char conflict_flg:1;
  unsigned char relay_num:4;
  
  unsigned char channel:4;    
  unsigned char encode_flg:4;
  
  unsigned char estmate_num;
  
  unsigned char com_rate_low;
  unsigned char com_rate_high:7;
  unsigned char com_rate_type:1;

  unsigned char seq_num;
}down_info_t;

typedef struct 
{
  unsigned char route_flg:1;
  unsigned char rsvr1_0:1;
  unsigned char target_flg:1;
  unsigned char rsvr2_0:1;
  unsigned char relay_num:4;
  
  unsigned char channel:4;    
  unsigned char rsvr3_0:4;
  
  unsigned char real_phase:4;
  unsigned char channel_character:4;

  unsigned char end_command_quality:4; 
  unsigned char end_reply_quality:4;

  unsigned char event_flg:1;
  unsigned char rsvr:7;
  
  unsigned char seq_num;
}up_info_t;


typedef volatile struct 
{
  unsigned char state;//0表示空闲，其他表示忙
  unsigned char length;
  unsigned char num;
  unsigned char buf[UART_FRAME_MAX_LENGTH+5];
}uart_send_t;



typedef volatile struct 
{
  unsigned char state;//0表示空闲，其他表示忙
  unsigned char verify;//校验阶段
  unsigned char num;
  unsigned char buf[UART_FRAME_MAX_LENGTH+5];
}uart_receive_t;



typedef struct
{
  unsigned char state;//1表示等待网络层回传，0表示空闲
  unsigned char s_buf_num;
  unsigned char r_buf_num;
  unsigned char send_buf[UART_FRAME_MAX_LENGTH];
  unsigned char receive_buf[UART_FRAME_MAX_LENGTH];
}nwk_app_io_t;




typedef struct
{
  unsigned char com_mode:4;
  unsigned char route_manage_mode:1;
  unsigned char subnode_info_mode:1;
  unsigned char cyclic_meter_read_mode:2;
}mode1_t;


typedef struct
{
  unsigned char delay_arg_support:3;
  unsigned char fail_node_sw_mode:2;
  unsigned char broadcast_cfm_mode:1;
  unsigned char broadcast_ch_exemode:2;
}mode2_t;


typedef struct
{
  unsigned char channel_num:4;
  unsigned char power_down_info:4;//低压电网掉电信息
}mode3_t;

typedef struct
{
  unsigned char rate_num:4;
  unsigned char reserv:4;//
}mode4_t;

typedef struct
{
  unsigned char auto_report:1;
  unsigned char mt_flg:1;//路由维护标记，启动路由维护后该标记置位，清除网络该标记失效
  unsigned char rsvr:6;
}bitflg_t;


//64字节
typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];//标准地址
  unsigned short net_addr;//网络地址
  unsigned short panid;//个域网网号
  unsigned short max_node_num;//可容纳的节点数量
  unsigned short current_num;// 当前节点数量
  unsigned short has_neighbour_node_num;//有邻居节点的通讯节点数量
  unsigned short renumbered_seq;//短地址重复后重新编号所用的地址
  unsigned char channel_group;//信道组号
  unsigned char work_status;//当前工作状态，可以为空闲、组网、维护及02F1,13F1，15F1，03F1，04F2，04F3，05F3中的一种，
                            //只有在空闲状态才能够向非空闲状态转换，非空闲状态不能够直接向另外一个非空闲状态转换，必须先转换
                            //到空闲状态，非空闲状态可以响应主节点查询命令，通信类指令以"忙"响应
  unsigned char power_lev;//发射功率等级
  unsigned char rssi_thr;//接收门限
  unsigned char rsvr[2];//第一个字节为网络最大层次号，主节点所在层次为0
  unsigned char ver[VER_NUM];//版本相关信息
  unsigned char mcu_id[MCU_ID_LEN];//---45
  mode1_t local_com_byte1;
  mode2_t local_com_byte2;
  mode3_t local_com_byte3;
  mode4_t local_com_byte4;
  unsigned char max_13f1_time;
  unsigned short max_broadcast_time;
  unsigned short max_frame_length;
  unsigned short max_trans_file_frame_len;
  unsigned char update_wait_time;
  unsigned char protocal_releas_year;//通信协议发布日期
  unsigned char protocal_releas_month;//通信协议发布日期
  unsigned char protocal_releas_date;//通信协议发布日期
  unsigned char protocal_regist_year;//通信协议备案日期
  unsigned char protocal_regist_month; //通信协议备案日期
  unsigned char protocal_regist_date;  //通信协议备案日期
  unsigned char bootloader_ver;       //bootloader版本
  unsigned short com_rate;            //通信速率
  unsigned short flash_size;          //数据存储器大小
  unsigned int dbg_switch;            //调试开关
  unsigned short hardware_ver;       //硬件版本
  unsigned short net_size;           //网络规模
  unsigned int software_ver;
  unsigned char add_node_flg;       //新添电表标志
  bitflg_t flg;
  unsigned char space[2];
  unsigned int work_mode;
}host_t;


typedef struct
{
  unsigned int creat_net_flg:1;
  unsigned int geted_rtc_flg:1;
  unsigned int net_maintain_flg:1;
  unsigned int reserve:29;
}period_event_t;


typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];
  unsigned char protocal;
}add_node_t;









//2字节
typedef struct
{
  unsigned short relay_lev:4;
  unsigned short signal_q:4;
  unsigned short phase:3;
  unsigned short protocal_type:3;
  unsigned short reserve:2;
}sub_node_info_t;



//2字节
typedef struct
{
  unsigned char rssi:7;
  unsigned char flg:1;
}rssi_flg_t;


//1字节
typedef struct
{
  unsigned char run_lev:4;//实际运行时的网络层次,从0开始
  unsigned char net_ini_lev:4;//创建网络时的层次
}rank_t;


//4字节
typedef struct
{
  unsigned short netaddr;//邻居表的网络地址
  rssi_flg_t rssi;//收到该邻居设备发送数据的rssi及相互关心标志
  unsigned char rate;//成功率
//  rank_t rank;//包括组网层次及运行层次
//  unsigned char reserve[3];
}neighbour_t;

typedef struct
{
  unsigned char inter_nb:1;//互为邻居标识
  unsigned char mt:1;//维护成功标记
  unsigned char lev:4;//网络等级标识
  unsigned char rsv:2;//暂时保留
}nb_app_bit_t;

typedef struct
{
  nb_app_bit_t flg;//互为邻居标记指示
  unsigned char rssi;//rssi值
  unsigned char seq;//对应邻居表的位置号
}neighbour_app_t;

typedef struct
{
  unsigned char speed:2; //该路径应当使用的速度 0-10k;1-1.2k;2-500
  unsigned char node_num:6;
}num_speed_t;

typedef struct
{
  unsigned short index;//最高位表示主节点路径类型，1表示短地址，0标识长地址
  num_speed_t node_num;//路径中包含的节点数
  unsigned char path[TRACE_MAX_NODE_NUM*STD_ADDR_LENGTH];
}mix_path_t;



//16字节
typedef struct
{
  num_speed_t  relay_num;//本条路径中的中继节点数量
  unsigned char succ_rate;
  unsigned short relay[MAX_RELAY_NUM];//路径中的中继节点列表
}path_t;


//标准地址和rssi
typedef struct
{
  unsigned char addr[STD_ADDR_LENGTH];
  unsigned char rssi;
}comnode_rssi_t;



typedef struct
{
  unsigned char addr[STD_ADDR_LENGTH];
  unsigned char config_flg;
}config_and_std_addr_t;

typedef struct
{
  unsigned char cfg_flg:1;//配置标记，成功配置为1，失败配置0
  unsigned char add_flg:1;//添加了邻居标记，添加了置位
  unsigned char com_flg:1;//组网过程出现过标记，如果组网过程出现过配置为1
  unsigned char rsv_flg:5;
}flg_t;

typedef struct
{
  unsigned char mt_fail_times:4;//记录连续经历了几次网络维护失败过程
  unsigned char rsv:4;
}node_cnt_t;
/********************电表资料存储区*****************************/
//电表资料单条记录
//12字节
typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];
  unsigned short net_addr;//网络地址。如果为电表则自动拷贝，如果为采集器则要求集中器设置
  flg_t flg;              //添加邻居表及配置成功标志
  unsigned char as_nb_num;//其他设备记录本设备作为邻居的数量
  node_cnt_t cnt;//各类统计计数
  unsigned char rsv;
}subnode_t;

typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];
  unsigned short net_addr;//网络地址。如果为电表则自动拷贝，如果为采集器则要求集中器设置
}node_t;


/********************路由表位图存储区*****************************/
//格式为字节存储，长度为METER_BIT_MAP_SIZE



/********************集中器管理通讯节点网络地址存储区*****************************/
//格式为网络地址数组连续存储，第一个地址为主节点的网络地址，网络地址数量为
//METER_BIT_MAP_SIZE*8+1
typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];
  unsigned short net_addr;//网络地址。如果为电表则自动拷贝，如果为采集器则要求集中器设置
}comnode_t;


typedef struct
{
  unsigned char seq:7;
  unsigned char rsv:1;
}cur_pre_seq_t;

/********************邻居表存储区*****************************/
typedef struct
{
  neighbour_t pnb;
  unsigned char worst_rssi;//邻居相互最差场强
  unsigned char lev;//邻居等级
  unsigned char rate;//邻居当前路径的稳定度
  unsigned char inter_nb_flg:1;//互为邻居标记
  unsigned char cfg_flg:1;//配置标记
  unsigned char mt_flg:1;//维护成功标记
  unsigned char path_flg:1;//备用路径有效标记
  unsigned char rsv_flg:4;
  unsigned char nbq;//邻居品质
}nb_sort_t;


typedef struct
{
  unsigned char cur_seq:2;//使用的当前路径,0表示没有当前路径，1表示当前路径为pathtable中的path[0],
  unsigned char mt_flg:1;//路由维护标记，一轮维护开始前清空，维护成功置位
  unsigned char has_t_flg:1;//存在维护成功的上级邻居标识
  unsigned char cfg_flg:1;//配置标记
  unsigned char cur_path_status:1;//当前路径状态，0为备用路径状态，1为寻找路径状态
  unsigned char read_nb_suc_flg:1;//该节点邻居已经读取完毕
  unsigned char mt_com_suc_flg:1;//维护过程只是通信成功但是维护不合格标记
}route_mfb_t;

typedef struct 
{
  unsigned char cur:4;//当前路径连续通讯失败次数
  unsigned char total:4;//记录本通信节点连续通信失败次数
}com_fail_cnt_t;

//邻居关系表单条记录
//176字节
typedef struct  
{
  unsigned short net_addr;//网络地址
  unsigned char neighbour_num;//邻居数量
  rank_t lev;//包括节点的组网层次及运行层次
  unsigned char rsv;//保留
  cur_pre_seq_t curpre;//当前节点所使用的前级邻居在邻居表的序号
  route_mfb_t mfb;//多功能字
  com_fail_cnt_t fail_cnt;//该通信节点连续通信失败次数  
  neighbour_t neighbours[MAX_NEIGHBOUR_NODE_NUM];//120字节
  path_t path[MAX_PATH_NUM];  
}route_table_t;

/*************网络维护专用数据结构****************/
typedef struct 
{
  unsigned char worst_rssi[MAX_NEIGHBOUR_NODE_NUM];//邻居表相互场强值为邻居之间最弱一方接收场强的2倍，单通的情况对应值直接为255
  unsigned short seq[MAX_NEIGHBOUR_NODE_NUM];//邻居表对应邻居的存储序号
  unsigned int mt_bit_map;//当前邻居列表中已经配置过的邻居位图表
  unsigned char flag;//标记是否已经填充了rssi和seq，已经填充为0，未填充为1
}mt_nbinter_t;

typedef struct
{
  unsigned char m_cur_lev;//当前维护的网络等级，0表示主节点，1表示和主节点直接通信的设备，以此类推
  unsigned char m_cur_stat;//当前维护节点所处的维护阶段，包括
//  unsigned char path_used_bit_map;//最低3位用于记录当前路径表中备份的路径用于维护的尝试情况，尝试过后相应位置位，最低位对应pathtable的path[0]
  unsigned char cycle_num;//从1级维护到最高级的轮数
  unsigned char rcv_flg;//接收到场强维护信息标记，1-成功接收，2-接收失败，0-等待接收状态
  unsigned char rcvbuf[20];//场强信息存储位置
  unsigned short mted_num;//已经完成的维护数量
  unsigned short start_time;//维护开始的时间
  unsigned short end_time;//维护结束的时间
  unsigned int m_index;//待维护节点的索引号，在维护的第一阶段用于寻找维护节点，1表示第一个通讯节点(非主节点)
//  unsigned int nb_used_bit_map;//已经尝试过的邻居位图，最低位表示邻居表存储左起第一个邻居，尝试过后为1，未尝试为0
  unsigned short netaddrpath[MAX_RELAY_NUM+1];//用于存储网络地址构成的路径，序号0必须为主节点
  mix_path_t mt_mxpath;//维护过程需要使用的混合路径
  unsigned char node_num;//生成维护路径的节点数
  unsigned char suc_com;//维护过程存在通信成功但是没有维护成功的情况
  unsigned char seq;//维护过程通信成功但是没有维护成功的路径序号
//  mt_nbinter_t nb_addtion;//邻居表附加信息
//  route_table_t m_route;//待维护节点的路由信息
}net_maintain_t;


/****************************************************************/
typedef struct
{
    unsigned char comnode_stdaddr[STD_ADDR_LENGTH];//路由表表头短地址对应的标准地址
    unsigned short index_num;//所在路由表条目顺序号，1开始
    unsigned char seq_in_table;//当前访问的邻居节点序号,下次寻找节点地址以读取其邻居表信息时被累加
    unsigned char neighbour_stdaddr[STD_ADDR_LENGTH];//当前访问的邻居表内的节点对应的长地址
    route_table_t route;//路由表信息
}route_index_t;


#endif













