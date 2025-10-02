#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char* bin_to_decimal(char binary_subnet_mask[]) {
    unsigned char k, x;
    int ans = 0;
    char* decimal_subnet_mask_value = (char*)malloc(33 * sizeof(char));  

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

char* n_times_one(int n) {
    if (n < 0 || n > 8) return NULL;

    char* ans = (char*)malloc(9);  // Allocate 9 bytes (8 characters + null terminator)
    if (!ans) return NULL;  // Check for allocation failure

    for (int i = 0; i < n; i++) {
        ans[i] = '1';
    }
    for (int i = 0; i < (8 - n); i++) {
        ans[n + i] = '0';
    }
    
    ans[8] = '\0';
    return ans; 
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
        if(ipv4[*k] == '/')
            return octet;
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
    printf("Subnet Mask in binary: %s\n", find_subnet_mask);

    char* binary_to_decimal_subnet = bin_to_decimal(find_subnet_mask);
    printf("Bin to dec: %s", binary_to_decimal_subnet);

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

void type_2_subnetting() {
    char input[32];  
    unsigned char first, second, third, fourth, slash;
    char find_class;
    
    printf("Insert IPv4 Address with its mask without any white space (e.g., 192.168.1.1/24): ");
    fgets(input, sizeof(input), stdin);
    
    if (sscanf(input, "%hhu.%hhu.%hhu.%hhu/%hhu", &first, &second, &third, &fourth, &slash) != 5) {
        printf("Invalid input format!\n");
        return;
    }

    printf("\nAll octets: %hhu.%hhu.%hhu.%hhu\n", first, second, third, fourth);
    printf("After Slash: %hhu\n", slash);

    // Determine IP Class & Subnet Mask Prefix
    unsigned int bit, cntOf255;
    if (slash >= 8 && slash < 16) {
         bit = 8; 
         cntOf255 = 1;
         find_class = 'A';
    }
    else if (slash >= 16 && slash < 24) { 
        bit = 16; 
        cntOf255 = 2; 
        find_class = 'B';
    }
    else { 
        bit = 24;
        cntOf255 = 3;
        find_class = 'C';
    }

    unsigned int period = bit + 8 - slash;
    unsigned int forSubnett = abs(slash - bit);

    printf("n Times one: %d\n", forSubnett);
    printf("Define class: %c\n", find_class);
    printf("period: %d\n", period);

    char find_SM[32];
    char* result = n_times_one(forSubnett);  // Get dynamically allocated string

    if (result) {
        strcpy(find_SM, result);  
        free(result);
    }

    printf("Subnet: %s\n", find_SM);

    int network_address, tmp;
    char NA[32], BA[32];

    if(find_class == 'A') {
        network_address = pow(2, period);
        tmp = network_address;
        network_address = (second / network_address) * network_address;
        sprintf(NA, "%d.%d.0.0", first, network_address);
        sprintf(BA, "%d.%d.255.255", first, (network_address + tmp - 1));
    }
    else if(find_class == 'B') {
        network_address = pow(2, period);
        tmp = network_address;
        network_address = (third / network_address) * network_address;
        sprintf(NA, "%d.%d.%d.0", first, second, network_address);
        sprintf(BA, "%d.%d.%d.255", first, second, (network_address + tmp - 1));
    }
    else {
        network_address = pow(2, period);
        tmp = network_address;
        network_address = (fourth / network_address) * network_address;
        sprintf(NA, "%d.%d.%d.%d", first, second, third, network_address);
        sprintf(BA, "%d.%d.%d.%d", first, second, third, (network_address + tmp - 1));
    }

    printf("Network Address: %s\n", NA);
    printf("Broadcast Address: %s\n", BA);

    int k = 4;
    k -= cntOf255;
    char subnet_mask[32] = "";

    char* part_of_SM = bin_to_decimal(find_SM);
    
    // Add parts with 255
    while (cntOf255 > 0) {
        strcat(subnet_mask, "255.");
        cntOf255--;
    }
    
    // Add the calculated part
    if (part_of_SM != NULL) {
        char temp[10];
        sprintf(temp, "%s.", part_of_SM);
        strcat(subnet_mask, temp);
        free(part_of_SM);  // Free allocated memory
    }
    
    // Add remaining parts with 0
    while (k > 0) {
        strcat(subnet_mask, "0.");
        k--;
    }
    
    // Remove the trailing dot
    int len = strlen(subnet_mask);
    if (len >= 2) {
        subnet_mask[len - 3] = '\0';
    }
    
    printf("Subnet Mask: %s\n", subnet_mask);
}

void eui64() {
    printf("EUI 64 in progress");
}

void host() {
    printf("Hosts in progress");
}

void banner() {
    printf("\033[0;34m"  // bright blue
"                _                      __                             \n"
"   / | / /__  / /__      ______  _____/ /__(_)___  ____ _\n"
"  /  |/ / _ \\/ __/ | /| / / __ \\/ ___/ //_/ / __ \\/ __ `/\n"
" / /|  /  __/ /_ | |/ |/ / /_/ / /  / ,< / / / / / /_/ / \n"
"/_/ |_/\\___/\\__/ |__/|__/\\____/_/  /_/|_/_/_/ /_/\\__, /  \n"
 "                                                /____/ \n"
           "\033[0m\n\n");

    printf("\033[1;33mNetworking\033[0m CISCO Tool "
           "\033[1;31mintelkumi\033[0m v1.0\n\n");
}

void header() {
    printf("\33[1;32m"
        "================== Cisco Tool ==================\n"
        "          Powered by El Kumi Cipher Labs (v1.0)\n"
        "================================================\n"
    "\033[0m");
}

void help() {
    printf("\033[1;33mUsage:\033[0m\n"
        "   ./networking -subt1 - Subnetting Type 1\n"
        "   ./networking -subt2 -Subnetting Type 2\n"
        "   ./networking -eui64 <00-11-ff-ca-fe-00> - specify MAC in the following format \n"
        "   ./networking -host - Count N# of hosts in the network"
    );
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        help();

        return 1;
    }

    banner();
    header();

    char task[50];

    // printf("Operation: ");
    // fgets(task, sizeof(task), stdin);
    // task[strcspn(task, "\n")] = '\0';

    if (strcmp(argv[1], "-subt1") == 0) {
        type_1_subnetting();
    } 
    else if (strcmp(argv[1], "-subt2") == 0) {
        type_2_subnetting();
    } 
    else if (strcmp(argv[1], "-eui64") == 0) {
        eui64();
    } 
    else if (strcmp(argv[1], "-host") == 0) {
        host();
    } 
    else {
        printf("Unknown operation: %s\n", argv[1]);
        help(); // Optional: show help on invalid command
        return 1;
    }

    return 0;
}
