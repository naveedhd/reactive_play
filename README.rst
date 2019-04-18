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

**Simple Source/Sink**

.. code:: bash

    Simple function duration: 8762 ns
    Observable duration: 74957 ns
    frp duration: 1315947 ns
    Boost duration: 743377 ns
    CppReact duration: 733117 ns
    reactive duration: 173099 ns

    Observable / Simple function : 8
    frp / Simple function : 150
    Boost / Simple function : 84
    CppReact / Simple function : 83
    reactive / Simple function : 19

    frp / Observable : 17
    Boost / Observable : 9
    CppReact / Observable : 9
    reactive / Observable : 2
