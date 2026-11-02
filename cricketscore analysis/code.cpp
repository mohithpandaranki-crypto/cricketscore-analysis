#include <iostream>
#include <string>
using namespace std;

const int MAX_PLAYERS = 11;

class Batsman
{
public:
    string name;
    int runs;
    int balls;
    int fours;
    int sixes;
    bool out;

    Batsman()
    {
        name = "";
        runs = 0;
        balls = 0;
        fours = 0;
        sixes = 0;
        out = false;
    }

    void addRuns(int r)
    {
        runs += r;
        balls++;

        if (r == 4)
            fours++;
        else if (r == 6)
            sixes++;
    }

    void display()
    {
        cout << name << "\t" << runs << "\t" << balls
             << "\t" << fours << "\t" << sixes << endl;
    }
};

class Bowler
{
public:
    string name;
    int ballsBowled;
    int runsGiven;
    int wickets;

    Bowler()
    {
        name = "";
        ballsBowled = 0;
        runsGiven = 0;
        wickets = 0;
    }

    void addBall()
    {
        ballsBowled++;
    }

    void display()
    {
        cout << name << "\t"
             << ballsBowled / 6 << "."
             << ballsBowled % 6 << "\t"
             << runsGiven << "\t"
             << wickets << endl;
    }
};

class Team
{
public:
    string name;
    Batsman players[MAX_PLAYERS];
    int totalRuns;
    int totalWickets;
    int extras;

    Team()
    {
        totalRuns = 0;
        totalWickets = 0;
        extras = 0;
    }

    void inputTeam()
    {
        cout << "Enter team name: ";
        cin >> name;

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            cout << "Enter player " << i + 1 << " name: ";
            cin >> players[i].name;
        }
    }

    void displayScore()
    {
        cout << "\n------ " << name << " SCOREBOARD ------\n";
        cout << "Score: " << totalRuns << "/" << totalWickets << endl;

        cout << "\nBatter\tRuns\tBalls\t4s\t6s\n";

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (players[i].name != "")
                players[i].display();
        }

        cout << "Extras: " << extras << endl;
    }
};

class Match
{
private:
    Team team1, team2;
    Bowler bowler;

    int overs;
    int striker;
    int nonStriker;
    int nextBatsman;

public:
    void startMatch()
    {
        cout << "Enter number of overs: ";
        cin >> overs;

        cout << "\nEnter details for Team 1\n";
        team1.inputTeam();

        cout << "\nEnter details for Team 2\n";
        team2.inputTeam();

        cout << "\nEnter bowler name: ";
        cin >> bowler.name;

        striker = 0;
        nonStriker = 1;
        nextBatsman = 2;

        playInnings();
    }

    void playInnings()
    {
        int totalBalls = overs * 6;

        for (int ball = 1; ball <= totalBalls; )
        {
            if (team1.totalWickets == 10)
                break;

            cout << "\nBall " << ball << endl;
            cout << "1. Dot ball\n";
            cout << "2. Runs (1, 2, 3, 4, 6)\n";
            cout << "3. Wicket\n";
            cout << "4. Wide\n";
            cout << "5. No-ball\n";

            int choice;
            cout << "Enter event: ";
            cin >> choice;

            if (choice == 1)
            {
                team1.players[striker].balls++;
                bowler.addBall();
                ball++;
            }
            else if (choice == 2)
            {
                int runs;
                cout << "Enter runs: ";
                cin >> runs;

                if (runs == 1 || runs == 2 || runs == 3 ||
                    runs == 4 || runs == 6)
                {
                    team1.players[striker].addRuns(runs);
                    team1.totalRuns += runs;
                    bowler.runsGiven += runs;

                    if (runs % 2 == 1)
                    {
                        int temp = striker;
                        striker = nonStriker;
                        nonStriker = temp;
                    }

                    bowler.addBall();
                    ball++;
                }
                else
                {
                    cout << "Invalid runs. Try again.\n";
                }
            }
            else if (choice == 3)
            {
                team1.players[striker].balls++;
                team1.players[striker].out = true;
                team1.totalWickets++;
                bowler.wickets++;
                bowler.addBall();
                ball++;

                if (nextBatsman < MAX_PLAYERS)
                {
                    striker = nextBatsman;
                    nextBatsman++;
                }
            }
            else if (choice == 4)
            {
                team1.totalRuns++;
                team1.extras++;
                bowler.runsGiven++;
            }
            else if (choice == 5)
            {
                int runs;
                cout << "Enter runs scored off the bat: ";
                cin >> runs;

                team1.totalRuns += runs + 1;
                team1.extras++;
                bowler.runsGiven += runs + 1;
            }
            else
            {
                cout << "Invalid choice. Try again.\n";
            }

            team1.displayScore();

            cout << "\nBowling Figures\n";
            cout << "Bowler\tOvers\tRuns\tWickets\n";
            bowler.display();
        }

        cout << "\nInnings completed!\n";
        team1.displayScore();
    }
};

int main()
{
    Match match;
    match.startMatch();

    return 0;
}