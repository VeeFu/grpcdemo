#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <iostream>
#include <memory>
#include <string>
#include <map>

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

class ResumeData
{
  ResumeData(){} // private constructor
public:
  std::map<std::string, Resume> data;

  static ResumeData load()
  {
    Resume sample;
    sample.set_id("id0x0x0x0x");
    sample.set_name("Vincent Drake");
    sample.set_title("Senior Software Engineer");
    sample.set_experience("20+ years of experience");
    sample.set_education("Incomplete Bachelor Degree");

    ResumeData ret;
    ret.data.insert({sample.id(), std::move(sample)});
    return ret;
  }
};

class ResumeServiceImpl final : public ResumeService::Service
{
private:
  ResumeData data;

public:
  ResumeServiceImpl() : data{ResumeData::load()}
  {}

  Status GetAll(ServerContext* context, const Empty* request, ResumeSummaryList* response) override 
  {
    for (const auto& [resumeID, resume] : data.data)
    {
      auto summary = response->add_list();
      summary->set_id(resumeID);
      summary->set_name(resume.name());
    }
    return Status::OK;
  }

  Status Get(ServerContext* context, const GetRequest* request, Resume* response)
  {
    const auto& found = data.data.find(request->id());
    if (found != data.data.end()){
      *response = found->second;
    }
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
