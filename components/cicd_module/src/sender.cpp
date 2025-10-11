#include "sender.h"

DataSender::DataSender(sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
    : AdaptorInterfaces(connection, std::move(objectPath))
{
    std::cout << "DataSender constructor" << std::endl;
    registerAdaptor();
}

DataSender::~DataSender()
{
    std::cout << "DataSender destructor" << std::endl;
    unregisterAdaptor();
}

std::string DataSender::PipelineStatusFetch()
{
    std::cout << "PipelineStatusFetch" << std::endl;
    std::string status = fetcher_.StartFetching();
    return status;
}
