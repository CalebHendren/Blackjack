# Blackjack Game

A console-based Blackjack game written in C++.

## Features

- Standard 52-card deck with automatic shuffling
- Player starts with $1000
- Betting system with input validation
- Proper Ace handling (11 or 1 value)
- Blackjack detection with 1.5x payout
- Dealer follows standard rules (hits on <17, stands on ≥17)

## Controls

- **W** - Hit (take another card)
- **S** - Stand (keep current hand)
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

- Get as close to 21 as possible without going over
- Aces count as 11 or 1 (whichever is better)
- Face cards (J, Q, K) count as 10
- Dealer must hit on 16 and stand on 17
- Blackjack (21 with first 2 cards) pays 1.5:1
- Regular wins pay 1:1
- Ties are a push (bet returned)

## File Structure

- `Card.h/cpp` - Card class with suit and value
- `Deck.h/cpp` - 52-card deck with shuffling
- `Hand.h/cpp` - Hand management and value calculation
- `Game.h/cpp` - Main game logic and flow
- `main.cpp` - Entry point
