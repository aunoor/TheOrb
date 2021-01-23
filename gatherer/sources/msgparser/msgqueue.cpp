#include "msgqueue.h"

//--------------------------------------------------------------------------------------------------------------------//

void MsgQueue::AddEvent(const std::string& message) {
    {
        //Берем себе mutex и кладем событие в очередь
        std::lock_guard<std::mutex> lk(m_mutex);
        m_messageQueue.push(message);
    }
    //Mutex автоматически разблокируется и мы уведомляем ожидающие потоки что мы что-то положили в очередь
    m_cv.notify_one();
}

//--------------------------------------------------------------------------------------------------------------------//

std::string MsgQueue::WaitEvent() {
    //Берем себе mutex
    std::unique_lock<std::mutex> lk(m_mutex);
    //и через condition_variable начинаем ждать появления события в очереди
    m_cv.wait(lk, [this]{return !m_messageQueue.empty();});
    //после того как в очереди что-то появилось, m_cv автоматически заберет mutex и мы можем спокойно достать событие
    auto message = m_messageQueue.front();
    m_messageQueue.pop();
    //отпускаем mutex
    lk.unlock();
    //и возвращаем забранное событие
    return message;
}
