#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "absl/strings/str_format.h"

#include "demo.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using ::google::protobuf::Empty;
using resumedemo::ResumeService;
using resumedemo::ResumeSummary;
using resumedemo::ResumeSummaryList;
using resumedemo::Resume;
using resumedemo::GetRequest;

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

// Logic and data behind the server's behavior.
class ResumeServiceImpl final : public ResumeService::Service
{
  Status GetAll(ServerContext* context, const Empty* request, ResumeSummaryList* response) override 
  {
    auto summary = response->add_list();
    summary->set_id("id0x0x0x0x0x");
    summary->set_name("Vincent Drake");
    return Status::OK;
  }

  Status Get(ServerContext* context, const GetRequest* request, Resume* response)
  {
    response->set_id("id0x0x0x0x");
    response->set_name("Vincent Drake");
    response->set_title("Senior Software Engineer");
    response->set_experience("20+ years of experience");
    response->set_education("Incomplete Bachelor Degree");
    return Status::OK;
  }
};

void RunServer(uint16_t port) {
  std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
  ResumeServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  absl::InitializeLog();
  RunServer(absl::GetFlag(FLAGS_port));
  return 0;
}
