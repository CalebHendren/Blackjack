#include "Hand.h"

void Hand::addCard(const Card& card) {
    cards.push_back(card);
}

int Hand::getValue() const {
    int value = 0;
    int aces = 0;
    
    for (const auto& card : cards) {
        int cardValue = card.getValue();
        if (cardValue == 1) {
            aces++;
            value += 11;
        } else if (cardValue > 10) {
            value += 10;
        } else {
            value += cardValue;
        }
    }
    
    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }
    
    return value;
}

bool Hand::isBusted() const {
    return getValue() > 21;
}

bool Hand::isBlackjack() const {
    return cards.size() == 2 && getValue() == 21;
}

void Hand::clear() {
    cards.clear();
}

std::string Hand::toString() const {
    std::string result;
    for (size_t i = 0; i < cards.size(); i++) {
        if (i > 0) result += " ";
        result += cards[i].toString();
    }
    return result;
}

size_t Hand::size() const {
    return cards.size();
}

bool Hand::canSplit() const {
    if (cards.size() != 2) return false;
    
    int value1 = cards[0].getValue();
    int value2 = cards[1].getValue();
    
    // Aces can be split
    if (value1 == 1 && value2 == 1) return true;
    
    // Face cards and 10s can be split with each other
    if ((value1 >= 10 || value1 == 1) && (value2 >= 10 || value2 == 1)) {
        return (value1 >= 10 && value2 >= 10) || (value1 == value2);
    }
    
    // Same value cards can be split
    return value1 == value2;
}

Card Hand::getCard(int index) const {
    if (index >= 0 && index < cards.size()) {
        return cards[index];
    }
    return Card(Suit::HEARTS, 1); // Return a default card if index is invalid
}

std::vector<Card> Hand::getCards() const {
    return cards;
}
