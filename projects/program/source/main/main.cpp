#ifndef _SELECT_MAIN
#   define _SELECT_MAIN 0
#endif


#if _SELECT_MAIN == 0
#   include "light_scene.hpp"
#elif _SELECT_MAIN == 1
#   include "light_scene2.hpp"
#else

#endif 


int main()
{
#if _SELECT_MAIN == 0
    return light_scene();
#elif _SELECT_MAIN == 1
    return light_scene2();
#else

#endif
}