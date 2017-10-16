/** Implementation of the Infix_Evaluator. */

#include <iostream>
#include "Infix_Evaluator.h"
#include <sstream>
#include <cctype>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

const string Infix_Evaluator::OPERATORS = "+-*/%()[]{}";
const int Infix_Evaluator::PRECEDENCE[] = { 1, 1, 2, 2, 2, -1, -1, -1, -1, -1, -1 };

/** Evaluates an infix expression.
@param expression The expression to be evaluated
@return The value of the expression
@throws Syntax_Error if a syntax error is detected
*/
int Infix_Evaluator::eval(const std::string& expression) {
	// Be sure the stacks are empty
	while (!operand_stack.empty())
		operand_stack.pop();

	while (!operator_stack.empty())
		operator_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;


	// Ex. 1) For the number 10, the token stream reads in the char '1' -> continued in if-statement

	// Ex. 2) For the operator '*', the token stream reads in char '*' - > continued in else-if statement


	while (tokens >> next_char) {

		// Ex. 1 cont) char '1' is definitely a number, so isdigit() returns true
		if (isdigit(next_char)) {
			// Undoes the initial read in from the while loop (?)
			tokens.putback(next_char);
			// Now that we know it is a number, we want to be sure we read in the *entire* number
			int value;
			// Now the token stream reads in the int 10
			tokens >> value;

			value = char(value);

			// The number 10 is pushed onto the operand stack
			operand_stack.push(value);
		}

		// Ex. 2 cont) char '*' is definitely an operator, so is_operator() returns true
		else if (is_operator(next_char)) {
			// If stack is empty and next_char is an opening paren, push onto operator stack
			// ...Unless it's a closing paren, in which case, throw Syntax_Error

			if (operator_stack.empty() || (next_char == '(') || (next_char == '[') || (next_char == '{')) {
				if (next_char == ')' || next_char == ']' || next_char == '}') {
					throw Syntax_Error("Unmatched close parenthesis");
					return -1;
				}
				operator_stack.push(next_char);
			}

			// Need to determine if or how to evaluate sub expression
			else {
				// Store operator in a temp variable
				char op = next_char;
				// If operator has a equal  to or lower precedence than 1, push operator onto the operator stack
				if (!operator_stack.empty()
					&& (precedence(next_char) <= 1)) {
					operator_stack.push(next_char);
				}
				// If operator has a greater precedence than 1 (meaning * / or %), let's evaluate the sub-expression
				else if (precedence(op) > 1) {
					// Store lhs in a temp variable
					int lhs = operand_stack.top();
					// Pop the lhs off the top of the operand stack
					operand_stack.pop();

					// Read in RHS of expression
					int rhs;
					tokens >> next_char;
					if (isdigit(next_char)) {
						tokens.putback(next_char);
						tokens >> rhs;
					}
					else if (is_operator(next_char)) {
						throw Syntax_Error("Expected RHS operand");
						return -1;
					}
					else {
						throw Syntax_Error("Expected RHS operand");
						return -1;
					}
					int result = eval_op(lhs, op, rhs);
					// The number result is pushed onto the operand stack
					operand_stack.push(result);
				}
				else if (precedence(op) <= 1) {
					// If operator has a equal  to or lower precedence than 1, push operator onto the operator stack
					if (!operator_stack.empty()
						&& (precedence(op) <= 1)) {
						operator_stack.push(op);
					}
				}
				else {
					throw Syntax_Error("Precedence eval error");
					return -1;
				}
			}
		}

		else {
			throw Syntax_Error("Invalid character encountered");
			return -1;
		}
	}
	// If we reach this point, that means there were no more tokens to read in
	// We also should have evaluated all sub-expressions at this point
	int answer = 0;
	while (!operand_stack.empty()) {
		if ((!operand_stack.empty()) && (operator_stack.empty())) {
			if (operand_stack.size() == 1) {
				// If the operand stack is not empty and the operator stack is empty, and there is only 1 item in the operand stack, that means the answer must be on top of the stack
				answer = operand_stack.top();
				operand_stack.pop();
			}
		}
		else if ((!operand_stack.empty()) && (!operator_stack.empty())) {
			// The operand stack is not empty, and there's more than 1 item in it - and the operator stack is not yet empty - time to evaluate the rest of the expression
			if ((operand_stack.size() > 1) && (operator_stack.size() > 0)) {
				int lhs = operand_stack.top();
				operand_stack.pop();

				char op = operator_stack.top();
				operator_stack.pop();

				int rhs = operand_stack.top();
				operand_stack.pop();

				int result = eval_op(lhs, op, rhs);
				operand_stack.push(result);
			}

			else {
				throw Syntax_Error("Stack logic error");
				return -1;
			}
		}

		else {
			throw Syntax_Error("Stack should be empty (?)");
			return -1;
		}
	}

	// If the stack *is* empty, that means we already have the answer - return it
	if (operand_stack.empty()) {
		return answer;
	}

	else {
		throw Syntax_Error("I have no idea.");
	}
}

/** Evaluates the current operator.
This function pops the two operands off the expression
stack and applies the operator.
@param op A character representing the operator
@throws Syntax_Error if top is attempted on an empty stack
*/
int Infix_Evaluator::eval_op(int lhs, char op, int rhs) {
	int result = 0;
	switch (op) {
	case '+': result = lhs + rhs;
		break;
	case '-': result = lhs - rhs;
		break;
	case '*': result = lhs * rhs;
		break;
	case '/': result = lhs / rhs;
		break;
	case '%': result = lhs % rhs;
		break;
	}
	return result;
}

int main() {

	//string expr = "10 2 * 5 / 6 2 5 * + -";

	string expr = "10 - 2 * 5 + 6 * 2 / 2";

	std::cout << "Current infix string to be evaluated is '" << expr << "'\n\n";

	Infix_Evaluator new_eval;

	int answer;

	answer = new_eval.Infix_Evaluator::eval(expr);

	std::cout << "Answer is: " << answer << "\n\n";

	return 0;

}