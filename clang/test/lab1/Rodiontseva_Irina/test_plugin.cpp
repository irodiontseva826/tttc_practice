/*
Вариант 1.
Напечатать информацию о пользовательском типе данных
1) Поля
2) Методы
3) Базовые классы
*/

struct Human {
  unsigned age;
  unsigned height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};

struct Engineer : Human {
  unsigned salary;
  void sleep() override { /* something */ }
  void eat() override { /* something */ }
  void work() { /* something */ }
};

/*
Примерный вывод:

Human
|_Fields
| |_ age (unsigned int|public)
| |_ height (unsigned int|public)
|
|_Methods
| |_ sleep (void()|public|virtual|pure)
| |_ eat (void()|public|virtual|pure)

Engineer -> Human
|_Fields
| |_ salary (unsigned int|public)
|
|_Methods
| |_ sleep (void()|public|override)
| |_ eat (void()|public|override)
| |_ work (void()|public)

*/
