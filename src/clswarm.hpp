/*clswarm.hpp
houses class and function prototypes for swarm class
for opencl version

Copyright 2018 by Aaron Klapatch,
code derived from http://www.swarmintelligence.org/tutorials.php
along with some help from Dr. Ebeharts presentation at IUPUI.
*/

#ifndef _CLSWARM_HPP_
#define _CLSWARM_HPP_

#include <vector>
#include <cmath>
#include <random>

#ifdef __APPLE__	
#include <OpenCL/opencl.hpp>	
#else	
#include <CL/cl.hpp>	
#endif	

#define KER_SIZE 0x100000
#define PLATFORM_NUM 1
#define DEVICE_NUM 1
#define DEFAULT_DIM 1
#define DEFAULT_PARTNUM 100
#define DEFAULT_W 1
#define C1 1.492
#define C2 2

///there is no particle class, the swarm class has all the data
///that might change later on
class clswarm {
    private:
        /// no. of particles, no. of dimensions
        cl_uint dimnum=DEFAULT_DIM,partnum=DEFAULT_PARTNUM;
        cl::Buffer dimnumbuf, partnumbuf;

        ///best particle dimensions, its fitness, swarm bounds
        cl::Buffer gbestbuf, upperboundbuf, lowerboundbuf;

        ///set that so all fitness numbers will show up
        cl_float gfitness=-HUGE_VALF;
        cl::Buffer gfitbuf;

        ///inertial weight and 2 behavioral constants
        cl_float w=DEFAULT_W, c1=C1, c2=C2;
        cl::Buffer wbuf, c1buf, c2buf;

        ///particle data
        cl::Buffer presentbuf, pbestbuf, vbuf;
        cl::Buffer pfitnessbuf, fitnessbuf;
		
		///all the opencl stuff
        std::vector<cl::Platform> platforms;
        std::vector<cl::Device> devices;
        cl::Context context;
        cl::Program::Sources sources;
        cl::Program program;
        cl::CommandQueue queue;
        cl::Kernel distr, cmpre, updte, updte2;
        cl_int ret;

    public:
        ///defaults to 100 particles and 1 dimension
        clswarm();

        ///sets no. particles and no. dimensions and w
        clswarm(cl_uint, cl_uint,cl_float,cl_float,cl_float);

        ///frees all swarm memory
        ~clswarm();

        ///sets the number of particles
        void setPartNum(cl_uint);

		///get particle numbers
        cl_uint getPartNum();

        ///sets no. of dimensions
        void setDimNum(cl_uint);

		///get dimension number
		cl_uint getDimNum();

        ///sets inertial weight
        void setWeight(cl_float);

		///returns inertial weight
		cl_float getWeight();

        ///sets 2 behavioral constants of the swarm
        void setConstants(cl_float, cl_float);

		///returns array with two those 2 constants
		cl_float * getConstants();

        /// sets upper and lower bounds and distributes linearly between them
        /** lower bound is first argument, upper bound is second argument */
        void distribute(cl_float * , cl_float* );

        /// updates (int) number of times with *fitness as a fitness function
        void update(unsigned int);

        ///returns the best position in the swarm.
        cl_float* getGBest();

		///sets particle data
		void setPartData(cl_float *);

        ///returns all particle data
        cl_float * getPartData();

        ///returns the fitness of the best particle
        cl_float getGFitness();
};

#endif