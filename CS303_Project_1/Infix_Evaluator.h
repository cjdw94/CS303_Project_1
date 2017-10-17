#ifndef INFIX_EVALUATOR_H_
#define INFIX_EVALUATOR_H_

#include <stack>
#include <string>
#include "Syntax_Error.h"

/** NOTE:

	Alias key for double char operators
	-----------------------------------
	"||"			=			'|'
	"&&"			=			'&'
	"=="			=			'='
	"!="			=			'?'
	">="			=			'@'
	"<="			=			'`'
	Negative sign	=			'#'
	"--"			=			'~'
	"++"			=			'$'

*/

struct partExpression {

	int lhs, rhs, prec_8_tally;
	char op1, op2;

	partExpression(char op1 = ' ', char op2 = ' ', int lhs = -9999999, int rhs = -9999999, int prec_8_tally = NULL) :
		op1(op1), op2(op2), lhs(lhs), rhs(rhs), prec_8_tally(prec_8_tally) {}

};

class Infix_Evaluator {
	// Public member functions
public:
	//Systematically parses the full expression and handles the highest precedence (7 or 8) subexpressions first
	void high_prec_eval(const std::string& expression);

	//Rebuilds input expression string after first run - through high_prec_eval - with correct left - to - right ordering
	std::string rebuild_expression();

	//Concatenates subexpressions parsed by the rebuild_expression() function
	std::string create_new_string(std::string new_string, std::ostringstream &rebuilt_expression);

	//Final function to finish evaluation of an infix expression.
	int equate(const std::string& expression);

	//Wrapper function to help readability of code - checks/set true flag for bool alias variables
	bool subexpress_define(partExpression subexpress);

	//Resets all true flags assigned by subexpress_define back to false
	bool subexpress_define_reset();

	//Evaluates the current operator.  
	int eval_op(char op, int rhs, int lhs);

	//Evaluates the current operator -  overloaded to deal with prefix operators
	int eval_op(char op, int rhs);

	//Wrapper function to help readability of code - checks/set true flag for bool alias variables
	int eval(const std::string& expression);
	

	/** Determines whether a character is an operator.
	@param ch The character to be tested
	@return true if the character is an operator
	*/
	bool is_operator(char ch) const {
		return OPERATORS.find(ch) != std::string::npos;
	}

	/** Determines what precedence is associated with a specific operator
	@param op The operator character to be checked
	@return The integer value of the precedence of the operator
	*/
	int precedence(char op) const {
		return PRECEDENCE[OPERATORS.find(op)];
	}

	// Data fields
	static const std::string OPERATORS;
	static const int PRECEDENCE[];
	std::stack<char> operator_stack_1;
	std::stack<char> operator_stack_2;
	std::stack<int> operand_stack_1;
	std::stack<int> operand_stack_2;
	std::stack<partExpression> high_prec_stack;
	std::stack<partExpression> high_prec_stack_reversal;
};

#endif
