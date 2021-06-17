
#include <iostream>
#include <windows.h>
using namespace std;

int priority(char);
double calculate(char*, int, int, int&);
double calcOper(double, char, double, int&);

//cout << "      ______               __       __                                                  \n";
//cout << "     |  ___|      /\\     | |      | |                                             \n";
//cout << "     | |         /  \\    | |      | |                                             \n";
//cout << "     | |        / /\\ \\  | |      | |                                            \n";
//cout << "     | |____   / ____ \\  | |____  | |____                                         \n";
//cout << "     |______| /_/    \\_\\|______| |______|                        \n\n";

int main() 
{
	char str[100];
	int sizeStr = 0;
	bool* errorInPut = new bool;
	double result = 0;
	int error = 0;
	do {
		*errorInPut = 0;
		cout << "Enter a string (no spaces) to evaluate the expression" << endl;
		cout << "Operations supported: +,-, *,/, ^" << endl;
		cin >> str;
		int* countBrackets = new int; 
		*countBrackets = 0;	
		sizeStr = strlen(str);	
		for (int i = 0; i < sizeStr; i++) { 
			if (!((*(str + i) >= 48 && *(str + i) <= 57) || priority(*(str + i)) > 0 || *(str + i) == '.')) 
			{ 
				*errorInPut = 1;	
				cout << "Input error: before \"(\" there must be an operator sign or another \"(\" in position " << i << endl;
				break;
			}
			if (*(str + i) == '(') { 
				(*(countBrackets))++;
				if (!(i == 0 || (i > 0 && (priority(*(str + i - 1)) > 1 || *(str + i - 1) == '(')))) { 
					*errorInPut = 1;
					cout << "Input error: after \"(\" there must be a digit or another \"(\" in position " << i << endl;
					break;
				}
				if (!(i + 1 < sizeStr && ((*(str + i + 1) >= 48 && *(str + i + 1) <= 57) || *(str + i + 1) == '('))) 
				{
					*errorInPut = 1;
					cout << "Input error: before \")\" there must be a digit or one more \")\" in position" << i << endl;
					break;
				}
			}
			if (*(str + i) == ')') {
				(*(countBrackets))--;
				if (!(i > 0 && ((*(str + i - 1) >= 48 && *(str + i - 1) <= 57) || *(str + i - 1) == ')'))) { 
					*errorInPut = 1;
					cout << "Input error: before \") \" there must be a digit or one more \")\"in position"<< i << endl;
					break;
				}
				if (!(i == sizeStr - 1 || (i + 1 < sizeStr && (priority(*(str + i + 1)) > 1 || *(str + i + 1) == ')')))) {	
					*errorInPut = 1;
					cout << "Input error: after \")\" there must be an operator sign or another \")\" in position" << i << endl;
					break;
				}
			}
			if (*(str + i) == '.') { 
				if (!(i > 0 && *(str + i - 1) >= 48 && *(str + i - 1) <= 57)) { 
					*errorInPut = 1;
					cout << "Input error: there must be a digit in position before \". \" " << i << endl;
					break;
				}
				if (!(i + 1 < sizeStr && *(str + i + 1) >= 48 && *(str + i + 1) <= 57)) {	
					*errorInPut = 1;
					cout << "Input error: after \". \" There must be a digit in position" << i << endl;
					break;
				}
			}
			if (priority(*(str + i)) > 1) {
				if (!(i > 0 && ((*(str + i - 1) >= 48 && *(str + i - 1) <= 57)) || priority(*(str + i - 1)) == 1)) {
					*errorInPut = 1;
					cout << "Input error: the operator sign must be preceded by a digit or a parenthesis in position " << i << endl;
					break;
				}
				if (!(i + 1 < sizeStr && ((*(str + i + 1) >= 48 && *(str + i + 1) <= 57)) || priority(*(str + i + 1)) == 1)) {	
					*errorInPut = 1;
					cout << "Input error: after the operator sign there must be a number or a parenthesis in position " << i << endl;
					break;
				}
			}
		}
		if (*countBrackets != 0 && *errorInPut != 1) { 
			*errorInPut = 1;
			cout << "Input error: quantity \"(\" does not match the quantity \")\"" << endl;
		}
		cout << endl;
		delete countBrackets; 
	} while (*errorInPut);
	delete errorInPut;
	result = calculate(str, 0, sizeStr, error); 
	if (error == 1) cout << "Calculation error: division by zero" << endl;	
	else cout << "The result of evaluating the expression:   " << result << endl;
}

double calculate(char* str, int startStr, int endStr, int& error)
{	
	double number[3] = { 0.0,0.0,0.0 };	
	int operation[3] = { 0,0,0 }; 
	int numberPos = 0, operationPos = 0;
	bool digit = 0;
	double result;	
	for (int i = startStr; i < endStr; i++) 
	{

		if (*(str + i) == '(') { 
			int countBrackets = 1;	
			int start = i + 1;	
			for (i++; i < endStr; i++) { 
				if (*(str + i) == '(') { 
					countBrackets++;
				}
				else if (*(str + i) == ')') { 
					countBrackets--;
					if (countBrackets == 0) { 
						break;
					}
				}
			}
			number[numberPos] = calculate(str, start, i, error); 
			if (!numberPos) result = number[numberPos];
			numberPos++; 
		}
		else if (priority(*(str + i)) == 0) { 
			if (!digit) { 
				digit = 1;	
				number[numberPos] = atof(str + i);	
				if (!numberPos) result = number[numberPos]; 
				numberPos++; 
			}
		}
		else if (priority(*(str + i)) > 1) { 
			digit = 0;	
			if (operationPos == 0) { 
				operation[operationPos] = i; 
				operationPos++; 
			}
			else if (priority(*(str + i)) > priority(*(str + operation[operationPos - 1]))) { 
				operation[operationPos] = i; 
				operationPos++; 
			}
			else { 
				for (int j = operationPos; j > 0; j--) {
					result = calcOper(number[j - 1], *(str + operation[j - 1]), number[j], error);	
					number[j - 1] = result;
				} 
				result = calcOper(result, *(str + i), calculate(str, i + 1, endStr, error), error);
				if (error > 0) return 0; 
				return result;
			}
		}
	}
	if (operationPos) {
		for (int j = operationPos; j > 0; j--) {
			result = calcOper(number[j - 1], *(str + operation[j - 1]), number[j], error);
			number[j - 1] = result;
		}
	}
	if (error > 0) return 0;
	return result;
}

int priority(char symbol)
{
	if (symbol == '(' || symbol == ')')
		return 1;
	else if (symbol == '+' || symbol == '-')
		return 2;
	else if (symbol == '*' || symbol == '/')
		return 3;
	else if (symbol == '^')
		return 4;
	else
		return 0;
}

double calcOper(double num1, char operat, double num2, int& error) {
	switch (operat)
	{
	case '+':
		return num1 + num2;
	case '-':
		return num1 - num2;
	case '*':
		return num1 * num2;
	case '/':
		if (num2 == 0)
		{
			error = 1;
			return 0;
		}
		return num1 / num2;
	case '^':
		return pow(num1, num2);
	}
}
