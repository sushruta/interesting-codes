#include <iostream>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

class Sequence {
  public:
    virtual std::string whatAmI() = 0;
  private:
};

template<typename T>
class FibonacciSequence :
  public Sequence,
  public std::iterator<std::bidirectional_iterator_tag, const T> {
  public:
    std::string whatAmI() {
      return "Fibonacci Sequence";
    }
    FibonacciSequence(T c=0, T n=1) {
      curr = c;
      next = n;
    }

    const T& operator* () const {
      return curr;
    }
    const T* operator-> () const {
      return &**this;
    }

    FibonacciSequence& operator++ () {
      T nextnext = curr + next;
      curr = next;
      next = nextnext;

      return *this;
    }
    
    FibonacciSequence& operator-- () {
      T prevcurr = next - curr;
      next = curr;
      curr = prevcurr;

      return *this;
    }

  private:
    T curr, next;
};

template<typename T>
class PowerSequence :
  public Sequence,
  public std::iterator<std::bidirectional_iterator_tag, const T> {
  public:
    std::string whatAmI() {
      return "Power Sequence";
    }

    PowerSequence(T c=1) {
      curr = c;
    }

    const T& operator* () const {
      return curr;
    }
    const T* operator-> () const {
      return &**this;
    }

    PowerSequence& operator++ () {
      curr *= 2;

      return *this;
    }
    
    PowerSequence& operator-- () {
      curr /= 2;

      return *this;
    }
  private:
    T curr;
};

template<class T>
class GeometricSequence :
  public Sequence,
  public std::iterator<std::bidirectional_iterator_tag, const T> {
  public:
    std::string whatAmI() {
      return "Geometric Progression Sequence";
    }
    
    GeometricSequence(T c=1, T r=1) {
      curr = c;
      ratio = r;
    }

    const T& operator* () const {
      return curr;
    }
    const T* operator-> () const {
      return &**this;
    }

    GeometricSequence& operator++ () {
      curr *= ratio;

      return *this;
    }
    
    GeometricSequence& operator-- () {
      curr /= ratio;

      return *this;
    }
  private:
    T curr;
    T ratio;
};

class Context {
  public:
    Context() {}
    void setSequence(Sequence *m_sequence) {
      sequence = m_sequence;
    }
    std::string description() {
      return sequence->whatAmI();
    }
  private:
    Sequence *sequence;
};

int main() {
  FibonacciSequence<long long> fc;
  PowerSequence<long long> pc;
  GeometricSequence<long long> gc(2, 3);

  Context context;

  std::vector<Sequence*> sequences;
  sequences.push_back(&fc);
  sequences.push_back(&pc);
  sequences.push_back(&gc);

  for (int i=0; i<sequences.size(); i++) {
    context.setSequence(sequences[i]);
    std::cout << context.description() << std::endl;

    for (int i=0; i<10; i++) {
      std::cout << *(context.getIterator()) << " ";
    }
    std::cout << std::endl;
  }

  return 0;
} 
