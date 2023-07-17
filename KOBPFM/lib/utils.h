#ifndef utilfunch
#define utilfunch

namespace get{


sycl::queue cpu(){
    sycl::property_list queue_properties{sycl::property::queue::enable_profiling()};
    sycl::queue Q(sycl::cpu_selector_v, queue_properties);
    std::cout << "main>> Q initiated on device :\nmain>>   " << Q.get_device().get_info<sycl::info::device::name>() << "\n";
    return Q ; 
}

sycl::queue gpu(){
    sycl::property_list queue_properties{sycl::property::queue::enable_profiling()};
    sycl::queue Q(sycl::gpu_selector_v, queue_properties);
    std::cout << "main>> Q initiated on device :\nmain>>   " << Q.get_device().get_info<sycl::info::device::name>() << "\n";
    return Q ; 
}

float exec_time(sycl::event event){
    auto end = event.get_profiling_info<sycl::info::event_profiling::command_end>();
    auto start = event.get_profiling_info<sycl::info::event_profiling::command_start>();
    return (end-start)/1.0e9 ;
    
}

/*
template<int N>
sycl::accessor<float,N> accessor(sycl::handler& h, std::vector<float> VEC, sycl::int3 ar){}

template< >
sycl::accessor<float,3> accessor<3>(sycl::handler& h,std::vector<float> VEC, sycl::int3 ar){
    sycl::range<3> arange(ar[0], ar[1], ar[2]) ; 
    sycl::buffer<float,3> buff(VEC.data(), arange) ;
    sycl::accessor acc{buff,h} ;
    return acc ;
}

*/



}

#endif