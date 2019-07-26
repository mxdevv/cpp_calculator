#ifndef __CALC_CPP__
#define __CALC_CPP__

/* Автор: Визгалов Максим (mxdevv)
 * Дата начала: 23.07.19
 * Программа: калькулятор
 *
 * Описание: Программа-калькулятор, как вступительное задание для устройства в
 *   cqg.
 *
 * Ширина кода: 80 символов. Используется 2 пробела, вместо табуляции.
 */

#include <iostream>
#include <sstream>
#include <limits>
#include <string>
#include <vector>
#include <math.h>

#include "calc.h"

void Calculator::read(const std::string&& str) {
  expression = str;
  expression.push_back('\0');
  state = e_state::READED;
}

void Calculator::read(const std::string& str) {
  expression = str;
  expression.push_back('\0');
  state = e_state::READED;
}

void Calculator::parse() {
  tokens.clear();

  Token::e_type cur_state = Token::e_type::NONE,
      prev_state = Token::e_type::NONE;

  int parse_start_pos = 0, parse_end_pos = 0;

  char ch;
  for(; parse_end_pos < expression.size(); parse_end_pos++) {
    ch = expression[parse_end_pos];
    prev_state = cur_state;

    switch(ch) {
      case 0:
        cur_state = Token::e_type::NONE;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case '0' ... '9':
        if (prev_state == Token::e_type::INTEGER
            || prev_state == Token::e_type::UNARY_PLUS
            || prev_state == Token::e_type::UNARY_MINUS) {
          cur_state = Token::e_type::INTEGER;
        } else if (prev_state == Token::e_type::FLOAT) {
          /* nothing */
        } else {
          cur_state = Token::e_type::INTEGER;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        }
        break;
      case '.':
        if (prev_state == Token::e_type::INTEGER) {
          cur_state = Token::e_type::FLOAT;
        }
        break;
      case 'a' ... 'z':
      case 'A' ... 'Z':
        if (prev_state != Token::e_type::STRING) {
          cur_state = Token::e_type::STRING;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        }
        break;
      case '+':
        if (prev_state == Token::e_type::OPERATOR_PLUS
            || prev_state == Token::e_type::OPERATOR_MINUS) {
          cur_state = Token::e_type::UNARY_PLUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else if (prev_state == Token::e_type::INTEGER
            || prev_state == Token::e_type::FLOAT
            || prev_state == Token::e_type::STRING) {
          cur_state = Token::e_type::OPERATOR_PLUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else {
          add_token(prev_state, parse_start_pos, parse_end_pos);
          cur_state = Token::e_type::UNARY_PLUS;
        }
        break;
      case '-':
        if (prev_state == Token::e_type::OPERATOR_PLUS
            || prev_state == Token::e_type::OPERATOR_MINUS) {
          cur_state = Token::e_type::UNARY_MINUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else if (prev_state == Token::e_type::INTEGER
            || prev_state == Token::e_type::FLOAT
            || prev_state == Token::e_type::STRING) {
          cur_state = Token::e_type::OPERATOR_MINUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else {
          add_token(prev_state, parse_start_pos, parse_end_pos);
          cur_state = Token::e_type::UNARY_MINUS;
        }
        break;
      case '*':
        cur_state = Token::e_type::OPERATOR_MULT;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case '/':
        cur_state = Token::e_type::OPERATOR_DIV;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case '(':
      case '[':
        cur_state = Token::e_type::OPEN_BRACKET;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case ')':
      case ']':
        cur_state = Token::e_type::CLOSE_BRACKET;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
    }
  }

  #ifdef CALCULATOR_DEBUG
    std::cerr << "[parse log]\n";
    std::cerr << "tokens: ";
    tokens_short_check(tokens);
  #endif

  state = e_state::PARSED;
}

void Calculator::precheck() const {
  for(const Token& tok : tokens) {
    switch(tok.type) {
      case Token::e_type::STRING:
        throw precheck_unidentified_string_exception();
      case Token::e_type::UNARY_PLUS:
        throw precheck_unary_plus_exception();
      case Token::e_type::UNARY_MINUS:
        throw precheck_unary_minus_exception();
    }
  }
  if (tokens.empty()) {
    throw precheck_empty_exception();
  }
}

void Calculator::add_token(const Token::e_type type, int& parse_start_pos,
    const int parse_end_pos) {

  double val;
  switch(type) {
    case Token::e_type::NONE: {
      return;
    }
    case Token::e_type::INTEGER:
    case Token::e_type::FLOAT: {
      std::stringstream ss;
      ss << expression.substr(parse_start_pos, parse_end_pos - parse_start_pos);
      ss >> val;
      tokens.push_back(*new Token(Token::e_type::FLOAT, val));
      break;
    }
    default:
      tokens.push_back(*new Token(type,
          std::numeric_limits<double>::quiet_NaN()));
      break;
  }

  parse_start_pos = parse_end_pos;
}

void Calculator::tokens_short_check(
    const std::vector<Token>& tokens) const {
  for(const Token& tkn : tokens) {
    switch(tkn.type) {
      case Token::e_type::NONE:
        std::cerr << '?';
        break;
      case Token::e_type::INTEGER:
        std::cerr << 'I';
        std::cerr << tkn.value;
        break;
      case Token::e_type::FLOAT:
        std::cerr << tkn.value;
        break;
      case Token::e_type::STRING:
        std::cerr << 'S';
        break;
      case Token::e_type::SYMBOL:
        std::cerr << 'C';
        break;
      case Token::e_type::OPERATOR_PLUS:
        std::cerr << '+';
        break;
      case Token::e_type::OPERATOR_MINUS:
        std::cerr << '-';
        break;
      case Token::e_type::OPERATOR_MULT:
        std::cerr << '*';
        break;
      case Token::e_type::OPERATOR_DIV:
        std::cerr << '\\';
        break;
      case Token::e_type::CONSTANT:
        std::cerr << '[';
        std::cerr << tkn.value;
        std::cerr << ']';
        break;
      case Token::e_type::VARIABLE:
        std::cerr << 'V';
        break;
      case Token::e_type::FUNCTION:
        std::cerr << 'F';
        break;
      case Token::e_type::UNARY_PLUS:
        std::cerr << "U+";
        break;
      case Token::e_type::UNARY_MINUS:
        std::cerr << "U-";
        break;
      case Token::e_type::OPEN_BRACKET:
        std::cerr << "(";
        break;
      case Token::e_type::CLOSE_BRACKET:
        std::cerr << ")";
        break;
    }
    std::cerr << ' ';
  }
  std::cerr << '\n';
}

void Calculator::reverse_polish_notation() {
  std::vector<Token> stack, output;
  Token tok2;
  for(Token& tok : tokens) {
    switch(tok.type) {
      case Token::e_type::FLOAT:
      case Token::e_type::CONSTANT:
        output.push_back(tok);
        break;
      case Token::e_type::OPERATOR_PLUS:
      case Token::e_type::OPERATOR_MINUS:
      case Token::e_type::OPERATOR_MULT:
      case Token::e_type::OPERATOR_DIV:
        while(!stack.empty()) {
          tok2 = stack.back();
          stack.pop_back();
          if (operator_precedence(tok.type) <= operator_precedence(tok2.type)) {
            output.push_back(tok2);
          } else {
            stack.push_back(tok2);
            break;
          }
        }
        stack.push_back(tok);
        break;
      case Token::e_type::OPEN_BRACKET:
        stack.push_back(tok);
        break;
      case Token::e_type::CLOSE_BRACKET:
        while(1) {
          if (!stack.empty()) {
            tok2 = stack.back();
            stack.pop_back();
            if (tok2.type != Token::e_type::OPEN_BRACKET) {
              output.push_back(tok2);
              continue;
            }
            break;
          }
          throw reverse_polish_notation_brackets_exception();
        }
        break;
    }
  }
  while(!stack.empty()) {
    tok2 = stack.back();
    stack.pop_back();
    output.push_back(tok2);
  }

  tokens = output;

  #ifdef CALCULATOR_DEBUG
    std::cerr << "[reverse_polish_notation log]\n";

    std::cerr << "stack: ";
    tokens_short_check(stack);

    std::cerr << "output: ";
    tokens_short_check(output);

    std::cerr << "tokens: ";
    tokens_short_check(tokens);
  #endif

  state = e_state::POLISH_NOTATION_COMPLETE;
}

int Calculator::operator_precedence(const Token::e_type type) const {
  switch(type) {
    case Token::e_type::OPERATOR_MULT:
    case Token::e_type::OPERATOR_DIV:
      return 2;
    case Token::e_type::OPERATOR_PLUS:
    case Token::e_type::OPERATOR_MINUS:
      return 1;
    case Token::e_type::OPEN_BRACKET:
      return 0;
  }
}

double Calculator::polish_calc() {
  std::vector<Token> stack;

  Token tok2, tok3;
  for(Token& tok : tokens) {
    switch(tok.type) {
      case Token::e_type::FLOAT:
      case Token::e_type::CONSTANT:
        stack.push_back(tok);
        break;
      case Token::e_type::OPERATOR_PLUS:
        if (stack.size() < 2) {
          throw polish_calc_exception();
        }
        tok3 = stack.back(); stack.pop_back();
        tok2 = stack.back(); stack.pop_back();
        stack.push_back(*new Token(Token::e_type::FLOAT,
            tok2.value + tok3.value));
        break;
      case Token::e_type::OPERATOR_MINUS:
        if (stack.size() < 2) {
          throw polish_calc_exception();
        }
        tok3 = stack.back(); stack.pop_back();
        tok2 = stack.back(); stack.pop_back();
        stack.push_back(*new Token(Token::e_type::FLOAT,
            tok2.value - tok3.value));
        break;
      case Token::e_type::OPERATOR_DIV:
        if (stack.size() < 2) {
          throw polish_calc_exception();
        }
        tok3 = stack.back(); stack.pop_back();
        tok2 = stack.back(); stack.pop_back();
        stack.push_back(*new Token(Token::e_type::FLOAT,
            tok2.value / tok3.value));
        break;
      case Token::e_type::OPERATOR_MULT:
        if (stack.size() < 2) {
          throw polish_calc_exception();
        }
        tok3 = stack.back(); stack.pop_back();
        tok2 = stack.back(); stack.pop_back();
        stack.push_back(*new Token(Token::e_type::FLOAT,
            tok2.value * tok3.value));
        break;
    }
  }
  
  #ifdef CALCULATOR_DEBUG
    std::cerr << "[polish_calc log]\n";

    std::cerr << "tokens: ";
    tokens_short_check(tokens);

    std::cerr << "stack: ";
    tokens_short_check(stack);
  #endif

  if (stack.size() > 0)
    return stack.back().value;
  else
    throw polish_calc_exception();
}

Calculator::Calculator(const std::string&& str) 
    : Calculator() {
  read(std::move(str));
}

double Calculator::calculate() {
  parse();
  precheck();
  reverse_polish_notation();
  polish_calc();
}

double Calculator::calculate(const std::string&& str) {
  read(std::move(str));
  return calculate();
}

double Calculator::calculate(const std::string& str) {
  read(str);
  return calculate();
}

void Calculator::operator << (const std::string&& str) {
  read(std::move(str));
}

void Calculator::operator >> (double& value) {
  value = calculate();
}

#endif // __CALC_CPP__
