#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

// #include "demo.pb.h"
#include "demo.grpc.pb.h"

ABSL_FLAG(std::string, target, "localhost:50051", "Server address");

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using resumedemo::ResumeSummary;
using resumedemo::ResumeSummaryList;
using resumedemo::Resume;
using resumedemo::GetRequest;
using resumedemo::ResumeService;

class ResumeClient
{
 public:
  ResumeClient(std::shared_ptr<Channel> channel)
      : stub_(ResumeService::NewStub(channel)) {}

  ResumeSummaryList GetAll()
  {
    ClientContext context;
    ResumeSummaryList reply;
    if (Status status = stub_->GetAll(&context, {}, &reply); status.ok())
    {
      return reply;
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      throw 0;
    }
  }

  Resume Get(const std::string& id)
  {
    ClientContext context;
    GetRequest request;
    Resume reply;
    request.set_id(id);
    
    if (Status status = stub_->Get(&context, request, &reply); status.ok())
    {
      return reply;
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      throw 0;
    }
  }

 private:
  std::unique_ptr<resumedemo::ResumeService::Stub> stub_;
};

void formatResume(const Resume& resume)
{
  const auto resumeProperties = std::array{
    std::tuple{"ID",          &Resume::id},
    std::tuple{"Name",        &Resume::name},
    std::tuple{"Title",       &Resume::title},
    std::tuple{"Experience",  &Resume::experience},
    std::tuple{"Education",   &Resume::education}
  };

  for(const auto& [name, valueMethod] : resumeProperties)
  {
    std::cout << "\t" <<std::setw(15) << name << " : " << (resume.*valueMethod)() << "\n";
  }
}

int main(int argc, char** argv)
{
  absl::ParseCommandLine(argc, argv);
  std::string target_str = absl::GetFlag(FLAGS_target);
  ResumeClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  auto allResumes = client.GetAll();
  std::cout << "Retrieving " << allResumes.list_size() << " resumes ...\n";
  for (int i{0}; i < allResumes.list_size(); ++i)
  {
    const auto& summary = allResumes.list(i);
    auto completeResume = client.Get(summary.id());
    formatResume(completeResume);
    std::cout << "\n";
  }

  return 0;
}

