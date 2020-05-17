#ifndef MYCALCULATOR_H
#define MYCALCULATOR_H
#include <iostream>
#include <cstring>
using namespace std;
string infixtopostfix(string str);  //中缀计算表达式转后缀计算表达式
bool priority(char a, char b);      //辅助函数 判断两个运算符的优先级
bool isnum(char str);               //判断是否是数字
void definite_digit(int* bit, string& str);       //获得位数
void signtoenglish(string& str);      //将字符串中所有成对出现的中文圆括号替换成英文圆括号
void bracketbnum(string& str);     //确定括号是否成对出现
int compute(string str, int* bit);  //计算后缀表达式
int power(int a, int c);    //计算幂的函数
int factorial(int a);               //计算阶乘的函数

#endif // MYCALCULATOR_H
