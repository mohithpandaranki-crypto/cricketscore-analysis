#include <iostream>
#include <string>
using namespace std;

const int MAX_PLAYERS = 11;

class Batsman
{
public:
    string name;
    int runs, balls, fours, sixes;
    bool out;

    Batsman()
    {
        runs = balls = fours = sixes = 0;
        out = false;
    }

    void addRuns(int r)
    {
        runs += r;

        if (r == 4) fours++;
        if (r == 6) sixes++;
    }

    void display()
    {
        cout << name << "\t" << runs << "\t" << balls
             << "\t" << fours << "\t" << sixes << "\t"
             << (out ? "Out" : "Not out") << endl;
    }
};

class Bowler
{
public:
    string name;
    int ballsBowled, runsGiven, wickets;

    Bowler()
    {
        ballsBowled = runsGiven = wickets = 0;
    }

    
    void display()
    {
        double economy = 0;

        if (ballsBowled > 0)
            economy = (double)runsGiven * 6 / ballsBowled;

        cout << name << "\t"
             << ballsBowled / 6 << "."
             << ballsBowled % 6 << "\t"
             << runsGiven << "\t"
             << wickets << "\t"
             << economy << endl;
    }
};

class Team
{
public:
    string name;
    Batsman players[MAX_PLAYERS];

    int totalRuns, totalWickets, extras;
    int wides, noBalls, byes, legByes;

    Team()
    {
        totalRuns = totalWickets = extras = 0;
        wides = noBalls = byes = legByes = 0;
    }

    void inputTeam()
    {
        cout << "Enter team name: ";
        getline(cin >> ws, name);

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            cout << "Enter player " << i + 1 << " name: ";
            getline(cin >> ws, players[i].name);
        }
    }


    void displayScore(int legalBalls)
    {
        cout << "\n------ " << name << " SCOREBOARD ------\n";

        cout << "Score: " << totalRuns << "/"
             << totalWickets;

        cout << "   Overs: "
             << legalBalls / 6 << "."
             << legalBalls % 6 << endl;

        cout << "Run rate: ";

        if (legalBalls == 0)
            cout << "0.00\n";
        else
            cout << (double)totalRuns * 6 / legalBalls << endl;

        cout << "\nBatter\tRuns\tBalls\t4s\t6s\tStatus\n";

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (players[i].name != "")
                players[i].display();
        }

       
        cout << "\nExtras: " << extras;
        cout << " (Wides: " << wides;
        cout << ", No-balls: " << noBalls;
        cout << ", Byes: " << byes;
        cout << ", Leg-byes: " << legByes << ")\n";
    }
};

class Match
{
private:
    Team team1, team2;

    Team *battingTeam;
    Team *bowlingTeam;

    Bowler bowlers[MAX_PLAYERS];

    int currentBowler;
    int previousBowler;
    int maxOversPerBowler;

    int overs;
    int striker, nonStriker, nextBatsman;
    int legalBalls;

    
    int fowScore[10];
    int fowPlayer[10];

    
    int findPlayer(Team &t, string n)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (t.players[i].name == n)
                return i;
        }

        return -1;
    }

   
    void doToss()
    {
        cout << "\n------ TOSS ------\n";

        int tossWinner;

        cout << "Who won the toss? 1. " << team1.name
             << "  2. " << team2.name << " : ";
        cin >> tossWinner;

        int decision;

        cout << "1. Bat first  2. Bowl first. Enter choice: ";
        cin >> decision;

        bool team1Bats =
            ((tossWinner == 1 && decision == 1) ||
             (tossWinner == 2 && decision == 2));

        if (team1Bats)
        {
            battingTeam = &team1;
            bowlingTeam = &team2;
        }
        else
        {
            battingTeam = &team2;
            bowlingTeam = &team1;
        }

        cout << battingTeam->name << " will bat first.\n";
    }

    void chooseBowler(int overNo)
    {
        while (true)
        {
            string n;

            cout << "Over " << overNo
                 << " - Enter bowler name from "
                 << bowlingTeam->name << ": ";

            getline(cin >> ws, n);

            int idx = findPlayer(*bowlingTeam, n);

            if (idx == -1)
            {
                cout << "Invalid bowler. Choose a player from the bowling team.\n";
            }
            else if (idx == previousBowler)
            {
                cout << "Same bowler cannot bowl consecutive overs.\n";
            }
            else if (bowlers[idx].ballsBowled / 6 >= maxOversPerBowler)
            {
                cout << "Bowler has reached maximum overs limit.\n";
            }
            else
            {
                currentBowler = idx;
                bowlers[idx].name = n;
                break;
            }
        }
    }

    
    void showBatters()
    {
        cout << "\nOn crease:\n";

        cout << "Striker: "
             << battingTeam->players[striker].name
             << " (" << battingTeam->players[striker].runs << " runs)\n";

        cout << "Non-striker: "
             << battingTeam->players[nonStriker].name
             << " (" << battingTeam->players[nonStriker].runs << " runs)\n";
    }

    void displayBowling()
    {
        cout << "\nBowler\tOvers\tRuns\tWickets\tEconomy\n";

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (bowlers[i].name != "")
                bowlers[i].display();
        }
    }

   
    void displayFOW()
    {
        cout << "\nFall of wickets: ";

        if (battingTeam->totalWickets == 0)
            cout << "None";

        for (int i = 0; i < battingTeam->totalWickets; i++)
        {
            cout << fowScore[i] << "/"
                 << i + 1 << " ("
                 << battingTeam->players[fowPlayer[i]].name
                 << ")  ";
        }

        cout << endl;
    }

   
    void displayScorecard()
    {
        battingTeam->displayScore(legalBalls);

        displayFOW();

        cout << "\n------ BOWLING SCORECARD ------\n";
        displayBowling();
    }

    void displayCurrentBowler(){
         Bowler &bowler = bowlers[currentBowler];
         bowler.display();
    }

   
    void playInnings(int inningsNo, int target)
    {
        
        for (int i = 0; i < MAX_PLAYERS; i++)
            bowlers[i] = Bowler();

        currentBowler = -1;
        previousBowler = -1;

        striker = 0;
        nonStriker = 1;
        nextBatsman = 2;

        legalBalls = 0;

        cout << "\n========== INNINGS "
             << inningsNo << " ==========\n";

        int maxBalls = overs * 6;
        int lastOver = 0;

        while (legalBalls < maxBalls &&
               battingTeam->totalWickets < 10)
        {
            
            if (inningsNo == 2 &&
                battingTeam->totalRuns >= target)
                break;

            int overNo = legalBalls / 6 + 1;

            if (overNo != lastOver)
            {
                if (legalBalls > 0)
                    previousBowler = currentBowler;

                chooseBowler(overNo);
                lastOver = overNo;
            }

            Bowler &bowler = bowlers[currentBowler];

            cout << "\n" << overNo << "."
                 << legalBalls % 6 + 1
                 << " Bowler: " << bowler.name << endl;

            cout << "1. Dot ball\n";
            cout << "2. Runs\n";
            cout << "3. Wicket\n";
            cout << "4. Wide\n";
            cout << "5. No-ball\n";
            cout << "6. Bye\n";
            cout << "7. Leg-bye\n";

            int choice;
            cout << "Enter event: ";
            cin >> choice;

            bool legalDelivery = true;
            bool changeStrike = false;
            bool wicket = false;

            if (choice == 1)
            {
                battingTeam->players[striker].balls++;
            }

            
            else if (choice == 2)
            {
                int r;
                cout << "Enter runs (1, 2, 3, 4, 6): ";
                cin >> r;

                while (r == 5)
                {
                    cout << "5 is not allowed. Enter 1, 2, 3, 4 or 6: ";
                    cin >> r;
                }

                battingTeam->players[striker].balls++;
                battingTeam->players[striker].addRuns(r);

                battingTeam->totalRuns += r;
                bowler.runsGiven += r;

                if (r % 2 == 1)
                    changeStrike = true;
            }

           
            else if (choice == 3)
            {
                cout << "1. Bowled\n";
                cout << "2. Caught\n";
                cout << "3. LBW\n";
                cout << "4. Run out\n";

                int type;
                cout << "Select wicket type: ";
                cin >> type;

                battingTeam->players[striker].balls++;
                battingTeam->players[striker].out = true;

                battingTeam->totalWickets++;

                
                fowScore[battingTeam->totalWickets - 1] =
                    battingTeam->totalRuns;

                fowPlayer[battingTeam->totalWickets - 1] = striker;

              
                if (type != 4)
                    bowler.wickets++;

                wicket = true;
            }

            
            else if (choice == 4)
            {
                int extraRuns;
                cout << "Additional wide runs (0 to 6): ";
                cin >> extraRuns;

                int totalWide = 1 + extraRuns;

                battingTeam->totalRuns += totalWide;
                battingTeam->extras += totalWide;
                battingTeam->wides += totalWide;

                bowler.runsGiven += totalWide;

                
                legalDelivery = false;

                if (extraRuns % 2 == 1)
                    changeStrike = true;
            }

            
            else if (choice == 5)
            {
                int r;
                cout << "Runs scored off the bat (0, 1, 2, 3, 4 or 6): ";
                cin >> r;

                while (r == 5)
                {
                    cout << "5 is not allowed. Enter 0, 1, 2, 3, 4 or 6: ";
                    cin >> r;
                }

                battingTeam->totalRuns += r + 1;

                battingTeam->extras++;
                battingTeam->noBalls++;

                battingTeam->players[striker].balls++;
                battingTeam->players[striker].addRuns(r);

                bowler.runsGiven += r + 1;

                
                legalDelivery = false;

                if (r % 2 == 1)
                    changeStrike = true;
            }

            
            else if (choice == 6 || choice == 7)
            {
                int r;
                cout << "Enter runs (1 to 3): ";
                cin >> r;

                battingTeam->totalRuns += r;
                battingTeam->extras += r;

                if (choice == 6)
                    battingTeam->byes += r;
                else
                    battingTeam->legByes += r;

                battingTeam->players[striker].balls++;

              
                if (r % 2 == 1)
                    changeStrike = true;
            }

          
            if (changeStrike)
            {
                int temp = striker;
                striker = nonStriker;
                nonStriker = temp;
            }

          
            if (legalDelivery)
            {
                legalBalls++;
                bowler.ballsBowled++;
            }

            if (wicket && nextBatsman < MAX_PLAYERS)
            {
                striker = nextBatsman;
                nextBatsman++;
            }

            
            if (legalDelivery &&
                legalBalls % 6 == 0 &&
                legalBalls < maxBalls &&
                battingTeam->totalWickets < 10)
            {
                int temp = striker;
                striker = nonStriker;
                nonStriker = temp;
            }

           
            //battingTeam->displayScore(legalBalls);
            showBatters();
            displayCurrentBowler();

            if (inningsNo == 2 &&
                battingTeam->totalRuns >= target)
                break;
        }

        cout << "\n========== INNINGS COMPLETE ==========\n";

        displayScorecard();
    }

public:

    void startMatch()
    {
        cout << "Enter number of overs (1 to 50): ";
        cin >> overs;

      
        maxOversPerBowler = (overs + 4) / 5;

        cout << "\nEnter details for Team 1\n";
        team1.inputTeam();

        cout << "\nEnter details for Team 2\n";
        team2.inputTeam();

        doToss();

        
        Team *firstBatting = battingTeam;
        Team *firstBowling = bowlingTeam;

       
        playInnings(1, -1);

       
        int target = firstBatting->totalRuns + 1;

        cout << "\nTarget for " << firstBowling->name
             << ": " << target << endl;

       
        battingTeam = firstBowling;
        bowlingTeam = firstBatting;

       
        playInnings(2, target);

       
        cout << "\n========== MATCH RESULT ==========\n";

        cout << team1.name << ": "
             << team1.totalRuns << "/"
             << team1.totalWickets << endl;

        cout << team2.name << ": "
             << team2.totalRuns << "/"
             << team2.totalWickets << endl;

        if (firstBowling->totalRuns >= target)
        {
            cout << firstBowling->name << " won by "
                 << 10 - firstBowling->totalWickets
                 << " wickets.\n";
        }
        else if (firstBatting->totalRuns ==
                 firstBowling->totalRuns)
        {
            cout << "Match tied.\n";
        }
        else
        {
            cout << firstBatting->name << " won by "
                 << firstBatting->totalRuns -
                    firstBowling->totalRuns
                 << " runs.\n";
        }
    }
};

int main()
{
	
    Match match;
    match.startMatch();

    return 0;
}