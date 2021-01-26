#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

///Класс для реализации мультипотоковой очереди.
// Принцип работы:
// Один поток являеться поставщиком данных и загружает события в очередь через AddEvent().
// Другие потоки, заинтересованные в получении событий вызывают WaitEvent().
// Если очередь не содержит событий, то выполнение потока, вызвовшего WaitEvent() останавливается.
template <class T>
class ConcurrentQueue {
public:
    ConcurrentQueue() {
        m_inBatch = false;
    }
    ///Функция для добавления записи в очередь
    void AddItem(const T& item) {
        {
            //Берем себе mutex и кладем событие в очередь
            if (!m_inBatch) std::lock_guard<std::mutex> lk(m_mutex);
            m_messageQueue.push(item);
        }
        //Mutex автоматически разблокируется и мы уведомляем ожидающие потоки что мы что-то положили в очередь
        if(!m_inBatch) m_cv.notify_one();
    }
    ///Функция включает режим пакетного добавления элементов
    void StartAdding() {
        m_mutex.lock();
        m_inBatch = true;
    }
    ///Функция выключает режим пакетного добавления
    void StopAdding() {
        m_inBatch = false;
        m_mutex.unlock();
        m_cv.notify_one();
    }

    ///Возвращает запись из очереди
    T GetItem() {
        return waitItem(false);
    }
    ///Возвращает запись из очереди не удаляя ее
    T PeekItem() {
        return waitItem(true);
    }
    ///Возвращает количество записей в очереди
    size_t Size() {
        //Берем себе mutex
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_messageQueue.size();
    }
private:
    ///Экземпляр очереди
    std::queue<T> m_messageQueue;
    ///Мютекс для разграничения доступа
    std::mutex m_mutex;
    ///Экземпляр семафора
    std::condition_variable m_cv;

    bool m_inBatch;

    ///Функция для ожидания записи из очереди
    T waitItem(bool peek) {
        //Берем себе mutex
        std::unique_lock<std::mutex> lk(m_mutex);
        //и через condition_variable начинаем ждать появления события в очереди
        m_cv.wait(lk, [this]{return !m_messageQueue.empty();});
        //после того как в очереди что-то появилось, m_cv автоматически заберет mutex и мы можем спокойно достать событие
        auto item = m_messageQueue.front();
        if (!peek) {
            m_messageQueue.pop();
        }
        //отпускаем mutex
        lk.unlock();
        //и возвращаем забранное событие
        return item;
    }
};


#endif //CONCURRENTQUEUE_H
