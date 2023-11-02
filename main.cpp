#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>


using namespace std;

set<string> splitString(const string& s, char delimiter) {
    set<string> output;

    string word;
    for (char c : s) {

        if (c == delimiter) {
            output.insert(word);
            word = "";
            continue;
        }

        if (isalnum(c)) {
            word += tolower(c);
        }

        if (c == s.back()) {
            output.insert(word);
        }
    }

    return output;
}

vector<string> storeDocs(ifstream &input) {
    string doc;
    vector<string> docs;

    // indexes starting at 1 instead of 0
    // this is a placeholder
    docs.emplace_back("");

    while (!input.eof()) {
        getline(input, doc);
        docs.push_back(doc);
    }

    return docs;
}

struct Value {
    string word;
    int hashValue = -1;
    vector<int> postingList;
};

class HashTable {

    int N = 999883;
    Value table[999883];

public:

    HashTable() {
        for (int i = 0; i < N; i++) {
            table[i] = Value();
        }
    }

    int hashFunc(const string &s) {
        int hash = 0;
        int N = 999883;
        for (char i : s)
            hash = hash + i;
        return hash % N;
    }

    int createHash(const string &s) {
        int index = hashFunc(s);
        Value v = table[index];
        do {
            if (v.hashValue == -1 || v.word == s) {
                break;
            }
        } while (index++);
        v.hashValue = index;
        v.word = s;
        return index;
    }

    void put (string &s, int document) {

    }

    void print() {
        for (const auto& thing : table) {
            cout << thing.hashValue << endl;
        }
    }

};


int main(int argc, char *argv[]) {

    ifstream documents(argv[1]);

    if (!documents.good()) {
        cout << "error opening file" << endl;
        return 0;
    }

    auto docs = storeDocs(documents);
    auto* table = new HashTable();

    for (int i = 1; i < docs.size(); ++i) {
        auto words = splitString(docs[i], ' ');
        for (const auto &word : words) {
            cout << word << ":";
            cout << table->createHash(word) << endl;
        }
        cout << endl;
    }

    return 0;
}
