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
#ifndef _FRP_STATIC_PUSH_SINK_H_
#define _FRP_STATIC_PUSH_SINK_H_

#include <frp/internal/namespace_alias.h>
#include <frp/internal/operator.h>
#include <frp/util/observable.h>
#include <frp/util/reference.h>
#include <frp/util/storage.h>
#include <memory>
#include <stdexcept>

namespace frp {
namespace stat {
namespace push {

template<typename T>
struct sink_type {

	template<typename Dependency>
	friend auto sink(Dependency &&dependency)
		->sink_type<typename util::unwrap_reference_t<Dependency>::value_type>;

	typedef T value_type;

	sink_type() = default;

	struct reference {
		template<typename U>
		friend struct sink_type;

		typedef T value_type;

		operator bool() const {
			return !!value;
		}

		const auto &operator*() const {
			if (!value) {
				throw std::domain_error("value not available");
			}
			else {
				return value->value;
			}
		}

		const auto operator->() const {
			return &operator*();
		}

		operator const T &() const {
			return operator*();
		}

	private:
		explicit reference(std::shared_ptr<util::storage_type<T>> &&value)
			: value(std::forward<std::shared_ptr<util::storage_type<T>>>(value)) {}
		std::shared_ptr<util::storage_type<T>> value;
	};

	reference operator*() const {
		return reference(provider());
	}

private:
	template<typename Dependency>
	struct template_storage_type {

		explicit template_storage_type(Dependency &&dependency)
			: dependency(std::forward<Dependency>(dependency)) {}

		std::shared_ptr<util::storage_type<T>> get() const {
			return std::atomic_load(&value);
		}

		void evaluate() {
			std::atomic_store(&value,
				internal::get_storage(util::unwrap_container(dependency)));
		}

		std::shared_ptr<util::storage_type<T>> value; // Use atomics!
		Dependency dependency;
	};

	template<typename Dependency>
	static auto make(Dependency &&dependency) {
		return sink_type(std::make_shared<template_storage_type<Dependency>>(
			std::forward<Dependency>(dependency)));
	}

	template<typename Storage>
	explicit sink_type(const std::shared_ptr<Storage> &storage)
		: provider([=]() { return storage->get(); })
		, callback(util::add_callback(util::unwrap_reference(storage->dependency),
			[weak_storage = std::weak_ptr<Storage>(storage)]() {
				auto s(weak_storage.lock());
				if (s) {
					s->evaluate();
				}
			})) {
		storage->evaluate();
	}

	std::function<std::shared_ptr<util::storage_type<T>>()> provider;
	util::observable_type::reference_type callback;
};

template<typename Dependency>
auto sink(Dependency &&dependency)
		->sink_type<typename util::unwrap_reference_t<Dependency>::value_type> {
	typedef typename util::unwrap_reference_t<Dependency>::value_type value_type;
	static_assert(!std::is_void<value_type>::value, "T must not be void type.");
	static_assert(std::is_move_constructible<value_type>::value,
		"T must be move constructible.");
	return sink_type<value_type>::make(std::forward<Dependency>(dependency));
}

} // namespace push
} // namespace stat
} // namespace frp

#endif // _FRP_STATIC_PUSH_SINK_H_
