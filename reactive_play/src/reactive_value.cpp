/*
 * Another use case is of `ReactiveValues` where a value is constructed
 * and modified by one or more signal/values going through some function
 * 
 * and we also observe the value
 */

#include <observable/observable.hpp>
#include <frp/frp.h>
#include <react/react.h>
#include <reactive/reactive.h>

#include "data_types.h"
#include "utility.h"

Foo merge(Foo a, Foo b) { return Foo(a.getData() + b.getData()); }

/** observable stuff */
OBSERVABLE_ADAPT_FILTER(merger, merge)

/** cpp react stuff */
REACTIVE_DOMAIN(D, react::sequential)

int main() {

  /* simple function */
  Foo foo;
  const auto simple_duration = time_run(
    [&foo] () {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        foo = merge(Foo(i % 3), Foo(i % 5));
        consume(foo);
      }
    });
  
  print_duration("Simple function", simple_duration);

  /* Observable Example */
  observable::value<Foo> a;
  observable::value<Foo> b;

  const auto c = observe( merger(a, b) );
  c.subscribe(consume);

  const auto observable_duration = time_run(
    [&a, &b]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        a = Foo(i % 3);
        b = Foo(i % 5);
      }
    });

  print_duration("Observable", observable_duration);

  /** CppReact example **/
  react::VarSignal<D, Foo> a_signal = react::MakeVar<D>(Foo(0));
  react::VarSignal<D, Foo> b_signal = react::MakeVar<D>(Foo(0));

  react::Signal<D, Foo> c_signal = react::MakeSignal<D>(With(a_signal, b_signal), merge);
  react::Observe(c_signal, consume);

  const auto react_duration = time_run(
    [&a_signal, &b_signal]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        a_signal <<= Foo(i % 3);
        b_signal <<= Foo(i % 5);
      }
    });

  print_duration("CppReact", react_duration);

  /** reactive example **/
  reactive::ObservableProperty<Foo> a_prop = Foo();
  reactive::ObservableProperty<Foo> b_prop = Foo();

  reactive::ReactiveProperty<Foo> c_prop;
  c_prop.set(merge, a_prop, b_prop);

  c_prop += consume;

  const auto reactive_duration = time_run(
    [&a_prop, &b_prop]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        a_prop = Foo(i % 3);
        b_prop = Foo(i % 5);
      }
    });

  print_duration("reactive", reactive_duration);

  /* stats */
  cout << endl;

  print_duration_diff("Simple function", simple_duration, "Observable", observable_duration);
  print_duration_diff("Simple function", simple_duration, "CppReact", react_duration);
  print_duration_diff("Simple function", simple_duration, "reactive", reactive_duration);

  cout << endl;

  print_duration_diff("Observable", observable_duration, "CppReact", react_duration);
  print_duration_diff("Observable", observable_duration, "reactive", reactive_duration);

  /* exit */
  return 0;

}