#ifndef __CALC_H__
#define __CALC_H__

/* Автор: Визгалов Максим (mxdevv)
 * Дата начала: 23.07.19
 * Программа: калькулятор
 *
 * Описание: Программа-калькулятор, как вступительное задание для устройства в
 *   cqg.
 *
 * Ширина кода: 80 символов. Используется 2 пробела, вместо табуляции.
 */

//#define CALCULATOR_DEBUG

#include <string>

class Token;
class Calculator;
class Tester;

class Token {
public:
  enum class e_type {
    NONE, INTEGER, FLOAT, STRING, SYMBOL, OPERATOR_MINUS, OPERATOR_PLUS,
    OPERATOR_MULT, OPERATOR_DIV, CONSTANT, VARIABLE, FUNCTION, UNARY_PLUS,
    UNARY_MINUS, OPEN_BRACKET, CLOSE_BRACKET
  };
  e_type type = e_type::NONE;
  double value;

  Token() = default;

  Token(e_type type, double value)
    : type(type), value(value)
  { }
};

class Calculator {
  std::string expression;
  std::vector<Token> tokens;

  enum class e_state {
    NONE, READED, PARSED, POLISH_NOTATION_COMPLETE
  };
  e_state state = e_state::NONE;

  /** парсит строку expression на токены **/
  void parse();

  /** помогает распознать некорректное выражение **/
  void precheck() const;

  /** добавляет токен **/
  void add_token(const Token::e_type type, int& parse_start_pos,
      const int parse_end_pos);

  /** выводит токены в короткой форме **/
  void tokens_short_check(const std::vector<Token>& tokens) const;

  /** приводит токены в обратную польскую нотацию **/
  void reverse_polish_notation();

  /** возвращает приоритет оператора **/
  int operator_precedence(const Token::e_type type) const;

  /** считает обратную польскую запись **/
  double polish_calc();

public:
  /** Принимает выражение в виде c++ строки **/
  void read(const std::string&& str);
  void read(const std::string& str);

  /** Высчитывает выражение из строки в буфере **/
  double calculate();

  /** Тоже самое + read(...) **/
  double calculate(const std::string&& str);
  double calculate(const std::string& str);

  Calculator() = default;
  
  /** Конструктор, как конструктор по-умолчанию + метод read() **/
  Calculator(const std::string&& str);

  /** Перегрузка оператора на ввод, аналог read() **/
  void operator << (const std::string&& str);

  /** Перегрузка оператора на вывод, аналог calculate() **/
  void operator >> (double& value);
  
  class precheck_unidentified_string_exception { };
  class precheck_unary_plus_exception { };
  class precheck_unary_minus_exception { };
  class precheck_empty_exception { };
  class reverse_polish_notation_brackets_exception { };
  class polish_calc_exception { };
};

#endif // __CALC_H__
