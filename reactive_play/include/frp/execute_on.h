/*
 * Copyright 2016 Google Inc. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _FRP_EXECUTE_ON_H_
#define _FRP_EXECUTE_ON_H_

#include <utility>

namespace frp {
namespace internal {

template<typename F, typename E>
struct execute_on_type {
	typedef E executor_type;
	typedef F function_type;

	E executor;
	F function;
};

struct execute_immediate_type {
	template<typename F>
	void operator()(F f) const {
		f();
	}
};

template<typename F>
struct from_function_type {
	typedef execute_immediate_type executor_type;
	typedef F function_type;

	static auto executor(F &&f) {
		return execute_immediate_type();
	}

	static decltype(auto) function(F &&f) {
		return std::forward<F>(f);
	}
};

template<typename F, typename E>
struct from_function_type<execute_on_type<F, E>> {
	typedef E executor_type;
	typedef F function_type;

	static decltype(auto) executor(execute_on_type<F, E> &&f) {
		return std::move(f.executor);
	}

	static decltype(auto) function(execute_on_type<F, E> &&f) {
		return std::move(f.function);
	}
};

template<typename F>
using get_executor_t = typename from_function_type<F>::executor_type;

template<typename F>
using get_function_t = typename from_function_type<F>::function_type;

template<typename F>
decltype(auto) get_executor(F &&f) {
	return from_function_type<F>::executor(std::forward<F>(f));
}

template<typename F>
decltype(auto) get_function(F &&f) {
	return from_function_type<F>::function(std::forward<F>(f));
}

} // namespace internal

template<typename E, typename F>
internal::execute_on_type<F, E> execute_on(E executor, F function) {
	return { std::forward<E>(executor), std::forward<F>(function) };
}

} // namespace frp

#endif // _FRP_EXECUTE_ON_H_
