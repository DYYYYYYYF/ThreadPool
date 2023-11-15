#include "thread_pool.hpp"
#include <iostream>
#include <ctime>

#ifdef _WIN32
#include <sys/utime.h>
#include <time.h>
#endif

#ifdef __APPLE__
#include <sys/_types/_time_t.h>
#include <xlocale/_time.h>
#endif

void add(int a) {
	long long int res = 1;
	for (int i = 0; i < 100; i++) {
		res += i;
		for (int j = 0; j< 100; j++){
			res += j;
		}
	}

	std::cout << res << std::endl;
}


int main(int argc, char* argv[]) {

	time_t begin_tick = time(NULL);
	localtime(&begin_tick);

	for (int i = 0; i < 400; ++i) {
		std::thread th(add, 1);
		th.join();
	}

	time_t end_tick = time(NULL);
	tm* end = localtime(&end_tick);

	std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;
	std::cout << "Used time: " << end_tick - begin_tick << std::endl;

	system("PAUSE");

	return 0;
}