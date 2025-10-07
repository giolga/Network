#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

char bit_flip(char* c) {
    short hx = 0;

    if (*c >= '0' && *c <= '9') {
        hx = *c - '0';
    } else if (*c >= 'a' && *c <= 'f') {
        hx = 10 + (*c - 'a');
    } else {
        printf("Invalid hex character: %c\n", *c);
        return '\0';
    }

    char bin_str[5]; // 4 bits + '\0'
    bin_str[4] = '\0';

    // Convert to binary
    for (int i = 3; i >= 0; i--) {
        bin_str[i] = (hx % 2) + '0';
        hx /= 2;
    }

    // printf("Binary: %s\n", bin_str);

    if(bin_str[2] == '0') bin_str[2] = '1';
    else bin_str[2] = '0';

    // printf("flipped Binary: %s\n", bin_str);

    short dec = 0;
    for(int i = 3; i >= 0; i--) {
        dec += (bin_str[i] - '0') * pow(2, 3 - i);
    }

    // printf("dec flipped %d\n", dec);

    char flipped_char;
    if (dec < 10) {
        flipped_char = '0' + dec;
    } else {
        flipped_char = 'a' + (dec - 10);
    }

    // printf("flipped_char %c\n", flipped_char);
    
    return flipped_char;
}


void eui() {
    char mac[64];
    char interface_id[64] = "";
    const char fffe[]={"ff:fe"};
    fgets(mac, sizeof(mac), stdin); //12-32-cc-dd-ab-cd
    printf("%s", mac);

    char flip = bit_flip(&mac[1]);
    mac[1] = (char)flip;

    int mac_len = strlen(mac);
    bool hextet = false;

    for (int i = 0; i < mac_len / 2; i++) {
        if (mac[i] == '-' && !hextet) {
            hextet = true;
            continue;
        }
        if(mac[i] == '-' && hextet) {
            strcat(interface_id, ":");
            hextet = false;
            continue;
        }

        char temp[2] = { mac[i], '\0' };
        strcat(interface_id, temp);
    }

    strcat(interface_id, fffe);

    for (int i = mac_len / 2; i < mac_len; i++) {
        if (mac[i] == '-' && !hextet) {
            hextet = true;
            continue;
        }
        if(mac[i] == '-' && hextet) {
            strcat(interface_id, ":");
            hextet = false;
            continue;
        }

        char temp[2] = { mac[i], '\0' };
        strcat(interface_id, temp);
    }

    printf("Interface ID: %s\n", interface_id);
}

int main() {
    printf("poatan\n");
    eui();

    return 0;
}