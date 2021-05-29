#include <stdlib.h>

void atto_set_from_env(char *env_var_name, char **dest) {
    char *value = getenv(env_var_name);
    if (value != NULL) {
        *dest = value;
    }
}