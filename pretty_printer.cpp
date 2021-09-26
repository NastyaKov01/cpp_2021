#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <tuple>

struct Printer {
    std::stringstream pr_ss;
    Printer() {};

    std::string str() const
    {
        return pr_ss.str();
    }

    template<typename T>
    Printer& format(T val)
    {
        pr_ss << val;
        return *this;
    }

    template<typename T>
    Printer& format(std::set<T> cur_set)
    {
        pr_ss << "{ ";
        for (auto& el: cur_set) {
            format(el);
            if (el != *cur_set.rbegin()) {
                pr_ss << ", ";
            }
        }
        pr_ss << " }";
        return *this;
    }

    template<typename T>
    Printer& format(std::vector<T> cur_vec)
    {
        pr_ss << "[ ";
        for (auto& el: cur_vec) {
            format(el);
            if (el != *cur_vec.rbegin()) {
                pr_ss << ", ";
            }
        }
        pr_ss << " ]";
        return *this;
    }

    template<typename T1, typename T2>
    Printer& format(std::pair<T1, T2> cur_pair)
    {
        pr_ss << "( ";
        format(cur_pair.first);
        pr_ss << ", ";
        format(cur_pair.second);
        pr_ss << " )";
        return *this;
    }

    template<std::size_t I = 0, typename... Targs>
    inline typename std::enable_if<I == sizeof...(Targs), void>::type
    get_el(std::stringstream& ss, std::tuple<Targs...> tup)
    { }

    template<std::size_t I = 0, typename... Targs>
    inline typename std::enable_if<I < sizeof...(Targs), void>::type
    get_el(std::stringstream& ss, std::tuple<Targs...> tup)
    {
        format(std::get<I>(tup));
        if (I != sizeof...(Targs) - 1) {
            ss << ", ";
        }
        get_el<I + 1, Targs...>(ss, tup);
    }

    template<typename ...Targs>
    Printer& format(std::tuple<Targs...> cur_tup)
    {
        pr_ss << "( ";
        get_el(this->pr_ss, cur_tup);
        pr_ss << " )";
        return *this;
    }
};

template<typename T>
std::string format(const T& t) {
    return Printer().format(t).str();
}

int main()
{
    //Примеры 
    std::set<int> s = {1, 2, 3};
    std::vector<float> v = {1.2, 3, 5.8};
    std::pair<int, std::string> p = {1, "one"};
    auto tup = std::make_tuple(20, "two", -3.5);
    std::cout << format("abc") << std::endl;
    std::cout << format(1) << std::endl;
    std::cout << format(-100.5) << std::endl;
    std::cout << Printer().format(s).str() <<std::endl;
    std::cout << Printer().format(v).str() <<std::endl;
    std::cout << Printer().format(p).str() <<std::endl;
    std::cout << Printer().format(tup).str() <<std::endl;
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int> > v2 = {{1, 4}, {5, 6}};
    std::cout << Printer().format(" vector: ").format(v).format(" ! ").str() << std::endl;

    std::string s1 = Printer().format(" vector: ").format(v2).str();
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
    std::cout << s1 << std::endl;  // " vector: [ (1, 4), (5, 6) ]"
    std::cout << s2 << std::endl;  // "( xyz, 1, 2 ) ! 0"

    std::set<std::set<int>> set2 = {{1, 4, 5, 42}, {3, -10}, {0, 12}, {3}};
    std::cout << Printer().format("*").format("set: ").format(set2).format("*").str() << std::endl;
}