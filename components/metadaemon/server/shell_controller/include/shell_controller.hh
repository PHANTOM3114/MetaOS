#pragma once

//Standard Includes
#include <mutex>
#include <thread>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <condition_variable>
#include <pty.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/wait.h>

//gRPC Includes
#include <grpcpp/grpcpp.h>
#include "protocols/grpc/metaos/metadaemon/system.grpc.pb.h"
#include "protocols/grpc/metaos/metadaemon/system.pb.h"

namespace MetaOS
{
    namespace Controller
    {
        namespace Shell
        {
            class ShellReactor : public grpc::ServerBidiReactor<
                    MetaOS::ExecuteShellRequest, MetaOS::ExecuteShellResponse>
            {
            public:
                using OutputCallback = std::function<void(const std::string&)>;

                explicit ShellReactor(int master_fd, pid_t pid, OutputCallback output_callback = nullptr);

                MetaOS::ExecuteShellRequest* GetRequest() { return &request_; }

                void Start()
                {
                    StartRead(&request_);
                }

                void OnReadDone(bool ok) override;
                void OnWriteDone(bool ok) override;

                void OnDone() override
                {
                    running_ = false;
                    close(master_fd_);

                    if (epoll_cycle.joinable())
                    {
                        epoll_cycle.join();
                    }

                    waitpid(pid_, nullptr, 0);
                    close(epoll_fd_);

                    delete this;
                }

            private:
                void DoNextWrite();

                int master_fd_;
                pid_t pid_;
                int epoll_fd_;

                std::thread epoll_cycle;
                std::queue<std::string> shell_output_queue_;
                std::mutex queue_mutex_;
                std::condition_variable data_notifier;
                std::atomic<bool> running_{true};
                std::atomic_flag writing_in_progress_;

                OutputCallback output_callback_;

                MetaOS::ExecuteShellRequest request_;
                MetaOS::ExecuteShellResponse response_;
            };

            class AbortedReactor : public grpc::ServerBidiReactor<
                    MetaOS::ExecuteShellRequest, MetaOS::ExecuteShellResponse>
            {
            public:
                explicit AbortedReactor(const grpc::Status& status)
                {
                    Finish(status);
                }

                void OnDone() override { delete this; }

                void OnReadDone(bool ok) override
                {
                }

                void OnWriteDone(bool ok) override
                {
                }
            };

            class ProcessingImplemantation : public MetaOS::ShellControllerService::CallbackService
            {
            public:
                grpc::ServerBidiReactor<MetaOS::ExecuteShellRequest, MetaOS::ExecuteShellResponse>* ExecuteShell(
                    grpc::CallbackServerContext* context) override
                {
                    try
                    {
                        int master_fd;
                        pid_t pid = forkpty(&master_fd, nullptr, nullptr, nullptr);

                        if (pid < 0)
                        {
                            return new AbortedReactor(grpc::Status(grpc::StatusCode::INTERNAL, "forkpty failed"));
                        }

                        if (pid == 0)
                        {
                            execlp("bash", "bash", nullptr);
                            exit(1);
                        }

                        auto* reactor = new ShellReactor(master_fd, pid);
                        reactor->Start();
                        return reactor;
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "ShellReactor creation failed: " << e.what() << std::endl;
                        return new AbortedReactor(grpc::Status(grpc::StatusCode::INTERNAL, e.what()));
                    }
                }
            };
        }
    }
}
