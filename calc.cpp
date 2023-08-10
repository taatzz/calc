#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct elem
{
    float n;
    char c;
    bool flag;
};
stack<elem> num; // 数字栈
stack<elem> sign; // 符号栈

bool is_num(char c)
{
    if(c <= '9' && c >= '0')
        return true;
    else
        return false;
}

int sign_prio(elem e)
{
    if(e.c == '*' || e.c == '/')
        return 2;
    else if(e.c == '+' || e.c == '-')
        return 1;
    else
        return 0;
}
/* 中缀表达式转后缀表达式
    1.从左至右扫描表达式，当遇到数字的时候直接入数字栈
    2.当遇到符号时
        1）符号栈为空时直接入栈，遇到（ 是也直接入栈
        2) 当前要插入的符号优先级高于栈顶的符号优先级时直接入栈
        3) 当前要插入的符号优先级小于栈顶的符号优先级时依次将栈中小于插入符号优先级的符号出栈插入到数字栈中
           直到栈顶的符号优先级等于或大于要插入的符号优先级
        4）当前插入的符号是）时，依次出栈顶的元素直到遇到（，另外将（ 也出栈
    3.扫描完成将符号栈中的剩余的元素依次出栈插入到数字栈中
    4.将数字栈中的元素依次出栈插入到符号栈。
    5.符号栈中的就是当前的后缀表达式
*/
void bef_conv_after(string expression)
{   
    int i = 0;
    bool f = false; // 负数标志当为true时表示当前数字为负数
    while(i < expression.size())
    {
        if(expression[i] == ' ')
        {
            i++;
            continue;
        }
        elem tmp;
        if(is_num(expression[i]))
        {
            tmp.flag = true;
            tmp.n = expression[i] - '0';
            int j = i + 1;
            // 小数点前的位数
            while(j < expression.size() && is_num(expression[j]))
            {
                tmp.n = (tmp.n * 10) + expression[j] - '0';
                j++;
            }
            // 小数点后的位数
            if(expression[j] == '.')
            {
                int _aft = 1;
                j = j + 1;
                while(j < expression.size() && is_num(expression[j]))
                {
                    float _tmp = expression[j] - '0';
                    int x = _aft;
                    // x = 1 0.1,  x = 2 0.01,  x = 3 0.001
                    while(x > 0)
                    {
                        _tmp = _tmp * 0.1;
                        x--;
                    }
                    tmp.n += _tmp;
                    j++;
                    _aft++;
                }
            }
            // 负数
            if(f)
            {
                tmp.n = 0 - tmp.n;
                j++;
                f = false;
            }
            num.push(tmp);
            i = j;
        }
        else // 符号
        {
            tmp.c = expression[i];
            tmp.flag = false;
            if(sign.empty())
            {
                sign.push(tmp);
            }
            else
            {
                if(tmp.c == '(')
                {
                    if(expression[i + 1] == '-')
                    {
                        f = true;
                        i += 2;
                        continue;
                    }
                    sign.push(tmp);
                }
                else
                {
                    if(tmp.c == ')')
                    {
                        while(sign.top().c != '(')
                        {
                            num.push(sign.top());
                            sign.pop();
                        }
                        sign.pop();
                    }
                    else if(sign_prio(tmp) >= sign_prio(sign.top()))
                        sign.push(tmp);
                    else if(sign_prio(tmp) < sign_prio(sign.top()))
                    {
                        while(!sign.empty() && sign_prio(tmp) < sign_prio(sign.top()))
                        {
                            num.push(sign.top());
                            sign.pop();
                        }
                        sign.push(tmp);
                    }
                }
            }
            i++;
        }
    }
    while(!sign.empty())
    {
        num.push(sign.top());
        sign.pop();
    }
    while(!num.empty())
    {
        sign.push(num.top());
        num.pop();
    }
}

// void print()
// {
//     while(!sign.empty())
//     {
//         elem tmp;
//         tmp = sign.top();
//         if(tmp.flag == true)
//             cout << tmp.n << " ";
//         else
//             cout << tmp.c << " ";
//         sign.pop();
//     }
// }

elem comp(elem num1, elem num2, elem s)
{
    elem tmp;
    tmp.flag = true;
    if(s.c == '*')
        tmp.n = num1.n * num2.n;
    else if(s.c == '/')
        tmp.n = num1.n / num2.n;
    else if(s.c == '+')
        tmp.n = num1.n + num2.n;
    else
        tmp.n = num1.n - num2.n;
    return tmp;
}   

/* 计算后缀表达式
    1.依次扫描符号栈（后缀表达式）遇到数字直接插入到数字栈中
    2.当遇到符号时从数字栈中取出两个数字（先取出的数字为num2,后取出的数字为num1, num1 - num2）
    3.符号栈中为空时，数字栈中留下的元素就是表达式的结果
*/
void calc()
{
    int i = 0;
    while(!sign.empty())
    {
        if(sign.top().flag == true)
        {
            num.push(sign.top());
            sign.pop();
        }
        else
        {
            elem s, num1, num2;
            if(!sign.empty())
            {
                s = sign.top();
                sign.pop();
            }
            if(!num.empty())
            {
                num2 = num.top();
                num.pop();
            }
            if(!num.empty())
            {
                num1 = num.top();
                num.pop();
            }
        
            elem tmp = comp(num1, num2, s);
            num.push(tmp);
        }
    }
    cout << num.top().n << endl;
}

int main(int argc, char* argv[])
{
    string s = "((-3.11)+4)*5-6";
    bef_conv_after(s);
    //print();
    calc();

    return 0;
}