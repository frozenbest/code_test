/*
* 单例模式
*/

// 单例模板
#ifndef __SINGLETON_TEMPLATE__
#define __SINGLETON_TEMPLATE__
template<typename InstanceType>
class SingletonTemplate
{
public:
	static void Init(InstanceType* a_instance) {
		m_instance = a_instance;
	}
	static InstanceType* Intance() {
	    return m_instance;		
	}

    // SingletonTemplate() {}
	// ~SingletonTemplate() {}
private:
	static InstanceType* m_instance;
	
};

template<typename InstanceType>
InstanceType* SingletonTemplate<InstanceType>::m_instance = NULL;

#endif // __SINGLETON_TEMPLATE__




