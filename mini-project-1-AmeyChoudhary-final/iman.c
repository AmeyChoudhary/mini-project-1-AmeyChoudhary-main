#include "headers.h"

// used Chatgpt for assistance
int manPage ( char * command)
{
    int sockfd;
    struct addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    if (getaddrinfo(HOSTNAME, PORT, &hints, &res) != 0) {
        perror("Error in DNS resolution");
        exit(1);
    }

    for (p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol);
        if (sockfd == -1) {
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) != -1) {
            break;
        }

        close(sockfd);
    }
    
   
    if (res == NULL) {
        perror("Error in connecting to server");
        exit(1);
    }

    char request[STRING_SIZE];
    char query_string[STRING_SIZE];


    sprintf(query_string, "topic=%s&section=all", command);

    char q [strlen(query_string) + 1];
    strcpy(q, query_string);

    snprintf(request, STRING_SIZE, "GET /?%s HTTP/1.1\r\nHost: %s\r\n\r\n",q, HOSTNAME);

    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("Error in sending request");
        exit(1);
    }

    char buffer[STRING_SIZE];

    int name_found = 0;
    int see_also = 0;
    int print_Flag =0;

    while (1) {
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0 || see_also) {
            break;
        }

        // Check if we've found the "NAME" section
        if (!name_found) {
            char *name_start = strstr(buffer, "NAME\n");
            if (name_start != NULL) {
                name_found = 1;
                fwrite(name_start, 1, bytes_received - (name_start - buffer), stdout);
                print_Flag = 1;
            }
        } else {
            char *see_also_start = strstr(buffer, "SEE ALSO");
            if (see_also_start != NULL) {
                see_also = 1;
                fwrite(buffer, 1, see_also_start - buffer, stdout);
            } else {
                fwrite(buffer, 1, bytes_received, stdout);
            }
        }
    }

    close(sockfd);

    freeaddrinfo(res);

    if(!print_Flag)
    {
        printf("iMan: no manual entry for %s\n", command);
        return 1;
    }

    return 0;
}

int iman(int fore_background, char **token_array, int size_array)
{
    if ( size_array != 2 )
    {
        printf("iMan: invalid number of arguments\n");
        return 1;
    }

    char *command = token_array[1];

    manPage(command);

    return 0;
}
