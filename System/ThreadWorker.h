#pragma once

//class std::thread;
class ThreadWorker
{
public:
	explicit ThreadWorker();
	~ThreadWorker();

	virtual void Process();
	void		 Join();

private:
	//std::thread* pThread;
};