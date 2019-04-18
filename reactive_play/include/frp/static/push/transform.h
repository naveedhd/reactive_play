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
#ifndef _FRP_STATIC_PUSH_TRANSFORM_H_
#define _FRP_STATIC_PUSH_TRANSFORM_H_

#include <frp/internal/namespace_alias.h>
#include <frp/static/push/repository.h>

namespace frp {
namespace stat {
namespace push {

template<typename Comparator, typename Function, typename... Dependencies>
auto transform(Function &&function, Dependencies... dependencies) {
	static_assert(util::all_true_type<typename util::is_not_void<
		typename util::unwrap_container_t<Dependencies>::value_type>::type...>::value,
		"Dependencies can not be void type.");

	typedef util::transform_return_type<Function, Dependencies...> value_type;
	typedef util::commit_storage_type<value_type, sizeof...(Dependencies)> commit_storage_type;
	typedef typename commit_storage_type::revisions_type revisions_type;

	return details::make_repository<value_type, commit_storage_type, Comparator>(
		[function = internal::get_function(util::unwrap_reference(std::forward<Function>(function))),
		 executor = internal::get_executor(util::unwrap_reference(std::forward<Function>(function)))](
			auto &&callback, const auto &previous, const auto &storage) {
		executor([=, callback = std::move(callback)]() {
			auto current(util::invoke([&](const auto&... storage) {
				return std::make_tuple(internal::get_storage(util::unwrap_container(storage))...);
			}, *storage));
			auto revisions(util::invoke([&](const auto&... storage) {
				return revisions_type{ storage->revision... };
			}, current));
			auto last(std::atomic_load(&*previous));
			if (!last || last->is_newer(revisions)) {
				callback(util::invoke([&](const auto&... storage) {
					revisions_type revisions{ storage->revision... };
					return commit_storage_type::make(std::bind(std::ref(function),
						std::cref(storage->value)...), revisions);
				}, current));
			}
		});
	}, std::forward<Dependencies>(dependencies)...);
}

template<typename Function, typename... Dependencies>
auto transform(Function &&function, Dependencies... dependencies) {
	typedef util::transform_return_type<Function, Dependencies...> value_type;
	return transform<std::equal_to<value_type>, Function, Dependencies...>(
		std::forward<Function>(function), std::forward<Dependencies>(dependencies)...);
}

} // namespace push
} // namespace stat
} // namespace frp

#endif // _FRP_STATIC_PUSH_TRANSFORM_H_
