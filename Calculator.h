#pragma once

#include <iostream>
#include <exception>
#include <vector>
#include <list>
#include <stack>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include <ctime>


//�궨��
#define DLL_EXPORTS//����DLL
#define INSIDE_DLL//DLL�ڲ�

#ifdef __linux__
#ifdef DLL_EXPORTS
#undef DLL_EXPORTS
#endif
#endif

//������̬��
#ifdef DLL_EXPORTS
#ifdef INSIDE_DLL
#define TEXT_ANALYSIS_EXPORT __declspec(dllexport)
#else
#define TEXT_ANALYSIS_EXPORT __declspec(dllimport)
#endif
#else
#define TEXT_ANALYSIS_EXPORT
#endif


namespace NS_Calculator
{

	using namespace std;

	//�쳣��
	class Exception :public std::exception
	{
		///һЩ����
	public:
		//ö�� �쳣����{��ֵ��ȡ����,���Ų�ƥ��,����}
		enum ExceptionType
		{
			//Calculator������쳣
			UnknownSymbol,//δ֪����
			BracketMismatching,//���Ų�ƥ��
			NoOperand,//�޲�����
			NoOperator,//�޲�����
			SyntaxError,//�﷨����
			MathError,//��ѧ����
			ExpressionNotSet,//�ޱ��ʽ
			ExpressionTooLong,//���ʽ����
			ValueOverflow,//���

			//����̨�쳣
			UnknownCommand,//δ֪����
			UnknownOption,//δ֪ѡ��
			ExpectMoreOption,//��������ѡ��
			ArgumentMismatching,//������ƥ��
			MutexOptionExists,//���ڻ���ѡ��
			KeyOptionNotFound,//ȱ�ٱ�Ҫѡ��

			OtherException,//�����쳣
			ExceptionType_End//�ڱ�
		};
		static const string exceptionDetail[ExceptionType_End];

		///�����Ա
	private:
		ExceptionType exceptionType;//�쳣����
		string info_extra;//������Ϣ

	public:
		explicit Exception(const ExceptionType& exceptionType = OtherException, string _info_extra= "No more infomations.");
		~Exception();
		virtual const char* what()const;

	};

	/**	�滮һ��:
	 *	1.�����ַ���,�õ���׺���ʽ
	 *	2.����׺���ʽת��Ϊ��׺���ʽ
	 *	3.�����׺���ʽ
	 *	��ַ��¼
	 *	https://www.cnblogs.com/hantalk/p/8734511.html
	 */


	namespace Dependencies
	{
		//����Ƿ��Ǵ�д�ַ�
		inline bool check_upperCaseChar(const char& c);
		inline bool check_lowerCaseChar(const char& c);
	}


	 //������
	class TEXT_ANALYSIS_EXPORT Calculator
	{
		//һЩ����:
	public:

		struct Operator
		{
			bool enable = true;//������
			int priority;//���ȼ�
			int type;//��������Ŀ��, ����[0,2]
			int location;//0����ǰ�������, 1�����������������������, -1�����������(����)
			string identifier;//���������
		};


		typedef int Index_operator;//������
		typedef int Index_constant;//����
		typedef long double Operand;//������

		//ö�� ������ {�Ӻ�"+",����"-",�˺�"*",����"/",��"^",�׳�"!",ģ"%",����"log",����"sin",����"cos",����"tan",������"arcsin",������"arccos",������"arctan",����"cot",����"ARR",���"COM",������"(",������")",����'+',����"-"};
		enum Operators
		{
			None = 0,//��
			Plus,//�Ӻ�"+"
			Minu,//����"-"
			Mult,//�˺�"*"
			Divi,//����"/"
			Pow,//��"^"
			Fact,//�׳�"!"
			Mod,//ģ"%"
			Abs,//����ֵ"ABS"
			Log,//����"log"
			Sin,//����"sin"
			Cos,//����"cos"
			Tan,//����"tan"
			ASin,//������"arcsin"
			ACos,//������"arccos"
			ATan,//������"arctan"
			Cot,//����"cot"
			Arra,//������"ARR"
			Comb,//�����"COM"
			Dice,//����"D"
			BraL,//������"("
			BraR,//������")"
			Act,//��"+"
			Neg,//��"-"
			Operator_End//�ڱ�
		};
		//ö�� ���� {Բ����"PI",��Ȼ����'e'}
		enum Constants
		{
			NotConstant = -1,
			PI,//Բ����"PI"
			NaLo,//��Ȼ����"e"
			Ans,//��һ�ν��
			Constant_End//�ڱ�
		};
		//ö�� ��ʽģʽ {λ��,��ѧ,����}
		enum FormatMode { Fix = 0, Sci, Norm };
		//ö�� �Ƕȵ�λ {�Ƕ�,����}
		enum AngleUnit { Deg = 0, Rad };


		static const Operator operators[Operator_End];
		//����ĳЩ��������! ���ڷ�װ�����������������
		//static const int offset;//ƫ����
		//static const char* operatorsIdentifier[Operator_End];//�������ʶ��
		//static const int operatorsIdentifierLength[Operator_End];//�������ʶ������
		static const Operand constantsValue[Constant_End];//����ֵ
		//static const int priority[Operator_End];//���ȼ�
		//static const int operatorsType[Operator_End];//������Ŀ��
		//static const int operatorsPosition[Operator_End];//������λ��
		static const Operand angleUnitsConvertConstant;//�ǵ�λת������
		static const size_t lenLimit_expression;//���ʽ��������



	public:


		class Node//�ڵ�
		{
		public:
			Index_operator _operator;//������
			Operand _operand;//������
		};
		typedef std::list<Node> Expression;//���ʽ
		typedef std::stack<Node> OperatorStack;//������ջ
	//    typedef std::list<Node>::iterator IteratorToNode;//������

		typedef std::stack<Operand> OperandStack;//������ջ


	private:
		//�����Ա
	//		std::string expression;//�ַ������ʽ
		char* expression;//�ַ������ʽ
		Operand result;//���
		Expression infixExpression;//��׺���ʽ
		Expression postfixExpression;//��׺���ʽ

		FormatMode formatMode;//��ǰ��ʽģʽ
		AngleUnit angleUnit;//�Ƕȵ�λ
		int precision;//����
		bool displayDigitSeparator;//��ʾλ�ָ���


	public:
		//�����Ա����

		//���캯��
		Calculator();
		Calculator(const Calculator& another) = delete;//��֧�ָ��ƹ���
		//��������
		~Calculator();

		//������������
		void setFormat(const FormatMode& format = Norm, const int& precision = 2);
		void setPrecision(const int& precision);//���þ���
		void setAngleUnit(const AngleUnit& unit = Rad);//���ýǶȵ�λ
		void setDisplayDigitSeparator(const bool& displayState = false);//�����Ƿ���ʾ���ַָ���","

		Operand getResult()const;//��ȡ������(����double)
		string getFormattedResult()const;//��ȡ��ʽ�����(�����ַ���)
		Operand calculate(const std::string& expression);//����(��Я����, �ڲ�������������ʵ��)
		Operand calculate();//����
		bool setExpression(const string& tarExpression);//���ñ��ʽ
		string getInfixExpression();//��ȡ��׺���ʽ
		string getPostfixExpression();//��ȡ��׺���ʽ

		Calculator& operator=(const Calculator& another) = delete;//��֧�ָ�ֵ����

	//    static Operand Calculate(const std::string &expression);//��̬��������

	private:
		//�ڲ�����
		void _jumpToNext(int& index);
		bool _parseString();//�����ַ���(�����׳��쳣)
		bool _convertToPostfix();//ת��Ϊ��׺���ʽ(�����׳��쳣)
		Operand _calculatePostfix();//�����׺���ʽ���(�����׳��쳣)
		Operand _getValue(const Operand& operand1, const Operand& operand2, const Index_operator& op);//����ֵ
		Index_operator _scanOperator(int& index);//ɨ�������,����index
		Index_constant _scanConstant(int& index);//ɨ�賣��,����index
		int _operatorCopmare(const char* str, const Index_operator& op)noexcept;//������ƥ��(��Сд������)

	};
}








