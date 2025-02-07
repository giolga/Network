#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char* bin_to_decimal(char binary_subnet_mask[]) {
    unsigned char k, x;
    int ans = 0;
    char* decimal_subnet_mask_value = (char*)malloc(4 * sizeof(char));  

    if (decimal_subnet_mask_value == NULL) {
        printf("Memory allocation failed\n");
        return NULL; 
    }

    k = strlen(binary_subnet_mask);
    x = k - 1;

    for (unsigned char i = 0; i < k; i++) {
        ans += (binary_subnet_mask[i] - '0') * pow(2, x);
        x--;
    }

    sprintf(decimal_subnet_mask_value, "%d", ans);  
    return decimal_subnet_mask_value;  
}

int is_valid_ip(char ip[]) {
    int first_octet, second_octet, third_octet, fourth_octet;

    // Assume you already split the IP address into four octets
    if (sscanf(ip, "%d.%d.%d.%d", &first_octet, &second_octet, &third_octet, &fourth_octet) != 4) {
        printf("Invalid IP format\n");
        return 0;
    }

    // Validate each octet is within the valid range (0-255)
    if (first_octet < 0 || first_octet > 255 || 
        second_octet < 0 || second_octet > 255 || 
        third_octet < 0 || third_octet > 255 || 
        fourth_octet < 0 || fourth_octet > 255) {
        printf("Invalid IP: Each octet must be between 0 and 255\n");
        return 0;
    }

    return 1;  // Valid IP
}

char* get_binary_string(unsigned char n) {
    unsigned char zero_cnt = 8 - n;
    char* binary_string = (char*)malloc(9 * sizeof(char)); 
    binary_string[8] = '\0'; 
    if (binary_string == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        binary_string[i] = '1';
    }

    for (int i = n; i < 8; i++) {
        binary_string[i] = '0';
    }

    return binary_string;
}

unsigned char get_octets(int *k, char ipv4[]) {
    unsigned char octet = 0;

    while (ipv4[*k] != '.' && ipv4[*k] != '\0')
    {
        octet = octet * 10 + (ipv4[(*k)++] - 48);
    }

    return octet;
}

void type_1_subnetting() {
    char ipv4[16];

    printf("Insert IPv4 address: ");
    fgets(ipv4, sizeof(ipv4), stdin);
    ipv4[strcspn(ipv4, "\n")] = '\0';

    printf("Insert required subnets: ");
    unsigned char subnet;
    scanf("%hhu", &subnet);

    if (!is_valid_ip(ipv4)) {
        printf("Invalid IP address.\n");
        return ;
    }

    printf("IPv4: %s, Required Subnets: %hhu\n", ipv4, subnet);

    unsigned char first_octet, second_octet, third_octet, fourth_octet;
    int k = 0;
    
    first_octet = get_octets(&k, ipv4);

    k++;
    second_octet = get_octets(&k, ipv4);

    k++;
    third_octet = get_octets(&k, ipv4);

    k++;
    fourth_octet = get_octets(&k, ipv4);
      
    unsigned char subnet_mask_cnt = 0;
    char network_class;    

    if(first_octet > 0 && first_octet < 127) {
        subnet_mask_cnt = 1;
        network_class = 'A';
    }
    else if(first_octet > 127 && first_octet < 192) {
        subnet_mask_cnt = 2;
        network_class = 'B';
    }
    else if(first_octet > 191 && first_octet < 224) {
        subnet_mask_cnt = 3;
        network_class = 'C';
    }
    else {
        printf("specify ip address correctly!\n");
        return ;
    }

    k = 4; //for S/M, if cntOf255=3 then there is one zero left, so k should be equal to 4 for while loop
    k -= subnet_mask_cnt;

    char subnet_mask[16] = "";
    unsigned char i = 0;
    while(subnet_mask_cnt > 0) {
        strcat(subnet_mask, "255.");
        subnet_mask_cnt--;
    }

    unsigned char n = log2(subnet);
    if(pow(2, n) < subnet)
        n++;

    char* find_subnet_mask = get_binary_string(n); 
    // printf("Subnet Mask in binary: %s\n", find_subnet_mask);

    char* binary_to_decimal_subnet = bin_to_decimal(find_subnet_mask);
    strcat(subnet_mask, binary_to_decimal_subnet);

    k--;
    while(k > 0) {
        strcat(subnet_mask, ".0");
        k--;
    }

    size_t len = strlen(subnet_mask);
    if (len > 0 && subnet_mask[len - 1] == '.') {
        subnet_mask[len - 1] = '\0';
    }

    printf("Subnet mask: %s\n", subnet_mask);

    int host_left = 8 - n;
    int range = pow(2, host_left);
    
    char first_available_host_of_subnet1[16] = "";

    if(network_class == 'A') {
        sprintf(first_available_host_of_subnet1, "%hhu.%hhu.%hhu.%hhu", first_octet, range, third_octet, fourth_octet + 1);
    }
    else if(network_class == 'B') {
        sprintf(first_available_host_of_subnet1, "%hhu.%hhu.%hhu.%hhu", first_octet, second_octet, range, fourth_octet + 1);
    }
    else {
        sprintf(first_available_host_of_subnet1, "%hhu.%hhu.%hhu.%hhu", first_octet, second_octet, third_octet, range + 1);
    }

    printf("1st available host address of subnet1: %s\n", first_available_host_of_subnet1);

    int available_hosts;

    if(network_class == 'A') {
        available_hosts = (256 - atoi(binary_to_decimal_subnet)) * 256 * 256;
    }
    else if(network_class == 'B') {
        available_hosts = (256 - atoi(binary_to_decimal_subnet)) * 256;
    }
    else {
        available_hosts = 256 - atoi(binary_to_decimal_subnet);
    }

    available_hosts -= 2;
    printf("Max #N of hosts: %d\n", available_hosts);

    free(find_subnet_mask); 
    free(binary_to_decimal_subnet);
}


int main(int argc, char *argv[]) {
    char task[50];

    printf("Operation: ");
    fgets(task, sizeof(task), stdin);
    task[strcspn(task, "\n")] = '\0';

    if (strcmp(task, "subnet-type1") == 0) {
        type_1_subnetting();
    } else {
        printf("Invalid input\n");
    }

    return 0;
}
