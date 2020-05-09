#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <iostream>
#include <cstring>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    string dectobin(double& x);              //十进制转二进制
    string infixtopostfix();  //中缀计算表达式转后缀计算表达式
    bool priority(char a, char b);      //辅助函数 判断两个运算符的优先级
    bool isnum(char str);               //判断是否是数字
    bool logicand(double& x, double& y);      //逻辑与
    bool logicor(double& x, double& y);        //逻辑或
    bool logicnot(double& x);               //逻辑非
    void definite_digit(double* bit);       //获得位数
    void signtoenglish();      //将字符串中所有成对出现的中文圆括号替换成英文圆括号
    bool bracketbnum();     //确定括号是否成对出现
    bool past();
    void extended(string& a, string& b); //对其二进制数位
    void reset();
    bool errorcheck();
    double compute(string str, double* bit);  //计算后缀表达式
    double power(double a, double c);    //计算幂的函数
    double factorial(double a);               //计算阶乘的函数
    double AND(double& x, double& y);            //按位与
    double OR(double& x, double& y);             //按位或
    double NOT(double x);                      //按位非


private slots:

    void on_num0_clicked();

    void on_num1_clicked();

    void on_num2_clicked();

    void on_num3_clicked();

    void on_num4_clicked();

    void on_num5_clicked();

    void on_num6_clicked();

    void on_num7_clicked();

    void on_num8_clicked();

    void on_num9_clicked();

    void on_backspace_clicked();

    void on_clear_clicked();

    void on_division_clicked();

    void on_equal_clicked();

    void on_minus_clicked();

    void on_multiplication_clicked();

    void on_nagete_clicked();

    void on_plus_clicked();

    void on_AND_clicked();

    void on_OR_clicked();

    void on_NOT_clicked();

    void on_logicand_clicked();

    void on_logicor_clicked();

    void on_logicnot_clicked();

    void on_leftparenthesis_clicked();

    void on_rightparenthesis_clicked();

    void on_factorial_clicked();

    void on_power_clicked();
private:
    Ui::MainWindow* ui;
    string str;
    string str1;
    string status;
    QString ans;
    bool flag;
    bool empty;
};
#endif // MAINWINDOW_H
