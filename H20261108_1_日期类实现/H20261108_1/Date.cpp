#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;

class Date
{
    // 声明友元函数，允许它们访问 private 成员
    friend ostream& operator<<(ostream& _cout, const Date& d);
    friend istream& operator>>(istream& _cin, Date& d);

public:
    // 获取某年某月的天数
    int GetMonthDay(int year, int month)
    {
        static int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        int day = days[month];
        // 判断闰年
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        {
            day += 1;
        }
        return day;
    }

    // 判断日期是否合法
    bool IsValid()
    {
        if (_year < 1 || _month < 1 || _month > 12 || _day < 1 || _day > GetMonthDay(_year, _month))
        {
            return false;
        }
        return true;
    }

    // 全缺省的构造函数（默认年份为 1）
    Date(int year = 1, int month = 1, int day = 1)
    {
        if (year < 1 || month < 1 || month > 12 || day < 1 || day > GetMonthDay(year, month))
        {
            cout << "非法日期" << endl;
        }
        _year = year;
        _month = month;
        _day = day;
    }

    // 拷贝构造函数
    Date(const Date& d)
    {
        this->_year = d._year;
        _month = d._month;
        _day = d._day;
    }

    // 赋值运算符重载 (d2 = d3)
    Date& operator=(const Date& d)
    {
        if (this != &d) // 防止自赋值
        {
            this->_year = d._year;
            this->_month = d._month;
            this->_day = d._day;
        }
        return *this;
    }

    // 析构函数
    ~Date()
    {
        // 日期类没有动态开辟的内存，不需要清理
    }

    // 日期 += 天数 (d1 += 10)
    Date& operator+=(int day)
    {
        if (day < 0)
        {
            return *this -= -day; // 加上负数等于减去正数
        }
        _day += day;
        // 如果天数超过了当月最大天数，就进位
        while (_day > GetMonthDay(_year, _month))
        {
            _day -= GetMonthDay(_year, _month);
            _month++;
            if (_month == 13)
            {
                _year++;
                _month = 1;
            }
        }
        return *this;
    }

    // 日期 + 天数 (d1 + 10)
    Date operator+(int day)
    {
        Date ret(*this); // 拷贝一份，不改变原本的日期
        ret += day;
        return ret; // 返回新对象
    }

    // 日期 -= 天数 (d1 -= 10)
    Date& operator-=(int day)
    {
        if (day < 0)
        {
            return *this += -day;
        }
        _day -= day;
        // 如果天数 <= 0，说明借位了
        while (_day <= 0)
        {
            --_month;
            if (_month == 0)
            {
                --_year;
                _month = 12;
            }
            _day += GetMonthDay(_year, _month); // 补上上个月的天数
        }
        return *this;
    }

    // 日期 - 天数 (d1 - 10)
    Date operator-(int day)
    {
        Date ret(*this);
        ret -= day;
        return ret;
    }

    // 前置++ (++d)
    Date& operator++()
    {
        *this += 1;
        return *this;
    }

    // 后置++ (d++)
    Date operator++(int) // int是占位符，区分前置和后置
    {
        Date ret(*this); // 保存原来的值
        *this += 1;
        return ret; // 返回原来的值
    }

    // 前置-- (--d)
    Date& operator--()
    {
        *this -= 1;
        return *this;
    }

    // 后置-- (d--)
    Date operator--(int)
    {
        Date ret(*this);
        *this -= 1;
        return ret;
    }

    // >运算符重载
    bool operator>(const Date& d)
    {
        if (_year > d._year)
            return true;
        else if (_year == d._year)
        {
            if (_month > d._month)
                return true;
            else if (_month == d._month)
            {
                if (_day > d._day)
                    return true;
            }
        }
        return false;
    }

    // ==运算符重载
    bool operator==(const Date& d)
    {
        return _year == d._year && _month == d._month && _day == d._day;
    }

    // 复用上面两个实现其他的比较运算符
    bool operator>=(const Date& d)
    {
        return *this > d || *this == d;
    }

    bool operator<(const Date& d)
    {
        return !(*this >= d);
    }

    bool operator<=(const Date& d)
    {
        return !(*this > d);
    }

    bool operator!=(const Date& d)
    {
        return !(*this == d);
    }

    // 日期 - 日期 返回天数 (d1 - d2)
    int operator-(const Date& d)
    {
        int flag = 1;
        Date max = *this;
        Date min = d;
        // 找出谁大谁小
        if (*this < d)
        {
            max = d;
            min = *this;
            flag = -1; // 如果左边小，结果为负数
        }

        int day = 0;
        // 让小的日子一天天加上去，直到等于大的日子
        while (min < max)
        {
            ++(min);
            ++day;
        }
        return day * flag; // 根据大小关系加上正负号
    }

private:
    int _year;
    int _month;
    int _day;
};

// ================= 类外实现输入输出 =================

// 输出流重载 (cout << d)
ostream& operator<<(ostream& _cout, const Date& d)
{
    // 因为加了 friend，这里可以直接访问 d._year
    _cout << d._year << "/" << d._month << "/" << d._day;
    return _cout;
}

// 输入流重载 (cin >> d)
istream& operator>>(istream& _cin, Date& d)
{
    while (1)
    {
        cout << "请依次输入年月日: ";
        _cin >> d._year >> d._month >> d._day;

        // 调用类里面的 IsValid 检查合法性
        if (d.IsValid())
        {
            break;
        }
        else
        {
            cout << "日期非法，请重新输入" << endl;
        }
    }
    return _cin;
}