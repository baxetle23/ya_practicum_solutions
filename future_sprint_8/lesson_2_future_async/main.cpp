#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats& other) {
        for (const auto& [word, frequency] : other.word_frequences) {
            word_frequences[word] += frequency;
        }
    }
};

using KeyWords = set<string, less<>>;

vector<string_view> Split(string_view str) {
    vector<string_view> result;
    while (true) {
        const auto space = str.find(' ');
        if (space != 0 && !str.empty()) {
            result.push_back(str.substr(0, space));
        }
        if (space == str.npos) {
            break;
        } else {
            str.remove_prefix(space + 1);
        }
    }
    return result;
}

Stats ExploreLine(const KeyWords& key_words, string_view line) {
    Stats result;
    for (const string_view word : Split(line)) {
        if (key_words.count(word) > 0) {
            ++result.word_frequences[string(word)];
        }
    }
    return result;
}

Stats ExploreBatch(const KeyWords& key_words, vector<string> lines) {
    Stats result;
    for (const string& line : lines) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const KeyWords& key_words, istream& input) {
    const size_t MAX_BATCH_SIZE = 5000;

    vector<string> batch;
    batch.reserve(MAX_BATCH_SIZE);

    vector<future<Stats>> futures;

    for (string line; getline(input, line); ) {
        batch.push_back(move(line));
        if (batch.size() >= MAX_BATCH_SIZE) {
            futures.push_back(
                async(ExploreBatch, ref(key_words), move(batch))
            );
            batch.reserve(MAX_BATCH_SIZE);
        }
    }

    Stats result;

    if (!batch.empty()) {
        result += ExploreBatch(key_words, move(batch));
    }

    for (auto& f : futures) {
        result += f.get();
    }

    return result;
}


int main() {
    const KeyWords key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    for (const auto& [word, frequency] : ExploreKeyWords(key_words, ss).word_frequences) {
        cout << word << " " << frequency << endl;
    }

    return 0;
}
