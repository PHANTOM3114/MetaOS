#pragma once

//Related header
#include <providers/ipipeline_provider.h>
#include "CiCdAdaptor.h"

//C++ Standard Includes
#include <memory>
#include <vector>

class CiCdDbusAdapter final : public sdbus::AdaptorInterfaces<org::metaos::CiCd::Interface_adaptor> {
public:
    CiCdDbusAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath);
    ~CiCdDbusAdapter();

    std::string PipelineStatusFetch() override;
    bool UpdateToken(const std::string& providerName, const std::string& token) override;

private:
    std::vector<std::unique_ptr<IPipelineProvider>> providers_;
};