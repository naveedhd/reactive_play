/*
 * An elementary use case for a reactive framework is source/sink
 *
 * We would like to send a value and listen at the other end
 */

#include <observable/observable.hpp>
#include <frp/frp.h>
#include <boost/signals2.hpp>
#include <react/react.h>
#include <reactive/reactive.h>

#include "data_types.h"
#include "utility.h"

REACTIVE_DOMAIN(D, react::sequential)

static size_t const LOOP_COUNT = 1000;

void consume(Foo f) { f(); }

int main() {

  /* Simple function call */
  const auto simple_duration = time_run(
    [] () {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        consume(Foo(i));
      }
    });
  
  print_duration("Simple function", simple_duration);

  /* Observable Example */
  observable::subject<void(Foo)> subject;
  subject.subscribe(consume); // [](auto&& foo) { consume(foo); });

  const auto observable_duration = time_run(
    [&subject]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        subject.notify(Foo(i));
      }
    });

  print_duration("Observable", observable_duration);

  /* frp example */
  const auto source(frp::stat::push::source<Foo>());
  const auto sink(frp::stat::push::sink(std::ref(source)));

  const auto frp_duration = time_run(
    [&source, &sink]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        source = Foo(i);
        consume(**sink);
      }
    });

  print_duration("frp", frp_duration);

  /* Boost example */
  boost::signals2::signal<void(Foo)> sig;
  sig.connect(consume); // [](auto&& foo) { consume(foo); });

  const auto boost_duration = time_run(
    [&sig]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        sig(Foo(i));
      }
    });

  print_duration("Boost", boost_duration);

  /** CppReact example **/
  react::VarSignal<D, Foo> signal = react::MakeVar<D>(Foo(0));

  react::Observe(signal, [] (Foo foo) {
      consume(foo);
  });

  const auto react_duration = time_run(
    [&signal]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        signal <<= Foo(i);
      }
    });

  print_duration("CppReact", react_duration);

  /** reactive example **/
  reactive::Event<Foo> event;

  event += consume;

  const auto reactive_duration = time_run(
    [&event]() {
      for(auto i = 0; i < LOOP_COUNT; ++i) {
        event(Foo(i));
      }
    });

  print_duration("reactive", reactive_duration);

  cout << endl;

  /* stats */
  print_duration_diff("Simple function", simple_duration, "Observable", observable_duration);
  print_duration_diff("Simple function", simple_duration, "frp", frp_duration);
  print_duration_diff("Simple function", simple_duration, "Boost", boost_duration);
  print_duration_diff("Simple function", simple_duration, "CppReact", react_duration);
  print_duration_diff("Simple function", simple_duration, "reactive", reactive_duration);

  cout << endl;

  print_duration_diff("Observable", observable_duration, "frp", frp_duration);
  print_duration_diff("Observable", observable_duration, "Boost", boost_duration);
  print_duration_diff("Observable", observable_duration, "CppReact", react_duration);
  print_duration_diff("Observable", observable_duration, "reactive", reactive_duration);


  /* exit */
  return 0;

}