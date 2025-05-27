#pragma once

#include <queue>
#include <deque>
#include <thread>
#include <functional>
#include <utility>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <chrono>

namespace cpp_utils
{

constexpr std::uint16_t MAXINFIFOSIZE = 20;
constexpr std::uint16_t SLEEP_MICROSECONDS = 10;

/**
 * @brief Base class for a stage in a pipeline.
 *
 * This class provides a thread-safe FIFO queue for input and output data,
 * as well as a mechanism for processing the data in a separate thread.
 *
 * @tparam InputType Type of input data.
 * @tparam OutputType Type of output data.
**/
template <class InputType, class OutputType>
class StageBase
{
protected:
    std::unique_ptr<std::thread> ThreadHandle;
    std::mutex mtx;
    
    bool ShouldClose = false;
    bool IsReady_flag = false;

    std::queue<InputType> InFIFO;
    std::queue<OutputType> OutFIFO;
    virtual void ThreadFunction(void);
    virtual bool ProcessFunction(InputType &input, OutputType &output) = 0;

    std::chrono::_V2::steady_clock::time_point t1;
    std::chrono::_V2::steady_clock::time_point t2;
    std::chrono::milliseconds duration{0};
    std::chrono::milliseconds total_dt{0};
    std::size_t n_iterations{0};

public:
    StageBase(void);
    ~StageBase();

    virtual void Post(InputType &DataIn);
    virtual bool Get(OutputType &DataOut);
    virtual uint16_t GetInFIFOSize(void);
    virtual uint16_t GetOutFIFOSize(void);
    virtual void Terminate(void);
    bool IsReady(void);
};

template <class InputType, class OutputType>
StageBase<InputType, OutputType>::StageBase(void)
{
    this->IsReady_flag = false;
    this->ShouldClose = false;
}

template <class InputType, class OutputType>
StageBase<InputType, OutputType>::~StageBase()
{
}

template <class InputType, class OutputType>
void StageBase<InputType, OutputType>::ThreadFunction(void)
{
    this->IsReady_flag = true;
    while (!this->ShouldClose)
    {
        {
            std::lock_guard<std::mutex> lck(this->mtx);
            if (!this->InFIFO.empty())
            {
                OutputType output;
                InputType &input = this->InFIFO.front();
                if (this->ProcessFunction(input, output)){
                    this->OutFIFO.push(output);
                }
                this->InFIFO.pop();
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_MICROSECONDS));
    }

    std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_MICROSECONDS));
}

template <class InputType, class OutputType>
void StageBase<InputType, OutputType>::Post(InputType &DataIn)
{
    std::lock_guard<std::mutex> lck(this->mtx);
    this->InFIFO.push(DataIn);
}

template <class InputType, class OutputType>
bool StageBase<InputType, OutputType>::Get(OutputType &DataOut)
{
    if (!this->OutFIFO.empty())
    {
        DataOut = this->OutFIFO.front();
        this->OutFIFO.pop();
        return true;
    }
    return false;
}

template <class InputType, class OutputType>
uint16_t StageBase<InputType, OutputType>::GetInFIFOSize(void)
{
    return static_cast<uint16_t>(this->InFIFO.size());
}

template <class InputType, class OutputType>
uint16_t StageBase<InputType, OutputType>::GetOutFIFOSize(void)
{
    return static_cast<uint16_t>(this->OutFIFO.size());
}

template <class InputType, class OutputType>
void StageBase<InputType, OutputType>::Terminate(void)
{
    this->ShouldClose = true;
    this->ThreadHandle->join();
}

template <class InputType, class OutputType>
bool StageBase<InputType, OutputType>::IsReady(void)
{
    return this->IsReady_flag;
}

} // namespace cpp_utils