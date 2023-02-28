#include <algorithm>   // e.g., for std::shuffle
#include <compare>     // e.g., for operator<=>
#include <iostream>    // e.g., for std::cout, std::clog
#include <istream>     // e.g., for std::istream
#include <iterator>    // e.g., for std::inserter
#include <optional>    // e.g., for std::optional
#include <ostream>     // e.g., for std::ostream
#include <random>      // e.g., for std::random_device
#include <set>         // e.g., for std::set
#include <stdexcept>   // e.g., for std::domain_error
#include <string>      // e.g., for std::string
#include <type_traits> // e.g., for std::underlying_type_t
#include <vector>      // e.g., for std::vector

#include "a4-provided.hpp"

std::optional<card_face> read_card_face(std::istream &is)
{
    std::optional<card_face> retval;
    auto ch = is.get();
    if (!std::istream::traits_type::eq_int_type(ch, std::istream::traits_type::eof()))
    {

        switch (ch)
        {
        case 'A':
            retval = card_face::ace;
            break;
        case '2':
            retval = card_face::two;
            break;
        case '3':
            retval = card_face::three;
            break;
        case '4':
            retval = card_face::four;
            break;
        case '5':
            retval = card_face::five;
            break;
        case '6':
            retval = card_face::six;
            break;
        case '7':
            retval = card_face::seven;
            break;
        case '8':
            retval = card_face::eight;
            break;
        case '9':
            retval = card_face::nine;
            break;
        case 'J':
            retval = card_face::jack;
            break;
        case 'C':
            retval = card_face::knight;
            break;
        case 'Q':
            retval = card_face::queen;
            break;
        case 'K':
            retval = card_face::king;
            break;
        case 'R':
            retval = card_face::red_joker;
            break;
        case 'W':
            retval = card_face::white_joker;
            break;
        case '1':
        {

            auto ch2 = is.peek();
            if (!std::istream::traits_type::eq_int_type(ch2, std::istream::traits_type::eof()))
            {
                if (ch2 == '0')
                {
                    retval = card_face::ten;
                    is.ignore();
                }
            }
            break;
        }
        default:
            is.unget();                          // return ch to stream
            is.setstate(std::ios_base::failbit); // fail the stream
            break;
        }
    }
    if (!retval.has_value())
    {
        is.setstate(std::ios_base::failbit);
    }
    return retval;
}
std::istream &operator>>(std::istream &is, card_face &s)
{

    auto temp = read_card_face(is);
    if (temp.has_value())
    {
        s = temp.value();
    }
    return is;
}
std::ostream &operator<<(std::ostream &os, card_face const &s)
{
    switch (s)
    {
    case card_face::ace:
        os.put('A');
        break;
    case card_face::two:
        os.put('2');
        break;
    case card_face::three:
        os.put('3');
        break;
    case card_face::four:
        os.put('4');
        break;
    case card_face::five:
        os.put('5');
        break;
    case card_face::six:
        os.put('6');
        break;
    case card_face::seven:
        os.put('7');
        break;
    case card_face::eight:
        os.put('8');
        break;
    case card_face::nine:
        os.put('9');
        break;
    case card_face::jack:
        os.put('J');
        break;
    case card_face::knight:
        os.put('C');
        break;
    case card_face::queen:
        os.put('Q');
        break;
    case card_face::king:
        os.put('K');
        break;
    case card_face::red_joker:
        os.put('R');
        break;
    case card_face::white_joker:
        os.put('W');
        break;
    case card_face::ten:
    {
        os.put('1');
        os.put('0');
        break;
    }
    default:
        // os.unsetf();                         // return ch to stream
        os.setstate(std::ios_base::failbit); // fail the stream
        break;
    }
    return os;
}
class playing_card
{
private:
    card_face var_card_face;
    std::optional<card_suit> var_playing_card_suit;

public:
    playing_card() = delete;
    playing_card(card_face const &f, std::optional<card_suit> const &s = std::nullopt)
        : var_card_face(f), var_playing_card_suit(s)
    {
        if ((!var_playing_card_suit.has_value()) && var_card_face != card_face::red_joker && (var_card_face != card_face::white_joker))
        {
            throw std::domain_error("playing card without suit must be a joker");
        }
    }

    card_face const &face() const
    {
        return var_card_face;
    }
    bool has_suit() const
    {
        return var_playing_card_suit.has_value();
    }
    auto const &suit() const
    {
        return var_playing_card_suit.value();
    }
    friend bool operator==(playing_card const &, playing_card const &) = default;
    friend std::strong_ordering operator<=>(playing_card const &, playing_card const &) = default;
};
std::optional<playing_card> read_playing_card(std::istream &is)
{

    std::optional<playing_card> retval;

    auto face_opt = read_card_face(is);
    if (face_opt)
    {
        auto face = *face_opt;
        if (face != card_face::red_joker && face != card_face::white_joker)
        {
            auto suit_opt = read_card_suit(is);
            if (suit_opt)
            {
                auto suit = *suit_opt;
                retval.emplace(face, suit);
            }
        }
        else
        {
            retval.emplace(face);
        }
    }

    if (!retval)
    {
        is.setstate(std::ios_base::failbit);
    }

    return retval;
}
std::istream &operator>>(std::istream &is, playing_card &c)
{

    auto temp = read_playing_card(is);
    if (temp.has_value())
    {
        c = temp.value();
    }
    return is;
}
std::ostream &operator<<(std::ostream &os, playing_card const &c)
{
    os << c.face();
    if (c.has_suit())
    {
        os << c.suit();
    }
    return os;
}

int main()
{

    using namespace std;

    // declare a vector to hold a deck of cards read cards from cin...
    vector<playing_card> deck;
    while (auto card = read_playing_card(cin))
        deck.push_back(*card);

    // output the playing cards read in from cin to clog...
    clog << "DEBUG: " << deck.size() << " playing_cards: ";
    for (auto const &e : deck)
        clog << e;
    clog << '\n';

    // using std::random_device, instantiate a Mersenne Twister RNG...
    random_device rd;
    mt19937 rg(rd());

    // declare a set to hold playing cards sorted...
    set<playing_card> s;

    // and select up to 5 cards without replacement from deck for s...
    sample(deck.begin(), deck.end(), inserter(s, s.end()), 5, rg);

    // output the sampled cards...
    cout << "cards: ";
    for (auto const &e : s)
        cout << e << ' ';
    cout << '\n';
}