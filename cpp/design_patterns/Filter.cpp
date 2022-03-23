/*
* Filter Pattern 过滤器模式
*  属于结构性模式
* 使用场景: 
* 优点: 
* 缺点: 
*/

// #include <thread>
// #include <memory>
#include <iostream>
#include <vector>

using namespace std;


class Person{
public:
	Person(string n, string g, string m)
	{
		this->name = n;
		this->gender = g;
		this->marital_status = m;
	}
	~Person() {}
public:
	string getName() const {
		return this->name;
	}
	string getGender() const {
		return this->gender;
	}
	string getMaritalStatus() const {
		return this->marital_status;
	}
	// friend 防止访问非public函数或成员变量
	friend std::ostream& operator << (std::ostream & os, const Person& person) {
		os << " Person : { Name: "+ person.getName() + " Gender: " + person.getGender() + " Marital Status: " + person.getMaritalStatus() + " } \n";
		return os;
	}

private:
	string name;
	string gender; // 性别 Male  Female
	string marital_status; // Single Married
};

// 接口类
class Criteria{
public:
	virtual vector<Person> meetCriteria(const vector<Person>& persons) { std::cout<<" Criteria test. \n"; }
};

// 男性
class CriteriaMale:public Criteria{
public:
	vector<Person> meetCriteria(const vector<Person>& persons) {
		vector<Person> male_persons;
		for(vector<Person>::const_iterator it = persons.begin(); it != persons.end(); ++it)
		{
			if(it->getGender() == "Male")
			{
				male_persons.push_back(*it);
			}
		}
		return male_persons;
	}
};

// 女性
class CriteriaFemale:public Criteria{
public:
	vector<Person> meetCriteria(const vector<Person>& persons) {
		vector<Person> female_persons;
		for(vector<Person>::const_iterator it = persons.begin(); it != persons.end(); ++it)
		{
			if(it->getGender() == "Female")
			{
				female_persons.push_back(*it);
			}
		}
		return female_persons;
	}
};

// 单身
class CriteriaSingle:public Criteria{
public:
	vector<Person> meetCriteria(const vector<Person>& persons) {
		vector<Person> single_persons;
		for(vector<Person>::const_iterator it = persons.begin(); it != persons.end(); ++it)
		{
			if(it->getMaritalStatus() == "Single")
			{
				single_persons.push_back(*it);
			}
		}
		return single_persons;
	}
};

// 交集
class AndCriteria:public Criteria{
public:
	// 构造函数
	AndCriteria(Criteria* one, Criteria* other) {
		this->one_persons = one;
		this->other_persons = other;
	}
	// 默认析构函数
public:
	vector<Person> meetCriteria(const vector<Person>& persons) {
		vector<Person> first = one_persons->meetCriteria(persons);
		return other_persons->meetCriteria(first);
	}
private:
	Criteria* one_persons;
	Criteria* other_persons;
};

// 并集
class OrCriteria:public Criteria{
public:
	// 构造函数
	OrCriteria(Criteria* one, Criteria* other) {
		this->one_persons = one;
		this->other_persons = other;
	}
	// 默认析构函数
public:
	vector<Person> meetCriteria(const vector<Person>& persons) {
		vector<Person> first = one_persons->meetCriteria(persons);
		vector<Person> second = other_persons->meetCriteria(persons);
		for(vector<Person>::iterator it = second.begin(); it != second.end(); ++it) {
			// if( std::find(first.begin(), first.end(), *it) == first.end() ) {
			first.push_back(*it); // TODO 会有重复
			// }
		}
		return first;
	}
private:
	Criteria* one_persons;
	Criteria* other_persons;
};

// 打印
static void printPersons(const vector<Person>& persons) {
	for(vector<Person>::const_iterator it = persons.begin(); it != persons.end(); ++it) {
		std::cout<<(*it)<<"\n";
	}
	std::cout<<"\n";
}

int main() {
	vector<Person> persons;

    persons.push_back(Person("Robert","Male", "Single"));
    persons.push_back(Person("John","Male", "Married"));
    persons.push_back(Person("Laura","Female", "Married"));
    persons.push_back(Person("Diana","Female", "Single"));
    persons.push_back(Person("Mike","Male", "Single"));
    persons.push_back(Person("Bobby","Male", "Single"));
 
    Criteria* male = new CriteriaMale();
    Criteria* female = new CriteriaFemale();
    Criteria* single = new CriteriaSingle();
    Criteria* singleMale = new AndCriteria(single, male);
    Criteria* singleOrFemale = new OrCriteria(single, female);
 
    std::cout<<"Males: \n";
    printPersons(male->meetCriteria(persons));
 
    std::cout<<"Females: \n";
    printPersons(female->meetCriteria(persons));
 
    std::cout<<"SingleMale: \n";
    printPersons(singleMale->meetCriteria(persons));
 
    std::cout<<"Single Or Females: \n";
    printPersons(singleOrFemale->meetCriteria(persons));
}

