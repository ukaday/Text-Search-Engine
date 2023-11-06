#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>


using namespace std;

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

    int hashFunc(const string &s) const {
        int hash = 0;
        for (char i : s)
            hash = hash + i;
        return hash % N;
    }

    int createHash(const string &s) {
        int index = hashFunc(s);
        int startIndex = index;
        Value *v = &table[index];

        while (v->hashValue != -1 && v->word != s) {
            index = (index + 1) % N;
            if (index == startIndex) {
                cerr << "Hash table is full" << endl;
                return -1;
            }
            v = &table[index];
        }

        v->hashValue = index;
        v->word = s;

        return index;
    }

    void put (string &s, int document) {

    }

    void print() {
        for (const auto& value : table) {
            cout << value.hashValue << endl;
        }
    }

};

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

HashTable* hashDocs(vector<string> &docs) {
    auto* table = new HashTable();

    set<string> vocab;
    for (int i = 1; i < docs.size(); i++) {
        auto words = splitString(docs[i], ' ');
        for (const auto &word : words) {
            vocab.insert(word);
        }
    }

    for (const auto &word : vocab) {
        table->createHash(word);
    }

    return table;
}

vector<set<int>> createDocMatrix(vector<string> &docs) {
    auto table = hashDocs(docs);
    vector<set<int>> output;

    for (int i = 1; i < docs.size(); i++) {
        auto words = splitString(docs[i], ' ');
        set<int> postingList;
        for (const auto &word : words) {
            postingList.insert(table->createHash(word));
        }
        output.push_back(postingList);
    }
    return output;
}

void printDocMatrix(vector<set<int>> docMatrix) {
    int docIndex = 1;
    for (const auto& posting : docMatrix) {
        cout << docIndex << "->" << "[";
        for (auto it = posting.begin(); it != posting.end(); ++it) {
            if (it == --posting.end()) {
                cout << *it;
                break;
            }
            cout << *it << ", ";
        }
        cout << "]" << endl;
        docIndex++;
    }
}

int main(int argc, char *argv[]) {

    ifstream documents(argv[1]);

    if (!documents.good()) {
        cout << "error opening file" << endl;
        return 0;
    }

    auto docs = storeDocs(documents);
    auto docMatrix = createDocMatrix(docs);
    printDocMatrix(docMatrix);

    return 0;
}
