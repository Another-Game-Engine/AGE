#include "queue.hpp"

using namespace TMQ;

moodycamel::ConcurrentQueue<MessageBase*>              TaskManager::RenderThreadQueue::individualQueue;
moodycamel::ConcurrentQueue<MessageBase*>              TaskManager::MainThreadQueue::individualQueue;
moodycamel::ConcurrentQueue<MessageBase*>              TaskManager::TaskQueue::queue;

moodycamel::details::mpmc_sema::LightweightSemaphore   TaskManager::RenderThreadQueue::individualSemaphore;
moodycamel::details::mpmc_sema::LightweightSemaphore   TaskManager::TaskQueue::semaphore;

