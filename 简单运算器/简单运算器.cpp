// ��������.cpp : �������̨Ӧ�ó������ڵ㡣
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
		  allSpilit.push_back("��");
		  allSpilit.push_back("��");
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

    //��β����һ�������=����
    vector<string> getAllPatterns(string src) {

        //ǰ����,�ײ��ո񣬽�β�Ⱥ�
        src = " " + src + "=";
        //��ʼѰ�ҷָ���
        vector<int> indexArray;
        for (int i = 0; i < src.size(); i++) {
            auto temp = src[i];
            if (isSplit(temp)) {
                indexArray.push_back(i);
            }
        }

        //��������
        vector<string> result;
        for (int i = 1; i < indexArray.size(); i++) {
            auto lastPos = indexArray[i - 1];
            auto curPos = indexArray[i];
            //��ͷ�ո񲻴��룬������ǰ׺������
            if (src[lastPos] != ' ') {
                result.push_back(string(1, src[lastPos]));
            }
            //��������ָ����м������ֵĻ�����ô����Ҳ����
            if (curPos - lastPos > 1) {
                string temp = src.substr(lastPos + 1, curPos - 1 - lastPos - 1 + 1);
                result.push_back(temp);
            }
        }

        return result;
    }

    //��������ȼ�
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


    //�ж��ǲ��ǼӼ��˳���������֮һ
    bool isFourOp(string c) {
        if (c == "+" || c == "-" || c == "*" || c == "/")
            return true;
        return false;
    }

    //�б��ǲ�������
    bool isDigit(string c) {
        return !isSplit(c[0]);
    }


    //��ز�������
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
        //��ʼҪ����һ��#��֤��һ��Ԫ��Ҳ�ܹ���ջ
        op.push('#');

        int i = 0;
        while (i != input.size()) {
            string current = input[i];
            //��������֣���ֱ����ջ
            if (isDigit(current)) {
                digit.push(toDouble(current));
                i++;
            }
            //�����������Ļ�
            else {
                //���������������Ļ�
                if (isFourOp(current)) {
                    //���ջ��Ԫ�������ţ�ѡ��ֱ����ջ
                    if (op.top() == '('||op.top() == '��') {
                        op.push(current[0]);
                        i++;
                    }
                    //������Ҫ������������������ȼ�������
                    else {
                        //�����ǰ��������ȼ���ջ��Ԫ�ظߣ���ֱ����ջ
                        if (getPriority(op.top()) < getPriority(current[0])) {
                            op.push(current[0]);
                            i++;
                        }
                        //�����ջ��Ԫ�ؽ��г�ջ����
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
                //���������������Ļ�
                else if (current[0] == '('||op.top() == '��') {
                    op.push(current[0]);
                    i++;
                }
                //����������һ�������ŵ��������ʱ��Ҫ������ջ
                else if (current[0] == ')'||op.top() == '��') {
                    while (op.top() != '('||op.top() == '��') {
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

        //��������������ջ�ͷ���ջ�п��ܻ��ж����Ԫ�أ�����Ҫ������ջ���м������������ֱ������ջֻ��һ��Ԫ��Ϊֹ(���߷���ջֻ���ʼ�����һ��#Ϊֹ)
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
		cout<<"��һ�γ�ʼ�������ļ����,�����´򿪿�ִ���ļ�,����input.txt�а���д����ʽ"<<endl;
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
