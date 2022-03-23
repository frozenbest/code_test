/*
* smart_ptr 智能指针
* 
* 使用场景: 
* 优点: 
* 缺点: 
*/


#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

template<typename T>
class smart_value{
public:
	typedef smart_value<T> __MyType;
public:
	smart_value(T* v = 0, bool auto_release = true):ref_count(0),__auto_release(auto_release),value(v) 
	{
	}; // 构造
	virtual ~smart_value() {
		if(value && __auto_release)
			delete value;
		value = 0;
	} // 析构

    /* 增加引用 */
	inline int add_ref()
	{
		return __sync_add_and_fetch(&ref_count, 1);
	}
	/* 减少引用 */
	inline int release_ref()
	{
		int result = __sync_sub_and_fetch(&ref_count, 1);
		if(0 != result)
			return result;
		delete this;
		return 0;
	}
    inline bool operator == (const __MyType& right)
    {
    	return 0 == value ? false : (value == right.value);
    }
    inline bool operator != (const __MyType& right)
    {
    	return 0 == value ? true : (value != right.value); 
    }
    inline bool operator == (const T& right)
    {
    	return 0 == value ? false : (&right == value);
    }
    inline bool operator != (const T& right)
    {
    	return 0 == value ? true : (&right != value)
    }
    inline bool operator == (T* right)
    {
    	return 0 == value ? false : (value == right);
    }
    inline bool operator != (T* right)
    {
    	return 0 == value ? true : (value != right);
    }
    inline bool operator ! () const
    {
    	return value ? false : true;
    }
    inline T& operator * () const
    {
    	return *value;
    }
    inline operator bool () const
    {
    	return value ? true : false;
    }
    inline operator T* () const
    {
    	return value;
    }

protected:
	volatile int ref_count; // 每次使用必须从其地址读取 不会被编译器优化省略掉
	bool __auto_release;
	T*   value;
};


template<typename T, template<typename E> class SV = smart_value>
class smart_ptr
{
public:
    typedef smart_ptr<T,SV> __MyType;
    typedef SV<T>           __ValueType;
    typedef T               __OriginalType;
public:
    smart_ptr()
    :ptr_value(0)
    {
    }
    smart_ptr(T* v)
    :ptr_value(new __ValueType(v))
    {
        ptr_value->add_ref();
    }
    smart_ptr(const __MyType& right)
    {
        ptr_value = right.ptr_value;
        if(ptr_value)
            ptr_value->add_ref();
    }
    smart_ptr(__MyType* right)
    {
        if(right)
        {
            ptr_value = right->ptr_value;
            if(ptr_value)
                ptr_value->add_ref();
        }
        else
        {
            ptr_value = 0;
        }
    }
    smart_ptr(__ValueType* right)
    {
        if(right)
        {
            ptr_value = right;
            if(ptr_value)
                ptr_value->add_ref();
        }
        else
        {
            ptr_value = 0;
        }
    }
    virtual ~smart_ptr()
    {
        if(ptr_value)
            ptr_value->release_ref();
        ptr_value = 0;
    }
    inline __MyType& operator = (const __MyType& right)
    {
        if(ptr_value)
            ptr_value->release_ref();
        ptr_value = right.ptr_value;
        if(ptr_value)
            ptr_value->add_ref();
        return *this;
    }
    inline __MyType& operator = (__ValueType* right)
    {
        if(ptr_value)
            ptr_value->release_ref();
        if(right)
        {
            ptr_value = right;
            if(ptr_value)
                ptr_value->add_ref();
        }
        else
        {
            ptr_value = 0;
        }
        return *this;
    }
    inline bool operator == (const __MyType& right)
    {
        return 0 == ptr_value ? false : (*right.ptr_value == *ptr_value);
    }
    inline bool operator == (const __MyType& right) const
    {
        return 0 == ptr_value ? false : (*right.ptr_value == *ptr_value);
    }
    inline bool operator != (const __MyType& right)
    {
        return 0 == ptr_value ? true : (*right.ptr_value != *ptr_value);
    }
    inline bool operator != (const __MyType& right) const
    {
        return 0 == ptr_value ? true : (*right.ptr_value != *ptr_value);
    }
    inline bool operator == (const __ValueType& right)
    {
        return 0 == ptr_value ? false : (right == *ptr_value);
    }
    inline bool operator == (const __ValueType& right) const
    {
        return 0 == ptr_value ? false : (right == *ptr_value);
    }
    inline bool operator != (const __ValueType& right)
    {
        return 0 == ptr_value ? true : (right != *ptr_value);
    }
    inline bool operator != (const __ValueType& right) const
    {
        return 0 == ptr_value ? true : (right != *ptr_value);
    }
    inline bool operator == (const T& right)
    {
        return 0 == ptr_value ? false : (*ptr_value == right);
    }
    inline bool operator == (const T& right) const
    {
        return 0 == ptr_value ? false : (*ptr_value == right);
    }
    inline bool operator != (const T& right)
    {
        return 0 == ptr_value ? true : (*ptr_value != right);
    }
    inline bool operator != (const T& right) const
    {
        return 0 == ptr_value ? true : (*ptr_value != right);
    }
    inline bool operator == (const T* right)
    {
        return 0 == ptr_value ? false : (*ptr_value == right);
    }
    inline bool operator == (const T* right) const
    {
        return 0 == ptr_value ? false : (*ptr_value == right);
    }
    inline bool operator != (const T* right)
    {
        return 0 == ptr_value ? true : (*ptr_value != right);
    }
    inline bool operator != (const T* right) const
    {
        return 0 == ptr_value ? true : (*ptr_value != right);
    }
    inline bool operator ! () const
    {
        return ptr_value ? !(*ptr_value) : true;
    }
    inline T& operator * () const
    {
        return *(*ptr_value);
    }
    inline operator bool () const
    {
        return ptr_value ? (bool)(*ptr_value) : false;
    }
    inline T* operator -> () const
    {
        if(0 == ptr_value)
            return 0;
        return (T*)(*ptr_value);
    }
    inline operator T* () const
    {
        if(0 == ptr_value)
            return 0;
        return (T*)(*ptr_value);
    }
    inline T* get() const
    {
        if(0 == ptr_value)
            return 0;
        return (T*)(*ptr_value);
    }

    template<typename X>
    inline X* convert_to()
    {
        if(0 == ptr_value)
            return 0;
        return (X*)((T*)(*ptr_value))
    }
    // 强转
    template<typename X>
    inline const X* convert_to()
    {
        if(0 == ptr_value)
            return 0;
        return (X*)((T*)(*ptr_value))
    }

private:
    __ValueType* ptr_value;
};





#endif // __SMART_PTR__






// 主函数入口
int main(int argc, char* argv[]) {

	std::cout<<" nonveg_meal "<<nonveg_meal<<"\n";


}

