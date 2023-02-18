#include <cstddef> // for std::size_t
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility> // for std::move, std::forward
#include <vector>
using namespace std;
class xml_node_base
{
public:
    virtual ~xml_node_base() = default;

    void output(std::ostream &os) const
    {
        do_output_open(os);
        do_output_body(os);
        do_output_close(os);
    }

protected:
    virtual void do_output_open(std::ostream &os) const = 0;
    virtual void do_output_body(std::ostream &) const {}
    virtual void do_output_close(std::ostream &os) const = 0;
};

using xml_node_base_t = std::shared_ptr<xml_node_base>;
using xml_node_bases_t = std::vector<xml_node_base_t>;

template <typename T, typename... Args>
inline xml_node_base_t make_xml_node(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
// !!________________________________________________
class xml_node : virtual public xml_node_base
{

private:
    string name;

public:
    xml_node() = delete;
    xml_node(string const &name) : name(name) {}

protected:
    void do_output_open(ostream &os) const override
    {
        os << '<' << name << '>';
    }
    void do_output_close(ostream &os) const override
    {
        os << "</" << name << '>';
    }
};
// !!________________________________________________
class xml_node_with_children : public xml_node
{

private:
    xml_node_bases_t children_;

public:
    xml_node_with_children() = delete;

    xml_node_with_children(string const &name) : xml_node(name) {}
    xml_node_with_children(std::string const &name, std::size_t reserve) : xml_node_with_children(name)
    {
        children_.reserve(reserve);
    }
    xml_node_with_children(std::string const &name, xml_node_bases_t children) : xml_node(name), children_{std::move(children)} {}

protected:
    auto &children() { return children_; };
    auto const &children() const { return children_; };

    void do_output_body(std::ostream &os) const override
    {
        for (auto const &iterator : children_)
        {
            iterator->output(os);
        }
    }
};

// !!________________________________________________
template <typename T>
class value_node : public xml_node
{

private:
    T datum;

public:
    value_node(std::string const &name, T const &v) : xml_node(name), datum(v)
    {
    }

protected:
    void do_output_body(std::ostream &os) const override
    {

        os << datum;
    }
};

// !!________________________________________________

class note : public xml_node_with_children
{
public:
    note() = delete;
    note(std::string const &to, std::string const &from, std::string const &subject, std::string const &message) : xml_node_with_children("note", 4)
    {

        children().push_back(make_xml_node<value_node<std::string>>("to", to));
        children().push_back(make_xml_node<value_node<std::string>>("from", from));
        children().push_back(make_xml_node<value_node<std::string>>("subject", subject));
        children().push_back(make_xml_node<value_node<std::string>>("message", message));
    }
};

// !!________________________________________________

class root : protected xml_node_with_children
{
public:
    using xml_node_with_children::children;
    using xml_node_with_children::output;
    using xml_node_with_children::xml_node_with_children;
    // ostream &operator<<(std::ostream &os, const &root r)
    // {
    //     os << r.output(os);
    //     return os;
    // }
};
ostream &operator<<(std::ostream &os, const root &r)
{
    r.output(os);
    return os;
} /*
 ostream &operator<<(std::ostream &os, root &r)
 {
     r.output(os);
     return os;
 }*/
// !! **********************************************
int main()
{
    root notes{"notes"};
    notes.children().push_back(
        make_xml_node<note>("Joe", "UPS", "Reminder", "Pick up your package!"));
    notes.children().push_back(
        make_xml_node<value_node<double>>("priority", 54.23));
    cout << notes << "\n";
}