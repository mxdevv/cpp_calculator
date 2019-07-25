
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

#include <string>

class Token;
class Calculator;
class Tester;

class Token {
public:
  enum class e_type {
    NONE, INTEGER, FLOAT, STRING, SYMBOL, OPERATOR_MINUS, OPERATOR_PLUS,
    OPERATOR_MULT, OPERATOR_DIV, VARIABLE, FUNCTION, UNARY_PLUS, UNARY_MINUS,
    OPEN_BRACKET, CLOSE_BRACKET
  };
  e_type type;
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
  bool debug = false;

  /** парсит строку expression на токены **/
  void parse();

  /** помогает распознать некорректное выражение **/
  void precheck();

  /** добавляет токен **/
  void add_token(Token::e_type type, int& parse_start_pos, int& parse_end_pos);

  /** выводит токены **/
  void tokens_check(std::vector<Token>& tokens);

  /** выводит токены в короткой форме **/
  void tokens_short_check(std::vector<Token>& tokens);

  /** приводит токены в обратную польскую нотацию **/
  void reverse_polish_notation();

  /** возвращает приоритет оператора **/
  int operator_precedence(Token::e_type type);

  /** считает обратную польскую запись **/
  double polish_calc();

public:
  /** Принимает выражение в виде c++ строки **/
  void read(std::string&& str);
  void read(std::string& str);

  /** Высчитывает выражение из строки в буфере **/
  double calculate();

  /** Тоже самое + read(...) **/
  double calculate(std::string&& str);

  Calculator() = default;
  
  /** Конструктор, как конструктор по-умолчанию + метод read() **/
  Calculator(std::string&& str);

  /** Перегрузка оператора на ввод, аналог read() **/
  void operator << (std::string&& str);

  /** Перегрузка оператора на вывод, аналог calculate() **/
  void operator >> (double& value);

  class polish_calc_exception { };
  class precheck_unary_plus_exception { };
  class precheck_unary_minus_exception { };
};

#endif
