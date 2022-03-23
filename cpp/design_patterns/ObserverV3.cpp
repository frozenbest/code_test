/*
* Observer 观察者模式
* 对象间存在一对多关系时 使用观察者模式 一个对象被修改，会自动通知依赖塔的对象. 行为型模式
* 使用场景: 一个目标对象变化,所有观察者都将得到广播通知
* 优点: 1.观察者和被观察者是抽象耦合的 2.建立一套触发机制
* 缺点: 1.观察者太多,通知所有观察者耗时太长 2.观察者不知道被观察目标怎么发生变化,只知道目标发生变化
*/
/* 前置声明
* 前置声明的类 不能定义对象   可以定义指向这个类型的指针和引用 
* 前置声明的类是不完全的类 不能操作成员或成员函数
*/

#include <thread>
#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>

using namespace std;

enum enum_member_relation_status{
    enum_member_relation_weak    = 0, // 弱
    enum_member_relation_strong  = 1, // 强
};

// 队伍成员状态变化 定义
enum enum_team_status_chg_code{
    enum_member_chg_new     = 1, // 添加
    enum_member_chg_remove  = 2, // 删除
	enum_member_chg_relive  = 4, // 复活
	enum_member_chg_durable = 8, // 耐久
	enum_member_chg_route   = 16, // 航线
    enum_member_chg_anger   = 32, // 怒气
};

struct TEAM_STATUS_INFO{
	bool is_relive;                                 //等待复活中
    int  durability;                                //资源持久值
    int  strong_state;                              // 强弱关系
    TEAM_STATUS_INFO()
    :is_relive(0),durability(0),strong_state(0)
    {
    }
};

typedef std::shared_ptr<TEAM_STATUS_INFO> TEAM_INFO_PTR;

class Observer; // 前置声明  订阅队伍状态变化的用户
typedef std::shared_ptr<Observer> auto_observer_ptr; //扩展多态


class BeSubscriber // 被订阅者  (队伍)
{
public:
	 /* 定义Key */
    struct MEMBER_KEY
    {
        unsigned long long int user_id;
        unsigned long long int ship_id;

        inline bool operator < (const struct MEMBER_KEY& right) const
        {
            if(user_id == right.user_id)
            {
                return ship_id < right.ship_id;
            }
            return user_id < right.user_id;
        }
    };

public:
    BeSubscriber(unsigned long long int tid);
    ~BeSubscriber();
public:

	/* 队伍成员状态变化
	* user_id 队伍成员ID
	* ship_id 队伍成员船ID
	* chg_code 队员信息状态变化码
	* team_info 队员状态信息
	*/
    void member_update(unsigned long long int user_id, unsigned long long int ship_id, int chg_code, TEAM_INFO_PTR team_info);

	// 订阅
    void attach(unsigned long long int uid, unsigned long long int sid, auto_observer_ptr observer);
    // 取消订阅 队员可能提前还船
    void detach(unsigned long long int uid, unsigned long long int sid);
    // notify
    void broadcast();


public:
    inline unsigned int get_observers_count() const {
    	unsigned int count = observers_map.size();
    	return count;
    }

    inline unsigned int get_members_info_count() const {
        unsigned int count = memebers_info_map.size();
        return count;
    }

    inline unsigned long long int get_team_id() const {
        return team_id;
    }

    std::map<MEMBER_KEY, TEAM_INFO_PTR> memebers_info_map; // 队员信息map

private:
	std::map<unsigned long long int, auto_observer_ptr> observers_map;
    unsigned long long int team_id; // 被订阅的队伍id
};

class Observer
{
public:
    Observer(BeSubscriber* b_p, unsigned long long int uid, unsigned long long int sid);

    Observer(Observer* right);

public:
	void pushTeamStatusIDPack(unsigned long long int uid);

    inline unsigned long long int get_user_id() const {
        return user_id;
    }
    inline unsigned long long int get_ship_id() const {
        return ship_id;
    }

protected:
	BeSubscriber* besubscriber_ptr;
    unsigned long long int user_id;
    unsigned long long int ship_id;
};



// 变化者
BeSubscriber::BeSubscriber(unsigned long long int tid):team_id(tid)
{

}

BeSubscriber::~BeSubscriber()
{
    memebers_info_map.clear();
    observers_map.clear();
}

void BeSubscriber::member_update(unsigned long long int user_id, unsigned long long int ship_id, int chg_code, TEAM_INFO_PTR team_info)
{
    if(!team_info)
        return;

    bool notify_all = false; // 是否广播给所有队伍成员
    if(chg_code & enum_member_chg_new)
    {
        auto_observer_ptr new_observer = auto_observer_ptr(new Observer(this, user_id, ship_id) );
        // Observer* new_observer = new Observer(this, user_id, ship_id);
        notify_all = true;
    }
    if(chg_code & enum_member_chg_remove) // 理论上不会走到这
    {
        this->detach(user_id, ship_id);
        return;
    }

    MEMBER_KEY member_key;
    member_key.user_id = user_id;
    member_key.ship_id = ship_id;

    std::map<MEMBER_KEY, TEAM_INFO_PTR>::iterator it = memebers_info_map.find(member_key);
    if(it == memebers_info_map.end() )
        return;

    if(chg_code & enum_member_chg_relive)
    {
        it->second->is_relive = team_info->is_relive;
        notify_all = true;
    }
    if (chg_code & enum_member_chg_durable)
    {
        it->second->durability = team_info->durability;
        notify_all = true;
    }
    // 广播给队员
    if(notify_all)
    {
    	std::cout<<"FUNCTION: "<<__FUNCTION__<<"() "<<__LINE__<<" notify all. user_id= "<<user_id<<" ship_id= "<<ship_id<<" chg_code= "<<chg_code<<" \n";
        broadcast();
    }

}



// 订阅
void BeSubscriber::attach(unsigned long long int uid, unsigned long long int sid, auto_observer_ptr observer)
{
    MEMBER_KEY key;
    key.user_id = uid;
    key.ship_id = sid;

    observers_map[uid] = observer;

    std::cout<<"FUNCTION: "<< __FUNCTION__<<"() "<<__LINE__<<" uid "<<uid
            <<" observer.user_id= "<<observer->get_user_id() <<" observer.ship_id= "<<observer->get_ship_id()<<" use_count= "<<observer.use_count()<<" \n";
    
    TEAM_INFO_PTR team_info = TEAM_INFO_PTR(new TEAM_STATUS_INFO());
    memebers_info_map[key] = team_info;
}
// 取消订阅
void BeSubscriber::detach(unsigned long long int uid, unsigned long long int sid)
{
    MEMBER_KEY key;
    key.user_id = uid;
    key.ship_id = sid;

    auto o_it = observers_map.find(key.user_id);
    if(o_it != observers_map.end() )
    {
        observers_map.erase(o_it);
    }

    auto m_it = memebers_info_map.find(key);
    if(m_it != memebers_info_map.end() )
    {
        memebers_info_map.erase(m_it);
    }
}

void BeSubscriber::broadcast()
{
    for(auto it = observers_map.begin(); it != observers_map.end(); ++it)
    {
        MEMBER_KEY key;
        key.user_id = it->second->get_user_id();
        key.ship_id = it->second->get_ship_id();
        std::cout<<"FUNCTION: "<<__FUNCTION__<<"() "<<__LINE__<<" team_id= "<<get_team_id()
            <<" user_id= "<<key.user_id
            <<" ship_id= "<<key.ship_id
            <<" it->second= "<<it->second
            <<" use_count= "<<it->second.use_count()
            <<" \n";

        auto o_it = memebers_info_map.find(key);
        if(o_it == memebers_info_map.end())
            continue;

        // 推送ID包
        (it->second)->pushTeamStatusIDPack(key.user_id);
        std::cout<<"FUNCTION: "<< __FUNCTION__<<"() "<<__LINE__<<" team_id= "<<get_team_id()
            <<" user_id= "<<key.user_id <<" memebers_info_map.size= "<<memebers_info_map.size() <<" \n";
    }
    std::cout<<"FUNCTION: "<<__FUNCTION__<<"() "<<__LINE__<<" team_id= "<<get_team_id()
        <<" observers_count= "<<get_observers_count()
        <<" members_count= "<<get_members_info_count()<<" \n";

}

// 贸易 订阅仇敌通知
Observer::Observer(BeSubscriber* b_p, unsigned long long int uid, unsigned long long int sid)
{
    user_id = uid;
    ship_id = sid;
    besubscriber_ptr = b_p;
    std::cout<<"FUNCTION: "<<__FUNCTION__<<"() "<<__LINE__<<" user_id= "<<user_id<<" ship_id= "<<ship_id<<" besubscriber_ptr= "<<besubscriber_ptr<<" \n";
    besubscriber_ptr->attach(user_id, ship_id, auto_observer_ptr(this) );
}

Observer::Observer(Observer* right)
{
    user_id = right->get_user_id();
    ship_id = right->get_ship_id();
    besubscriber_ptr = right->besubscriber_ptr;
    std::cout<<"FUNCTION: "<<__FUNCTION__<<"() "<<__LINE__<<" copy_struct  user_id"<<user_id<<" ship_id= "<<ship_id<<" \n";
}

/////// 订阅者
void Observer::pushTeamStatusIDPack(unsigned long long int uid)
{
	std::cout<<"FUNCTION: "<<__FUNCTION__<<"() "<<__LINE__<<" push_user_id= "<<uid<<" \n";
}


// 主函数入口
int main(int argc, char* argv[]) {

    BeSubscriber subscriber(123456);

    int chg_code_1 = 0;
    chg_code_1 |= enum_member_chg_new;
    unsigned long long int user_1 = 9001;
    unsigned long long int ship_1 = 3001;
    TEAM_INFO_PTR team_ptr_1 = TEAM_INFO_PTR(new TEAM_STATUS_INFO());
    subscriber.member_update(user_1, ship_1, chg_code_1, team_ptr_1);

    int chg_code_2 = 0;
    chg_code_2 |= enum_member_chg_new;
    unsigned long long int user_2 = 9002;
    unsigned long long int ship_2 = 3002;
    TEAM_INFO_PTR team_ptr_2 = TEAM_INFO_PTR(new TEAM_STATUS_INFO());
    subscriber.member_update(user_2, ship_2, chg_code_2, team_ptr_2);

    int chg_code_3 = 0;
    chg_code_3 |= enum_member_chg_new;
    unsigned long long int user_3 = 9003;
    unsigned long long int ship_3 = 3003;
    TEAM_INFO_PTR team_ptr_3 = TEAM_INFO_PTR(new TEAM_STATUS_INFO());
    subscriber.member_update(user_3, ship_3, chg_code_3, team_ptr_3);


}

