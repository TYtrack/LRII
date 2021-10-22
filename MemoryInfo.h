
class MemOccupy{
private:
    double total_mem;       //单位为M
    double free_mem;
    double mem_rate;        //内存使用律
    double total_swap_mem;
    double free_swap_mem;
    double swap_mem_rate;  //交换内存使用律
    long boot_time;        //系统运行时间 分钟
public:
    void getCurrentInfo();
    void getInfoString(char * retMessage,int msgLen);
}

