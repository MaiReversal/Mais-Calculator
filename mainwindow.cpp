#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstring>
#include <QString>
#include <stack>
#include <conio.h>
using namespace std;
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(410, 556);
    flag = false;
    empty = false;
    ui->textEdit->setText("please press the button.");
}
MainWindow::~MainWindow()
{
    delete ui;
}
string MainWindow::infixtopostfix()
{
    string res;
    stack<char> st;
    double size = str.size();
    for (double i = 0; i < size; i++)
    {
        if (isnum(str[i]))
            res += str[i];//数字直接放入算术表达式
        if (str[i] == '(')//'('左括号优先级最高 直接入栈
        {
            st.push(str[i]);
        }
        if ((str[i] != '(') && (str[i] != ')') && (!isnum(str[i])))//不是数字和左右圆括号即'*' '+' '-''/'
        {
            if (st.empty())
                st.push(str[i]);//栈空 算术符号入栈
            else//否则根据算术符号优先级出栈
                while (st.size() != 0)   //循环的作用是再执行else后并且栈非空，能进行下一次判断
                {                   //其实括号内写个大于0的数更好，但是为了美观，还是写st.size() != 0
                    char temp = st.top();//栈顶算术符号
                    if (priority(str[i], temp))//栈顶算术符号优先级高于当前算术符号
                    {
                        st.push(str[i]);//入栈
                        break;//出循环
                    }
                    else
                    {
                        res += temp;//否则栈顶算术符号放入算术表达式
                        st.pop();//直到当前算术符号优先级小于栈顶算术符号
                        if (st.empty())//如果栈空 那么当前算术符号入栈
                        {
                            st.push(str[i]);
                            break;//出循环
                        }
                    }
                }

        }
        if (str[i] == ')')//如果是右括号
        {
            while (st.top() != '(')//算术符号出栈 直到栈顶为左括号
            {
                res += st.top();   //这步不能省略，因为构造的符号不是string型
                st.pop();
            }
            st.pop();//'('出栈 且不放入算术表达式
        }
    }
    while (!st.empty())//栈中剩余算术符号放入算术表达式
    {
        res += st.top();
        st.pop();
    }
    return res;//转换后的算术表达式
}
string MainWindow::dectobin(double& x)
{
    string temp;
    bool positive;
    int i;
    int a[100] = { 0 };
    if(x > 0)
    {
        i = x;
        positive = true;
    }
    else
    {
        i = -x;
        positive = false;
    }
    int j = 0;
    while (i != 0)
    {
        a[j] = i % 2;
        i /= 2;
        j++;
    }
    if(positive)
        a[j] = 0;
    else
        a[j] = 1;
    if(x != 0)
    {
        for(int z = j; z >= 0; z--)
            temp += to_string(a[z]);
    }
    else
    {
      temp = "0";

    }
    return temp;
}
double MainWindow::compute(string str, double* bit)//根据后缀算术表达式计算值
{
    stack<double> st;
    int x = -1;
    for (unsigned int i = 0; i < str.size(); i++)
    {
        double temp = 0;           //temp储存数字
        if (isnum(str[i]))
        {
            x++;
            while (bit[x]--)     //bit[x]的值表示第x+1个数字的位数
            {                   //执行顺序，循环条件判断->bit[x]自减->循环开始
                temp *= 10;     //用于处理位数大于一时的情况，用于将第一位数在十进制下左移一位
                temp += (str[i] - '0');//将字符型转化为整形
                if (bit[x] > 0)  //如果数字不是一位的，则马上处理下一位数字，跳过下面的符号判断
                    i++;            //例如在while循环前bit[x]是2，则表示该数字是十位为字符串中第x个数字，个位为字符串中第x+1个数字
            }
            st.push(temp);      //数字转换完成后入栈
        }
        if (!isnum(str[i]) && str[i] != '!' && str[i] != '|' && str[i] != '?' && str[i] != '@')//执行双目运算符入栈//后缀表示法遇到+-*/^即进行计算
        {
            double a, b;
            if (st.size() > 1 && str[i] != '@')
            {
                a = st.top();
                st.pop();
                b = st.top();//取栈顶两元素
                st.pop();
            }           
            if (str[i] == '+')
                st.push(b + a);//加法的和入栈
            if (str[i] == '-')
                st.push(b - a);//减法的差入栈
            if (str[i] == '*')
                st.push(b * a);//乘法的积入栈
            if (str[i] == '/')
            {
                if (a == 0)  //对于除法不规范输入的处理
                {
                    flag = true;
                    return 0;
                }
                else
                    st.push(b / a);//除法的商入栈
            }
            if (str[i] == '^')//幂运算
            {
                a = power(b, a);
                st.push(a);//幂入栈
            }
            if (str[i] == '{')
            {
                double temp = AND(b, a);
                st.push(temp);
            }
            if (str[i] == '}')
            {
                double temp = OR(b, a);
                st.push(temp);
            }
            if (str[i] == '<')
            {
                double temp = logicand(b, a);
                st.push(temp);
            }
            if (str[i] == '>')
            {
                double temp = logicor(b, a);
                st.push(temp);
            }
        }
        if (str[i] == '!')//阶乘是单目运算符，单独讨论
        {
            if(st.size() != 0)
            {
                double a = st.top();
                st.pop();
                a = factorial(a);
                st.push(a);//阶乘计算结果入栈
            }
            else
            {
                st.push(0);
                empty = true;
            }
        }
        if (str[i] == '|')
        {
            if(st.size() != 0)
            {
                double a = st.top();
                st.pop();
                a = NOT(a);
                st.push(a);//阶乘计算结果入栈
            }
            else
            {
                st.push(0);
                empty = true;
            }
        }
        if (str[i] == '?')
        {
            if(st.size() != 0)
            {
                double a = st.top();
                st.pop();
                a = logicnot(a);
                st.push(a);
            }
            else
            {
                st.push(0);
                empty = true;
            }
        }
        if (str[i] == '@')
        {
            if(st.size() != 0)
            {
                double a = st.top() * -1;
                st.pop();
                st.push(a);
            }
            else
            {
                st.push(0);
                empty = true;
            }
        }
    }
    if(st.size() > 0)
        return st.top();//返回后缀表达式的计算结果
    else
        return 0;
}
double MainWindow::power(double a, double c)         //幂运算的结果
{
    double temp = 1;
    while (c != 0)
    {
        temp *= a;
        c--;
    }
    return temp;
}
double MainWindow::factorial(double a)                    //阶乘运算的函数
{
    double temp = 1;
    while (a > 0)
    {
        temp *= a;
        a--;
    }
    return temp;
}
double MainWindow::AND(double& x, double& y)
{
        string a = dectobin(x);
        string b = dectobin(y);
        a = tftocr(a);
        b = tftocr(b);
        extended(a, b);
        string ans;
        double temp = 0;
        for (unsigned int i = 0; i < a.size(); i++)
        (a[i] == '1' && b[i] == '1') ? ans += '1' : ans += '0';
        ans = tftocr(ans);
        for (double i = ans.size(), j = 0; i > 1; i--)
        {
            temp += (ans[i - 1] - '0') * power(2, j);
            j++;
        }
        if(ans[0] == '1')
        {
            temp *= -1;
            temp--;
        }
        return temp;
}
double MainWindow::OR(double& x, double& y)
{
        string a = dectobin(x);
        string b = dectobin(y);//获得原码
        a = tftocr(a);
        b = tftocr(b);
        extended(a, b);
        string ans;
        double temp = 0;
        for (unsigned int i = 0; i < a.size(); i++)
        (a[i] == '1' || b[i] == '1') ? ans += '1' : ans += '0';
        ans = tftocr(ans);
        for (double i = ans.size(), j = 0; i > 1; i--)
        {
            temp += (ans[i - 1] - '0') * power(2, j);
            j++;
        }
        if(ans[0] == '1')
        {
            temp *= -1;
            temp--;
        }
        return temp;

}
double MainWindow::NOT(double x)
{
    if (x >= 0)
    {
        double sign;
        (x >= 0) ? sign = -1 : sign = 1;
        x++;
        string ans = dectobin(x);
        double temp = 0;
        for (double i = ans.size(), j = 0; i > 0; i--)
        {
            temp += (ans[i - 1] - '0') * power(2, j);
            j++;
        }
        return temp * sign;
    }
    else
        return 0;

}
bool MainWindow::past()
{
    return (!isnum(str.back()) && str.back() != ')');
}
bool MainWindow::logicnot(double& x)
{
    return (x == 0);
}

bool MainWindow::priority(char a, char b)
{   //由于右圆括号不入栈，故栈中最多有+-*/(!^五种符号(可能与中文冲突的阶乘和括号已事先处理)
    bool temp = (b == '+' || b == '-' || b == '*' || b == '/');//临时的变量，表示b是+-*/中的一个
    //算术优先级a>b 返回true
    if (((a == '+') || (a == '-')) && (b == '('))//由于(括号优先级最低故单独讨论
        return true;
    if (((a == '*') || (a == '/')) && temp)       //这+-*/(这五个运算符中显然*/优先级最高
        return true;
    if (((a != '+') && (a != '-') && (a != '*') && (a != '/')) && b != '@') //显然，幂运算和阶乘运算优先级最高
        return true;
    else
        return false;
}
bool MainWindow::isnum(char str)
{
    return (str >= '0' && str <= '9');               //判断用，条件太长，写成布尔函数方便些
}
bool MainWindow::logicand(double& x, double& y)
{
    if (x != 0 && y != 0)
        return 1;
    else
        return 0;
}
bool MainWindow::logicor(double& x, double& y)
{
    return (x != 0 || y != 0);
}
void MainWindow::signtoenglish()
{
    //处理括号用，将字符串中成对出现的中文括号替换为英文括号
    while (str.find("（") != str.npos)               //string::nops是一个用来表示没有找到该字符串的的常数
    {
        str.replace(str.find("（"), 2, "(");
    }
    while (str.find("）") != str.npos)
    {
        str.replace(str.find("）"), 2, ")");
    }
    //处理中文！符号,将其替换为英文!，ps ^不用处理，中文没有这个符号
    while (str.find("！") != str.npos)
    {
        str.replace(str.find("！"), 2, "!"); //中文感叹号转阶乘符号
    }
    bracketbnum();
}
void MainWindow::reset()
{
    str.clear();
    str1.clear();
    status.clear();
    flag = false;
    empty = false;
    ui->textEdit->setText("please press the button.");
}
bool MainWindow::bracketbnum()
{
    unsigned int i = 0;
    double left = 0, right = 0;
    while (i < str.length() && left >= right)   //统计左右括号的个数，由于每一步都会先判断左括号数目大于等于右括号，当出现
    {                                          //一组括号不匹配时例如（1+2）+）3+4
        if (str[i] == '(')                      //执行到第三个括号时，由于right > left ,即可判断括号不匹配
        {
            left++;
            i++;
            continue;
        }
        if (str[i] == ')')                       //不能用else，因为符号可能并不是这两个中的任何一个
        {
            right++;
            i++;
            continue;
        }
        i++;
    }
    if (left != right)                           //判断括号是否匹配
    {
        reset();
        return false;
    }
    return true;
}
void MainWindow::definite_digit(double* bit)          //算数字位数用
{
    int x = -1;
    for (unsigned int i = 0; i < str.size(); i++)             //遍历字符串
    {
        if (isnum(str[i]))                           //找到数字
        {
            x++;                                    //数字数量加一
            bit[x]++;                               //位数加一
            while (isnum(str[i + 1]) && i + 1 < str.size())    //如果字符串后一位仍然是数字且数组没越界
            {
                bit[x]++;                           //位数再加一
                i++;                                //计算字符串的下一位
            }
        }
    }
}
void MainWindow::extended(string& a, string& b)
{
    if (a.size() > b.size())
    {
        double temp = a.size() - b.size();
        string::iterator it;
        it = b.begin();
        b.insert(it, temp, b[0]);


    }
    else
    {
        double temp = b.size() - a.size();
        string::iterator it;
        it = a.begin();
        a.insert(it, temp, a[0]);
    }
}
void MainWindow::on_num0_clicked()
{
    str += '0';
    str1 += '0';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num1_clicked()
{
    str += '1';
    str1 += '1';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num2_clicked()
{
    str += '2';
    str1 += '2';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num3_clicked()
{
    str += '3';
    str1 += '3';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num4_clicked()
{
    str += '4';
    str1 += '4';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num5_clicked()
{
    str += '5';
    str1 += '5';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num6_clicked()
{
    str += '6';
    str1 += '6';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num7_clicked()
{
    str += '7';
    str1 += '7';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num8_clicked()
{
    str += '8';
    str1 += '8';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_num9_clicked()
{
    str += '9';
    str1 += '9';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_plus_clicked()
{
    if(past())
        return;
    str += '+';
    str1 += '+';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_minus_clicked()
{
    if(past())
        return;
    str += '-';
    str1 += '-';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_multiplication_clicked()
{
    if(past())
        return;
    str += '*';
    str1 += '*';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_division_clicked()
{
    if(past())
        return;
    str += '/';
    str1 += '/';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_factorial_clicked()
{
    str += '!';
    str1 += '!';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_power_clicked()
{
    if(past())
        return;
    str += '^';
    str1 += '^';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_leftparenthesis_clicked()
{
    str += '(';
    str1 += '(';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_rightparenthesis_clicked()
{
    str += ')';
    str1 += ')';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_AND_clicked()
{
    if (!isnum(str.back()))
        return;
    str += '{';
    str1 += '&';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_OR_clicked()
{
    if (!isnum(str.back()))
        return;
    str += '}';
    str1 += '|';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_NOT_clicked()
{
    str += '|';
    str1 += '!';
    status += '1';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_logicand_clicked()
{
    if (!isnum(str.back()))
        return;
    str += '<';
    str1 += '&';
    str1 += '&';
    status += '2';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_logicor_clicked()
{
    if (!isnum(str.back()))
        return;
    str += '>';
    str1 += '|';
    str1 += '|';
    status += '2';
    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_logicnot_clicked()
{
    str += '?';
    str1 += '!';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_nagete_clicked()
{
    str += '@';
    str1 += '-';
    status += '1';

    QString temp = QString::fromStdString(str1);
    ui->textEdit->setText(temp);
}
void MainWindow::on_backspace_clicked()
{
    if (status.size() > 0)
    {
        str.pop_back();
        str1.pop_back();
        if (status.back() == '2')
            str1.pop_back();
        status.pop_back();
        QString temp = QString::fromStdString(str1);
        ui->textEdit->setText(temp);

    }
    else
        ui->textEdit->setText("please press the button.");


}
void MainWindow::on_clear_clicked()
{
    reset();
    ui->textEdit->setText("please press the button.");

}
void MainWindow::on_equal_clicked()
{


    if (!bracketbnum())
    {
        reset();
        ui->textEdit->setText("Bracket pairing errorr");
        return;
    }
    if(errorcheck())
    {
        reset();
        ui->textEdit->setText("Missing parameter");
        return;
    }
    if (str.size() > 0 && status.size() != 0)
    {
        signtoenglish();
        double bit[10] = { 0 };           //记录数字位数用
        definite_digit(bit);
        string str2 = infixtopostfix();//改写的后缀表达式
        int64_t n = compute(str2, bit);
        if (flag)
        {
            reset();
            ui->textEdit->setText("Divisor cannot be zero");
            return;
        }
        else
        {

            if(empty )
            {
                reset();
                ui->textEdit->setText("Missing parameter");
                return;

            }
            ans = QString::number(n);
            ui->textEdit->setText(ans);
            str = to_string(n);
            str1 = to_string(n);
            status.clear();
            return;
        }
        //str.clear();
        //str1.clear();
    }

}
bool MainWindow::errorcheck()
{
    if(!isnum(str.back()))
    {
        switch (str.back())
        {
            case '!':
            case '@':
            case '|':
            case '?':
            case ')':
            return false;break;
        default: return true;break;
        }
    }
    else
       return false;
}
string MainWindow::tftocr(string &str)
{

    if(str[0] == '0')
        return str;
    for(unsigned int i = 0; i < str.size(); i++)
    {
        (str[i] == '0') ? str[i] = '1' : str[i] = '0';
    }
    int size = str.size() - 1;
     if(str[size] == '0')
        str[size] = '1';
     else
        str[size] = '2';
     while(str[size] == '2' && size >= 0)
     {
         str[size] = '0';
         size--;
         (str[size] == '0') ? str[size] = '1' : str[size] = '2';
     }
     return str;
}
