#include "thread_pool.hpp"

std::mutex mutex;
int a = 0;

void add(int a) {

	while (a < 1000) {
		std::unique_lock<std::mutex> mlock(mutex);
		std::cout << a++ << std::endl;
		mlock.unlock();
	}

}


int main(int argc, char* argv[]) {
	mt::Test();

	std::thread th1(add, a);
	std::thread th2(add, a);

	th1.join();
	th2.join();


	return 0;
}