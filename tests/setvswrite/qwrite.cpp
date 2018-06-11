///an example to demonstrate usage of the opencl 1.2 C++ wrapper
/// code example usage https://github.com/Dakkers/OpenCL-examples/blob/master/example00/main.cpp
///timing code from https://www.pluralsight.com/blog/software-development/how-to-measure-execution-time-intervals-in-c--


#include <iostream>
#ifdef __APPLE__
    #include <OpenCL/cl.hpp>
#else
    #include <CL/cl.hpp>
#endif
#include <chrono>

#define ITEMS 100

int main(){
    ///retrieve platforms
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    ///get platfrom and print
 mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm                            cl::Platform platform = platforms[0];
    std::cout << "Platform: " << platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    ///get devices
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    ///set devices
    cl::Device device=devices[0];
    cl::Device device1=devices[1];
    std::cout << "Device: " << device.getInfo<CL_DEVICE_NAME>() << "\n";
    std::cout << "Device1: " << device1.getInfo<CL_DEVICE_NAME>() << "\n";

    ///get context
    cl::Context context({device});

    ///make source and push it into source
    cl::Program::Sources sources;
    char  kersrc[]=
        "void kernel add (){"
        "int i=get_global_id(0);"
        "}";
    sources.push_back({kersrc, sizeof(kersrc)});

    ///build program
    cl::Program program(context,sources);
    if (program.build({device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        exit(1);
    }

    ///make queue
    cl::CommandQueue queue(context,device);

    ///make+init data
    int n=ITEMS,i=0;
    int * A = new int [n];
    for(i=0;i<n;++i){
        A[i]=0;
    }

    ///make buffer and write to it;
    cl::Buffer Abuf;
    Abuf=cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * n);

    ///set args
    cl::Kernel add=cl::Kernel(program,"add");
    //add.setArg(0,Abuf);

    std::vector<cl::Event> vents(1);
    std::vector<cl::Event> ovent(1);

    ///measure time
    auto start = std::chrono::high_resolution_clock::now();
    
    ///write to kernel
    queue.enqueueWriteBuffer(Abuf, CL_TRUE, 0, sizeof(int)*n, A,NULL,&ovent[0]);

    ///run kernel + read buffer answer
    queue.enqueueNDRangeKernel(add,cl::NullRange, cl:: NDRange(n),cl::NullRange,&ovent,&vents[0]);
    cl::WaitForEvents(vents);

    auto finish = std::chrono::high_resolution_clock::now();

    ///std::chrono::duration elapsed = finish - start;

    auto msec=std::chrono::duration_cast<std::chrono::microseconds>(finish-start);

    std::cout <<"Time to execute " << msec.count()<<"\n";

    ///print all answers
    for(i=0;i<n;++i){
        std::cout << "A["<<i<< "] " << A[i] <<"\t";
    }

    return 0;
}