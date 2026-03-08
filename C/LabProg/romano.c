#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to get the decimal value of a Roman numeral character
int value(char c) {
    switch (toupper(c)) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return -1;
    }
}

// Function to check if the character is a valid Roman numeral
int isValidRomanChar(char c) {
    char validChars[] = "IVXLCDM";
    c = toupper(c);
    for (int i = 0; validChars[i]; i++) {
        if (c == validChars[i]) {
            return 1;
        }
    }
    return 0;
}

// Function to validate and convert Roman numeral to decimal
int romanToDecimal(char *roman, char **error) {
    int result = 0;
    int length = strlen(roman);
    
    // Check for empty string
    if (length == 0) {
        *error = "O numeral romano não pode estar vazio.";
        return -1;
    }
    
    // Check for invalid characters
    for (int i = 0; i < length; i++) {
        if (!isValidRomanChar(roman[i])) {
            *error = "Caracter inválido: só são permitidos I, V, X, L, C, D, M.";
            return -1;
        }
    }
    
    // Convert Roman to decimal
    int prev = 0;
    int repeats = 1;
    
    for (int i = length - 1; i >= 0; i--) {
        int curr = value(roman[i]);
        
        // Check for repetition rules
        if (i < length - 1 && toupper(roman[i]) == toupper(roman[i + 1])) {
            repeats++;
            if (repeats > 3 && (curr == 1 || curr == 10 || curr == 100 || curr == 1000)) {
                *error = "Um símbolo não pode ser repetido mais de 3 vezes consecutivas.";
                return -1;
            } else if (repeats > 1 && (curr == 5 || curr == 50 || curr == 500)) {
                *error = "Os símbolos V, L e D não podem ser repetidos.";
                return -1;
            }
        } else {
            repeats = 1;
        }
        
        // Calculate value based on Roman numeral rules
        if (curr >= prev) {
            result += curr;
        } else {
            // Check subtraction rule - only certain combinations are allowed
            char current = toupper(roman[i]);
            char next = toupper(roman[i + 1]);
            
            if ((current == 'I' && (next != 'V' && next != 'X')) ||
                (current == 'X' && (next != 'L' && next != 'C')) ||
                (current == 'C' && (next != 'D' && next != 'M')) ||
                (current == 'V' || current == 'L' || current == 'D')) {
                *error = "Subtração inválida. Apenas I antes de V/X, X antes de L/C, e C antes de D/M são permitidos.";
                return -1;
            }
            
            result -= curr;
        }
        
        prev = curr;
    }
    
    return result;
}

int main() {
    char roman[100];
    char *error = NULL;
    
    printf("Digite um numeral romano: ");
    scanf("%99s", roman);
    
    int decimal = romanToDecimal(roman, &error);
    
    if (decimal < 0) {
        printf("Erro: %s\n", error);
    } else {
        printf("O valor decimal de %s é %d\n", roman, decimal);
    }
    
    return 0;
}