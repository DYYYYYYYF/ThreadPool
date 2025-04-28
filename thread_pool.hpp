#pragma once

#include <thread>
#include <vector>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <utility>
#include <atomic>
#include <condition_variable>

namespace mt {

	template <typename Elem>
	class TaskQueue {
	public:
		TaskQueue() {}
		TaskQueue(TaskQueue&& other) {}
		virtual ~TaskQueue() {}

		void Enqueue(Elem& elem) {
			std::unique_lock lock(m_Mutex);
			m_Queue.emplace(elem);
		}

		bool Dequeue(Elem& elem) {

			std::unique_lock lock(m_Mutex);

			if (m_Queue.empty()) return false;
			elem = std::move(m_Queue.front());

			m_Queue.pop();

			return true;
		}

		bool empty() {
			std::unique_lock lock(m_Mutex);
			return m_Queue.empty();
		}

		int size() {
			std::unique_lock lock(m_Mutex);
			return m_Queue.size();
		}

	private:
		std::queue<Elem> m_Queue;
		std::mutex m_Mutex;
	};

	class ThreadPool {
	private:
		// Inner class
		class ThreadWorker {
		public:
			ThreadWorker(ThreadPool* thread_pool, const int id) : m_pThreadPool(thread_pool), m_ID(id) {}

			void operator()() {
				std::function<void()> func;

				bool dequeued;

				while (!m_pThreadPool->m_bShutdown) {
					{
						std::unique_lock lock(m_pThreadPool->m_Mutex);

						if (m_pThreadPool->m_Tasks.empty()) {
							m_pThreadPool->m_Condition.wait(lock);
						}

						dequeued = m_pThreadPool->m_Tasks.Dequeue(func);
					}

					if (dequeued) func();
				}
			}

		private:
			int m_ID;
			ThreadPool* m_pThreadPool;

		};

	// Thread Pool
	public:
		ThreadPool() : m_Threads(std::vector<std::thread>(std::thread::hardware_concurrency())), m_bShutdown(false), is_initialized(false){}
		ThreadPool(const int thread_count) : m_Threads(std::vector<std::thread>(thread_count)), m_bShutdown(false), is_initialized(false) {}
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;
		virtual ~ThreadPool() {}

		void Init() {
			if (is_initialized) {
				return;
			}

			for (int i = 0; i < m_Threads.size(); ++i) {
				m_Threads.at(i) = std::thread(ThreadWorker(this, i));
			}
			is_initialized = true;
		}

		void Shutdown() {
			m_bShutdown = true;
			m_Condition.notify_all();

			for (int i = 0; i < m_Threads.size(); ++i) {
				if (m_Threads.at(i).joinable()) {
					m_Threads.at(i).join();
				}
			}
			is_initialized = false;
		}

		template <typename Func, typename... Args>
		auto Commit(Func&& func, Args&&... args)
			-> std::future<decltype(std::invoke(std::forward<Func>(func), std::forward<Args>(args)...))>
		{
			using ReturnType = decltype(std::invoke(std::forward<Func>(func), std::forward<Args>(args)...));
			auto task = std::make_shared<std::packaged_task<ReturnType()>>(
				[func = std::forward<Func>(func),
				args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
					return std::apply([&func](auto&&... args) {
						return std::invoke(func, std::forward<decltype(args)>(args)...);
						}, std::move(args));
				});

			std::function<void()> newTask = [task]() {
				(*task)();
				};

			m_Tasks.Enqueue(newTask);
			m_Condition.notify_one();
			return task->get_future();
		}

	
	private:
		bool is_initialized;
		std::vector<std::thread> m_Threads;
		TaskQueue<std::function<void()>> m_Tasks;
		std::mutex m_Mutex;
		std::condition_variable m_Condition;

		bool m_bShutdown;

	};
};
