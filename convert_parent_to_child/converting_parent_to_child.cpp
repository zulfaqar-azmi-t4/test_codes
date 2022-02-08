#include <iostream>
#include <utility>

class Parent {
public:
  Parent(int Var) : var(Var) {}
  int getVar() const { return var; }

private:
  int var;
};

class Child : public Parent {
public:
  Child() = default;

  // converting constructors
  Child(const Parent &p) : Parent(p) { std::cout << "copy\n"; }
  Child(Parent &&p) : Parent(std::move(p)) { std::cout << "move\n"; }

  int extraVariable = 999;
  int extraMethod() { return 666; }
};

// demo version of framwork.get()
Parent framework_get() {
  static int x = 0;
  return {++x};
}

int main() {
  Child c(framework_get());
  std::cout << c.getVar() << '\n';

  Parent p = framework_get();
  c = p;
  std::cout << c.getVar() << '\n';
}
