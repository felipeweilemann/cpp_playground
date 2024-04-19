#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <algorithm>

template<typename T>
class Message {
public:
    T data;
    std::string timestamp;
    std::string id;
};

class Topic {
public:
    std::string name;
    std::string id;
};

template<typename T>
class IPublisher {
public:
    virtual void publish(const Message<T>& message) = 0;
    virtual ~IPublisher() {}
};

template<typename T>
class ISubscriber {
public:
    virtual void receive(const Message<T>& message) = 0;
    virtual ~ISubscriber() {}
};

class IManager {
public:
    template<typename T, typename Dummy = void>
    void registerSubscriber(std::shared_ptr<ISubscriber<T>> subscriber, std::shared_ptr<Topic> topic) {
        static_assert(sizeof(T) == 0, "This template instantiation must be overridden in derived classes.");
    }
    template<typename T, typename Dummy = void>
    void unregisterSubscriber(std::shared_ptr<ISubscriber<T>> subscriber){
        static_assert(sizeof(T) == 0, "This template instantiation must be overridden in derived classes.");
    }
    template<typename T, typename Dummy = void>
    std::shared_ptr<IPublisher<T>> createPublisher(std::shared_ptr<Topic> topic){
        static_assert(sizeof(T) == 0, "This template instantiation must be overridden in derived classes.");
    }
    template<typename T, typename Dummy = void>
    void unregisterPublisher(std::shared_ptr<IPublisher<T>> publisher){
        static_assert(sizeof(T) == 0, "This template instantiation must be overridden in derived classes.");
    }
    virtual void createTopic(const std::string& name) = 0;
    virtual void deleteTopic(const std::string& name) = 0;
    virtual std::shared_ptr<Topic> getTopic(const std::string& name) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~IManager() {}
};

template<typename T>
class Publisher : public IPublisher<T> {
public:
    void publish(const Message<T>& message) override {
        // Implementation of publish message
    }
};

template<>
class Publisher<std::string> : public IPublisher<std::string> {
public:
    Publisher(std::shared_ptr<std::string> &message_data,
        std::shared_ptr<ISubscriber<std::string>> last_string_subscriber) : 
    message_data(message_data), last_string_subscriber(last_string_subscriber) {}
    void publish(const Message<std::string>& message) override {
        // *message_data = message.data;
        last_string_subscriber->receive(message);
    }
private:
    std::shared_ptr<std::string> message_data;
    std::shared_ptr<ISubscriber<std::string>> last_string_subscriber;
};

template<typename T>
class Subscriber : public ISubscriber<T> {
public:
    void receive(const Message<T>& message) override {
        // Implementation of receive message
    }
};

template<>
class Subscriber<std::string> : public ISubscriber<std::string> {
public:
    void receive(const Message<std::string>& message) override {
        std::cout << message.data <<std::endl;
    }
};


class PubSubManager : public IManager {
private:
    std::list<std::shared_ptr<Topic>> topics;
    // std::list<std::shared_ptr<ISubscriber>> subscribers;
    // std::list<std::shared_ptr<IPublisher>> publishers;
    std::shared_ptr<std::string> message_string;
    std::shared_ptr<ISubscriber<std::string>> last_string_subscriber;

public:
    template<typename T>
    void registerSubscriber(std::shared_ptr<ISubscriber<T>> subscriber, std::shared_ptr<Topic> topic){
        // Implementation for registering a subscriber
    }

    template<typename T>
    void unregisterSubscriber(std::shared_ptr<ISubscriber<T>> subscriber) {
        // Implementation for unregistering a subscriber
    }

    template<typename T>
    std::shared_ptr<IPublisher<T>> createPublisher(std::shared_ptr<Topic> topic) {
        // Implementation for creating a publisher
        return std::make_shared<Publisher<T>>();
    }

    template<typename T>
    void unregisterPublisher(std::shared_ptr<IPublisher<T>> publisher) {
        // Implementation for unregistering a publisher
    }

    void createTopic(const std::string& name) override {
        // Implementation for creating a new topic
        topics.push_back(std::make_shared<Topic>(Topic{name, std::to_string(topics.size())}));
    }

    void deleteTopic(const std::string& name) override {
        // Implementation for deleting a topic
        topics.remove_if([&name](const std::shared_ptr<Topic>& topic) { return topic->name == name; });
    }

    std::shared_ptr<Topic> getTopic(const std::string& name) {
        auto it = std::find_if(topics.begin(), topics.end(), [&name](const std::shared_ptr<Topic>& topic) {
            return topic->name == name;
        });
        return (it != topics.end()) ? *it : nullptr;
    }

    void start() override {
        // Implementation for starting the manager
    }

    void stop() override {
        // Implementation for stopping the manager
    }
};

template<>
void PubSubManager::registerSubscriber(std::shared_ptr<ISubscriber<std::string>> subscriber, std::shared_ptr<Topic> topic){
    last_string_subscriber = subscriber;
}

template <>
std::shared_ptr<IPublisher<std::string>> PubSubManager::createPublisher(std::shared_ptr<Topic> topic) {
    // Implementation for creating a publisher
    return std::make_shared<Publisher<std::string>>(message_string, last_string_subscriber);
}


// Example usage
int main() {
    std::shared_ptr<PubSubManager> manager = std::make_shared<PubSubManager>();
    manager->createTopic("Example");
    std::shared_ptr<Topic> topic = manager->getTopic("Example");
    std::shared_ptr<Subscriber<std::string>> subscriber = std::make_shared<Subscriber<std::string>>();
    manager->registerSubscriber<std::string>(subscriber, topic);

    Message<std::string> message{"Hello, World!", "2023-04-18T12:34:56", "123456"};
    auto publisher = manager->createPublisher<std::string>(topic);
    publisher->publish(message);

    return 0;
}
