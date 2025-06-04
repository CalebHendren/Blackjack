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