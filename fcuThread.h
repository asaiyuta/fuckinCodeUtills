#include <thread>
#include <cassert>

namespace fcu {
	class EasyThread{
	public:	
		EasyThread(){}
		virtual ~EasyThread(){
			endThread();
		}		
		virtual void threadFunction(){}
		
		const void lockGlobal(){
			globalMutex.lock();
		}
		
		const void unlockGlobal(){
			globalMutex.unlock();
		} 	
	
		const void startThread(){
			assert(!isRunThread);
			isStopThread = false;
			std::thread func(&EasyThread::indeedThreadFunction,this);
			thread = std::move(func);
			assert(thread.joinable());
			thread.detach();
			isRunThread = true;
		}
		const void endThread(){
			if(isRunThread){
				endMutex.lock();
				isStopThread = true;
				endMutex.unlock();
			}
			while(isRunThread){}
		}
		
		void indeedThreadFunction(){
			try {
				for(;;){
					threadFunction();
					endMutex.lock();
					throwEndException();
					endMutex.unlock();
				}
			}catch(endException& e){
				isRunThread = false;
			}			
		}
		
		std::mutex& getGlobalMutex(){
			return globalMutex;
		}

	private:
		struct endException{};
		const void throwEndException(){
			if(isStopThread){
				throw endException();
			}
		}
		bool isRunThread;
		bool isStopThread;		
		std::thread thread;
		std::mutex endMutex;
		std::mutex globalMutex;

	};
	
	class FuncFreeRunner{
		public:
			FuncFreeRunner(void(*func)(void)){
				function = func;
				std::thread f(&FuncFreeRunner::threadFunction,this);
				thread = std::move(f);
				if(thread.joinable()){
					thread.detach();
					isRunThread = true;
				}
			}
			
			void threadFunction(){
				try{
					for(;;){
						function();
						throwEndException();	
					}
				}catch (endException& e){
						isRunThread = false;
				}
			}
			
			~FuncFreeRunner(){
				isStopThread = true;
				while(isRunThread){}
			}
			
		private:
			struct endException{};
			const void throwEndException(){
				if(isStopThread){
					throw endException();
				}
			}
				
			std::thread thread;
			void (*function)(void);
			bool isStopThread;	
			bool isRunThread;	
	};
	
	class FuncTimesRunner{
		public:
			FuncTimesRunner(void(*func)(void),unsigned long long int num = 1)
			: num(num)
			, counter(0)
			{
				function = func;
				std::thread f(&FuncTimesRunner::threadFunction,this);
				thread = std::move(f);
				if(thread.joinable()){
					thread.detach();
					isRunThread = true;
				}
			}
				
			void threadFunction(){
				try{
					for(;;){
						function();
						throwEndException();	
					}
				}catch(endException& e){
						isRunThread = false;
				}
			}
				
			~FuncTimesRunner(){
				counter = num;
				while(isRunThread){}
			}
			
			void waitEnd(){
				while (isRunThread) {}
			}
				
		private:
			struct endException{};
			const void throwEndException(){
				if(!(++counter < num)){
					throw endException();
				}
			}
					
			std::thread thread;
			void (*function)(void);
			const unsigned long long int num;
			unsigned long long int counter;
			bool isRunThread;	
	};
}