#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include <string>
using namespace std;

static const size_t MAX_HASH = 1e6;
int HashFunc(const string& key, int size);

#endif