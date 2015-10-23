#include "queue.hpp"

using namespace TMQ;

moodycamel::ConcurrentQueue<MessageBase*>              TaskManager::RenderThreadQueue::individualQueue;
TaskManager::CircularBuffer                            TaskManager::RenderThreadQueue::circularBuffer;
moodycamel::ConcurrentQueue<MessageBase*>              TaskManager::MainThreadQueue::individualQueue;
TaskManager::CircularBuffer                            TaskManager::MainThreadQueue::circularBuffer;
moodycamel::ConcurrentQueue<MessageBase*>              TaskManager::TaskQueue::queue;
TaskManager::CircularBuffer                            TaskManager::TaskQueue::circularBuffer;

moodycamel::details::mpmc_sema::LightweightSemaphore   TaskManager::RenderThreadQueue::individualSemaphore;
moodycamel::details::mpmc_sema::LightweightSemaphore   TaskManager::TaskQueue::semaphore;

