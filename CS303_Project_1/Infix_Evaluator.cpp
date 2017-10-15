/** Implementation of the Infix_Evaluator. */

#include <iostream>
#include "Infix_Evaluator.h"
#include <sstream>
#include <cctype>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

int def_zero = -9999999;
const string Infix_Evaluator::OPERATORS = "|&=?>@<`+-*/%^#~$!()[]{}";
const int Infix_Evaluator::PRECEDENCE[] = { 1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 8, 8, 8, 8, -1, -1, -1, -1, -1, -1 };
bool LHS_RHS_OP1_OP2, LHS_RHS_OP1, LHS_RHS_OP2, LHS_RHS, LHS_OP1_OP2, LHS_OP1, LHS_OP2, LHS, RHS_OP1_OP2, 
			RHS_OP1, RHS_OP2, RHS, OP1_OP2, OP1, OP2, NO_LHS_RHS_OP1_OP2;

/** Systematically parses the full expression and handles the highest precedence (8) subexpressions first
@param expression The expression to be evaluated
@return The value of the subexpression (precedence 8)
@throws Syntax_Error if a syntax error is detected
*/

void Infix_Evaluator::high_prec_eval(const std::string& expression) {
	// Be sure high_prec_stack is empty
	while (!high_prec_stack.empty())
		high_prec_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char, temp_char = 'a';
	int temp_int = 0;
	bool solo_negation = false;

	// Ex. 1) For the number 10, the token stream reads in the char '1' -> continued in if-statement

	// Ex. 2) For the operator '*', the token stream reads in char '*' - > continued in else-if statement

	// Two ways a subexpression can be returned from this function - a full expression, or simply an operator to connect expressions with ultimately higher precedence
	partExpression new_part, new_part_alone;

	// Declare the last operator's precedence, pre-set higher than max possible op precedence 
	int last_op_prec = 9;

	while (tokens >> next_char) {

		solo_negation = false;
		subexpress_define(new_part);

		// Ex. 1 cont) char '1' is definitely a number, so isdigit() returns true
		// Adds numbers to LHS and RHS of new_part
		if (isdigit(next_char)) {
			// Undoes the initial read in from the while loop by putting next_char back into the stream
			tokens.putback(next_char);
			// Now that we know it is a number, we want to be sure we read in the *entire* number
			int value;
			// Now the token stream reads in the int 10, instead of the char '1'
			tokens >> value;
			// The number 10 added to the LHS of subexpression new_part
			if ((new_part.lhs == def_zero) && (new_part.rhs == def_zero))
				new_part.lhs = value;
			// The number 10 added to the RHS of subexpression new_part
			else if ((new_part.lhs != def_zero) && (new_part.rhs == def_zero))
				new_part.rhs = value;
			else {
				throw Syntax_Error("Improper placement of digits in given infix expression");
			}
		}
		// Ex. 2 cont) char '*' is definitely an operator, so is_operator() returns true
		// Determines if next_char is an operator

		else if (is_operator(next_char)) {
			// If next_char is an opening paren, push onto high_prec_stack
			// ...Unless it's a closing paren, in which case, throw Syntax_Error
			if ((next_char == '(') || (next_char == '[') || (next_char == '{')) {
				if (next_char == ')' || next_char == ']' || next_char == '}') {
					throw Syntax_Error("Unmatched close parenthesis");
				}
				new_part_alone.op1 = next_char;
				high_prec_stack.push(new_part_alone);
				partExpression newer_part_alone;
				new_part_alone = newer_part_alone;
			}
			// Compensate for "||" logical OR operator (precedence 1)
			if (next_char == '|') {
				// Temporarily look ahead to see what the following char will be
				// If anything but the second half of OR operator, throw error (one should not exist without a second)
				tokens >> next_char;
				// See if a digit comes after complete OR operator
				if (tokens >> temp_char) {};
				// No alias necessary, because no other operator should use this char
				if (LHS && (next_char == '|') && (isdigit(temp_char))) {
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part);
					partExpression newer_part, newer_part_alone;
					new_part = newer_part;
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part_alone);
					new_part_alone = newer_part_alone;
					tokens.unget();
					continue;
				}
				else {
					throw Syntax_Error("Incomplete '||' logic operator.");
				}
			}
			// Compensate for "&&" logical AND operator (precedence 2)
			if (next_char == '&') {
				// Temporarily look ahead to see what the following char will be
				// If anything but the second half of OR operator, throw error (one should not exist without a second)
				tokens >> next_char;
				// See if a digit comes after complete AND operator
				if (tokens >> temp_char) {};
				// No alias necessary, because no other operator should use this char
				if (LHS && (next_char == '&') && (isdigit(temp_char))) {
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part);
					partExpression newer_part, newer_part_alone;
					new_part = newer_part;
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part_alone);
					new_part_alone = newer_part_alone;
					tokens.unget();
					continue;
				}
				else {
					throw Syntax_Error("Incomplete '&&' logic operator.");
				}
			}
			// Compensate for "is_equal" ("==") equality comparison (precedence 3)
			if (next_char == '=') {
				// Temporarily look ahead to see what the following char will be
				// If anything but the second half of "is_equal" operator, throw error (one should not exist without a second)
					// Note that other operators that use the '=' character use it on the *second* half of the complete operator
				tokens >> next_char;
				// See if a digit comes after complete "is_equal" operator
				if (tokens >> temp_char) {};
				// No alias necessary, because no other operator should use this char as the first half of a complete operator
				if (LHS && (next_char == '=') && (isdigit(temp_char))) {
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part);
					partExpression newer_part, newer_part_alone;
					new_part = newer_part;
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part_alone);
					new_part_alone = newer_part_alone;
					tokens.unget();
					continue;
				}
				else {
					throw Syntax_Error("Incomplete '==' equality operator.");
				}
			}
			// Compensate for "is_not_equal" ("!=") equality comparison (precedence 3)
			// Alias for "!=" will be '?'
			if (next_char == '!') {
				// Temporarily look ahead to see what the following char will be
				// If anything but the second half of "is_not_equal" operator, move on (could be '!' NOT operator alone)
				tokens >> next_char;
				if (next_char == '=') {
					// See if a digit comes after complete "is_equal" operator
					if (tokens >> temp_char) {}
					// No alias necessary, because no other operator should use this char as the first half of a complete operator
					if (LHS && (next_char == '=') && (isdigit(temp_char))) {
						new_part_alone.op1 = '?';
						high_prec_stack.push(new_part);
						partExpression newer_part, newer_part_alone;
						new_part = newer_part;
						new_part_alone.op1 = next_char;
						high_prec_stack.push(new_part_alone);
						new_part_alone = newer_part_alone;
						tokens.unget();
						continue;
					}
				}
				else {
					// If just plain '!' NOT operator, put the tokens that we peeked at back into the stream
					if (temp_char != 'a')
						tokens.unget();
					tokens.unget();
					next_char = '!';
				}
			}
			// Compensate for ">=" and "<=" comparison operators (precedence 4)
				// Alias for ">=" will be '@'
				// Alias for "<=" will be '`'
			if ((next_char == '>') || (next_char == '<')) {

				// Temporarily look ahead to see what the following char will be
				// If anything but the second half of comparison l/g_or_equal operator, move on 
					// (could be '>' or '<' comparison operator alone)

				// Temp char variable to hold first half of potential l/g_or_equal operator
				char temp_comparison = next_char;
				tokens >> next_char;
				// See if a digit comes after complete "is_equal" operator
				if (tokens >> temp_char) {}
				// Alias: ">=" will be '@', "<=' will be '`'
				if (LHS && (temp_comparison == '>') && (next_char == '=') && (isdigit(temp_char))) {
					next_char = '@';
					high_prec_stack.push(new_part);
					partExpression newer_part, newer_part_alone;
					new_part = newer_part;
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part_alone);
					new_part_alone = newer_part_alone;
					tokens.unget();
					temp_comparison = ' ';
					continue;
				}
				if (LHS && (temp_comparison == '<') && (next_char == '=') && (isdigit(temp_char))) {
					next_char = '`';
					high_prec_stack.push(new_part);
					partExpression newer_part, newer_part_alone;
					new_part = newer_part;
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part_alone);
					new_part_alone = newer_part_alone;
					tokens.unget();
					temp_comparison = ' ';
					continue;
				}
				else {
					// If just plain '<' or '>' operator, put the tokens that we peeked at back into the stream
					tokens.unget();
					tokens.unget();
					temp_comparison = ' ';
				}
			}
			subexpress_define(new_part);
			// Compensate for '-' negative sign operator or "--" decrement operator (precedence 8)
			if (next_char == '-') {
				// Temporarily look ahead to see what the following char will be
				// If anything but decrement operator, put the following char back where we found it
				tokens >> next_char;
				// Alias for negative sign will be '#' char
					// For negative sign, we don't want to put the character back - we want to grab it
					// and the digit that accompanies it, and immediately stick them into the
					// high_prec_stack together - then continue to the top of the while loop
				if ((new_part.lhs == def_zero) && (isdigit(next_char))) {
					tokens.unget();
					tokens >> temp_int;
					new_part.lhs = temp_int;
					new_part.op1 = '#';
					temp_int = 0;
					solo_negation = true;
					high_prec_stack.push(new_part);
					partExpression newer_part;
					new_part = newer_part;
					last_op_prec = 8;
					continue;
				}
				// Standard subtraction operator
				else if ((new_part.lhs != def_zero) && (isdigit(next_char))) {
					tokens.putback(next_char);
					next_char = '-';
				}
				// Alias for decrement operator will be '~' char
				else if ((next_char == '-') && (new_part.lhs == def_zero)) {
					next_char = '~';
				}
				else
					tokens.putback(next_char);
			}
			subexpress_define_reset();
			// Compensate for "++" increment operator (precedence 8)
			// Alias for increment operator will be '$' char
			if ((next_char == '+') && (new_part.lhs == def_zero)) {
				tokens >> next_char;
				if (next_char == '+')
					next_char = '$';
				else
					tokens.putback(next_char);
			}
			subexpress_define(new_part);
			// Any precedence less than 6 gets automatically pushed to the high_prec_stack by itself
				// Subexpressions should not need to be built with precedence operators less than 6
			if (Infix_Evaluator::precedence(next_char) < 6) {
				// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
				subexpress_define(new_part);
				// If previous subexpression was a number, and only a number, push it first
				if (LHS) {
					high_prec_stack.push(new_part);
					partExpression newer_part;
					new_part = newer_part;
				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
					subexpress_define_reset();
				}
				subexpress_define(new_part);
				// If previous subexpression was a full subexpression, push it first
				if (LHS_RHS_OP1) {
					high_prec_stack.push(new_part);
					partExpression newer_part;
					new_part = newer_part;
					// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
					subexpress_define_reset();
				}
				new_part_alone.op1 = next_char;
				high_prec_stack.push(new_part_alone);
				partExpression newer_part_alone;
				new_part_alone = newer_part_alone;
				last_op_prec = Infix_Evaluator::precedence(next_char);
			}
			// Nothing currently within (custom object) partExpression new_part (sub-expression) and precedence of 
			// next_char is ==6 -> push to high_prec_stack by itself.

			else if (NO_LHS_RHS_OP1_OP2 && (Infix_Evaluator::precedence(next_char) == 6)) {
				new_part_alone.op1 = next_char;
				high_prec_stack.push(new_part_alone);
				partExpression newer_part_alone;
				new_part_alone = newer_part_alone;
				last_op_prec = Infix_Evaluator::precedence(next_char);
				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
				subexpress_define_reset();
			}

			// No operators currently within (custom object) partExpression new_part (sub-expression)
			// Add in operator member variable op1 of new_part
			else if ((new_part.op1 == ' ') && (new_part.op2 == ' ')) {
				// If OP1, no LHS, no OP2 => push to stack by itself
				// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
				subexpress_define(new_part);
				if (OP1) {
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part_alone);
					partExpression newer_part_alone;
					new_part_alone = newer_part_alone;
					last_op_prec = Infix_Evaluator::precedence(next_char);
					// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
					subexpress_define_reset();
				}
				else {
					new_part.op1 = next_char;
					last_op_prec = Infix_Evaluator::precedence(new_part.op1);
				}	
			}
			// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
			subexpress_define(new_part);
			// If we are holding on to an operator == prec 6 and there is a full subexpression (with two operators) already, push the subexpression first, then push the operator we are holding on to (by itself)
			if ((LHS_RHS_OP1_OP2) && Infix_Evaluator::precedence(next_char) == 6) {
				high_prec_stack.push(new_part);
				partExpression newer_part;
				new_part = newer_part;
				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
				subexpress_define_reset();

				new_part_alone.op1 = next_char;
				high_prec_stack.push(new_part_alone);
				partExpression newer_part_alone;
				new_part_alone = newer_part_alone;
				last_op_prec = Infix_Evaluator::precedence(next_char);
			}

			// If we have only two things in the subexpression (high prec prefix operator like -, --, ++, or !) and a LHS number, we need to push it to the stack now

			// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
			subexpress_define(new_part);
			if ((LHS_OP1) && Infix_Evaluator::precedence(new_part.op1) == 8) {
				high_prec_stack.push(new_part);
				partExpression newer_part;
				new_part = newer_part;
				
				// Still holding on to current 'next_char' that was read in at the top of the loop - push it to the stack
				// by itself now

				// If next_char is a number
				if (isdigit(next_char)) {
					new_part_alone.lhs = next_char;
					high_prec_stack.push(new_part_alone);
				}

				// If next_char is an operator
				else {
					new_part_alone.op1 = next_char;
					high_prec_stack.push(new_part_alone);
					last_op_prec = Infix_Evaluator::precedence(new_part_alone.op1);
				}

				partExpression newer_part_alone;
				new_part_alone = newer_part_alone;

				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
				subexpress_define_reset();

			}

// ***   START TRYING TO APPROPRIATELY GROUP NEW_PART SUBEXPRESSIONS IF NEEDED BEFORE PUSHING TO STACK ***


			// One operator currently within new_part (sub-expression), and new operator is higher precedence than previous operator
			// LHS and op1 currently within existing sub-expression
			// Add operator to sub-expression as op2
			// Do not push to high_prec_stack (yet), because multiple prec 8 operators chained together need to be accounted for
			// Ex.) 2^!8 => we already read LHS, already read OP1, now read(ing) OP2, no RHS yet

			// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
			subexpress_define(new_part);
			if ((LHS_OP1) && (precedence(next_char) == 8)) {
				// LHS and op1 currently within existing sub-expression
				new_part.op2 = next_char;
				// Do not push yet, because we still need a RHS

				// Set most recent op precedence
				last_op_prec = Infix_Evaluator::precedence(new_part.op2);
				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
				subexpress_define_reset();
			}
			// One operator currently within new_part (sub-expression), and new operator is equal to or lower precedence than previous operator
			// LHS and RHS currently within existing sub-expression
			// Add operator to sub-expression as op2
			// Do not push to high_prec_stack (yet), because multiple prec 8 operators chained together need to be accounted for

			// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
			subexpress_define(new_part);
			if ((LHS_RHS_OP1) && (last_op_prec <= Infix_Evaluator::precedence(next_char))) {
				// If prec 8 operators being dealt with, tally them as needed
				if (Infix_Evaluator::precedence(next_char) == 8) {
					// First prec 8 operator in potential chain
					if (new_part.prec_8_tally == 0) {
						new_part.op2 = next_char;
						last_op_prec = Infix_Evaluator::precedence(new_part.op2);
						new_part.prec_8_tally++;
					}
					// If a chain is encountered, handle it with tallies
					else {
						new_part.prec_8_tally++;
						last_op_prec = Infix_Evaluator::precedence(new_part.op2);
					}
				}
				// If no prec 8 operators to deal with, consider current sub-expression complete and push to high_prec_stack
				// Reset sub-expression variables to defaults with empty constructor
				// Push just the new operator to high_prec_stack as new_part_alone (op1, no other data members)
				// Reset sub-expression variables to defaults with empty constructor
				else if (Infix_Evaluator::precedence(next_char) != 8) {
					// Push current complete sub-expression
					high_prec_stack.push(new_part);
					// Reset new_part to defaults
					partExpression newer_part;
					new_part = newer_part;
					// Wrap solo operator in 'new_part_alone' sub-expression
					new_part_alone.op1 = next_char;
					// Set most recent op precedence
					last_op_prec = Infix_Evaluator::precedence(new_part_alone.op1);
					// Push just the new operator wrapped in sub-expression struct - will be used to connect other sub-expressions later
					high_prec_stack.push(new_part_alone);
					// Reset new_part_alone to defaults
					partExpression newer_part_alone;
					new_part_alone = newer_part_alone;
				} 
				else {
					throw Syntax_Error("I have no idea.");
				}
				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
				subexpress_define_reset();
			}

			// One operator currently within new_part (sub-expression), and new operator is lower precedence than previous operator
				// If LHS and RHS currently within sub-expression
					// Push the currently complete sub-expression to high_prec_stack as new_part (op1, LHS, RHS data members)
			// Push just the new operator to high_prec_stack as new_part_alone (op1, no other data members)
			// Reset sub-expression variables to defaults with empty constructor

			// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
			subexpress_define(new_part);
			if (LHS_RHS_OP1 && (last_op_prec > Infix_Evaluator::precedence(next_char))) {
				// LHS and RHS currently within existing sub-expression
				
				// Push current complete sub-expression
				high_prec_stack.push(new_part);
				// Reset new_part to defaults
				partExpression newer_part;
				new_part = newer_part;
				// Wrap solo operator in 'new_part_alone' sub-expression
				new_part_alone.op1 = next_char;
				// Set most recent op precedence
				last_op_prec = Infix_Evaluator::precedence(new_part_alone.op1);
				// Push just the new operator wrapped in sub-expression struct - will be used to connect other sub-expressions later
				high_prec_stack.push(new_part_alone);
				// Reset new_part_alone to defaults
				partExpression newer_part_alone;
				new_part_alone = newer_part_alone;

				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
				subexpress_define_reset();

				// BAD LOGIC BELOW ???  (Crashes 2+2^2*3 on final push of int 3 to high_prec_eval stack)
				/** LHS present in sub-expression, but no RHS in sub-expression ** THROW ERROR **
				// Only time there should be one operator and one number (and valid) is with prec 8 operator(s)
				// - (so this is only possible if prec 8 is lower than anything, and it's not lower than ANYTHING)
				if (LHS_OP1)
				throw Syntax_Error("Improperly ordered sub-expression.");
				else {
				throw Syntax_Error("I have no idea.");
				} */
			}

			// If the function reaches this point and all member variables of new_part are defaults, that means we are at a place where there is a sub-expression of *JUST* a number - push it to the high_prec_stack wrapped in new_part

			else if ((isdigit(next_char)) && (NO_LHS_RHS_OP1_OP2)) {
				new_part.lhs = next_char;
				high_prec_stack.push(new_part);
				partExpression newer_part;
				new_part = newer_part;
				// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
				subexpress_define_reset();
			}
			// Check 'truth' function subexpress_define() against new_part (sets alias bool variable(s) to true if true)
			subexpress_define(new_part);
		}
	}
	if (!(tokens >> next_char)) {
		if (solo_negation != true) {
			high_prec_stack.push(new_part);
			partExpression newer_part;
			new_part = newer_part;
		}
		solo_negation = false;
	}
	else
		tokens.putback(next_char);

	// Reset short-hand subexpression definitions all back to false (for next subexpress_define() call)
	subexpress_define_reset();
}

/** Evaluates an infix expression/subexpression from the high_prec_stack and 
pushes to appropriate operator and operand stacks for final evaluation.
@param expression The expression to be evaluated
@return The value of the expression
@throws Syntax_Error if a syntax error is detected
*/

std::string Infix_Evaluator::rebuild_expression() {
	std::string new_string;
	partExpression current_sub_expression;
	std::ostringstream rebuilt_expression("");
	high_prec_stack_reversal = high_prec_stack;
	for (size_t i = high_prec_stack.size(); i > 0; i--) {
		high_prec_stack.pop();
	}
	for (size_t i = high_prec_stack_reversal.size(); i > 0; i--) {
		high_prec_stack.push(high_prec_stack_reversal.top());
		high_prec_stack_reversal.pop();
	}
	while (!(high_prec_stack.empty())) {
		current_sub_expression = high_prec_stack.top();
		high_prec_stack.pop();

		// Either only a number (LHS) or only an operator (op1) exist in the struct at the top of the high_prec_eval stack
		subexpress_define(current_sub_expression);
		if (LHS || OP1) {
			if (LHS) {
				rebuilt_expression << current_sub_expression.lhs;
				new_string += rebuilt_expression.str();
				new_string += " ";
				rebuilt_expression.str("");
				rebuilt_expression.clear();
			}
			else if (OP1) {
				rebuilt_expression << current_sub_expression.op1;
				new_string += rebuilt_expression.str();
				new_string += " ";
				rebuilt_expression.str("");
				rebuilt_expression.clear();
			}
			subexpress_define_reset();
		}

		// Just a LHS number and 1 operator exist within the subexpression on top of the high_prec_eval stack
		subexpress_define(current_sub_expression);
		if (LHS_OP1) {
			if (current_sub_expression.op1 == '#') {
				rebuilt_expression << current_sub_expression.lhs;
				rebuilt_expression << "#";
				new_string += rebuilt_expression.str();
				new_string += " ";
				rebuilt_expression.str("");
				rebuilt_expression.clear();
			}
			else {
				int result;
				int lhs = current_sub_expression.lhs;
				char op = current_sub_expression.op1;
				result = Infix_Evaluator::eval_op(op, lhs);
				rebuilt_expression << result;
				new_string += rebuilt_expression.str();
				new_string += " ";
				rebuilt_expression.str("");
				rebuilt_expression.clear();
			}
		}
		subexpress_define_reset();

		subexpress_define(current_sub_expression);
		if (LHS_RHS_OP1) {
			int result;
			int lhs = current_sub_expression.lhs;
			int rhs = current_sub_expression.rhs;
			char op = current_sub_expression.op1;
			result = Infix_Evaluator::eval_op(op, rhs, lhs);
			rebuilt_expression << result;
			new_string += rebuilt_expression.str();
			new_string += " ";
			rebuilt_expression.str("");
			rebuilt_expression.clear();
		}
		subexpress_define_reset();

		subexpress_define(current_sub_expression);
		if (LHS_OP1_OP2) {
			int result, lhs;
			char op;
			if (current_sub_expression.prec_8_tally == 1) {
				int lhs = current_sub_expression.rhs;
				char op = current_sub_expression.op2;
				result = Infix_Evaluator::eval_op(op, lhs);
			}
			else if (current_sub_expression.prec_8_tally > 1) {
				int tally_num = current_sub_expression.prec_8_tally;
				for (int i = 0; i < tally_num; i++) {
					int lhs = current_sub_expression.rhs;
					char op = current_sub_expression.op2;
					result = Infix_Evaluator::eval_op(op, lhs);
				}
			}
			int rhs = result;
			lhs = current_sub_expression.rhs;
			op = current_sub_expression.op1;
			result = Infix_Evaluator::eval_op(op, lhs);
			rebuilt_expression << result;
			new_string += rebuilt_expression.str();
			new_string += " ";
			rebuilt_expression.str("");
			rebuilt_expression.clear();
		}

		subexpress_define_reset();

		subexpress_define(current_sub_expression);
		if (LHS_RHS_OP1_OP2) {
			int result, lhs, rhs;
			char op;

			if (Infix_Evaluator::precedence(current_sub_expression.op1) == 8) {
				throw Syntax_Error("Improperly ordered sub expression.");
			}

			else if (Infix_Evaluator::precedence(current_sub_expression.op2) == 8) {
				if (current_sub_expression.prec_8_tally == 0) {
					lhs = current_sub_expression.rhs;
					op = current_sub_expression.op2;
					result = Infix_Evaluator::eval_op(op, lhs);
				}
				else if (current_sub_expression.prec_8_tally > 0) {
					int tally_num = current_sub_expression.prec_8_tally;
					for (int i = 0; i < tally_num; i++) {
						lhs = current_sub_expression.rhs;
						op = current_sub_expression.op2;
						result = Infix_Evaluator::eval_op(op, lhs);
					}
				}
				rhs = result;
				lhs = current_sub_expression.lhs;
				op = current_sub_expression.op1;
				result = Infix_Evaluator::eval_op(op, rhs, lhs);
				rebuilt_expression << result;
				new_string += rebuilt_expression.str();
				new_string += " ";
				rebuilt_expression.str("");
				rebuilt_expression.clear();
			}
		}

		subexpress_define_reset();

		subexpress_define(current_sub_expression);
		if ((NO_LHS_RHS_OP1_OP2) && (new_string != ""))
			return new_string;
		
		subexpress_define_reset();
	}

	return new_string;
}

/** Evaluates an infix expression.
@param expression The expression to be evaluated
@return The value of the expression
@throws Syntax_Error if a syntax error is detected
*/
int Infix_Evaluator::equate(const std::string& expression) {
	// Be sure the operand and operator stacks are empty
	while (!operand_stack.empty())
		operand_stack.pop();

	while (!operator_stack.empty())
		operator_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;
	// double_char_op declared outside scope of token retrieval loop to compensate/track for "++" and "--" operators 
	char double_char_op = '0';

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
			// The number 10 is pushed onto the operand stack
			operand_stack.push(value);
		}
		// Ex. 2 cont) char '*' is definitely an operator, so is_operator() returns true
		else if (is_operator(next_char)) {
			// If stack is empty and/or next_char is an opening paren, push onto operator stack
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

				// If operator stack is not empty, and the operator has a equal to or lower precedence than 5, push operator onto the operator stack
				if ((!operator_stack.empty()) && (precedence(next_char) <= 5))
					operator_stack.push(next_char);

				// If operator has a greater precedence than 5 (meaning * / or %), let's evaluate the sub-expression

				else if (precedence(op) > 5) {
					// Store lhs in a temp variable
					int lhs = operand_stack.top();
					// Pop the lhs off the top of the operand stack
					operand_stack.pop();
					// Read in RHS of expression
					int rhs;
					tokens >> next_char;
					// !!!(IF RHS IS A DIGIT, ALL GOOD)!!!
					if (isdigit(next_char)) {
						tokens.putback(next_char);
						tokens >> rhs;
					}
					// !!!(IF RHS IS AN OPERATOR, WHOOPS)!!!
					else if (is_operator(next_char)) {
						throw Syntax_Error("Expected RHS operand");
						return -1;
					}
					// !!!(IF RHS IS NEITHER OPERAND OR OPERATOR, EXTRA WHOOPS)!!!
					else {
						throw Syntax_Error("Expected RHS operand");
						return -1;
					}
					// Send operator, rhs, and lhs to the eval function and return a result
					int result = eval_op(op, rhs, lhs);
					// The number result is pushed onto the operand stack
					operand_stack.push(result);
				}

				else {
					throw Syntax_Error("Precedence eval error");
					return -1;
				}
			}
		}
		// Token is neither operand or operator - throw an error
		else {
			throw Syntax_Error("Invalid character encountered - neither operand or operator");
			return -1;
		}
	}

	// If we reach this point, that means there were no more tokens to read in
	// We also should have evaluated all sub-expressions at this point

	// Declare variable answer outside the scope of the loop(s)
	int answer = 0;

	// While the operand stack is not empty
	while (!operand_stack.empty()) {
		// If the operand stack has only one thing in it and the operator stack has only one thing in it,
		// it might be because it is a negation sign or a NOT operator -> eval and return together!
		if ((operand_stack.size() == 1) && (operator_stack.size() == 1)) {
			int lhs = operand_stack.top();
			operand_stack.pop();

			char op = operator_stack.top();
			operator_stack.pop();

			int result = eval_op(op, lhs);
			operand_stack.push(result);
		}

		// If the operand stack is not empty AND the operator stack IS empty
		// If the operand stack has only one thing in it, that's the answer -> return it!
		if ((!operand_stack.empty()) && (operator_stack.empty())) {
			if (operand_stack.size() == 1) {
				// If the operand stack is not empty and the operator stack is empty, and there is only 1 item in the operand stack, that means the answer must be on top of the stack
				answer = operand_stack.top();
				operand_stack.pop();
			}
		}
		// If the operand stack is not empty AND the operator stack is not empty
		// Still stuff left to evaluate -> let's do it
		else if ((!operand_stack.empty()) && (!operator_stack.empty())) {
			// The operand stack is not empty, and there's more than 1 item in it - and the operator stack is not yet empty - time to evaluate the rest of the expression
			if ((operand_stack.size() > 1) && (operator_stack.size() > 0)) {
				int lhs = operand_stack.top();
				operand_stack.pop();

				char op = operator_stack.top();
				operator_stack.pop();

				int rhs = operand_stack.top();
				operand_stack.pop();

				int result = eval_op(op, rhs, lhs);
				operand_stack.push(result);
			}
			// How did we get here if neither stack is empty and we don't already have the answer?  Throw an error.
			else {
				throw Syntax_Error("Stack logic error");
				return -1;
			}
		}
		// Only way to get here is if operand stack is empty - honestly should be impossible - if it happens, throw an error
		else {
			throw Syntax_Error("Operand stack should not be empty");
			return -1;
		}
	}

	// By this point, if the operand stack *is* empty, that means we already have the answer - return it
	if (operand_stack.empty()) {
		return answer;
	}

	// If the operand stack is not empty, it should have been caught in the above while loop - throw an error
	else {
		throw Syntax_Error("Operand stack should be empty - answer should have been returned.");
	}
}

// Wrapper function to help readability of code - checks/set true flag for bool alias variables
bool Infix_Evaluator::subexpress_define(partExpression subexpress) {
	if ((subexpress.lhs != def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 != ' '))
		return (LHS_RHS_OP1_OP2 = true);
	else if ((subexpress.lhs != def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 == ' '))
		return (LHS_RHS_OP1 = true);
	else if ((subexpress.lhs != def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 != ' '))
		return (LHS_RHS_OP1 = true);
	else if ((subexpress.lhs != def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 == ' '))
		return (LHS_RHS = true);
	else if ((subexpress.lhs != def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 != ' '))
		return (LHS_OP1_OP2 = true);
	else if ((subexpress.lhs != def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 == ' '))
		return (LHS_OP1 = true);
	else if ((subexpress.lhs != def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 != ' '))
		return (LHS_OP2 = true);
	else if ((subexpress.lhs != def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 == ' '))
		return (LHS = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 != ' '))
		return (RHS_OP1_OP2 = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 == ' '))
		return (RHS_OP1 = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 != ' '))
		return (RHS_OP2 = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs != def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 == ' '))
		return (RHS = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 != ' '))
		return (OP1_OP2 = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 != ' ') && (subexpress.op2 == ' '))
		return (OP1 = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 != ' '))
		return (OP2 = true);
	else if ((subexpress.lhs == def_zero) && (subexpress.rhs == def_zero) && (subexpress.op1 == ' ') && (subexpress.op2 == ' '))
		return (NO_LHS_RHS_OP1_OP2 = true);
	return false;
}

// When called, resets all true flags assigned by subexpress_define back to false
bool Infix_Evaluator::subexpress_define_reset() {
	return(LHS_RHS_OP1_OP2 = false,
		LHS_RHS_OP1 = false,
		LHS_RHS_OP2 = false,
		LHS_RHS = false,
		LHS_OP1_OP2 = false,
		LHS_OP1 = false,
		LHS_OP2 = false,
		LHS = false,
		RHS_OP1_OP2 = false,
		RHS_OP1 = false,
		RHS_OP2 = false,
		RHS = false,
		OP1_OP2 = false,
		OP1 = false,
		OP2 = false,
		NO_LHS_RHS_OP1_OP2 = false);
}

/** Evaluates the current operator.
This function pops the two operands off the expression
stack and applies the operator.
@param op A character representing the operator
@throws Syntax_Error if top is attempted on an empty stack
*/
int Infix_Evaluator::eval_op(char op, int rhs, int lhs) {
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
	case '^':
		if (rhs == 0) {
			result = 1;
			break;
		}
		else if (rhs == 1) {
			result = lhs;
			break;
		}
		else {
			result = lhs;
			for (int i = rhs; i > 1; i--) {
				result *= lhs;
			}
			break;
		}
	case '|':
		if ((lhs == 0) && (rhs == 0))
			result = 0;
		else
			result = 1;
	case '&':
		if ((lhs == 1) && (rhs == 1))
			result = 1;
	case '=':
		if (lhs == rhs)
			result = 1;
	case '?':
		if (lhs != rhs)
			result = 1;
	case '>':
		if (lhs > rhs)
			result = 1;
	case '@':
		if (lhs >= rhs)
			result = 1;
	case '<':
		if (lhs < rhs)
			result = 1;
	case '`':
		if (lhs <= rhs)
			result = 1;
	}
	return result;
}

// Overloaded to deal with prefix operators
int Infix_Evaluator::eval_op(char op, int rhs) {
	int result = 0;
	switch (op) {
		// 'Not' operator
	case '!': result = 0 * rhs;
		break;
		// '$' is a stand-in char for ++/increment operator
	case '$': result = ++rhs;
		break;
		// '~' is a stand-in char for --/decrement operator
	case '~': result = --rhs;
		break;
		// Negative operator
	case '#': result = (-1) * rhs;
		break;
	}
	return result;
}


// Wrapper function to incorporate all other necessary functions
int Infix_Evaluator::eval(const std::string& expression) {
	Infix_Evaluator::high_prec_eval(expression);
	std::string new_expression = Infix_Evaluator::rebuild_expression();

	return (Infix_Evaluator::equate(new_expression));
}


// Main function for testing purposes with console output display only
int main() {

	// Debug testing strings:

	//Works = 14
	//string expr = "2+2^2*3";

	//Works = 6
	//string expr = "10 - 2 * 5 + 6 * 2 / 2";

	//Works = 16
	//string expr = "10 + 2 * 3";

	//Works = 3
	//string expr = "++2";

	//Works = 1
	//string expr = "2^!8";

	//Depending on interpretation of acceptable 'division', this works per C++ language -> 2 + 0 * 8 = 2
	//string expr = "1 * 2 + 2 ^ !8 / ++3 * 8";

	//Works = 7
	//string expr = "1+2*3";

	//Works = 1
	//string expr = "2^0";

	//Works = 1 (true)
	//string expr = "1 <= 1";

	// Works = 1 (true)
	// string expr = "2 != 1";

	// Works = 1 (true)
	// string expr = "1 || 0";

	// Works = 1 (true)
	// string expr = "6 != 5";

	// Works = -1
	// string expr = "-1";

	// Works = -25
	// string expr = "-5 * 5";

	// Works = 1
	// string expr = "--2";

	// ***NEEDS DEBUGGING***
	// string expr = "6>5 && 4>5";

	std::cout << "Current infix string to be evaluated is '" << expr << "'\n\n";

	Infix_Evaluator new_eval;

	int answer;

	answer = new_eval.Infix_Evaluator::eval(expr);

	std::cout << "Answer is: " << answer << "\n\n";

	return 0;

}