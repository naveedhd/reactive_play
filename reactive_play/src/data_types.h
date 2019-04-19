#pragma once


/* An example data structure */
struct Foo {

  // Foo() = delete;
  Foo() : data_(0) {}
  Foo(int d) : data_(d) {}

  void operator()() { data_++; };

  int getData() const { return data_; }

  bool operator==(const Foo& rhs) const {
    return false;
  }

  bool operator!=(const Foo& rhs) const {
    return !operator==(rhs);
  }

 private:
  int data_;

};