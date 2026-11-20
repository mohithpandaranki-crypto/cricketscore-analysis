# 🏏 Cricket Score Analysis

A beginner-friendly, console-based cricket scoreboard simulation built using **C++ and Object-Oriented Programming (OOP)**.

Simulate a cricket innings ball by ball and watch the scoreboard update as runs, wickets, and extras are recorded.


## 📖 Overview

This project simulates a cricket match between two teams of 11 players each. The user acts as the scorer, entering every delivery event (dot ball, runs, wicket, wide, no-ball, bye, leg-bye) and the program maintains the full match state — score, overs, strike rotation, bowler limits, fall of wickets, and economy rates.

The match runs for a user-specified number of overs, includes a toss, two innings, and produces a final result (win by runs, win by wickets, or tie).

---

## ✨ Features

- **Team Setup** – Enter names for both teams and all 11 players per team.
- **Toss & Decision** – Choose toss winner and whether they bat or bowl first.
- **Ball-by-Ball Scoring** – Record each delivery with one of 7 event types:
  1. Dot ball
  2. Runs (1, 2, 3, 4, 6 — 5 is disallowed)
  3. Wicket (Bowled, Caught, LBW, Run out)
  4. Wide
  5. No-ball
  6. Bye
  7. Leg-bye
- **Automatic Strike Rotation** – Odd runs, odd extras, and end-of-over swaps are handled automatically.
- **Bowler Management**
  - Bowler cannot bowl consecutive overs.
  - Maximum overs per bowler is auto-calculated as `(overs + 4) / 5` (standard 20% rule).
  - Per-bowler stats: overs bowled, runs given, wickets, economy.
- **Batting Stats** – Runs, balls faced, fours, sixes, out/not-out status for every batter.
- **Extras Tracking** – Wides, no-balls, byes, leg-byes all tracked separately.
- **Fall of Wickets (FOW)** – Displays score and batter at each dismissal.
- **Live Updates** – Shows current batters and current bowler after every ball.
- **Match Result** – Determines win by runs, win by wickets, or a tie.

---

## 🗂️ Project Structure

The entire program is contained in a single source file (`main.cpp`). It is organized into four classes:

main.cpp
│
├── Batsman // Per-batter statistics
├── Bowler // Per-bowler statistics
├── Team // Team roster, score, extras, scoreboard display
└── Match // Core match engine (toss, innings, scoring loop)


### 1. `Batsman` Class
| Member | Description |
|---|---|
| `name` | Player name |
| `runs, balls, fours, sixes` | Batting statistics |
| `out` | Dismissal status (bool) |
| `addRuns(int r)` | Adds runs and updates 4s/6s counters |
| `display()` | Prints one row of the batting scorecard |

### 2. `Bowler` Class
| Member | Description |
|---|---|
| `name` | Bowler name |
| `ballsBowled, runsGiven, wickets` | Bowling statistics |
| `display()` | Prints overs, runs, wickets, and computed economy |

### 3. `Team` Class
| Member | Description |
|---|---|
| `name` | Team name |
| `players[MAX_PLAYERS]` | Array of 11 `Batsman` objects |
| `totalRuns, totalWickets, extras` | Team score |
| `wides, noBalls, byes, legByes` | Extras breakdown |
| `inputTeam()` | Prompts for team name and player names |
| `displayScore(int legalBalls)` | Prints full batting scoreboard |

### 4. `Match` Class
| Member | Description |
|---|---|
| `team1, team2` | The two competing teams |
| `battingTeam, bowlingTeam` | Pointers to active teams |
| `bowlers[MAX_PLAYERS]` | Bowling stats per player |
| `currentBowler, previousBowler` | Bowler tracking (prevents consecutive overs) |
| `maxOversPerBowler` | Auto-computed cap |
| `overs, legalBalls` | Match length and progress |
| `striker, nonStriker, nextBatsman` | Batting order tracking |
| `fowScore[10], fowPlayer[10]` | Fall-of-wicket records |

**Key methods:**
| Method | Purpose |
|---|---|
| `startMatch()` | Entry point — sets up teams, toss, runs both innings |
| `playInnings(inningsNo, target)` | Main scoring loop for one innings |
| `doToss()` | Handles toss and batting/bowling decision |
| `chooseBowler(overNo)` | Validates and selects a bowler each over |
| `findPlayer(Team&, string)` | Looks up a player index by name |
| `showBatters()` | Displays current striker and non-striker |
| `displayBowling()` | Prints full bowling scorecard |
| `displayFOW()` | Prints fall of wickets |
| `displayScorecard()` | Combines batting, FOW, and bowling displays |
| `displayCurrentBowler()` | Shows the current bowler's figures |

---

## ⚙️ How It Works (Flow)

1. **Match Start**
   - User enters number of overs (1–50).
   - Program computes `maxOversPerBowler = (overs + 4) / 5`.

2. **Team Input**
   - Enter team 1 name + 11 player names.
   - Enter team 2 name + 11 player names.

3. **Toss**
   - Choose toss winner (Team 1 or Team 2).
   - Choose decision (Bat first / Bowl first).
   - The batting and bowling teams are assigned via pointers.

4. **Innings 1**
   - Openers (index 0 and 1) are placed on strike.
   - A bowler is chosen each over (cannot be the previous over's bowler, cannot exceed max overs).
   - User enters an event for every delivery.
   - Legal deliveries increment `legalBalls`; illegal deliveries (wide, no-ball) do not.
   - Wickets bring in the next batter; strike is rotated as per cricket rules.
   - Strike swaps at the end of every over.

5. **Innings Break**
   - Target = Team 1's score + 1.
   - The teams swap roles (batting ↔ bowling).

6. **Innings 2**
   - Same loop, but the innings ends early if the target is reached.

7. **Result**
   - Win by runs: first-innings team defended successfully.
   - Win by wickets: chasing team reached the target (`10 - wicketsLost`).
   - Tie: both scores equal.

---

## 🧮 Scoring Rules Implemented

| Event | Legal Ball? | Batter Balls? | Strike Change? |
|---|---|---|---|
| Dot ball | ✅ | ✅ | ❌ |
| Runs (1, 3) | ✅ | ✅ | ✅ |
| Runs (2, 4, 6) | ✅ | ✅ | ❌ |
| Wicket | ✅ | ✅ | New batter comes in |
| Wide | ❌ | ❌ | If extra wide runs are odd |
| No-ball | ❌ | ✅ | If bat runs are odd |
| Bye / Leg-bye | ✅ | ✅ | If runs are odd |

- **5 runs** is explicitly rejected for bat runs and no-ball bat runs.
- **Run out** is the only wicket type that does **not** credit the bowler.
- **Strike rotation at over end** happens only if the innings continues and wickets remain.

---

## 🚀 How to Compile & Run

### Requirements
- A C++ compiler (g++, clang++, or MSVC)
- C++11 or later (works with older standards too)

### Compile
```bash
g++ main.cpp -o cricket
```
### Run
```bash
./cricket        # Linux / macOS
cricket.exe      # Windows
```

### Sample Output
```
Enter number of overs (1 to 50): 2

Enter details for Team 1
Enter team name: India
Enter player 1 name: Rohit
...

Enter details for Team 2
Enter team name: Australia
Enter player 1 name: Warner
...

------ TOSS ------
Who won the toss? 1. India  2. Australia : 1
1. Bat first  2. Bowl first. Enter choice: 1
India will bat first.

========== INNINGS 1 ==========
Over 1 - Enter bowler name from Australia: Starc

1.1 Bowler: Starc
Enter event: 2
Enter runs (1, 2, 3, 4, 6): 4
...
```
---

## ⚠️ Known Limitations

- No support for **free hits** after a no-ball.
- No **powerplay** or **fielding restrictions**.
- **Run out** handling is simplified (always dismisses the striker).
- **Player names must be unique** within a team for lookup to work correctly.
- No **Duckworth-Lewis** or rain interruptions.
- No **file persistence** — all data is lost when the program exits.
- **No validation** on team sizes beyond the fixed 11-player array.

---

## 💡 Possible Future Improvements

- Add **undo** for the last ball.
- Support for **super overs** in case of a tie.
- Store match data to a file for later review.
- Add **man of the match** selection.
- GUI or web front-end.
- Support for **test/ODI/T20** presets.
- Track **partnerships** and **maiden overs**.

---

## 👤 Author

- [mohithpandaranki-crypto](https://github.com/mohithpandaranki-crypto)
- [chandrasekhar-maker](https://github.com/chandrasekhar-maker)

---

## 📄 License

This project is released for educational purposes. Feel free to modify and reuse.


