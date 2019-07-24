/* Автор: Визгалов Максим (mxdevv)
 * Дата начала: 2.07.19
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

#include "calc.h"

using std::string;

void Calculator::read(string&& str) {
  std::cerr << "Calculator::read(...) run\n";

  expression = str;
  expression.push_back('\0');
}

void Calculator::parse() {
  std::cerr << "Calculator::parse() run\n";

  using e_state = Token::e_type;
  e_state cur_state = e_state::NONE, prev_state = e_state::NONE;

  int parse_start_pos = 0, parse_end_pos = 0;

  char ch;
  for(; parse_end_pos < expression.size(); parse_end_pos++) {
    ch = expression[parse_end_pos];
    prev_state = cur_state;

    switch(ch) {
      case 0 ... 32:
        cur_state = e_state::NONE;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case '0' ... '9':
        if (prev_state == e_state::INTEGER
            || prev_state == e_state::UNARY_PLUS
            || prev_state == e_state::UNARY_MINUS) {
          cur_state = e_state::INTEGER;
        } else if (prev_state == e_state::FLOAT) {
          /* nothing */
        } else {
          cur_state = e_state::INTEGER;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        }
        break;
      case '.':
        if (prev_state == e_state::INTEGER) {
          cur_state = e_state::FLOAT;
        }
        break;
      case 'a' ... 'z':
      case 'A' ... 'Z':
        if (prev_state == e_state::VARIABLE) {
          cur_state = e_state::FUNCTION;
        } else {
          cur_state = e_state::VARIABLE;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        }
        break;
      case '+':
        if (prev_state == e_state::OPERATOR_PLUS
            || prev_state == e_state::OPERATOR_MINUS) {
          cur_state = e_state::UNARY_PLUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else if (prev_state == e_state::INTEGER
            || prev_state == e_state::FLOAT) {
          cur_state = e_state::OPERATOR_PLUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else {
          add_token(prev_state, parse_start_pos, parse_end_pos);
          cur_state = e_state::UNARY_PLUS;
        }
        break;
      case '-':
        if (prev_state == e_state::OPERATOR_PLUS
            || prev_state == e_state::OPERATOR_MINUS) {
          cur_state = e_state::UNARY_MINUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else if (prev_state == e_state::INTEGER
            || prev_state == e_state::FLOAT) {
          cur_state = e_state::OPERATOR_MINUS;
          add_token(prev_state, parse_start_pos, parse_end_pos);
        } else {
          add_token(prev_state, parse_start_pos, parse_end_pos);
          cur_state = e_state::UNARY_MINUS;
        }
        break;
      case '*':
        cur_state = e_state::OPERATOR_MULT;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case '/':
        cur_state = e_state::OPERATOR_DIV;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case '(':
      case '[':
        cur_state = e_state::OPEN_BRACKET;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
      case ')':
      case ']':
        cur_state = e_state::CLOSE_BRACKET;
        add_token(prev_state, parse_start_pos, parse_end_pos);
        break;
    }

    std::cerr << ch << '\n';

    std::cerr << "parse_start_pos: " << parse_start_pos << '\n';
    std::cerr << "parse_end_pos: " << parse_end_pos << '\n';

    std::cerr << "prev_state:  ";
    switch(prev_state) {
      case e_state::NONE:
        std::cerr << "NONE";
        break;
      case e_state::INTEGER:
        std::cerr << "INTEGER";
        break;
      case e_state::FLOAT:
        std::cerr << "FLOAT";
        break;
      case e_state::STRING:
        std::cerr << "STRING";
        break;
      case e_state::SYMBOL:
        std::cerr << "SYMBOL";
        break;
      case e_state::OPERATOR_PLUS:
        std::cerr << "OPERATOR_PLUS";
        break;
      case e_state::OPERATOR_MINUS:
        std::cerr << "OPERATOR_MINUS";
        break;
      case e_state::OPERATOR_MULT:
        std::cerr << "OPERATOR_MULT";
        break;
      case e_state::OPERATOR_DIV:
        std::cerr << "OPERATOR_DIV";
        break;
      case e_state::VARIABLE:
        std::cerr << "VARIABLE";
        break;
      case e_state::FUNCTION:
        std::cerr << "FUNCTION";
        break;
      case e_state::UNARY_PLUS:
        std::cerr << "UNARY_PLUS";
        break;
      case e_state::UNARY_MINUS:
        std::cerr << "UNARY_MINUS";
        break;
      case e_state::OPEN_BRACKET:
        std::cerr << "OPEN_BRACKET";
        break;
      case e_state::CLOSE_BRACKET:
        std::cerr << "CLOSE_BRACKET";
        break;
    }
    std::cerr << '\n';

    std::cerr << "cur_state:  ";
    switch(cur_state) {
      case e_state::NONE:
        std::cerr << "NONE";
        break;
      case e_state::INTEGER:
        std::cerr << "INTEGER";
        break;
      case e_state::FLOAT:
        std::cerr << "FLOAT";
        break;
      case e_state::STRING:
        std::cerr << "STRING";
        break;
      case e_state::SYMBOL:
        std::cerr << "SYMBOL";
        break;
      case e_state::OPERATOR_PLUS:
        std::cerr << "OPERATOR_PLUS";
        break;
      case e_state::OPERATOR_MINUS:
        std::cerr << "OPERATOR_MINUS";
        break;
      case e_state::OPERATOR_MULT:
        std::cerr << "OPERATOR_MULT";
        break;
      case e_state::OPERATOR_DIV:
        std::cerr << "OPERATOR_DIV";
        break;
      case e_state::VARIABLE:
        std::cerr << "VARIABLE";
        break;
      case e_state::FUNCTION:
        std::cerr << "FUNCTION";
        break;
      case e_state::UNARY_PLUS:
        std::cerr << "UNARY_PLUS";
        break;
      case e_state::UNARY_MINUS:
        std::cerr << "UNARY_MINUS";
        break;
      case e_state::OPEN_BRACKET:
        std::cerr << "OPEN_BRACKET";
        break;
      case e_state::CLOSE_BRACKET:
        std::cerr << "CLOSE_BRACKET";
        break;
    }
    std::cerr << '\n';
  }
}

void Calculator::add_token(Token::e_type type, int& parse_start_pos,
    int& parse_end_pos) {
  
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

void Calculator::tokens_check() {
  std::cerr << "Calculator::tokens_check() run\n";

  for(Token& tkn : tokens) {
    std::cerr << "Token: ";
    switch(tkn.type) {
      case Token::e_type::NONE:
        std::cerr << "NONE";
        break;
      case Token::e_type::INTEGER:
        std::cerr << "INTEGER";
        break;
      case Token::e_type::FLOAT:
        std::cerr << "FLOAT";
        break;
      case Token::e_type::STRING:
        std::cerr << "STRING";
        break;
      case Token::e_type::SYMBOL:
        std::cerr << "SYMBOL";
        break;
      case Token::e_type::OPERATOR_PLUS:
        std::cerr << "OPERATOR_PLUS";
        break;
      case Token::e_type::OPERATOR_MINUS:
        std::cerr << "OPERATOR_MINUS";
        break;
      case Token::e_type::OPERATOR_MULT:
        std::cerr << "OPERATOR_MULT";
        break;
      case Token::e_type::OPERATOR_DIV:
        std::cerr << "OPERATOR_DIV";
        break;
      case Token::e_type::VARIABLE:
        std::cerr << "VARIABLE";
        break;
      case Token::e_type::FUNCTION:
        std::cerr << "FUNCTION";
        break;
      case Token::e_type::UNARY_PLUS:
        std::cerr << "UNARY_PLUS";
        break;
      case Token::e_type::UNARY_MINUS:
        std::cerr << "UNARY_MINUS";
        break;
      case Token::e_type::OPEN_BRACKET:
        std::cerr << "OPEN_BRACKET";
        break;
      case Token::e_type::CLOSE_BRACKET:
        std::cerr << "CLOSE_BRACKET";
        break;
    }
    std::cerr << ", value: " << tkn.value << '\n';
  }
}

void Calculator::tokens_short_check() {
  std::cerr << "Calculator::tokens_short_check() run\n";

  for(Token& tkn : tokens) {
    switch(tkn.type) {
      case Token::e_type::NONE:
        std::cerr << '?';
        break;
      case Token::e_type::INTEGER:
        std::cerr << '$';
        std::cerr << tkn.value;
        break;
      case Token::e_type::FLOAT:
        std::cerr << tkn.value;
        break;
      case Token::e_type::STRING:
        std::cerr << '#';
        break;
      case Token::e_type::SYMBOL:
        std::cerr << '@';
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
      case Token::e_type::VARIABLE:
        std::cerr << '&';
        break;
      case Token::e_type::FUNCTION:
        std::cerr << '!';
        break;
      case Token::e_type::UNARY_PLUS:
        std::cerr << "UNARY_PLUS";
        break;
      case Token::e_type::UNARY_MINUS:
        std::cerr << "UNARY_MINUS";
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

void Tester::test() {
  Calculator calc;
  calc.read("-2--1.1111+(-10-2)/3");
  calc.parse();
  calc.tokens_check();
  calc.tokens_short_check();
}

int main()
{
  Tester::test();
}