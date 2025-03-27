#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* ParseDynamicString(const char* data, const char* key);
bool ParseBool(const char* data, const char* key, bool* value);
bool ParseInt(const char* data, const char* key, int* value);
bool ParseFloat(const char* data, const char* key, float* value);

#endif // !PARSER_H
