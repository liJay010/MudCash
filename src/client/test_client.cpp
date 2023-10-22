#include "caclient.h"
#include <string>
int main()
{
    std::string ip = "127.0.0.1";
    int port = 8000;
    caclient cli(ip,port);
    cli.ca_start();
    return 0;
}