#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

// Elimina espacios iniciales y finales de una cadena
void trim(char *str) {
    // Eliminar espacios iniciales
    char *start = str;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);

    // Eliminar espacios finales
    char *end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) *end-- = '\0';
}

// Salta los números y separadores iniciales
char* skip_numbers(const char* line) {
    while (*line && isdigit(*line)) line++;
    while (*line && (*line == ' ' || *line == ':')) line++;
    return (char*)line;
}

// Elimina "https://"
char* remove_https(char* line) {
    char* url_start = strstr(line, "https://");
    if (url_start) {
        memmove(url_start, url_start + 8, strlen(url_start + 8) + 1);
    }
    return line;
}

// Extrae el dominio y el resto de la línea (sin espacios extra)
void get_domain_and_rest(const char* line, char* domain, char* rest) {
    int i = 0;
    // Copiar dominio
    while (line[i] && line[i] != ' ' && line[i] != ':') {
        domain[i] = line[i];
        i++;
    }
    domain[i] = '\0';

    // Saltar espacios y ':'
    while (line[i] && (line[i] == ' ' || line[i] == ':')) i++;

    // Copiar el resto
    strcpy(rest, line + i);
}

// Obtiene el directorio según el dominio
const char* get_directory(const char* domain) {
    if (strcmp(domain, "portal.colproba.org.ar/") == 0)
        return "";
    else if (strcmp(domain, "www.workmanagement.com.ar/") == 0)
        return "/inicio.html";
    else if (strcmp(domain, "www.nosis.com/es") == 0)
        return "/home/redirigiralogin";
    // Añade más dominios aquí si quieres
    else
        return "";
}

// Elimina espacios alrededor de los dos puntos en la parte "usuario : password"
void limpiar_espacios_colon(char* str) {
    char limpio[MAX_LINE];
    int j = 0;
    int i = 0;

    // Elimina espacios antes y después de los dos puntos
    while (str[i]) {
        // Copia carácter normal
        if (str[i] != ':') {
            limpio[j++] = str[i++];
        } else {
            // Eliminar espacios antes del ':'
            while (j > 0 && limpio[j-1] == ' ') j--;
            limpio[j++] = ':'; // Copia el ':'
            i++;
            // Eliminar espacios después del ':'
            while (str[i] == ' ') i++;
        }
    }
    limpio[j] = '\0';
    strcpy(str, limpio);
}

int main() {
    FILE *fin = fopen("entrada.txt", "r");
    FILE *fout = fopen("salida.txt", "w");
    char buffer[MAX_LINE];

    if (!fin || !fout) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    while (fgets(buffer, MAX_LINE, fin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        char* line = skip_numbers(buffer);
        remove_https(line);

        char domain[256], rest[MAX_LINE];
        get_domain_and_rest(line, domain, rest);

        trim(domain);
        trim(rest);
        limpiar_espacios_colon(rest);

        const char* directory = get_directory(domain);

        fprintf(fout, "%s%s:%s\n", domain, directory, rest);
    }

    fclose(fin);
    fclose(fout);

    printf("Proceso completado.\n");
    return 0;
}
