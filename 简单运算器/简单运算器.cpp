// 简单运算器.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <climits>
#include <algorithm>
#include <fstream>
using namespace std;

class Solution {
  public:
	
	  Solution(){
		  allSpilit.push_back("(");
		  allSpilit.push_back(")");
		  allSpilit.push_back("（");
		  allSpilit.push_back("）");
		  allSpilit.push_back("+");
		  allSpilit.push_back("-");
		  allSpilit.push_back("*");
		  allSpilit.push_back("/");
		  allSpilit.push_back("=");
		  allSpilit.push_back(" ");
	  }


private:
    double toDouble(string src) {
        stringstream ss;
        ss << src;
        double result;
        ss >> result;
        return result;
    }

    bool isSplit(char c) {
        if (find(allSpilit.begin(),allSpilit.end(),string(1,c))!=allSpilit.end()) {
            return true;
        }
        return false;
    }

    //结尾加入一个特殊的=处理
    vector<string> getAllPatterns(string src) {

        //前向处理,首部空格，结尾等号
        src = " " + src + "=";
        //开始寻找分隔符
        vector<int> indexArray;
        for (int i = 0; i < src.size(); i++) {
            auto temp = src[i];
            if (isSplit(temp)) {
                indexArray.push_back(i);
            }
        }

        //放入结果集
        vector<string> result;
        for (int i = 1; i < indexArray.size(); i++) {
            auto lastPos = indexArray[i - 1];
            auto curPos = indexArray[i];
            //开头空格不存入，其他的前缀都存入
            if (src[lastPos] != ' ') {
                result.push_back(string(1, src[lastPos]));
            }
            //如果两个分隔符中间有数字的话，那么数字也存入
            if (curPos - lastPos > 1) {
                string temp = src.substr(lastPos + 1, curPos - 1 - lastPos - 1 + 1);
                result.push_back(temp);
            }
        }

        return result;
    }

    //运算符优先级
    int getPriority(char c) {
        switch (c) {
            case '#':
                return 1;
            case '+':
            case '-':
                return 2;
            case '*':
            case '/':
                return 3;
        }
        return INT_MIN;
    }


    //判断是不是加减乘除四种运算之一
    bool isFourOp(string c) {
        if (c == "+" || c == "-" || c == "*" || c == "/")
            return true;
        return false;
    }

    //判别是不是数字
    bool isDigit(string c) {
        return !isSplit(c[0]);
    }


    //相关操作运算
    double getSingleResult(double d1, char op, double d2) {
        switch (op) {
            case '+':
                return d1 + d2;
            case '-':
                return d1 - d2;
            case '*':
                return d1 * d2;
            case '/':
                return d1 / d2;
        }

        return -1005;
    }

    double getMultiResult(vector<string>input) {
        stack<double> digit;
        stack<char> op;
        //开始要加入一个#保证第一个元素也能够入栈
        op.push('#');

        int i = 0;
        while (i != input.size()) {
            string current = input[i];
            //如果是数字，则直接入栈
            if (isDigit(current)) {
                digit.push(toDouble(current));
                i++;
            }
            //如果是运算符的话
            else {
                //如果是四种运算符的话
                if (isFourOp(current)) {
                    //如果栈顶元素是括号，选择直接入栈
                    if (op.top() == '('||op.top() == '（') {
                        op.push(current[0]);
                        i++;
                    }
                    //否则需要根据四种运算符的优先级来进行
                    else {
                        //如果当前运算符优先级比栈顶元素高，则直接入栈
                        if (getPriority(op.top()) < getPriority(current[0])) {
                            op.push(current[0]);
                            i++;
                        }
                        //否则对栈顶元素进行出栈操作
                        else {
                            double right = digit.top();
                            digit.pop();

                            double left = digit.top();
                            digit.pop();

                            char opChar = op.top();
                            op.pop();

                            double sum = getSingleResult(left, opChar, right);
                            digit.push(sum);
                        }
                    }
                }
                //如果是括号运算符的话
                else if (current[0] == '('||op.top() == '（') {
                    op.push(current[0]);
                    i++;
                }
                //否则就是最后一种右括号的情况，此时需要连续弹栈
                else if (current[0] == ')'||op.top() == '）') {
                    while (op.top() != '('||op.top() == '（') {
                        double right = digit.top();
                        digit.pop();

                        double left = digit.top();
                        digit.pop();

                        char opChar = op.top();
                        op.pop();

                        double sum = getSingleResult(left, opChar, right);
                        digit.push(sum);
                    }
                    op.pop();
                    i++;
                }
            }
        }

        //遍历结束，数字栈和符号栈中可能还有多余的元素，则需要对数字栈进行继续运算操作，直到数字栈只有一个元素为止(或者符号栈只有最开始加入的一个#为止)
        while (digit.size() != 1) {
            double right = digit.top();
            digit.pop();

            double left = digit.top();
            digit.pop();

            char opChar = op.top();
            op.pop();

            double sum = getSingleResult(left, opChar, right);
            digit.push(sum);
        }

        return digit.top();
    }

public:
    int solve(string s) {
        // write code here
        vector<string> patterns = getAllPatterns(s);
        return getMultiResult(patterns);
    }

private:
	vector<string> allSpilit;

};

int main(){
	string input;
	Solution s;
	//cin>>input;
	//cout<<s.solve(input)<<endl;

	fstream file("input.txt");
	if (!file.is_open())
	{
		cout<<"第一次初始化创建文件完成,请重新打开可执行文件,并在input.txt中按行写入表达式"<<endl;
		FILE* target=fopen("input.txt","w+");
		file.open("input.txt");
	}

	while(getline(file,input)){
		if (input.empty())
		{
			break;
		}
		cout<<input;
		if (input[input.size()-1]!='=')
		{
			cout<<"=";
		}
		cout<<s.solve(input)<<endl;

	}

	file.close();
	system("pause");
	return 0;
}
