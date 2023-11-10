#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>


using namespace std;

struct Value {
    string word;
    int hashValue = -1;
    set<int> postingList;
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

            //checks if index loops around
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

    void put(const string &s, int document) {
        int index = createHash(s);
        table[index].postingList.insert(document);
    }

    set<int> get(string &s) {
        int index = createHash(s);
        return table[index].postingList;
    }

    void print() {
        for (const auto& value : table) {
            cout << "{";
            for (const auto& word : value.postingList) {
                cout << word << " ";
            }
            cout << "} ";
        }
    }

};

set<string> splitStringToSet(const string& s, char delimiter) {
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

vector<string> splitStringToVector(const string& s, char delimiter) {
    vector<string> output;

    string word;
    for (char c : s) {

        if (c == delimiter) {
            output.push_back(word);
            word = "";
            continue;
        }

        if (isalnum(c)) {
            word += tolower(c);
        }

        if (c == s.back()) {
            output.push_back(word);
        }
    }

    return output;
}

vector<set<string>> storeDocs(ifstream &input) {
    string doc;
    vector<set<string>> docs;

    // indexes starting at 1 instead of 0
    // this is a placeholder
    docs.emplace_back();

    while (!input.eof()) {
        getline(input, doc);
        auto splitDoc = splitStringToSet(doc, ' ');
        docs.push_back(splitDoc);
    }

    return docs;
}

HashTable* hashDocs(vector<set<string>> &docs) {
    auto* table = new HashTable();

    //sorts all unique words in all documents before hashing
    /*set<string> vocab;
    for (int i = 1; i < docs.size(); i++) {
        for (const auto &word : docs[i]) {
            vocab.insert(word);
        }
    }

    for (const auto &word : vocab) {
        table->createHash(word);
    } */

    //hashes docs in order
    for (int i = 1; i < docs.size(); i++) {
        for (const auto &word : docs[i]) {
            table->createHash(word);
        }
    }

    return table;
}

vector<set<int>> createDocMatrix(HashTable* table, vector<set<string>> &docs) {
    vector<set<int>> output;

    for (int i = 1; i < docs.size(); i++) {
        set<int> hashedDocument;
        for (const auto &word : docs[i]) {
            hashedDocument.insert(table->createHash(word));
            table->put(word, i);
        }
        output.push_back(hashedDocument);
    }
    return output;
}

void writeDocMatrix(ofstream &file, vector<set<int>> docMatrix) {
    int docIndex = 1;
    for (const auto& posting : docMatrix) {
        file << docIndex << "->" << "[";
        for (auto it = posting.begin(); it != posting.end(); ++it) {
            if (it == --posting.end()) {
                file << *it;
                break;
            }
            file << *it << ", ";
        }
        file << "]" << endl;
        docIndex++;
    }
}

void executeInstructions(ifstream &instructions) {

    string instruction;
    while (!instructions.eof()) {
        getline(instructions, instruction);
        auto splitDoc = splitStringToVector(instruction, ' ');
        cout << instruction << endl;
    }
}

int main(int argc, char *argv[]) {

    ifstream documentsFile(argv[1]);
    ifstream instructionsFile(argv[2]);
    ofstream documentMatrixFile(argv[3]);
    ofstream instructionOutFile(argv[4]);

    if (!documentsFile.good() || !documentMatrixFile.good()
        || !instructionsFile.good() || !instructionOutFile.good()) {
        cout << "error opening file" << endl;
        return 0;
    }

    auto docs = storeDocs(documentsFile);
    documentsFile.close();

    auto table = hashDocs(docs);
    auto docMatrix = createDocMatrix(table, docs);
    writeDocMatrix(documentMatrixFile, docMatrix);
    documentMatrixFile.close();

    executeInstructions(instructionsFile);

    return 0;
}
