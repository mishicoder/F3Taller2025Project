#include "parser.h"

char* ParseDynamicString(const char *data, const char *key)
{
  char* start = strstr(data, key);
  if(!start) return NULL;
  
  start += strlen(key) + 1;
  char* end = strchr(start, ',');
  if(!end) end = start + strlen(start);

  size_t len = end - start;

  char* result = (char*)malloc(len + 1);
  if(!result) return NULL;

  if(strncpy_s(result, len + 1, start, len) != 0)
  {
    free(result);
    return NULL;
  }
  result[len] = '\0';

  return result;
}

bool ParseBool(const char *data, const char *key, bool *value)
{
  char* buffer = ParseDynamicString(data, key);
  if(!buffer) return false;

  if(strcmp(buffer, "true") == 0)
  {
    *value = true;
    free(buffer);
    return true;
  }
  if(strcmp(buffer, "false") == 0)
  {
    *value = false;
    free(buffer);
    return true;
  }

  free(buffer);
  return false;
}

bool ParseInt(const char *data, const char *key, int *value)
{
  char* buffer = ParseDynamicString(data, key);
  if(!buffer) return false;
  
  int reads = sscanf_s(buffer, "%d", value);
  free(buffer);

  return reads == 1;
}

bool ParseFloat(const char *data, const char *key, float *value)
{
  char* buffer = ParseDynamicString(data, key);
  if(!buffer) return false;

  int reads = sscanf_s(buffer, "%f", value);
  free(buffer);

  return reads == 1;
}
