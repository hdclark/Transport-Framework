//Memory.cc - A simple, in-RAM (heap) memory scheme. No databases, no file-storage. :)
//
//Programming notes:
//  -Do not make items here "const", because they will not show up when loading.
//  -Avoid using macro variables here because they will be obliterated during loading.
//  -Wrap dynamically-loaded code with extern "C", otherwise C++ compilation will mangle function names, etc.
//
// From man page for dlsym/dlopen:  For running some 'initialization' code prior to finishing loading:
// "Instead,  libraries  should  export  routines using the __attribute__((constructor)) and __attribute__((destructor)) function attributes.  See the gcc info pages for
//       information on these.  Constructor routines are executed before dlopen() returns, and destructor routines are executed before dlclose() returns."
//   ---for instance, we can use this to seed a random number generator with a random seed. However, in order to pass in a specific seed (and pass that seed to the library)
//      we need to define an explicitly callable initialization function. In general, these libraries should have both so that we can quickly adjust behaviour if desired.
//  

#include <iostream>
#include <string>
#include <list>

#include <memory>
#include <cmath>

#include "./Misc.h"

#include "./Constants.h"
#include "./Structs.h"

#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);
std::string FILE_TYPE("MEMORY");

bool VERBOSE = false;

//This is the chunk of memory we will use - a double-linked list of pointers to pieces of allocated memory.
//
//This is almost certainly a SLOW way to handle memory. However, it is an EASY way to handle memory too.
//
std::list< std::unique_ptr<base_particle> > pool;
//std::list< std::unique_ptr<base_particle> >::iterator place;  // <----(Needed for more elaborate sampling strategies only.)

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_memory.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_memory.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}


//Swallows a particle and appends it to the list.
void particle_sink( std::unique_ptr<base_particle> in ){
    pool.push_back( std::move( in ) );
    return;
}


//Returns the number of *particles* which can be stored (approximately.)
//
//In reality, try to keep list size below (far below?) about one-tenth of the value 
// reported here.
size_t how_much_more_room( void ){
    return pool.max_size() - pool.size();
}



//Returns a unique_ptr to the next active particle.
//
//RATIONALE: This is slow, but we handle two issues by doing this:
// 1) if the user deallocates the data, no harm is done.
// 2) we do not have to define a second clumsy "delete the particle which I have a pointer to" function to handle expiration of particles.
//
// and we gain the extra benefit that we can safely handle out additional particles without somehow protecting the particle in case we need to keep it.
//Although lists are slow, popping and pushing are two things they should theoretically be fastest at. Optimize me as needed.
std::unique_ptr<base_particle> get_next_particle(void){
    if(pool.size() != 0){
        //Can I do this all in one step? It hurts to look at it is probably so slow!
        //std::unique_ptr<base_particle> temp = std::move( pool.front() );


        
/*      //BAD.
        //Pop from the front - ie. flush each particle before moving onto secondary particles. 
        std::unique_ptr<base_particle> temp( std::move( pool.front() ) );
        pool.pop_front();
*/





      //BETTER.
        //Pop from the back - ie. run each particle and it's children prior to moving to the next particle. 
        //
        //This is the sane way to handle the issue. Otherwise, for large extended families the memory will undoubtedly page.
        std::unique_ptr<base_particle> temp( std::move( pool.back() ) );
        pool.pop_back();
        return std::move( temp );


/*
        //OK - maybe slow. Not needed for small particle sizes.
        //This is for large-size systems. We aggressively search out electrons and eliminate them (because for the present simulations they
        // are easily eliminated - localdump only.)
        //
        //If we go back too far, we will surely not find any electrons - so stop looking after going back _two_ particles.
        std::list< std::unique_ptr<base_particle> >::reverse_iterator iter;
        iter = pool.rbegin();

        if((iter == pool.rend()) || ((*iter) == nullptr)) return nullptr;
        if((*iter)->get_type() != Particletype::Photon){
            std::unique_ptr<base_particle> temp( std::move( (*iter) ) );
            pool.erase( (++iter).base() );  //Need to convert it into a (forward) iterator.
            return std::move( temp );
        }

        iter++;
        if((iter == pool.rend()) || ((*iter) == nullptr)) return nullptr;
        if((*iter)->get_type() != Particletype::Photon){
            std::unique_ptr<base_particle> temp( std::move( (*iter) ) );
            pool.erase( (++iter).base() );  //Need to convert it into a (forward) iterator.
            return std::move( temp );
        }

        iter++;
        if((iter == pool.rend()) || ((*iter) == nullptr)) return nullptr;
        if((*iter)->get_type() != Particletype::Photon){
            std::unique_ptr<base_particle> temp( std::move( (*iter) ) );
            pool.erase( (++iter).base() );  //Need to convert it into a (forward) iterator.
            return std::move( temp );
        }


        //If we are here, we just return the last element.
        std::unique_ptr<base_particle> temp( std::move( pool.back() ) );
        pool.pop_back();
        return std::move( temp );
*/


    }
    return nullptr;
}













#ifdef __cplusplus
    }
#endif

