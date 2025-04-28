#include "thread_pool.hpp"
#include <iostream>

#ifdef _WIN32
#include <sys/utime.h>
#include <time.h>
#endif

#ifdef __APPLE__
#include <sys/_types/_time_t.h>
#include <xlocale/_time.h>
#endif

class Task {
public:
	void add() {
		long long int res = 1;
		for (int i = 0; i < 100; i++) {
			res += i;
			for (int j = 0; j < 100; j++) {
				res += j;
			}
		}

		std::cout << res << std::endl;
	}

	float mutiple(float a, float b) {
		for (int i = 0; i < 100000; i++) {
			for (int j = 0; j < 100000; j++) {
				;
			}
		}

		std::cout << "thread id: " << std::this_thread::get_id() << " finished." << std::endl;
		return a * b;
	}
};

int main(int argc, char* argv[]) {

	time_t begin_tick = time(NULL);
	localtime(&begin_tick);

	mt::ThreadPool threadPool;
	threadPool.Init();

	Task task;
	auto fu = threadPool.Commit(&Task::mutiple, &task, 6.66f, 6.66f);
	for (int i = 0; i < 400; i++) {
		threadPool.Commit(&Task::add, &task);
	}

	float fMutipleResult = fu.get();
	std::cout << "Result: " << fMutipleResult << std::endl;

	time_t end_tick = time(NULL);
	tm* end = localtime(&end_tick);
	threadPool.Shutdown();

	std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;
	std::cout << "Used time: " << end_tick - begin_tick << std::endl;

#ifdef _WIN32
	system("PAUSE");
#endif

	return 0;
}
