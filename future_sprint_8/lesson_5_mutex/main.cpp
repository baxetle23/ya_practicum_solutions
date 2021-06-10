#include <iostream>
#include <numeric>
#include <vector>
#include <mutex>
#include <algorithm>
#include <execution>

using namespace std;



// template <typename Container, typename Predicate>
// vector<typename Container::value_type> CopyIfUnordered(const Container& container, Predicate predicate) {
//     vector<typename Container::value_type> result;
//     result.reserve(container.size());
//     mutex result_mutex;
//     for_each(
//             execution::par,
//             container.begin(), container.end(),
//             [predicate, &result_mutex, &result](const auto& value) {
//                 if (predicate(value)) {
//                     typename Container::value_type* destination;
//                     {
//                         lock_guard guard(result_mutex);
//                         destination = &result.emplace_back();
//                     }
//                     *destination = value;
//                 }
//             }
//     );
//     return result;
// }

template <typename Container, typename Predicate>
vector<typename Container::value_type> CopyIfUnordered(const Container& container, Predicate predicate) {
    vector<typename Container::value_type> result;
    result.reserve(container.size());
    mutex mutex_;
    for_each(execution::par, container.begin(), container.end(),
        [&result, &predicate, &mutex_](auto& elem) {
            if (predicate(elem)) {
                auto tmp = elem;
                lock_guard<mutex> guard(mutex_);
                result.push_back(move(tmp));
            };
        });
    return result;
}

int main() {
    vector<int> numbers(1'000);
    iota(numbers.begin(), numbers.end(), 0);
    
    const vector<int> even_numbers =
        CopyIfUnordered(numbers, [](int number) { return number % 2 == 0; });
    for (const int number : even_numbers) {
        cout << number << " "s;
    }
    cout << endl;
		// выведет все чётные числа от 0 до 999
}