# Blackjack Game

A console-based Blackjack game written in C++ with advanced features.

## Features

- Standard 52-card deck with automatic shuffling
- Player starts with $1000
- Betting system with input validation
- Proper Ace handling (11 or 1 value)
- Blackjack detection with 1.5x payout
- Dealer follows standard rules (hits on <17, stands on ≥17)

### Advanced Features

- **Splitting Pairs**: Split matching cards into separate hands (up to 4 hands)
- **Double Down**: Double your bet and receive exactly one more card
- **Surrender**: Give up half your bet to forfeit the hand early
- **Multiple Hand Management**: Play multiple hands simultaneously after splitting

## Controls

- **H** - Hit (take another card)
- **S** - Stand (keep current hand)
- **D** - Double Down (double bet, take one card, then stand)
- **P** - Split pairs (create two hands from matching cards)
- **R** - Surrender (forfeit hand for half bet back)
- **Enter bet amount** - Place bet to play another round
- **0** - Quit game

## How to Build

```bash
mkdir build
cd build
cmake ..
make
```

## How to Run

```bash
./Blackjack
```

## Game Rules

### Basic Rules
- Get as close to 21 as possible without going over
- Aces count as 11 or 1 (whichever is better)
- Face cards (J, Q, K) count as 10
- Dealer must hit on 16 and stand on 17
- Blackjack (21 with first 2 cards) pays 1.5:1
- Regular wins pay 1:1
- Ties are a push (bet returned)
- Splitting: Available when first two cards have same value. Creates two separate hands with equal bets. Aces
  receive only one additional card after splitting.
- Double Down: Available on first two cards only. Doubles the bet and deals exactly one more card.
- Surrender: Available on first two cards only, before any other action. Returns half the bet.
- Multiple Hands: After splitting, each hand is played separately with its own bet.

## File Structure

- `Card.h/cpp` - Card class with suit and value
- `Deck.h/cpp` - 52-card deck with shuffling
- `Hand.h/cpp` - Hand management, value calculation, and splitting logic
- `Game.h/cpp` - Main game logic with advanced features
- `main.cpp` - Entry point

## Strategy Tips

- Split Aces and 8s
- Never split 5s or 10s
- Double down on 11, and 10 against dealer's weak cards
- Surrender 16 against dealer's 9, 10, or Ace
- Consider surrendering 15 against dealer's 10
