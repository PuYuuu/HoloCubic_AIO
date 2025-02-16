#ifndef APP_CONTORLLER_H
#define APP_CONTORLLER_H

#include "Arduino.h"
#include "interface.h"
#include "driver/imu.h"

#define APP_MAX_NUM 15        // 最大的可运行的APP数量
#define WIFI_LIFE_CYCLE 60000 // wifi的生命周期（60s）

class AppController
{
public:
    AppController();
    ~AppController();
    void Display(void);                     // 显示接口
    int app_register(const APP_OBJ *app);   // 将APP注册到app_controller中
    int app_unregister(const APP_OBJ *app); // 将APP从app_controller中去注册（删除）
    int main_process(Imu_Action *act_info);
    void app_exit(void); // 提供给app退出的系统调用
    // 事件请求
    int req_event(const APP_OBJ *from, APP_EVENT event, int event_id);
    int req_event_deal(void); // 事件处理

private:
    int app_is_legal(const APP_OBJ *app_obj);

private:
    APP_OBJ *appList;                 // 预留10个APP注册位
    const APP_OBJ *m_app_req;         // 发送请求服务的APP
    unsigned int m_app_event_id;      // 发送请求服务的id
    APP_EVENT m_app_event_type;       // app的事件类型
    boolean m_wifi_status;            // 表示是wifi状态 true开启 false关闭
    unsigned long m_preWifiReqMillis; // 保存上一回请求的时间戳
    unsigned int app_num;
    boolean app_exit_flag; // 表示是否退出APP应用
    int cur_app_index;     // 当前运行的APP下标
    int pre_app_index;     // 上一次运行的APP下标
};

#endif