Reactive Play
=============

A simple benchmark of some open source C++ libraries that support Reactive
Programming/ObserverPattern for common use cases.

Following libraries are peeked on:

- `ddinu/observable <https://github.com/ddinu/observable>`_
- `google/cpp-frp <https://github.com/google/cpp-frp>`_
- `boostorg/signals2 <https://github.com/boostorg/signals2>`_
- `schlangster/cpp.react <https://github.com/schlangster/cpp.react>`_
- `tower120/reactive <https://github.com/tower120/reactive>`_
- `ReactiveX/RxCpp <https://github.com/ReactiveX/RxCpp>`_

**Simple Source/Sink**

.. code:: bash

    Simple function duration: 8741 ns
    Observable duration: 72559 ns
    frp duration: 1300638 ns
    Boost duration: 725709 ns
    CppReact duration: 735398 ns
    reactive duration: 173615 ns
    rxcpp duration: 777221 ns

    Observable / Simple function : 8
    frp / Simple function : 148
    Boost / Simple function : 83
    CppReact / Simple function : 84
    reactive / Simple function : 19
    rxcpp / Simple function : 88

    frp / Observable : 17
    Boost / Observable : 10
    CppReact / Observable : 10
    reactive / Observable : 2
    rxcpp / Observable function : 10
