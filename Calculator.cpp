#include "Calculator.h"


namespace NS_Calculator
{
	const string Exception::exceptionDetail[ExceptionType_End] =
	{
		//�����������
		"UnknownSymbol",
		"BracketMismatching",
		"NoOperand",
		"NoOperator",
		"SyntaxError",
		"MathError",
		"ExpressionNotSet",
		"ExpressionTooLong",
		"ValueOverflow",
		//����̨���
		"UnknownCommand",
		"UnknownOption",
		"ExpectMoreOption",
		"ArgumentMismatching",
		"MutexOptionExists",
		"KeyOptionNotFound",
		//����
		"OtherException"
	};

	Exception::Exception(const Exception::ExceptionType& _exceptionType,string _info_extra)
		:exceptionType(_exceptionType)
		,info_extra(_info_extra)
	{

	}


	Exception::~Exception()
	{

	}

	const char* Exception::what()const
	{
		//return (exceptionDetail[this->exceptionType]+": "+info_extra).c_str();
		return exceptionDetail[this->exceptionType].c_str();
	}



	namespace Dependencies
	{
		bool check_upperCaseChar(const char& c)
		{
			return (c >= 'A' && c <= 'Z');
		}

		bool check_lowerCaseChar(const char& c)
		{
			return (c >= 'a' && c <= 'z');
		}

	}






	//���˽�о�̬��������

	//����������
	const Calculator::Operator Calculator::operators[Calculator::Operator_End] =
	{//ע��, ����ı�ʶ��ֻ����Сд
	{true,10,0,0," "},
	{true,0,2,1,"+"},//�Ӻ�
	{true,0,2,1,"-"},//����
	{true,1,2,1,"*"},//�˺�
	{true,1,2,1,"/"},//����
	{true,3,2,1,"^"},//�ݴη�

	{true,5,1,1,"!"},//�׳�
	{true,1,2,1,"%"},//ȡģ
	{true,4,1,0,"abs"},//����ֵ
	{true,2,2,1,"log"},//����

	{true,2,1,0,"sin"},//����
	{true,2,1,0,"cos"},//����
	{true,2,1,0,"tan"},//����
	{true,2,1,0,"arcsin"},//������
	{true,2,1,0,"arccos"},//������
	{true,2,1,0,"arctan"},//������
	{true,2,1,0,"cot"},//����

	{true,2,2,1,"arr"},//������
	{true,2,2,1,"com"},//�����

	{true,2,2,1,"&&"},//��
	{true,2,2,1,"||"},//��
	{true,2,1,0,"!!"},//��
	{true,2,2,1,"!||"},//���

	{true,2,2,1,">"},//����
	{true,2,2,1,">="},//���ڵ���
	{true,2,2,1,"<"},//С��
	{true,2,2,1,"<="},//С�ڵ���
	{true,2,2,1,"=="},//����
	{true,2,2,1,"!="},//������

	{true,2,2,1,"<<"},//����λ
	{true,2,2,1,">>"},//����λ
	{true,2,2,1,"&"},//��λ��
	{true,2,2,1,"|"},//��λ��
	{true,2,1,0,"~"},//��λȡ��
	{true,2,2,1,"!|"},//��λ���

	{true,6,2,1,"d"},//����

	{true,10,0,-1,"("},//������
	{true,10,0,-1,")"},//������
	{false,9,1,0,"a"},//����
	{false,9,1,0,"n"},//����
	};


	//����
	//const int Calculator::offset = 6;

	//����
	/*const char* Calculator::operatorsIdentifier[Calculator::Operator_End] = 
	{ " ","+","-","*","/","^","!","%","abs","log","sin","cos","tan","arcsin","arccos","arctan","cot","arr","com","(",")","a","n" };*/
	
	//����
	//const int Calculator::operatorsIdentifierLength[Calculator::Operator_End] = 
	//{ -1,1,1,1,1,1,1,1,3,3,3,3,3,6,6,6,3,3,3,1,1,-1,-1 };
	//����
	//const int Calculator::priority[Calculator::Operator_End] = 
	//{ 10,0,0,1,1,3,5,1,4,2,2,2,2,2,2,2,2,2,2,10,10,9,9 };//���ȼ�
	//����
	//const int Calculator::operatorsType[Calculator::Operator_End] = 
	//{ 0,2,2,2,2,2,1,2,1,2,1,1,1,1,1,1,1,2,2,0,0,1,1 };//������Ŀ��(����)
	//����
	//const int Calculator::operatorsPosition[Calculator::Operator_End] =
	//{ 0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,-1,-1,0,0 };//������λ��

	const Calculator::Operand Calculator::constantsValue[Calculator::Constant_End] =
	{ 3.1415926535897932384626,2.71828183,1.0,0.0 };//���õ���������

	

	const Calculator::Operand Calculator::angleUnitsConvertConstant = 180 / (Calculator::constantsValue[Calculator::PI]);//�ǵ�λת������
	const size_t Calculator::lenLimit_expression = 1000;//��������Ϊ500


	//const vector<string> Calculator::cmdIdentifier[10] =
	//{
	//{"CAL","calculate"},
	//{"help","?"},
	//{"quit","end","bye","Q"}
	//};
	//int (*const Calculator::functions[2])(const Command& cmd, istream& is, ostream& os) = { Calculator::cmd_Cal,Calculator::cmd_Help };

	Calculator::Calculator()
		: result(0)
		, expression(nullptr)
		, formatMode(Fix)
		, precision(2)
		, angleUnit(Rad)
		, displayDigitSeparator(false)
	{
		srand(time(NULL));//�����������
	}

	Calculator::~Calculator()
	{
		if (expression != nullptr)//���
			delete expression;//�ͷ�
	}

	Calculator::Operand Calculator::calculate(const std::string& _expression)//����
	{
		this->setExpression(_expression);//���ñ��ʽ
		return this->calculate();//���ñ�׼����
	}

	Calculator::Operand Calculator::calculate()//����
	{
		try
		{
			//		setExpression(_expression);//�������ַ���
			if (expression == nullptr)
				throw Exception(Exception::ExpressionNotSet);//�׳��쳣(���ʽδ����)
			if (std::strlen(expression) > lenLimit_expression)
				throw Exception(Exception::ExpressionTooLong);//�׳��쳣(���ʽ����)
			_parseString();//�����ַ����õ���׺���ʽ
			_convertToPostfix();//ת��Ϊ��׺���ʽ
			this->result = _calculatePostfix();//������
		}
		catch (Exception& e)
		{
			//		cout<<e.what();
			throw e;
		}
		return result;
	}

	void Calculator::setFormat(const FormatMode& _format, const int& _precision)
	{
		this->formatMode = _format;
		this->precision = _precision;
	}

	void Calculator::setPrecision(const int& _precision)
	{
		if (_precision >= 0)
			this->precision = _precision;
	}

	void Calculator::setAngleUnit(const AngleUnit& unit)
	{
		this->angleUnit = unit;
	}

	void Calculator::setDisplayDigitSeparator(const bool& displayState)
	{
		this->displayDigitSeparator = displayState;
	}

	Calculator::Operand Calculator::getResult()const
	{
		return result;//���ؼ�����
	}

	//��ȡ��ʽ���Ľ���ַ���
	string Calculator::getFormattedResult()const
	{

		string resultStr;//����ַ���

		stringstream strStream;

		switch (formatMode)//������ģʽ
		{
		case Fix:
			strStream << fixed << setprecision(precision);
			//		strStream<<setiosflags(ios::fixed);//��ͬ����
			break;
		case Sci:
			strStream << scientific << setprecision(precision);
			break;
		case Norm:
			break;
		}

		strStream << this->result;
		resultStr = strStream.str();//��ȡ��������

		if (displayDigitSeparator)//����Ƿ���ʾ�ָ���
		{
			int i, size = resultStr.size();
			for (i = 0; i < size; i++)
			{
				if (resultStr[i] == '.' || resultStr[i] == 'e')
					break;
			}
			i -= 3;
			int temp = 0;
			if (this->result < 0)
				temp = 1;
			for (i; i > temp; i -= 3)
			{
				resultStr.insert(i, 1, ',');//����һ���ַ�
			}
		}

		return resultStr;
	}

	//���ñ��ʽ
	bool Calculator::setExpression(const string& tarExpression)//���ñ��ʽ
	{
		size_t length = tarExpression.size();

		try
		{
			if (expression != nullptr)//��ǰ���ʽ�ǿ�
				delete expression;//ɾ������
			expression = new char[length + 1];//�����ڴ�ռ�
		}
		catch (exception& e)
		{
			expression = nullptr;
			throw e;//�׳��쳣
		}

		if (tarExpression.copy(expression, length) != length)
		{
			//���Ʋ��ɹ�
			delete expression;//ɾ��������ڴ�
			expression = nullptr;
			return false;//�����˳�
		}
		expression[length] = '\0';
		return true;
	}

	string Calculator::getInfixExpression()//��ȡ��׺���ʽ
	{
		//	using namespace std;
		Expression::iterator iter, iter_end;
		string str;
		stringstream ss;
		ss << fixed << setprecision(2);
		iter = infixExpression.begin();
		iter_end = infixExpression.end();

		while (iter != iter_end)
		{
			if (iter->_operator == None)
				//			str.append(std::to_string(iter->_operand)+",");
				ss << iter->_operand << ",";
			else
			{
				ss << operators[iter->_operator].identifier << ",";
			}
			iter++;//����
		}
		str = ss.str();
		return str;
	}

	string Calculator::getPostfixExpression()//��ȡ��׺���ʽ
	{
		//	using namespace std;
		Expression::iterator iter, iter_end;
		string str;
		stringstream ss;
		ss << fixed << setprecision(2);
		iter = postfixExpression.begin();
		iter_end = postfixExpression.end();

		while (iter != iter_end)
		{
			if (iter->_operator == None)
				//			str.append(std::to_string(iter->_operand)+",");
				ss << iter->_operand << ",";
			else
			{
				//ss << operatorsIdentifier[iter->_operator] << ",";
				ss << operators[iter->_operator].identifier << ",";
			}
			iter++;//����
		}
		str = ss.str();
		return str;
	}

	//int Calculator::run(istream& is, ostream& os)
	//{
	//	os << TextPackage::text_title_Calculator;
	//	string temp;
	//	Command cmd;//�������
	//	while (1)
	//	{
	//		os << ">>";
	//		std::getline(is, temp);//��ȡһ������
	//		if (cmd.parseCommand(temp))//��������̨����(���ַ�������ת��ΪCommand�����)
	//		{
	//			int cmdIndex = -1;
	//			//�����ɹ�
	//			try
	//			{
	//				cmdIndex = selectCommand(cmd.name);//ѡ������
	//			}
	//			catch (const Exception& e)
	//			{
	//				os << e.what() << endl;
	//				continue;
	//			}
	//			if (cmdIndex == End)
	//				break;//�˳�
	//			else
	//			{
	//				try
	//				{
	//					functions[cmdIndex](cmd, is, os);//���������
	//				}
	//				catch (const Exception& e)
	//				{
	//					os << e.what() << endl;
	//					continue;
	//				}

	//			}
	//		}
	//		else
	//		{
	//			//����ʧ��
	//			continue;
	//		}
	//	}
	//	return 0;
	//}


	///�ڲ�����

	void Calculator::_jumpToNext(int& index)
	{
		char c = expression[index];
		while (c != '\0')
		{
			if ((c >= '0' && c <= '9') || c == '.')
				c = expression[++index];
			else
				break;//�˳�
		}
	}

	bool Calculator::_parseString()//�����ַ���
	{
		int index = 0;//��ǰ����λ��
	//    int flag=1;//,����ʵʱ���������
		bool lastIsOperator = 1;//��һ���ǲ�����
		Node crtNode;
		Index_constant constant;//
		crtNode._operator = None;
		if (!this->infixExpression.empty())//��׺���ʽ�ǿ�
			this->infixExpression.clear();//���
		int length = strlen(this->expression);

		while (index < length)//��ѭ��
		{
			if (expression[index] == ' ')//�����ո�
			{
				index++;
				continue;//����
			}
			crtNode._operator = _scanOperator(index);//ɨ�������
			if (crtNode._operator == None)//δ��⵽������
			{
				//������
				constant = _scanConstant(index);//ɨ�賣��
				if (constant == NotConstant)//���ǳ���
				{
					try
					{
						//������ȡ��ֵ
						crtNode._operand = std::stod(expression + index);
					}
					catch (std::exception& e)
					{
						//��ȡ��ֵʧ��
						throw Exception(Exception::UnknownSymbol);//�׳��쳣(δ֪����)
					}
				}
				else
				{
					//��鵽����
					if (constant == Ans)
						crtNode._operand = result;
					else
						crtNode._operand = constantsValue[constant];
				}

				//û���쳣

				this->infixExpression.push_back(crtNode);//�ڵ���ӵ�ĩβ
				lastIsOperator = 0;
				_jumpToNext(index);//����
			}
			else//������
			{
				//������
				if (crtNode._operator == Minu && lastIsOperator)//��ȡ������
					crtNode._operator = Neg;//ת��Ϊ����
				if (crtNode._operator == Plus && lastIsOperator)//��ȡ���Ӻ�
					crtNode._operator = Act;//ת��Ϊ����
				this->infixExpression.push_back(crtNode);//�ڵ���ӵ�ĩβ
				if (crtNode._operator != Fact && crtNode._operator != BraR)//��Ҫ����ǲ��Ǻ�׺�����
					lastIsOperator = 1;
			}
		}
		return true;//��������
	}

	bool Calculator::_convertToPostfix()//ת��Ϊ��׺���ʽ
	{
		if (!this->postfixExpression.empty())//�ǿ�
			postfixExpression.clear();//���

		bool lastIsOperand = false;//��һ���Ƿ��ǲ������ı��
		bool lastIsPrepositiveOperator = false;//��һ���Ƿ���ǰ�ò������ı��

		Expression::iterator iter, iter_end;//ָ��ڵ�ĵ�����
		iter = infixExpression.begin();//��ȡ��ʼ������
		iter_end = infixExpression.end();//��ȡĩβ������

		Node tempOperatorNode;//��ʱOP�ڵ�
		OperatorStack opStack;//������ջ

		while (iter != iter_end)//��ѭ��
		{
			if ((iter)->_operator == None)//����Ϊ������
			{
				postfixExpression.push_back(*iter);//ֱ������Ŀ���б�
				lastIsOperand = true;//���Ϊ��ֵ
				lastIsPrepositiveOperator = false;
			}
			else//����Ϊ������
			{
				if (iter->_operator == BraR)//������,������ֵ����
				{
					while (true)
					{
						if (opStack.empty())//ջΪ��
							throw Exception(Exception::BracketMismatching);//�׳��쳣(���Ų�ƥ��)
						tempOperatorNode = opStack.top();//��ȡջ��Ԫ��
						opStack.pop();//��ջ
						if (tempOperatorNode._operator == BraL)//��������������
							break;
						else
							postfixExpression.push_back(tempOperatorNode);//����Ŀ�����
					}//while
					lastIsOperand = true;//���Ϊ��ֵ
				}
				else if (iter->_operator == BraL)//������,������ֵ����
				{
					lastIsOperand = true;//���Ϊ��ֵ
					opStack.push(*iter);//����ջ
				}
				else
				{
					//�����﷨���
					//if (!lastIsOperand && operatorsPosition[iter->_operator] == 1)
					//{
					//	throw Exception(Exception::SyntaxError);//�׳��쳣(�﷨����)
					//}
					if (!lastIsOperand && operators[iter->_operator].location == 1)
					{
						throw Exception(Exception::SyntaxError);//�׳��쳣(�﷨����)
					}
					//if (lastIsPrepositiveOperator)
					//{
					//	if(iter->_operator!=BraL)
					//		throw Exception(Exception::SyntaxError);//�׳��쳣(�﷨����)
					//}
					//�﷨������


					if (opStack.empty())//ջΪ��
						;//�޷���������OP
					else
					{
						//�������ջ�бȽ�Ҫ��ջ��OP���ȼ��ߵ�OP(ǰ�����������)
						while ((!opStack.empty()))
						{
							Node& tempOperatorNode = opStack.top();//��ȡջ��Ԫ��
							int temp = operators[tempOperatorNode._operator].priority - operators[iter->_operator].priority;
							/*if (operators[opStack.top()._operator].priority >= operators[iter->_operator].priority)
								;*/
							if (temp < 0)
								break;
							
							if (tempOperatorNode._operator != BraL && operators[tempOperatorNode._operator].location != 0)
								postfixExpression.push_back(tempOperatorNode);//����Ŀ�����
							else
								break;
							opStack.pop();//��ջ
						}//while
					}

					opStack.push(*iter);//����ջ
					if (operators[iter->_operator].location == 0)//ǰ�������
					{
						//lastIsPrepositiveOperator = true;//���Ϊǰ�ò�����
						lastIsOperand = false;//���Ϊ������
					}
					else//����
					{
						lastIsOperand = true;//���Ϊ��ֵ
					}
				}
			}
			iter++;//����
		}//while

		//�﷨���
		if (!lastIsOperand)
		{
			throw Exception(Exception::SyntaxError);//�׳��쳣(�﷨����)
		}

		while (!opStack.empty())//��ջ��ʣ�����������
		{
			tempOperatorNode = opStack.top();//��ȡջ��Ԫ��
			if (tempOperatorNode._operator == BraL)
				throw Exception(Exception::BracketMismatching);//�׳��쳣(���Ų�ƥ��)
			opStack.pop();//��ջ
			postfixExpression.push_back(tempOperatorNode);//����Ŀ�����
		}
		return true;
	}

	Calculator::Index_operator Calculator::_scanOperator(int& index)//ɨ�������,δ�ҵ�����None
	{
		Index_operator index_bestMatch{ -1 };//���ƥ������
		int len_bestFit{ 0 };//���ƥ�䳤��
		//��������

		//����Ƚϲ�����
		for (int i = 1; i < Operator_End; i++)
		{
			if (!operators[i].enable)
				continue;//����ĳЩ������
			const int&& len = _operatorCopmare(this->expression + index, i);
			if (len != operators[i].identifier.length())
				continue;
			if (len > len_bestFit)
			{
				len_bestFit = len;//�������ƥ�䳤��
				index_bestMatch = i;//�������ƥ�����������
			}
				
		}
		if (index_bestMatch < 0)
			return None;
		else
			return index += len_bestFit, index_bestMatch;
	}

	Calculator::Index_constant Calculator::_scanConstant(int& index)//ɨ�賣��,����index
	{
		if (strncmp(this->expression + index, "PI", 2) == 0)
		{
			index += 2;//������λ
	//		return constantsValue[PI];
			return PI;
		}
		else if (expression[index] == 'e')
		{
			index++;
			//		return constantsValue[NaLo];
			return NaLo;
		}
		else if (strncmp(this->expression + index, "TRUE", 4) == 0)
		{
			index+=4;
			//		return constantsValue[NaLo];
			return True;
		}
		else if (strncmp(this->expression + index, "FALSE", 5) == 0)
		{
			index+=5;
			//		return constantsValue[NaLo];
			return False;
		}
		else if (strncmp(this->expression + index, "ANS", 3) == 0)
		{
			index += 3;
			return Ans;
		}
		else
			return NotConstant;//���طǳ������
	}

	Calculator::Operand Calculator::_calculatePostfix()//�����׺���ʽ���
	{
		OperandStack operandStack;//������ջ
		Calculator::Operand operand1 = 0, operand2 = 0;

		Expression::iterator iter, iter_end;
		Node crtNode;
		iter = postfixExpression.begin();//��ȡ��ʼ������
		iter_end = postfixExpression.end();//��ȡĩβ������

		while (iter != iter_end)//��ѭ��
		{
			crtNode = *iter;
			if (crtNode._operator == None)//������������
			{
				//������
				operand1 = iter->_operand;
				operandStack.push(operand1);//��������ջ
			}
			else//������������
			{
				//������
				//�������������(��,˫Ŀ)��ջ�л�ȡ������
				if (operators[crtNode._operator].type == 1)//��Ŀ�����
				{
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//�׳��쳣(�޲�����)
					operand1 = operandStack.top();//��ȡջ��Ԫ��
					operandStack.pop();//��ջ
				}
				else//˫Ŀ�����
				{
					//��ȡ�ڶ���������
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//�׳��쳣(�޲�����)
					operand2 = operandStack.top();//��ȡջ��Ԫ��
					operandStack.pop();//��ջ

					//��ȡ��һ��������
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//�׳��쳣(�޲�����)
					operand1 = operandStack.top();//��ȡջ��Ԫ��
					operandStack.pop();//��ջ

				}
				try
				{
					operand1 = _getValue(operand1, operand2, crtNode._operator);//������
				}
				catch (Exception& e)
				{
					//�����쳣
					throw e;//�׳�
				}
				operandStack.push(operand1);//��������ջ
			}

			iter++;
		}
		if (operandStack.size() > 1)
			throw Exception(Exception::NoOperator);//�׳��쳣(�޲�����)
		return operand1;

	}

	Calculator::Operand Calculator::_getValue(const Calculator::Operand& operand1, const Calculator::Operand& operand2, const Index_operator& op)
	{
		switch (op)
		{
		case Plus:
			return operand1 + operand2;
		case Minu:
			return operand1 - operand2;
		case Mult:
			return operand1 * operand2;
		case Divi:
		{
			if (operand2 == 0)//����Ϊ0
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return operand1 / operand2;
		}
		case Pow:
			return std::pow(operand1, operand2);

		case Fact://�׳�
		{
			Calculator::Operand temp = ceil(operand1);//����ȡ��
			if (temp > 1754)//�����㼫��ֵ
				throw Exception(Exception::ValueOverflow);//�׳��쳣(��ֵ���)
			if (temp != operand1 || operand1 < 0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			if (temp < 0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			Calculator::Operand res = 1.0, i;
			for (i = 2.0; i <= temp; i++)
				res *= i;
			return res;
		}
		case Mod:
		case Abs:
			return operand1 < 0 ? -operand1 : operand1;
		{
			long long operand_1 = ceil(operand1);
			long long operand_2 = ceil(operand2);
			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return (Calculator::Operand)(operand_1 % operand_2);
		}
		case Log:
		{
			if (operand1 == 0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return std::log(operand2) / std::log(operand1);//���׹�ʽ(operand1�ǵ���)
		}
		
		case Sin:
		{
			Calculator::Operand operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			return std::sin(operand_1);
		}
		case Cos:
		{
			Calculator::Operand operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			return std::cos(operand_1);
		}
		case Tan:
		{
			Calculator::Operand operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			if (operand1 == constantsValue[PI] / 2)//���ֵ����PI/2
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return std::tan(operand_1);
		}
		case ASin:
		{
			Calculator::Operand res;
			if (operand1 > 1.0 || operand1 < -1.0)
				throw Exception(Exception::MathError);
			res = std::asin(operand1);
			if (angleUnit == Deg)
				res *= angleUnitsConvertConstant;
			return res;
		}
		case ACos:
		{
			Calculator::Operand res;
			if (operand1 > 1.0 || operand1 < -1.0)
				throw Exception(Exception::MathError);
			res = std::acos(operand1);
			if (angleUnit == Deg)
				res *= angleUnitsConvertConstant;
			return res;
		}
		case ATan:
		{
			Calculator::Operand res;
			res = std::atan(operand1);
			if (angleUnit == Deg)
				res *= angleUnitsConvertConstant;
			return res;
		}
		case Cot:
		{
			Calculator::Operand operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			if (operand1 == 0)//���ֵ����0
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return 1.0 / std::tan(operand_1);
		}
		
		case Arra://������
		{
			Calculator::Operand operand_1 = ceil(operand1);
			Calculator::Operand operand_2 = ceil(operand2);

			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)

			Calculator::Operand temp = operand_1, res = 1, count = 0;

			for (; count < operand_2; count++)
			{
				res *= temp;
				temp -= 1.0;
			}
			return res;
		}
		case Comb:
		{
			Calculator::Operand operand_1 = ceil(operand1);
			Calculator::Operand operand_2 = ceil(operand2);
			Calculator::Operand temp = operand_1, res = 1, count = 0, fact = 1.0;
			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)

			for (; count < operand_2; count++)
			{
				res *= temp;
				temp -= 1.0;
			}
			temp = operand_2;
			for (Calculator::Operand i = 2.0; i <= temp; i++)
				fact *= i;
			res /= fact;
			return res;
		}
		
		case And:
			return (operand1 != 0) && (operand2 != 0);
		case Or:
			return (operand1 != 0) || (operand2 != 0);
		case Not:
			return (operand1 == 0);
		case Xor:
			return (operand1 != 0) != (operand2 != 0);


		case JG:
			return operand1 > operand2;
		case JGE:
			return operand1 >= operand2;
		case JL:
			return operand1 < operand2;
		case JLE:
			return operand1 <= operand2;
		case JE:
			return operand1 == operand2;
		case JNE:
			return operand1 != operand2;

		case BLS:
		{
			if(operand2<0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			Calculator::Operand temp = ceil(operand1);//����ȡ��
			if (temp != operand1)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			temp = ceil(operand2);
			if (temp != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return static_cast<Calculator::Operand>(static_cast<long long>(operand1)<< static_cast<long long>(operand2));
		}
		case BRS:
		{
			if (operand2 < 0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			Calculator::Operand temp = ceil(operand1);//����ȡ��
			if (temp != operand1)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			temp = ceil(operand2);
			if (temp != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return static_cast<Calculator::Operand>(static_cast<long long>(operand1) >> static_cast<long long>(operand2));
		}
		case Band:
		{
			Calculator::Operand temp = ceil(operand1);//����ȡ��
			if (temp != operand1)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			temp = ceil(operand2);
			if (temp != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return static_cast<Calculator::Operand>(static_cast<long long>(operand1) & static_cast<long long>(operand2));
		}
		case Bor:
		{
			Calculator::Operand temp = ceil(operand1);//����ȡ��
			if (temp != operand1)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			temp = ceil(operand2);
			if (temp != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return static_cast<Calculator::Operand>(static_cast<long long>(operand1) | static_cast<long long>(operand2));
		}
		case Bnot:
		{
			Calculator::Operand temp = ceil(operand1);//����ȡ��
			if (temp != operand1)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			temp = ceil(operand2);
			if (temp != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return static_cast<Calculator::Operand>(~static_cast<long long>(operand1));
		}
		case Bxor:
		{
			Calculator::Operand temp = ceil(operand1);//����ȡ��
			if (temp != operand1)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			temp = ceil(operand2);
			if (temp != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return static_cast<Calculator::Operand>(static_cast<long long>(operand1) ^ static_cast<long long>(operand2));
		}

		case Dice://����
		{
			Calculator::Operand operand_1 = ceil(operand1);
			Calculator::Operand operand_2 = ceil(operand2);
			int sum = 0;
			//����Ƿ�����, �����Ƿ���ڸ���, ͬʱ���Ӳ��ܳ���100��
			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2 || operand_2>100 || operand_1>1000000)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)

			if (operand_2 == 0)//�������, 0���������÷���0
				return 0;

			for (int i = 0; i < operand1; i++)
			{
				int temp = -1;
				temp = rand() % static_cast<int>(operand_2) + 1;
				//srand(temp * 9997);
				sum += temp;
			}
			return sum;
		}

		case Act:
			return operand1;
		case Neg:
			return -operand1;
		

		
		default:
			return 0;
		}
	}

	int Calculator::_operatorCopmare(const char* str, const Index_operator& op)noexcept
	{
		int i;
		char c;
		bool res = false;
		int len = operators[op].identifier.size();

		for (i = 0; i < len; i++)
		{
			if (str[i] == '\0')//����
				break;
			c = operators[op].identifier[i];
			if (str[i] == c || (Dependencies::check_lowerCaseChar(c) && str[i] == c - 32))//�����ִ�Сд
				continue;
			else
				break;
		}
		//if (i == len)
		//	return true;
		//else
		//	return false;
		return i;
	}


}


