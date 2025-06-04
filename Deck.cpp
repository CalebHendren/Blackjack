#include "Deck.h"
#include <algorithm>
#include <random>

Deck::Deck() {
    for (int suit = 0; suit < 4; suit++) {
        for (int value = 1; value <= 13; value++) {
            cards.emplace_back(static_cast<Suit>(suit), value);
        }
    }
    shuffle();
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(cards.begin(), cards.end(), gen);
}

Card Deck::deal() {
    if (cards.empty()) {
        *this = Deck();
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}

bool Deck::isEmpty() const {
    return cards.empty();
}
