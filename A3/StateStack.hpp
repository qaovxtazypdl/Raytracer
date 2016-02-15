#include <vector>
#include <iostream>

template <class T>
class StateStack {
public:
  StateStack() : m_undo_ptr(0), m_stack() {}

  void initialize(T initialState) {
    m_stack.push_back(initialState);
  }

  void push_state(T state) {
    if (m_undo_ptr < m_stack.size() - 1) {
      m_stack.erase(m_stack.begin() + m_undo_ptr + 1, m_stack.end());
    }

    m_stack.push_back(state);
    m_undo_ptr++;
  }

  bool undo_state() {
    if (m_undo_ptr == 0) return false;

    m_undo_ptr--;

    return true;
  }

  bool redo_state() {
    if (m_undo_ptr == m_stack.size() - 1) return false;

    m_undo_ptr++;

    return true;
  }

  void reset_state() {
    m_undo_ptr = 0;
    m_stack.erase(m_stack.begin() + 1, m_stack.end());
  }

  T get_current_state() {
    return m_stack[m_undo_ptr];
  }

private:
  std::vector<T> m_stack;
  unsigned int m_undo_ptr;
};
