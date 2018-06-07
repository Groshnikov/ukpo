#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "CuTest.h"
 
#define BUFLEN 2048
#define PORT 3000 

int s;
struct sockaddr_in server, si_other;
int slen , recv_len;
char buf[BUFLEN];
int slen = sizeof(si_other);

void should_connect_to_server(CuTest* testContext) {
    fflush(stdout);

    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) < 0) {
        printf("Test server: Socket error");
    }

    CuAssertStrEquals(testContext, "Client connected", buf);
}

void should_send_path_and_extension(CuTest* testContext) {
    fflush(stdout);

    memset(buf,'\0', BUFLEN);

    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) < 0) {
        printf("Test server: Socket error");
    }

    buf[recv_len] = '\0';

    CuAssertStrEquals(testContext, "./a/ js", buf);
}

void should_receive_result(CuTest* testContext) {
    fflush(stdout);

    memset(buf,'\0', BUFLEN);

    char * message = "app.js\nbundle.js\n";
    sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, slen);

    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) < 0) {
        printf("Test server: Socket error");
    }

    buf[recv_len] = '\0';

    CuAssertStrEquals(testContext, "Message received", buf);
}

CuSuite* test_suite() {
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, should_connect_to_server);
    SUITE_ADD_TEST(suite, should_send_path_and_extension);
    // SUITE_ADD_TEST(suite, should_receive_result);

    return suite;
}

int all_tests()
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, test_suite());
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);

	printf("%s\n", output->buffer);

    return suite->failCount > 0;
}

int main()
{
    // Create a socket
    s = socket(AF_INET, SOCK_DGRAM, 0 );
    printf("Socket created.\n");
    if (s < 0) {
        perror("socket");
        exit(1);
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons( PORT );
     
    // Bind
    bind(s ,(struct sockaddr *)&server , sizeof(server));
    puts("Bind done");

    int status = all_tests();

    char * message = "app.js\nbundle.js\n";
    sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, slen);

    close(s);
    return status;
}
