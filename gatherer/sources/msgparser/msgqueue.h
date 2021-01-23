#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

///Класс для реализации мультипотоковой очереди.
//Принцип работы:
// Один поток являеться поставщиком данных и загружает события в очередь через AddEvent().
// Другие потоки, заинтересованные в получении событий вызывают WaitEvent().
// Если очередь не содержит событий, то выполнение потока, вызвовшего WaitEvent() останавливается.

class MsgQueue {
public:
    ///Функция для добавления события в очередь
    void AddEvent(const std::string& message);
    ///Функция для ожидания события из очереди
    std::string WaitEvent();
private:
    ///Экземпляр очереди
    std::queue<std::string> m_messageQueue;
    ///Мютекс для разграничения доступа
    std::mutex m_mutex;
    ///Экземпляр семафора
    std::condition_variable m_cv;
};


#endif //NP_EVENTQUEUE_H
