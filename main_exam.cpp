#include <QCoreApplication>
#include "mytcpserver-ex.h"
#include "funcs.h"

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //MyTcpServer server;

    //return a.exec();

    //max_flow_search();

    max_flow_search(graph_generator());
}
