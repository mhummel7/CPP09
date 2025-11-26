/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:13 by mhummel           #+#    #+#             */
/*   Updated: 2025/11/26 14:13:36 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <climits> // for INT_MAX and INT_MIN

RPN::RPN() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other) {
	if (this != &other) {
		_stack = other._stack;
	}
	return *this;
}

RPN::~RPN() {}

int RPN::evaluate(const std::string& expression) {
	std::stringstream ss(expression);
	std::string token;
	while (ss >> token) {
		if (token == "+" || token == "-" || token == "*" || token == "/") {
			if (_stack.size() < 2) {
				throw std::runtime_error("Error: insufficient operands for operator");
			}
			int b = _stack.top(); _stack.pop();
			int a = _stack.top(); _stack.pop();
			if (token == "+") {
				if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
					throw std::runtime_error("Error: Integer overflow");
				}
				_stack.push(a + b);
			} else if (token == "-") {
				if ((b > 0 && a < INT_MIN + b) || (b < 0 && a > INT_MAX + b)) {
					throw std::runtime_error("Error: Integer overflow");
				}
				_stack.push(a - b);
			} else if (token == "*") {
				if (a > INT_MAX / abs(b) || b > INT_MAX / abs(a)) {
					throw std::runtime_error("Error: Integer overflow");
				}
				_stack.push(a * b);
			} else if (token == "/") {
				if (b == 0) throw std::runtime_error("Error: Division by zero");
				_stack.push(a / b);
			}
			// int b = _stack.top(); _stack.pop();
			// int a = _stack.top(); _stack.pop();
			// if (token == "+") _stack.push(a + b);
			// else if (token == "-") _stack.push(a - b);
			// else if (token == "*") _stack.push(a * b);
			// else if (token == "/") {
			// 	if (b == 0) throw std::runtime_error("Error: Division by zero");
			// 	_stack.push(a / b);
			// }
		} 	else {
			// Nur einzelne Ziffern von 0–9 erlaubt (wie im Subject verlangt)
				if (token.length() != 1 || !std::isdigit(static_cast<unsigned char>(token[0]))) {
					throw std::runtime_error("Error: 0-9");
				}
				int num = token[0] - '0';  // '0' → 0, '1' → 1, ..., '9' → 9
				_stack.push(num);
		}
	}
	if (_stack.size() != 1) {
		throw std::runtime_error("Error: Invalid expression (stack size != 1)");
	}
	int result = _stack.top();
	_stack.pop();
	return result;
}
