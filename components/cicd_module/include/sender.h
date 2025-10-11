#pragma once

#include <CiCdAdaptor.h>
#include <sdbus-c++/sdbus-c++.h>

#include "fetcher.h"

class DataSender final : public sdbus::AdaptorInterfaces<org::metaos::CiCd::Interface_adaptor>
{
public:
    DataSender(sdbus::IConnection& connection, sdbus::ObjectPath objectPath);
    ~DataSender();

    std::string PipelineStatusFetch() override;

private:
    ActionsDataFetcher fetcher_;
};