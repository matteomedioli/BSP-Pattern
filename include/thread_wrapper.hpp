
#include <thread>
#include <condition_variable>
#include <exception>

class ThreadWrapper {
private:

	enum class Action {
		sleep = 0, wakeup = 1,
		shallowAbort = wakeup | 2, deepAbort = wakeup | 4
	};

	class ThreadAbortException : std::exception {};

	void BodyWrapper() {
		try {
			Body();
		} catch (ThreadAbortException&) {
			Aborted();
		} catch (std::exception& ex) {
			ExceptionCaught(ex);
		} catch (...) {
			UnknownExceptionCaught();
		} //exception
		std::lock_guard<std::mutex> lock(joinMutex);
		done = true;
		joinEvent.notify_all();
		Terminated();
	} //BodyWrapper

	std::thread thread;
	std::condition_variable stateEvent, joinEvent;
	std::mutex stateMutex, joinMutex;
	Action state = Action::wakeup; // protected by statusMutex, statusEvent
	bool done = false; // protected by joinMutex, joinEvent

protected:

	void SyncPoint(bool autoReset = false) {
		std::unique_lock<std::mutex> ul(stateMutex);
		stateEvent.wait(ul, [=] {
			auto result = ((int)state & (int)Action::wakeup) > 0;
			if (state == Action::deepAbort)
				throw ThreadAbortException();
			else if (state == Action::shallowAbort)
				throw ShallowThreadAbortException();
			if (autoReset)
				state = Action::sleep;
			return result;
		});
	} //SyncPoint

	// exposed to ConveyorThreadWrapper, instead of exposing Action state:
	class ShallowThreadAbortException : ThreadAbortException {};

	void SetAbort(bool set, bool shallow = true) {
		std::lock_guard<std::mutex> lock(stateMutex);
		if (set) {
			if (shallow)
				state = Action::shallowAbort;
			else
				state = Action::deepAbort;
		} else //clear abort
			state = Action::wakeup;
		stateEvent.notify_one();
	} //SetAbort

public:
	void test() {}

	void Start(bool background = false) {
		thread = std::thread(&ThreadWrapper::BodyWrapper, this);
		if (background)
			thread.detach();
	}

	void Join() {
		if (thread.joinable())
			thread.join();
		else {
			std::unique_lock<std::mutex> ul(joinMutex);
			joinEvent.wait(ul, [=] { return done; });
		} // if
	} //Join

	void PutToSleep() {
		std::lock_guard<std::mutex> lock(stateMutex);
		state = Action::sleep;
		stateEvent.notify_one();
	} //PutToSleep

	void WakeUp() {
		std::lock_guard<std::mutex> lock(stateMutex);
		state = Action::wakeup;
		stateEvent.notify_one();
	} //WakeUp

	void Abort() { // works only at the point where thread Body calls SyncPoint
		SetAbort(true);
	} //Abort

}; /* class ThreadWrapper */