#pragma once

#include "CiCdAdaptor.h"

#include <iostream>
#include <memory>
#include <vector>
#include "./include/providers/ipipeline_provider.h"

class CiCdDbusAdapter final : public sdbus::AdaptorInterfaces<org::metaos::CiCd::Interface_adaptor>
{
public:
    CiCdDbusAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath);
    ~CiCdDbusAdapter();
    
    std::string PipelineStatusFetch() override;

private:
    std::vector<std::unique_ptr<IPipelineProvider>> providers_;
};