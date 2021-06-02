#include <algorithm>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include "profile.h"

using namespace std;

string GenerateString(mt19937& generator, int length) {
    string s(length, 'a');
    for (int i = 0; i < 10; ++i) {
        const int index = uniform_int_distribution(length * 9 / 10, length - 1)(generator);
        s[index] = uniform_int_distribution('a', 'z')(generator);
    }
    return s;
}

vector<string> GenerateStrings(mt19937& generator, int count, int length, bool sorted = false) {
    vector<string> strings;
    strings.reserve(count);
    for (int i = 0; i < count; ++i) {
        strings.push_back(GenerateString(generator, length));
    }
    if (sorted) {
        sort(execution::par, strings.begin(), strings.end());
    }
    return strings;
}

template <typename Function>
void Test(string_view mark, const vector<string>& strings, const vector<string>& requests, Function function) {
    LOG_DURATION(mark);
    for (const auto& request : requests) {
        function(strings.begin(), strings.end(), request);
    }
}

#define TEST(function) Test(#function, strings, requests, function<vector<string>::const_iterator, string>)

int main() {
    mt19937 generator;
    const auto strings = GenerateStrings(generator, 10, 100'000'000, /* sorted */ true);
    const auto requests = GenerateStrings(generator, 100, 100'000'000);

    TEST(lower_bound);
    TEST(LowerBound);
}