# Text Search Engine

A bonus project from my Data Structures course. I wanted to actually understand how search engines work under the hood, so I built a a basic inverted index from scratch with linear probing.

Each word in the document set gets hashed into a posting list (the set of documents containing that word). Boolean queries are parsed from infix to postfix and evaluated against those posting lists using set intersection and union.

## Usage

```bash
g++ -O2 -o main main.cpp
./main doc1.txt ins1.txt doc1_mat.txt output.txt
```

**Inputs**
- `doc1.txt`: the document set, one document per line
- `ins1.txt`: boolean queries, one per line (AND, OR, NOT supported)

**Outputs**
- `doc1_mat.txt`: the document matrix: each document mapped to the hash values of its words
- Query results (matching document numbers) print to stdout
