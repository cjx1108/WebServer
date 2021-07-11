#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include "../log/log.h"

class util_timer;
struct client_data
{
    sockaddr_in address;
    int sockfd;
    util_timer *timer;
};

class util_timer
{
public:
    util_timer() : prev(NULL), next(NULL) {}

public:
    time_t expire;//超时时间
    void (*cb_func)(client_data *);
    client_data *user_data;//连接资源
    util_timer *prev;
    util_timer *next;
};

class sort_timer_lst
{
public:
    sort_timer_lst() : head(NULL), tail(NULL) {}
    ~sort_timer_lst()
    {
        util_timer *tmp = head;
        while (tmp)
        {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }
    void add_timer(util_timer *timer) 
    {
        if (!timer)
        {
            return;
        }
        if (!head)
        {
            head = tail = timer;
            return;
        }
        //如果新的定时器超时时间小于当前头部结点
        //直接将当前定时器结点作为头部结点
        if (timer->expire < head->expire)
        {
            timer->next = head;
            head->prev = timer;
            head = timer;
            return;
        }
        add_timer(timer, head);
    }
    //调整定时器，任务发生变化时，调整定时器在链表中的位置
    void adjust_timer(util_timer *timer)
    {
        if (!timer)
        {
            return;
        }
        //被调整的定时器在链表尾部
        //定时器超时值仍然小于下一个定时器超时值，不调整
        util_timer *tmp = timer->next;
        if (!tmp || (timer->expire < tmp->expire))
        {
            return;
        }
        //被调整定时器是链表头结点，将定时器取出，重新插入
        if (timer == head)
        {
            head = head->next;
            head->prev = NULL;
            timer->next = NULL;
            add_timer(timer, head);
        }
        else
        {
            timer->prev->next = timer->next;
            timer->next->prev = timer->prev;
            add_timer(timer, timer->next);
        }
    }
    //删除定时器
    void del_timer(util_timer *timer)
    {
        if (!timer)
        {
            return;
        }
        //链表中只有一个定时器，需要删除该定时器
        if ((timer == head) && (timer == tail))
        {
            delete timer;
            head = NULL;
            tail = NULL;
            return;
        }
        //被删除的定时器为头结点
        if (timer == head)
        {
            head = head->next;
            head->prev = NULL;
            delete timer;
            return;
        }
        //被删除的定时器为尾结点
        if (timer == tail)
        {
            tail = tail->prev;
            tail->next = NULL;
            delete timer;
            return;
        }
        //被删除的定时器在链表内部，常规链表结点删除
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        delete timer;
    }
    //遍历定时器升序链表容器，从头结点开始依次处理每个定时器，直到遇到尚未到期的定时器
    //若当前时间小于定时器超时时间，跳出循环，即未找到到期的定时器
    //若当前时间大于定时器超时时间，即找到了到期的定时器，执行回调函数，然后将它从链表中删除，然后继续遍历
    void tick()
    {
        if (!head)
        {
            return;
        }
        //printf( "timer tick\n" );
        LOG_INFO("%s", "timer tick");
        Log::get_instance()->flush(); 
        time_t cur = time(NULL);
        util_timer *tmp = head;
        while (tmp)
        {   
            if (cur < tmp->expire)
            {
                break;
            }
            tmp->cb_func(tmp->user_data);
            head = tmp->next;
            if (head)
            {
                head->prev = NULL;
            }
            delete tmp;
            tmp = head;
        }
    }

private:
    void add_timer(util_timer *timer, util_timer *lst_head)
    {
        util_timer *prev = lst_head;
        util_timer *tmp = prev->next;
        //链表容器为升序排列
        //当前时间小于定时器的超时时间，后面的定时器也没有到期
        while (tmp)
        {
            if (timer->expire < tmp->expire)
            {
                prev->next = timer;
                timer->next = tmp;
                tmp->prev = timer;
                timer->prev = prev;
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        if (!tmp)
        {
            prev->next = timer;
            timer->prev = prev;
            timer->next = NULL;
            tail = timer;
        }
    }

private:
    util_timer *head;
    util_timer *tail;
};

#endif
