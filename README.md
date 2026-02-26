## gRPC demo

This is a simple demonstration of gRPC basics. Essentially, it is a method of
executing logic in remote locations, a concept that dates a long way back.
Personally, I've worked with SOAP tools, Microsoft's DCOM, and CORBA, which all
have similar goals with different trade-offs. I'm sure there is plenty to learn
about gRPC and protobuf's way of doing things, and this simple project
demonstrates only the basics.

## My steps
* Built gRPC from source code using CMake per instructions on the [Quick Start Guide](https://grpc.io/docs/languages/cpp/quickstart/)
* Skimmed documentation for the basics of the gRPC IDL language
* Came with with a simple service for retrieving resume details
* Borrowed build scripts from the cpp helloworld example
* Customized build scripts for the service and write simple implementation for resumeServer and resumeClient


## Caveats
Creation of this repo focused on haste more than correctness and reusability. There is plenty missing in my haste that prevents this from being considered a "good" repo. Kindly evaluate this repo with that in mind. Sore points I'm painfully aware of:

* Binary and intermediate files are checked into the repo, which is not correct.
* Building this repo locally likely depends on following the same quickstart guide I followed; i.e. installing gRPC tools to the `~/.local` directory.
* The resume schema is sorely lacking. Repeated experience fields with begin-end dates for roles would be better, of course.
* The demonstration does nothing but return static data. Adding a write interface to the service allowing uploading of resumes would be better, of course. This would lead to interesting problems to solve, like resumeServer serialization and thread synchronization.
