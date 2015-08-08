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
  unsigned char trace_flg;//1��ʾָ��·��
}wireless_trans_t;

typedef volatile struct 
{
  unsigned int tick_num;         //tick������
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
  unsigned char state;//0��ʾ���У�������ʾæ
  unsigned char length;
  unsigned char num;
  unsigned char buf[UART_FRAME_MAX_LENGTH+5];
}uart_send_t;



typedef volatile struct 
{
  unsigned char state;//0��ʾ���У�������ʾæ
  unsigned char verify;//У��׶�
  unsigned char num;
  unsigned char buf[UART_FRAME_MAX_LENGTH+5];
}uart_receive_t;



typedef struct
{
  unsigned char state;//1��ʾ�ȴ������ش���0��ʾ����
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
  unsigned char power_down_info:4;//��ѹ����������Ϣ
}mode3_t;

typedef struct
{
  unsigned char rate_num:4;
  unsigned char reserv:4;//
}mode4_t;

typedef struct
{
  unsigned char auto_report:1;
  unsigned char mt_flg:1;//·��ά����ǣ�����·��ά����ñ����λ���������ñ��ʧЧ
  unsigned char rsvr:6;
}bitflg_t;


//64�ֽ�
typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];//��׼��ַ
  unsigned short net_addr;//�����ַ
  unsigned short panid;//����������
  unsigned short max_node_num;//�����ɵĽڵ�����
  unsigned short current_num;// ��ǰ�ڵ�����
  unsigned short has_neighbour_node_num;//���ھӽڵ��ͨѶ�ڵ�����
  unsigned short renumbered_seq;//�̵�ַ�ظ������±�����õĵ�ַ
  unsigned char channel_group;//�ŵ����
  unsigned char work_status;//��ǰ����״̬������Ϊ���С�������ά����02F1,13F1��15F1��03F1��04F2��04F3��05F3�е�һ�֣�
                            //ֻ���ڿ���״̬���ܹ���ǿ���״̬ת�����ǿ���״̬���ܹ�ֱ��������һ���ǿ���״̬ת����������ת��
                            //������״̬���ǿ���״̬������Ӧ���ڵ��ѯ���ͨ����ָ����"æ"��Ӧ
  unsigned char power_lev;//���书�ʵȼ�
  unsigned char rssi_thr;//��������
  unsigned char rsvr[2];//��һ���ֽ�Ϊ��������κţ����ڵ����ڲ��Ϊ0
  unsigned char ver[VER_NUM];//�汾�����Ϣ
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
  unsigned char protocal_releas_year;//ͨ��Э�鷢������
  unsigned char protocal_releas_month;//ͨ��Э�鷢������
  unsigned char protocal_releas_date;//ͨ��Э�鷢������
  unsigned char protocal_regist_year;//ͨ��Э�鱸������
  unsigned char protocal_regist_month; //ͨ��Э�鱸������
  unsigned char protocal_regist_date;  //ͨ��Э�鱸������
  unsigned char bootloader_ver;       //bootloader�汾
  unsigned short com_rate;            //ͨ������
  unsigned short flash_size;          //���ݴ洢����С
  unsigned int dbg_switch;            //���Կ���
  unsigned short hardware_ver;       //Ӳ���汾
  unsigned short net_size;           //�����ģ
  unsigned int software_ver;
  unsigned char add_node_flg;       //�������־
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









//2�ֽ�
typedef struct
{
  unsigned short relay_lev:4;
  unsigned short signal_q:4;
  unsigned short phase:3;
  unsigned short protocal_type:3;
  unsigned short reserve:2;
}sub_node_info_t;



//2�ֽ�
typedef struct
{
  unsigned char rssi:7;
  unsigned char flg:1;
}rssi_flg_t;


//1�ֽ�
typedef struct
{
  unsigned char run_lev:4;//ʵ������ʱ��������,��0��ʼ
  unsigned char net_ini_lev:4;//��������ʱ�Ĳ��
}rank_t;


//4�ֽ�
typedef struct
{
  unsigned short netaddr;//�ھӱ�������ַ
  rssi_flg_t rssi;//�յ����ھ��豸�������ݵ�rssi���໥���ı�־
  unsigned char rate;//�ɹ���
//  rank_t rank;//����������μ����в��
//  unsigned char reserve[3];
}neighbour_t;

typedef struct
{
  unsigned char inter_nb:1;//��Ϊ�ھӱ�ʶ
  unsigned char mt:1;//ά���ɹ����
  unsigned char lev:4;//����ȼ���ʶ
  unsigned char rsv:2;//��ʱ����
}nb_app_bit_t;

typedef struct
{
  nb_app_bit_t flg;//��Ϊ�ھӱ��ָʾ
  unsigned char rssi;//rssiֵ
  unsigned char seq;//��Ӧ�ھӱ��λ�ú�
}neighbour_app_t;

typedef struct
{
  unsigned char speed:2; //��·��Ӧ��ʹ�õ��ٶ� 0-10k;1-1.2k;2-500
  unsigned char node_num:6;
}num_speed_t;

typedef struct
{
  unsigned short index;//���λ��ʾ���ڵ�·�����ͣ�1��ʾ�̵�ַ��0��ʶ����ַ
  num_speed_t node_num;//·���а����Ľڵ���
  unsigned char path[TRACE_MAX_NODE_NUM*STD_ADDR_LENGTH];
}mix_path_t;



//16�ֽ�
typedef struct
{
  num_speed_t  relay_num;//����·���е��м̽ڵ�����
  unsigned char succ_rate;
  unsigned short relay[MAX_RELAY_NUM];//·���е��м̽ڵ��б�
}path_t;


//��׼��ַ��rssi
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
  unsigned char cfg_flg:1;//���ñ�ǣ��ɹ�����Ϊ1��ʧ������0
  unsigned char add_flg:1;//������ھӱ�ǣ��������λ
  unsigned char com_flg:1;//�������̳��ֹ���ǣ�����������̳��ֹ�����Ϊ1
  unsigned char rsv_flg:5;
}flg_t;

typedef struct
{
  unsigned char mt_fail_times:4;//��¼���������˼�������ά��ʧ�ܹ���
  unsigned char rsv:4;
}node_cnt_t;
/********************������ϴ洢��*****************************/
//������ϵ�����¼
//12�ֽ�
typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];
  unsigned short net_addr;//�����ַ�����Ϊ������Զ����������Ϊ�ɼ�����Ҫ����������
  flg_t flg;              //����ھӱ����óɹ���־
  unsigned char as_nb_num;//�����豸��¼���豸��Ϊ�ھӵ�����
  node_cnt_t cnt;//����ͳ�Ƽ���
  unsigned char rsv;
}subnode_t;

typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];
  unsigned short net_addr;//�����ַ�����Ϊ������Զ����������Ϊ�ɼ�����Ҫ����������
}node_t;


/********************·�ɱ�λͼ�洢��*****************************/
//��ʽΪ�ֽڴ洢������ΪMETER_BIT_MAP_SIZE



/********************����������ͨѶ�ڵ������ַ�洢��*****************************/
//��ʽΪ�����ַ���������洢����һ����ַΪ���ڵ�������ַ�������ַ����Ϊ
//METER_BIT_MAP_SIZE*8+1
typedef struct
{
  unsigned char std_addr[STD_ADDR_LENGTH];
  unsigned short net_addr;//�����ַ�����Ϊ������Զ����������Ϊ�ɼ�����Ҫ����������
}comnode_t;


typedef struct
{
  unsigned char seq:7;
  unsigned char rsv:1;
}cur_pre_seq_t;

/********************�ھӱ�洢��*****************************/
typedef struct
{
  neighbour_t pnb;
  unsigned char worst_rssi;//�ھ��໥��ǿ
  unsigned char lev;//�ھӵȼ�
  unsigned char rate;//�ھӵ�ǰ·�����ȶ���
  unsigned char inter_nb_flg:1;//��Ϊ�ھӱ��
  unsigned char cfg_flg:1;//���ñ��
  unsigned char mt_flg:1;//ά���ɹ����
  unsigned char path_flg:1;//����·����Ч���
  unsigned char rsv_flg:4;
  unsigned char nbq;//�ھ�Ʒ��
}nb_sort_t;


typedef struct
{
  unsigned char cur_seq:2;//ʹ�õĵ�ǰ·��,0��ʾû�е�ǰ·����1��ʾ��ǰ·��Ϊpathtable�е�path[0],
  unsigned char mt_flg:1;//·��ά����ǣ�һ��ά����ʼǰ��գ�ά���ɹ���λ
  unsigned char has_t_flg:1;//����ά���ɹ����ϼ��ھӱ�ʶ
  unsigned char cfg_flg:1;//���ñ��
  unsigned char cur_path_status:1;//��ǰ·��״̬��0Ϊ����·��״̬��1ΪѰ��·��״̬
  unsigned char read_nb_suc_flg:1;//�ýڵ��ھ��Ѿ���ȡ���
  unsigned char mt_com_suc_flg:1;//ά������ֻ��ͨ�ųɹ�����ά�����ϸ���
}route_mfb_t;

typedef struct 
{
  unsigned char cur:4;//��ǰ·������ͨѶʧ�ܴ���
  unsigned char total:4;//��¼��ͨ�Žڵ�����ͨ��ʧ�ܴ���
}com_fail_cnt_t;

//�ھӹ�ϵ������¼
//176�ֽ�
typedef struct  
{
  unsigned short net_addr;//�����ַ
  unsigned char neighbour_num;//�ھ�����
  rank_t lev;//�����ڵ��������μ����в��
  unsigned char rsv;//����
  cur_pre_seq_t curpre;//��ǰ�ڵ���ʹ�õ�ǰ���ھ����ھӱ�����
  route_mfb_t mfb;//�๦����
  com_fail_cnt_t fail_cnt;//��ͨ�Žڵ�����ͨ��ʧ�ܴ���  
  neighbour_t neighbours[MAX_NEIGHBOUR_NODE_NUM];//120�ֽ�
  path_t path[MAX_PATH_NUM];  
}route_table_t;

/*************����ά��ר�����ݽṹ****************/
typedef struct 
{
  unsigned char worst_rssi[MAX_NEIGHBOUR_NODE_NUM];//�ھӱ��໥��ǿֵΪ�ھ�֮������һ�����ճ�ǿ��2������ͨ�������Ӧֱֵ��Ϊ255
  unsigned short seq[MAX_NEIGHBOUR_NODE_NUM];//�ھӱ��Ӧ�ھӵĴ洢���
  unsigned int mt_bit_map;//��ǰ�ھ��б����Ѿ����ù����ھ�λͼ��
  unsigned char flag;//����Ƿ��Ѿ������rssi��seq���Ѿ����Ϊ0��δ���Ϊ1
}mt_nbinter_t;

typedef struct
{
  unsigned char m_cur_lev;//��ǰά��������ȼ���0��ʾ���ڵ㣬1��ʾ�����ڵ�ֱ��ͨ�ŵ��豸���Դ�����
  unsigned char m_cur_stat;//��ǰά���ڵ�������ά���׶Σ�����
//  unsigned char path_used_bit_map;//���3λ���ڼ�¼��ǰ·�����б��ݵ�·������ά���ĳ�����������Թ�����Ӧλ��λ�����λ��Ӧpathtable��path[0]
  unsigned char cycle_num;//��1��ά������߼�������
  unsigned char rcv_flg;//���յ���ǿά����Ϣ��ǣ�1-�ɹ����գ�2-����ʧ�ܣ�0-�ȴ�����״̬
  unsigned char rcvbuf[20];//��ǿ��Ϣ�洢λ��
  unsigned short mted_num;//�Ѿ���ɵ�ά������
  unsigned short start_time;//ά����ʼ��ʱ��
  unsigned short end_time;//ά��������ʱ��
  unsigned int m_index;//��ά���ڵ�������ţ���ά���ĵ�һ�׶�����Ѱ��ά���ڵ㣬1��ʾ��һ��ͨѶ�ڵ�(�����ڵ�)
//  unsigned int nb_used_bit_map;//�Ѿ����Թ����ھ�λͼ�����λ��ʾ�ھӱ�洢�����һ���ھӣ����Թ���Ϊ1��δ����Ϊ0
  unsigned short netaddrpath[MAX_RELAY_NUM+1];//���ڴ洢�����ַ���ɵ�·�������0����Ϊ���ڵ�
  mix_path_t mt_mxpath;//ά��������Ҫʹ�õĻ��·��
  unsigned char node_num;//����ά��·���Ľڵ���
  unsigned char suc_com;//ά�����̴���ͨ�ųɹ�����û��ά���ɹ������
  unsigned char seq;//ά������ͨ�ųɹ�����û��ά���ɹ���·�����
//  mt_nbinter_t nb_addtion;//�ھӱ�����Ϣ
//  route_table_t m_route;//��ά���ڵ��·����Ϣ
}net_maintain_t;


/****************************************************************/
typedef struct
{
    unsigned char comnode_stdaddr[STD_ADDR_LENGTH];//·�ɱ��ͷ�̵�ַ��Ӧ�ı�׼��ַ
    unsigned short index_num;//����·�ɱ���Ŀ˳��ţ�1��ʼ
    unsigned char seq_in_table;//��ǰ���ʵ��ھӽڵ����,�´�Ѱ�ҽڵ��ַ�Զ�ȡ���ھӱ���Ϣʱ���ۼ�
    unsigned char neighbour_stdaddr[STD_ADDR_LENGTH];//��ǰ���ʵ��ھӱ��ڵĽڵ��Ӧ�ĳ���ַ
    route_table_t route;//·�ɱ���Ϣ
}route_index_t;


#endif













