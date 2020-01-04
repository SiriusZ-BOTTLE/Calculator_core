#include "Calculator.h"

void run()
{
	using namespace std;
	using namespace NS_Calculator;
	Calculator calculator;
	char exp[301] = "";

	calculator.setAngleUnit(Calculator::Deg);//���õ�λΪ�Ƕ�
	calculator.setDisplayDigitSeparator(true);
	while (1)
	{

		printf("Enter the expression(empty to quit):\n");
		cin.getline(exp, 100);
		if (exp[0] == 0)
			break;
		try
		{
			calculator.setExpression(exp);//���ñ��ʽ
			calculator.calculate();//������
			cout << "= " << calculator.getFormattedResult() << "\n\n";//��ȡ��ʽ�����
		}
		catch (Exception & e)
		{
			printf("<Warning> An exception occured! See detail next line:\n");
			cout << e.what() << "\n\n";
		}
		catch (exception & e)
		{
			printf("<Error> An error occured! Program may terminate!\n");
		}
		cout << "DebugInfo:" << endl;
		cout << "Infix  :" << calculator.getInfixExpression() << endl;
		cout << "Postfix:" << calculator.getPostfixExpression() << endl << endl;
	}
}




int main()
{
	run();


	//std::string s = "123";
	//char ss[5] = "123";

	////printf("%d", s.size());
	//printf("%d", std::strlen(ss));
}
















