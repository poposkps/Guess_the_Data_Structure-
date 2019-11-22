#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#ifndef ONLINE_JUDGE
#include <boost/assert.hpp>
#include <sstream>
#endif


enum operation{INSERT,TAKE};
struct Action
{
    Action() : operation_(INSERT), value_(0){}
    operation operation_;
    int value_;
};

class DataStructure
{
public:
    virtual std::string name() const = 0;
    virtual void insert(int value) = 0;
    virtual int take() = 0;
};

class Queue : public DataStructure
{
public:
    virtual std::string name() const { return "queue"; }
    virtual void insert(int value) { container_.push(value); }
    virtual int take()
    {
        int result = container_.front();
        container_.pop();
        return result;
    }

private:
    std::queue<int> container_;
};

class Stack : public DataStructure
{
public:
    virtual std::string name() const { return "stack"; }
    virtual void insert(int value) { container_.push(value); }
    virtual int take()
    {
        int result = container_.top();
        container_.pop();
        return result;
    }

private:
    std::stack<int> container_;
};

class PriorityQueue : public DataStructure
{
public:
    virtual std::string name() const { return "priority queue"; }
    virtual void insert(int value) { container_.push(value); }
    virtual int take()
    {
        int result = container_.top();
        container_.pop();
        return result;
    }

private:
    std::priority_queue<int> container_;
};

std::string guess_the_data_structure(const std::vector<Action> & actions, const std::vector<DataStructure*> & candidates)
{
    std::list<DataStructure*> candidates_guessed(candidates.begin(), candidates.end());

    for (const auto & action : actions)
    {
        if (action.operation_ == INSERT)
        {
            for (auto & candidate : candidates_guessed)
            {
                candidate->insert(action.value_);
            }
        }
        else
        {
            for (auto iter = candidates_guessed.begin(); iter != candidates_guessed.end();)
            {
                auto candidate = *iter;
                if (candidate->take() != action.value_)
                {
                    candidates_guessed.erase(iter++);
                }
                else
                {
                    ++iter;
                }
            }
        }
    }

    if (candidates_guessed.empty())
    {
        throw std::runtime_error("impossible");
    }
    else if (candidates_guessed.size() != 1)
    {
        throw std::runtime_error("not sure");
    }
    else
    {
        return candidates_guessed.front()->name();
    }
}

void run(std::istream & in, std::ostream & out)
{
    std::string input;

    while (!in.eof())
    {
        size_t count(0);
        in >> count;
        std::vector<Action> actions;
        for (size_t i = 0 ; i < count ; ++i)
        {
            int num_operation(0);
            int value(0);
            in >> num_operation >> value;
            Action action;
            action.operation_ = num_operation == 1 ? INSERT : TAKE;
            action.value_ = value;
            actions.push_back(action);
        }

        try
        {
            std::vector<DataStructure*> candidates;
            Stack stack_structure;
            Queue queue_structure;
            PriorityQueue priorityqueue_structure;
            candidates.push_back(&stack_structure);
            candidates.push_back(&queue_structure);
            candidates.push_back(&priorityqueue_structure);
            out << guess_the_data_structure(actions, candidates) << std::endl;
        }
        catch (const std::exception & e)
        {
            out << e.what() << std::endl;
        }
    }
}

#ifdef ONLINE_JUDGE

int main(int argc, char** argv)
{
    run(std::cin, std::cout);
    return 0;
}

#else

const std::string question = R"StringLiteral(6
1 1
1 2
1 3
2 1
2 2
2 3
6
1 1
1 2
1 3
2 3
2 2
2 1
2
1 1
2 2
4
1 2
1 1
2 1
2 2
7
1 2
1 5
1 1
1 3
2 5
1 4
2 4)StringLiteral";

const std::string answser = R"StringLiteral(queue
not sure
impossible
stack
priority queue
)StringLiteral";

int main(int argc, char** argv)
{
    std::istringstream stream_in(question);
    std::stringstream stream_out;
    run(stream_in, stream_out);

    std::string my_answer = stream_out.str();
    BOOST_ASSERT(my_answer == answser);
    return 0;
}
#endif
