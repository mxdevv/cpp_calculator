/* Автор: Визгалов Максим (mxdevv)
 * Дата начала: 2.07.19
 * Программа: калькулятор
 *
 * Описание: Программа-калькулятор, как вступительное задание для устройства в
 *   cqg.
 */

#include <iostream>
#include <vector>

using std::string;

//namespace mxdevv_cqg_task {

class Token;
class Calculator;
class Tester;

class Token {
  enum class e_type {
    INTEGER, FLOAT
  };
  e_type type;
  string value;
};

class Calculator {
  string expression;
  std::vector<Token> tokens;

  /* парсит строку expression на токены */
  void parse();

public:
  /** Принимает выражение в виде c++ строки **/
  void read(string&& str);

  /** Высчитывает выражение из строки в буфере **/
  void calculate();

  /** Конструктор, как конструктор по-умолчанию + метод read() **/
  //Calculator(const string& str);

  /** Перегрузка оператора на ввод, аналог read() **/
  //std::iostream& operator >> (string& str);

  /** Перегрузка оператора на вывод, аналог calculate() **/
  //std::iostream& operator << (string& str);
  
  friend Tester;
};

class Tester {
public:
  static void test() {
    Calculator calc;
    calc.read("2+2");
  }
};

void Calculator::read(string&& str) {
  std::cerr << "Calculator::read(...) run\n";

  expression = str;
}

void Calculator::parse() {
  enum class e_state {
    NONE, NUMBER, STRING, SYMBOL
  };
  e_state cur_state = e_state::NONE, prev_state = e_state::NONE;
}

//} // mxdevv_cqg_task

//namespace cqg = mxdevv_cqg_task;

int main()
{
  Tester::test();
}
