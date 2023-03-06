#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "a4-provided.hpp"
#include "a5-provided.hpp"
#include "a4-include.hpp"

auto operator<=>(playing_card_company const &lhs, playing_card_company const &rhs)
{
    return lhs.name() <=> rhs.name();
}

struct pcc
{

    playing_card_company company;

    playing_card card;
};
std::optional<pcc> read_pcc(std::istream &is)
{
    std::optional<playing_card> card = read_playing_card(is);
    playing_card_company company;

    if (card.has_value() && is >> company)
    {
        return pcc{company, *card};
    }
    return std::nullopt;
}

std::set<playing_card> generate_full_deck()
{
    using namespace std;
    set<playing_card> retval;
    for (auto const &face : {card_face::ace, card_face::two, card_face::three, card_face::four, card_face::five, card_face::six, card_face::seven, card_face::eight, card_face::nine, card_face::ten, card_face::jack, card_face::knight, card_face::queen, card_face::king})
    {
        for (auto &suit : {card_suit::diamonds, card_suit::spades, card_suit::hearts, card_suit::clubs})
        {
            retval.insert({face, suit});
        }
    }
    for (auto const &joker : {card_face::red_joker, card_face::white_joker})
    {
        retval.insert({joker});
    }
    return retval;
}

int main(int argc, char *argv[])
{
    using namespace std;
    namespace fs = std::filesystem;
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <path>\n";
        return 1;
    }

    map<playing_card_company, vector<playing_card>> all_cards;
    fs::path const base = argv[1];
    for (auto const &entry : fs::recursive_directory_iterator(base, fs::directory_options::skip_permission_denied))
    {
        ifstream input_file(entry.path().native());
        if (input_file)
        {
            // struct pcc pcc
            while (auto pcc = read_pcc(input_file))
            {
                all_cards[pcc->company].push_back(pcc->card);
            }
        }
    }

    size_t total_size = 0;
    for (auto const &entry : all_cards)
    {
        total_size += entry.second.size(); // size of vector
    }
    cout << "Total no of cards : " << total_size << "\n";
    cout << "Total no of companies : " << all_cards.size() << "\n";
    for (auto const &entry : all_cards)
    {
        cout << quoted(entry.first.name()) << "\n";
    }

    for (auto const &entry : all_cards)
    {
        cout << "\n";
        cout << quoted(entry.first.name()) << " card stats:\n";
        cout << "  Total number of cards: " << entry.second.size() << "\n";

        vector<set<playing_card>> decks;
        for (auto const &card : entry.second)
        {
            bool added_to_existing_deck = false;
            for (auto &deck : decks)
            {
                if (deck.insert(card).second)
                {
                    added_to_existing_deck = true;
                    break;
                }
            }
            if (!added_to_existing_deck)
            {
                decks.push_back({});
                decks.back().insert(card);
            }
        }

        cout << "  Total number of decks: " << decks.size() << "\n";

        size_t count = 1;
        for (auto const &deck : decks)
        {
            auto full_deck = generate_full_deck();
            vector<playing_card> missing;
            set_difference(full_deck.begin(), full_deck.end(),
                           deck.begin(), deck.end(),
                           back_inserter(missing));
            if (missing.empty())
            {
                cout << "  Deck " << count << " is not missing any cards.\n";
            }
            else
            {
                cout << "  Deck " << count << " is missing:";
                for (auto const &card : missing)
                {
                    cout << " " << card;
                }
                cout << "\n";
            }
            ++count;
        }
    }

}