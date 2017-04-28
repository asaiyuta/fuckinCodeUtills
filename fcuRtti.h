#pragma once
#include <typeinfo>
#include <cxxabi.h>
#include <string>
#include <stdlib.h>

namespace fcu{
	template<typename T>
	static std::string easyTypename(const T& obj){
		const std::type_info& id = typeid(obj);
		int stat(0);
        std::string nameBuffer;
        char *name = abi::__cxa_demangle(id.name(),0,0,&stat);
		if(name!=NULL) {
			if(stat == 0) {
                nameBuffer = name;
				free(name);
				return nameBuffer;
			}
			free(name);
		}
		return "unknown";
	}
}