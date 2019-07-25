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
#include <string>

#include "calc.cpp"

int main()
{
  Calculator calc;
  std::string expression;
  while(1) {
    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << "> ";
    std::getline(std::cin, expression);
    try {
      std::cout << ": " << calc.calculate(expression) << '\n';
    } catch (Calculator::polish_calc_exception& e) {
      std::cerr << "Ошибка в вычислениях. Проверьте выражение. (Функция"
                   " polish_calc() вернула исключение polish_calc_exception)\n";
    } catch (Calculator::precheck_unidentified_string_exception& e) {
      std::cerr << "Ошибка в разборе функции. Обнаружена неопознанная"
                   " константа, переменная или функция. (Функция precheck()"
                   " вернула исключение"
                   " precheck_unidentified_string_exception)\n";
    } catch (Calculator::precheck_unary_plus_exception& e) {
      std::cerr << "Ошибка в разборе функции. Проверьте выражение. Возможно, вы"
                   " ввели лишний знак сложения. (Функция precheck() вернула" 
                   " исключение precheck_unary_plus_exception)\n";
    } catch (Calculator::precheck_unary_minus_exception& e) {
      std::cerr << "Ошибка в разборе функции. Проверьте выражение. Возможно, вы"
                   " ввели лишний знак вычитания. (Функция precheck() вернула" 
                   " исключение precheck_unary_plus_exception)\n";
    } catch (Calculator::reverse_polish_notation_brackets_exception& e) {
      std::cerr << "Ошибка в подсчёте баланса скобок. Проверьте выражение."
                   " (Функция reverse_polish_notation() вернула исключение"
                   " reverse_polish_notation_brackets_exception)\n";
    }
  }
}
