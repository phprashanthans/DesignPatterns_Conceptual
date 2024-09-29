/* State: Behavorial Design Pattern
    This lets an object alter its behaviour when its internal state changes
    It apperars as if the object changed its class. Closely related to finite
    state machine with finite states and transitions. Example - a document can
    be in draft, moderation and published state. 
    The pattern suggests you create new classes for all possible states of an 
    object and extract all state-specific behaviors into these classes. The original
    object called as context, stores a reference to one of the state objects that 
    represents its current state and delegates all state-related work to that object
*/
#include <iostream>
#include <typeinfo>

class Context;
class State {
protected:
    Context* context_;
public:
    virtual ~State() {}
    void set_context(Context* context) {
        this->context_ = context;
    }
    virtual void Handle1() = 0;
    virtual void Handle2() = 0;
};
class Context {
private:
    State* state_;
public:
    Context(State* state) : state_(nullptr) {
        this->TransitionTo(state);
    }
    ~Context() {
        delete state_;
    }
    void TransitionTo(State* state) {
        std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
        if(this->state_ != nullptr)
            delete this->state_;
        this->state_ = state;
        this->state_->set_context(this);
    }
    void Request1() {
        this->state_->Handle1();
    }
    void Request2() {
        this->state_->Handle2();
    }
};
class ConcreteStateA : public State {
public:
    void Handle1() override;
    void Handle2() override {
        std::cout << "ConcreteStateA handles request2.\n";
    }
};
class ConcreteStateB : public State {
public:
    void Handle1() override {
        std::cout << "ConcreteStateB handles request1.\n";
    }
    void Handle2() override {
        std::cout << "ConcreteStateB handles request2.\n";
        std::cout << "ConcreteStateB wants to change the state of the context.\n";
        this->context_->TransitionTo(new ConcreteStateA());
    }
};
void ConcreteStateA::Handle1() {
    std::cout << "ConcreteStateA handles request1.\n";
    std::cout << "ConcreteStateA wants to change the state of the context.\n";
    this->context_->TransitionTo(new ConcreteStateB());
}
void ClientCode() {
    Context* context = new Context(new ConcreteStateA());
    context->Request1();
    context->Request2();
    delete context;
}
int main() {
    ClientCode();
    return 0;
}
