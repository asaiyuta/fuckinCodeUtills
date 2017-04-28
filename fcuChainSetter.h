#pragma once
namespace fcu {
	template<typename ARG, typename PARENT>
	class ChainSetter{
	public:
		ChainSetter(ARG arg,PARENT& parent)
		: body(arg)
		, holder(parent)
		{}
		
		ARG operator ()(){
			return body;
		}
		
		PARENT &operator ()(ARG arg){
			body = arg;
			return holder;
		}
		
	private:
		ARG body;
		PARENT& holder;
	};
}