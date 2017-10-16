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
	/** Parses an infix expression.
	@param expression The expression to be evaluated
	@return The value of the expression
	@throws Syntax_Error if a syntax error is detected
	*/
	void high_prec_eval(const std::string& expression);
	std::string rebuild_expression();
	int equate(const std::string& expression);
	int eval(const std::string& expression);

	// Private member functions
private:
	/** Evaluates the current operator.
	This function pops the two operands off the expression
	stack and applies the operator.
	@param op A character representing the operator
	@throws Syntax_Error if top is attempted on an empty stack
	*/
	int eval_op(char op, int rhs, int lhs);
	int eval_op(char op, int rhs);
	bool subexpress_define(partExpression subexpress);
	bool subexpress_define_reset();


	/** Determines whether a character is an operator.
	@param ch The character to be tested
	@return true if the character is an operator
	*/
	bool is_operator(char ch) const {
		return OPERATORS.find(ch) != std::string::npos;
	}

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
